#include <SFML/Graphics.hpp>

#include <cmath>
#include <ctime>

#include "dungeon.h"

int main () {
    sf::Vector2u resolution(800, 600);
    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Render");

    srand(time(NULL));
    dungeon dng(128, 128);

    time_t t_0 = clock();

    dng.process();
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    resolution = window.getSize();
                    break;
                case sf::Event::KeyReleased:
                    switch (event.key.code) {
                        case sf::Keyboard::Space:
                            dng.restart();
                            break;
                        default:
                            break;
                    }

                   break;
                default:
                    break;
            }
        }

        if (clock() - t_0 > 1) {
            t_0 = clock();
            dng.process();
        }

        window.clear();
        dng.render(window);        
        window.display();
    }
    
    return 0;
}
