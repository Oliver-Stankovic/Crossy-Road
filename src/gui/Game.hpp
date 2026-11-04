#pragma once
#include <SFML/Graphics.hpp>
#include "../model/Model.hpp"

using namespace sf;
class Game
{
    RenderWindow window;
    View camera;
    CrossyRoadModel model;

    RectangleShape rectDraw;

public:
    void onResize(const Vector2u size)
    {
        camera.setSize(Vector2f(size) / 50.f);
    }
    void Start()
    {
        window.create(VideoMode{Vector2u(1280, 720)}, "Crossy Road", State::Windowed, ContextSettings{0, 8});
        window.setVerticalSyncEnabled(true);
        onResize(window.getSize());
        camera.setCenter(Vector2f(MAP_WIDTH / 2.f, 0));

        model.Start();

        Update();
    }
    void UpdateCamera(const Time delta)
    {
        const float dir = model.getPlayerPosition().y - camera.getCenter().y - 5.f;
        if (std::abs(dir) < 0.001f)
            return;
        camera.move(Vector2f(0, dir * delta.asSeconds() * 2.f));
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
                    default:
                        break;
                    }
                }
            }
            model.Update(delta.asSeconds(), input);
            if (!model.isPlayerAlive())
                window.close();
            UpdateCamera(delta);

            window.clear();
            window.clearStencil(0x00);
            window.setView(camera);
            for (int32_t i = 0; i < model.getRowCount(); i++)
            {
                switch (model.getRow(i).getType())
                {
                case RowType::Grass:
                    rectDraw.setFillColor(Color::Green);
                    break;
                case RowType::River:
                    rectDraw.setFillColor(Color::Cyan);
                    break;
                case RowType::Road:
                    rectDraw.setFillColor(Color(128, 128, 128));
                    break;
                default:
                    break;
                }
                rectDraw.setSize(Vector2f(MAP_WIDTH, 1));
                rectDraw.setPosition(Vector2f(0, -model.getLastRowIndex() + (model.getRowCount() - i)));
                window.draw(rectDraw, StencilMode(StencilComparison::Always, StencilUpdateOperation::Increment, 0x00, 0xFF));
                for (int32_t j = 0; j < model.getRow(i).getObstacleCount(); j++)
                {
                    const Obstacle& obstacle = model.getRow(i).getObstacle(j);

                    switch (model.getRow(i).getType())
                    {
                    case RowType::Grass:
                        rectDraw.setSize(Vector2f(obstacle.sizeX, 1.f));
                        rectDraw.setFillColor(Color(128, 255, 128));
                        break;
                    case RowType::River:
                        rectDraw.setSize(Vector2f(obstacle.sizeX, 0.8f));
                        rectDraw.setFillColor(Color(128, 64, 0));
                        break;
                    case RowType::Road:
                        rectDraw.setSize(Vector2f(obstacle.sizeX, 0.8f));
                        rectDraw.setFillColor(Color::White);
                        break;
                    }
                    rectDraw.setPosition(Vector2f(obstacle.positionX, -model.getLastRowIndex() + (model.getRowCount() - i) + (1.f - rectDraw.getSize().y) / 2.f));
                    window.draw(rectDraw, StencilMode(StencilComparison::Equal, StencilUpdateOperation::Keep, 0x01, 0xFF));
                }
            }
            rectDraw.setSize(Vector2f(0.8f, 0.8f));
            rectDraw.setPosition(Vector2f(model.getPlayerPosition()) + (Vector2f(1.f, 1.f) - rectDraw.getSize()) / 2.f);
            rectDraw.setFillColor(Color::Yellow);
            window.draw(rectDraw);
            window.display();
        }
    }
};