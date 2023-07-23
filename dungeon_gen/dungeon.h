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

        builders.clear();
        builders.push_back(new architector(map, _size));
        builders[0]->spawn(_size / 2);
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

        builders.clear();
        builders.push_back(new architector(map, _size));
        builders[0]->spawn(_size / 2);
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

        builders.clear();
        builders.push_back(new architector(map, _size));
        builders[0]->spawn(_size / 2);
        map[_size.x / 2][_size.y / 2] = 1;
    }

    void process () {
        std::vector<architector*> builders_accum;
        std::vector<architector*>* buf = nullptr;

        for (int i = 0; i < builders.size(); i++) {
            buf = builders[i]->step();
            if (!buf) continue;

            builders_accum.insert(builders_accum.end(), (*buf).begin(), (*buf).end());
            delete buf;
        }

        // delete dead
        int dead_workers = 0;
        for (int i = 0; i < builders.size(); i++) {
            if (!builders[i]->alive()) {
                dead_workers++;
            } else if (dead_workers) {
                std::swap(builders[i - dead_workers], builders[i]);
            }
        }
        builders.resize(builders.size() - dead_workers);

        if (builders_accum.size() > 0) 
            builders.insert(builders.end(), builders_accum.begin(), builders_accum.end());

        draw();
    }

private:
    sf::Color convert(field_type cell) {
        return sf::Color(0, cell ? 255 : 0, 0);
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
