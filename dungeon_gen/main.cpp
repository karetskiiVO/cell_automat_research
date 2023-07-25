#include <cmath>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "dungeon.h"

int main () {
    sf::Vector2u resolution(800, 600);
    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Render");

    srand(time(NULL));
    dungeon dng(64, 64);

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
                            dng.process();
                            break;
                        case sf::Keyboard::Enter:
                            dng.process();
                            break;
                        default:
                            break;
                    }

                   break;
                default:
                    break;
            }
        }

        if (clock() - t_0 > 50) {
            t_0 = clock();
            dng.process();
        }

        window.clear();
        dng.render(window);        
        window.display();
    }
    
    return 0;
}
