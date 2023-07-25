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

    std::vector<architector*> s_hall_builders;

public:
    sf::Vector2i _size;
    std::vector<std::vector<field_type>> map;

    dungeon (unsigned int size = 1) {
        field_pic.create(size, size);

        _size = {(int)size, (int)size};

        map.resize(size);
        for (unsigned int i = 0; i < size; i++) {
            map[i].resize(size);
        }

        for (int x = 0; x < this->_size.x; x++) {
            for (int y = 0; y < this->_size.y; y++) {
                map[x][y] = 0;
            }
        }

        s_hall_builders.clear();
        s_hall_builders.push_back(new s_hall_builder(map, _size));
        s_hall_builders[0]->spawn(_size / 2);
        map[_size.x / 2][_size.y / 2] = 1;
    }

    dungeon (unsigned int x_size, unsigned int y_size) {
        field_pic.create(x_size, y_size);

        _size = {(int)x_size, (int)y_size};
        map.resize(x_size);
        for (unsigned int x = 0; x < x_size; x++) {
            map[x].resize(y_size);
        }

        for (int x = 0; x < this->_size.x; x++) {
            for (int y = 0; y < this->_size.y; y++) {
                map[x][y] = 0;
            }
        }

        s_hall_builders.clear();
        s_hall_builders.push_back(new s_hall_builder(map, _size));
        s_hall_builders[0]->spawn(_size / 2);
        map[_size.x / 2][_size.y / 2] = 1;
    }

    std::vector<field_type>& operator[] (unsigned int idx) {
        return this->map[idx];
    }

    void render (sf::RenderWindow& window) {
        if (window.getSize() != field_pic_buf.getSize()) {
            field_pic_buf = field_pic;
            upscale(field_pic_buf, window.getSize());
        }

        sf::Sprite render_buf;
        sf::Texture buf_texture;
        buf_texture.loadFromImage(field_pic_buf);
        render_buf.setTexture(buf_texture);
        window.draw(render_buf);
    }

    void draw () {
        for (unsigned int x = 0; x < (unsigned int)_size.x; x++) {
            for (unsigned int y = 0; y < (unsigned int)_size.y; y++) {
                field_pic.setPixel(x, y, convert(map[x][y]));
            }
        }

        field_pic_buf = field_pic;
    }

    void restart () {
        for (unsigned int x = 0; x < this->_size.x; x++) {
            for (unsigned int y = 0; y < this->_size.y; y++) {
                map[x][y] = 0;
            }
        }

        s_hall_builders.clear();
        s_hall_builders.push_back(new s_hall_builder(map, _size));
        s_hall_builders[0]->spawn(_size / 2);
        map[_size.x / 2][_size.y / 2] = 1;
    }

    void process () {
        std::vector<architector*> s_hall_builders_accum;
        std::vector<architector*>* buf = nullptr;

        for (int i = 0; i < s_hall_builders.size(); i++) {
            buf = s_hall_builders[i]->step();
            if (!buf) continue;

            s_hall_builders_accum.insert(s_hall_builders_accum.end(), (*buf).begin(), (*buf).end());
            delete buf;
        }

        // delete dead
        int dead_workers = 0;
        for (int i = 0; i < s_hall_builders.size(); i++) {
            if (!s_hall_builders[i]->alive()) {
                dead_workers++;
            } else if (dead_workers) {
                std::swap(s_hall_builders[i - dead_workers], s_hall_builders[i]);
            }
        }
        s_hall_builders.resize(s_hall_builders.size() - dead_workers);

        if (s_hall_builders_accum.size() > 0) 
            s_hall_builders.insert(s_hall_builders.end(), s_hall_builders_accum.begin(), s_hall_builders_accum.end());

        draw();
    }

private:
    sf::Color convert(field_type cell) {
        const sf::Color arr[] = {sf::Color::Black, sf::Color::Blue, sf::Color::Yellow, sf::Color::Red};

        return arr[cell];
    }

    void upscale (sf::Image& img, sf::Vector2u newres) {
        unsigned int scale = std::min(newres.x / img.getSize().x, newres.y / img.getSize().y);

        if (!scale) return;

        unsigned int cur_x = (newres.x - scale * img.getSize().x) / 2;
        unsigned int cur_y = (newres.y - scale * img.getSize().y) / 2;

        unsigned int start_y = cur_y;

        sf::Image buf(img);
        img.create(newres.x, newres.y, sf::Color(0x49423d));

        for (unsigned int x = 0; x < buf.getSize().x; x++) {
            for (unsigned int y = 0; y < buf.getSize().y; y++) {
                sf::Color buf_color = buf.getPixel(x, y);

                for (unsigned int dx = 0; dx < scale; dx++) {
                    for (unsigned int  dy = 0; dy < scale; dy++) {
                        img.setPixel(cur_x + dx, cur_y + dy, buf_color);
                    }
                }

                cur_y += scale;
            }

            cur_y = start_y;
            cur_x += scale;
        }
    }
};
