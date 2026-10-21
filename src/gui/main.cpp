#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    RenderWindow window(VideoMode{Vector2u(1280, 720)}, "Crossy Road");
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        while (const std::optional<Event> n = window.pollEvent())
        {
            if (n->is<Event::Closed>())
                window.close();
        }
        window.clear();
        window.display();
    }
}