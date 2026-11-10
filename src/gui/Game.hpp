#pragma once
#include <SFML/Graphics.hpp>
#include "../model/Model.hpp"

using namespace sf;
class Game
{
    RenderWindow window;
    View camera;
    View uiView;
    unique_ptr<CrossyRoadModel> model;

    RectangleShape rectDraw;
    unique_ptr<Text> score;
    unique_ptr<Text> gameOver;
    unique_ptr<Text> resetText;
    Texture bitmap;
    Texture cars;
    Font font;
    bool playerAlive = true;

public:
    void onResize(const Vector2u size)
    {
        camera.setSize(Vector2f(size) / 50.f);
        uiView.setCenter(Vector2f(size) / 2.f);
        uiView.setSize(Vector2f(size));
    }
    void Start()
    {
        window.create(VideoMode{Vector2u(1280, 720)}, "Crossy Road", State::Windowed, ContextSettings{0, 8});
        window.setVerticalSyncEnabled(true);
        onResize(window.getSize());
        if (!bitmap.loadFromFile("bitmap.png"))
            return;
        if (!cars.loadFromFile("cars.png"))
            return;
        if (!font.openFromFile("Montserrat.ttf"))
            return;
        score = make_unique<Text>(font);
        gameOver = make_unique<Text>(font, "Game over!", 40);
        resetText = make_unique<Text>(font, "Press R to restart", 30);
        bitmap.setRepeated(true);
        model = make_unique<CrossyRoadModel>();
        model->Start();
        camera.setCenter(Vector2f(MAP_WIDTH / 2.f, 0));

        Update();
    }
    void UpdateCamera(const Time delta)
    {
        const float dir = model->getPlayerPosition().y - camera.getCenter().y - 5.f;
        if (std::abs(dir) < 0.001f)
            return;
        camera.move(Vector2f(0, dir * delta.asSeconds() * 2.f));
    }
    void RenderBackground()
    {
        rectDraw.setFillColor(Color::White);
        for (int32_t i = model->getRowCount() - 1; i >= 0; i--)
        {
            switch (model->getRow(i).getType())
            {
            case RowType::Grass:
                rectDraw.setTextureRect(IntRect(Vector2i(0, 32), Vector2i(16 * MAP_WIDTH, 16)));
                break;
            case RowType::River:
                rectDraw.setTextureRect(IntRect(Vector2i(0, 16), Vector2i(16 * MAP_WIDTH, 16)));
                break;
            case RowType::Road:
                rectDraw.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(16 * MAP_WIDTH, 16)));
                break;
            default:
                break;
            }
            rectDraw.setTexture(&bitmap);
            rectDraw.setSize(Vector2f(MAP_WIDTH, 1));
            rectDraw.setPosition(Vector2f(0, -model->getLastRowIndex() + (model->getRowCount() - i)));
            window.draw(rectDraw, StencilMode(StencilComparison::Always, StencilUpdateOperation::Increment, 0x00, 0xFF));
        }
    }
    void RenderForeground(const bool shadow)
    {
        for (int32_t i = 0; i < model->getRowCount(); i++)
        {
            for (int32_t j = 0; j < model->getRow(i).getObstacleCount(); j++)
            {
                const Obstacle& obstacle = model->getRow(i).getObstacle(j);

                switch (model->getRow(i).getType())
                {
                case RowType::Grass:
                    rectDraw.setTexture(&bitmap);
                    rectDraw.setSize(Vector2f(obstacle.sizeX, 1.f));
                    rectDraw.setTextureRect(IntRect(Vector2i(0, 64), Vector2i(16 * obstacle.sizeX, 16)));
                    break;
                case RowType::River:
                    rectDraw.setTexture(&bitmap);
                    rectDraw.setSize(Vector2f(obstacle.sizeX, 0.6f));
                    rectDraw.setTextureRect(IntRect(Vector2i(0, 48), Vector2i(16 * obstacle.sizeX, 16)));
                    break;
                case RowType::Road:
                    rectDraw.setTexture(&cars);
                    rectDraw.setSize(Vector2f(obstacle.sizeX, 0.8f));
                    if (obstacle.sizeX == 1.f)
                        rectDraw.setTextureRect(IntRect(Vector2i(((model->getLastRowIndex() - model->getRowCount() + i) % 3) * 16, 0), Vector2i(16, 16)));
                    else if (obstacle.sizeX == 2.f)
                        rectDraw.setTextureRect(IntRect(Vector2i(0, 16), Vector2i(32, 16)));
                    else
                        rectDraw.setTextureRect(IntRect(Vector2i(0, 32), Vector2i(48, 16)));
                    if (model->getRow(i).getSpeed() < 0.f)
                        rectDraw.setTextureRect(IntRect(Vector2i(rectDraw.getTextureRect().position) + Vector2i(rectDraw.getTextureRect().size.x, 0),
                            Vector2i(-rectDraw.getTextureRect().size.x, rectDraw.getTextureRect().size.y)));
                    break;
                }
                const Vector2f pos = Vector2f(obstacle.positionX, -model->getLastRowIndex() + (model->getRowCount() - i) + (1.f - rectDraw.getSize().y) / 2.f);
                if (shadow)
                {
                    rectDraw.setPosition(pos + Vector2f(0.1f, 0.1f));
                    rectDraw.setFillColor(Color(0, 0, 0, 128));
                }
                else
                {
                    rectDraw.setPosition(pos);
                    rectDraw.setFillColor(Color::White);
                }
                window.draw(rectDraw, StencilMode(StencilComparison::Equal, StencilUpdateOperation::Keep, 0x01, 0xFF));
            }
        }
        if (model->isPlayerAlive())
        {
            rectDraw.setSize(Vector2f(0.8f, 0.8f));
            const Vector2f pos = Vector2f(model->getPlayerPosition()) + (Vector2f(1.f, 1.f) - rectDraw.getSize()) / 2.f;
            rectDraw.setTexture(&bitmap);
            rectDraw.setTextureRect(IntRect(Vector2i(0, 80), Vector2i(16, 16)));
            if (shadow)
            {
                rectDraw.setPosition(pos + Vector2f(0.1f, 0.1f));
                rectDraw.setFillColor(Color::Black);
            }
            else
            {
                rectDraw.setPosition(pos);
                rectDraw.setFillColor(Color::White);
            }
            window.draw(rectDraw);
        }
    }
    void Update()
    {
        Clock timer;
        Time delta;
        while (window.isOpen())
        {
            delta = timer.restart();
            Input input = Input::None;
            while (const std::optional<Event> n = window.pollEvent())
            {
                if (n->is<Event::Closed>())
                    window.close();
                else if (const auto m = n->getIf<Event::Resized>())
                    onResize(m->size);
                else if (const auto m = n->getIf<Event::KeyPressed>())
                {
                    switch (m->code)
                    {
                    case Keyboard::Key::Left: case Keyboard::Key::A:
                        input = Input::Left;
                        break;
                    case Keyboard::Key::Right: case Keyboard::Key::D:
                        input = Input::Right;
                        break;
                    case Keyboard::Key::Up: case Keyboard::Key::W:
                        input = Input::Up;
                        break;
                    case Keyboard::Key::Down: case Keyboard::Key::S:
                        input = Input::Down;
                        break;
                    case Keyboard::Key::R:
                        if (!model->isPlayerAlive())
                        {
                            model = make_unique<CrossyRoadModel>();
                            model->Start();
                            camera.setCenter(Vector2f(MAP_WIDTH / 2.f, 0));
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            if (!model->isPlayerAlive())
                input = Input::None;
            model->Update(delta.asSeconds(), input);
            UpdateCamera(delta);

            window.clear();
            window.clearStencil(0x00);
            window.setView(camera);
            RenderBackground();
            RenderForeground(true);
            RenderForeground(false);

            window.setView(uiView);
            score->setPosition(Vector2f());
            score->setOrigin(Vector2f(0, 0));
            score->setString("Score: " + to_string(model->getPlayerScore()));
            window.draw(*score);
            if (!model->isPlayerAlive())
            {
                rectDraw.setTexture(nullptr);
                rectDraw.setPosition(Vector2f(0.f, 0.f));
                rectDraw.setSize(uiView.getSize());
                rectDraw.setFillColor(Color(0, 0, 0, 128));
                window.draw(rectDraw);

                gameOver->setPosition(Vector2f(uiView.getSize().x / 2.f, uiView.getSize().y / 10.f * 4.f ));
                gameOver->setOrigin(Vector2f(gameOver->getLocalBounds().size / 2.f));
                window.draw(*gameOver);
                score->setPosition(uiView.getSize() / 2.f);
                score->setOrigin(Vector2f(score->getLocalBounds().size / 2.f));
                window.draw(*score);
                resetText->setPosition(Vector2f(uiView.getSize().x / 2.f, uiView.getSize().y / 10.f * 6.f ));
                resetText->setOrigin(Vector2f(resetText->getLocalBounds().size / 2.f));
                window.draw(*resetText);
            }
            window.display();
        }
    }
};