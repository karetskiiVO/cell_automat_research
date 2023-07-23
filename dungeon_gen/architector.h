#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>

class architector {
private:
    std::vector<std::vector<char>>* map;
    sf::Vector2i size;

    sf::Vector2i pos;
    int dir;
    bool _alive;

    const sf::Vector2i arr_dir[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

public:
    architector () = delete; 

    architector (std::vector<std::vector<char>>& map, sf::Vector2i size) {
        this->map  = &map;
        this->size = size;

        dir = 0;
        _alive = false;
    }

    architector (std::vector<std::vector<char>>* map, sf::Vector2i size) {
        this->map  = map;
        this->size = size;

        dir = 0;
        _alive = false;
    }

    bool alive () {
        return _alive;
    }

    void kill () {
        _alive = false;
    }

    void spawn (sf::Vector2i pos = {rand(), rand()}, int dir = rand()) {
        if (_alive) return;

        _alive = true;

        pos.x = std::abs(pos.x) % size.x;
        pos.y = std::abs(pos.y) % size.y;

        this->dir = std::abs(dir) % 4;
        this->pos = pos;
    } 

    std::vector<architector*>* step () {
        using namespace std;

        vector<architector*>* res = nullptr;
        int prev_dir = dir;
        
        random_device rd{};
        mt19937 gen{rd()};
        normal_distribution<double> gen_normal{10.0, 3.0};

        int len = max<int>(5, abs(round(gen_normal(gen))));
        // int len = max<int>(3, std::abs(rand()) % 20);

        sf::Vector2i buf_pos = pos + len * arr_dir[dir];

        if (buf_pos.x < 0       || buf_pos.y < 0 ||
            buf_pos.x >= size.x || buf_pos.y >= size.y) {
            kill();
            return res;
        }

        for (int i = 0; i < len; i++) {
            pos += arr_dir[dir];
            if ((*map)[pos.x][pos.y]) {
                kill();
                return res;
            }

            (*map)[pos.x][pos.y] = 1;
        }

        dir = next_random_direction(dir);

        for (int i = 0; i < 4; i++) {
            if (i == dir || i == prev_dir) continue;

            if (probability_random(0.15)) {
                if (!res) res = new vector<architector*> ();
                
                // here might be more types, such as secret and rooms
                architector* buf_architector = new architector(map, size);
                buf_architector->spawn(pos, i);
                res->push_back(buf_architector);
            }
        }

        return res;
    }

private:
    int next_random_direction (int curr_dir) {
        std::cout << curr_dir << " >> ";
        
        int res_dir = (dir + std::abs(rand()) % 3 + 3) % 4;

        std::cout << res_dir << std::endl;
        return res_dir;
    }

    bool probability_random (double probability) {
        return (1.0 * rand() < RAND_MAX * probability);
    }
};
