#include "field.h"
#include "cell_processor.h"
#include <SFML/Graphics.hpp>
#include <math.h>

int main () {
    sf::Vector2u resolution(800, 600);
    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Render");

    field fld(360, 289);
    fld.setPerocessor(antLRL_processor);

    fld.process(1);
    fld.render(window);
    srand(time(NULL));

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
                            
                            fld.process();

                            break;
                        default:
                            break;
                    }

                   break;
                default:
                    break;
            }
        }
        window.clear();
        fld.render(window);
        window.display();
    }
    
    return 0;
}
