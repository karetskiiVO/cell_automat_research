#pragma once

#include "architector.h"
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

typedef char field_type;

class dungeon {
private:
    sf::Image field_pic;
    sf::Image field_pic_buf;

    std::vector<architector*> builders;
    bool done;

    const sf::Vector2i arr_dir[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
public:
    sf::Vector2i _size;
    std::vector<std::vector<field_type>> map;

    dungeon (unsigned int size = 1);
    dungeon (unsigned int x_size, unsigned int y_size);

    std::vector<field_type>& operator[] (unsigned int idx);

    void render (sf::RenderWindow& window);
    void draw ();
    void restart ();
    void process ();
    void post_processing ();

private:
    sf::Color convert(field_type cell);
    void upscale (sf::Image& img, sf::Vector2u newres);
    int hall_caherency (sf::Vector2i pos);
    void hall_dtr (sf::Vector2i pos);
    void door_dtr (sf::Vector2i pos);
    int rev_dir (int dir);
};
