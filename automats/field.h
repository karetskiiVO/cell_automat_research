#pragma once

#include <cstdio>
#include <vector>
#include <random>
#include "SFML/Graphics.hpp"

typedef char field_type;

class field {
private:
    sf::Vector2u _size;
    std::vector<std::vector<field_type>> map;
    void (*processor)(sf::Vector2u size, std::vector<std::vector<field_type>>&, int);
    sf::Image field_pic;
    sf::Image field_pic_buf;

public:
    field (unsigned int size = 1) {
        field_pic.create(size, size);

        processor = nullptr;
        _size = {size, size};

        map.resize(size);
        for (unsigned int i = 0; i < size; i++) {
            map[i].resize(size);
        }

        for (unsigned int x = 0; x < this->_size.x; x++) {
            for (unsigned int y = 0; y < this->_size.y; y++) {
                map[x][y] = 0;
            }
        }
    }

    field (unsigned int x_size, unsigned int y_size) {
        field_pic.create(x_size, y_size);

        processor = nullptr;
        _size = {x_size, y_size};

        map.resize(x_size);
        for (unsigned int x = 0; x < x_size; x++) {
            map[x].resize(y_size);
        }

        for (unsigned int x = 0; x < this->_size.x; x++) {
            for (unsigned int y = 0; y < this->_size.y; y++) {
                map[x][y] = 0;
            }
        }
    }

    std::vector<field_type>& operator[] (unsigned int idx) {
        return this->map[idx];
    }

    sf::Vector2u size () {
        return _size;
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

    sf::Image& draw () {
        for (unsigned int x = 0; x < _size.x; x++) {
            for (unsigned int y = 0; y < _size.y; y++) {
                field_pic.setPixel(x, y, convert(map[x][y]));
            }
        }

        return field_pic;
    }

    void setPerocessor (void (*processor)(sf::Vector2u size, std::vector<std::vector<field_type>>&, int)) {
        this->processor = processor;
    }

    void process (int seed = rand()) {
        processor(_size, map, seed);
        draw();
        field_pic_buf = field_pic;
    }

private:
    sf::Color convert(field_type cell) {
        return sf::Color(0, static_cast<sf::Uint8>(cell), 0);
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