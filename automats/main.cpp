#include "field.h"
#include "cell_processor.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>

int main () {
    sf::Vector2u resolution(1600, 1200);
    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Render");

    field fld(512, 512);
    fld.render(window);
    srand(time(NULL));

    //LR4_universal_online drower(fld, {"RLR", "RLR", "RLR", "RLR", "RLR", "RLR", "RLR"});
    std::vector<const char*> buf;
    for (size_t i = 0; i < 15; i++)
        buf.push_back("LLRRRLRLRLLR");
    LR4_universal_online drower(fld, buf);
    time_t t_0 = clock();
    drower.start();

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
                            drower.restart();
                            break;
                        default:
                            break;
                    }

                   break;
                default:
                    break;
            }
        }

        /*if (clock() - t_0 > 1) {
            t_0 = clock();
            drower.step();
        }*/

        for (size_t i = 0; i < 10; i++)
            drower.step();
        window.clear();
        fld.render(window);
        window.display();
    }
    
    return 0;
}
