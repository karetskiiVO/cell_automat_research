#include "architector.h"

#include <cstdio>

s_hall_builder::s_hall_builder (std::vector<std::vector<char>>& map, sf::Vector2i size) {
    this->map  = &map;
    this->size = size;

    dir = 0;
    _alive = false;
}

s_hall_builder::s_hall_builder (std::vector<std::vector<char>>* map, sf::Vector2i size) {
    this->map  = map;
    this->size = size;

    dir = 0;
    _alive = false;
}

void s_hall_builder::spawn (sf::Vector2i pos, int dir) {
    if (_alive) return;

    _alive = true;

    pos.x = std::abs(pos.x) % size.x;
    pos.y = std::abs(pos.y) % size.y;

    this->dir = std::abs(dir) % 4;
    this->pos = pos;
} 

std::vector<architector*>* s_hall_builder::step () {
    if (!_alive) return nullptr;

    using namespace std;

    vector<architector*>* res = nullptr;
    
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> gen_normal{10.0, 2.0};

    int len = max<int>(5, abs(round(gen_normal(gen))));
    //int len = max<int>(3, std::abs(rand()) % 20);

    sf::Vector2i buf_pos = pos + len * arr_dir[dir];

    if (buf_pos.x < 0       || buf_pos.y < 0 ||
        buf_pos.x >= size.x || buf_pos.y >= size.y) {
        kill();
        return res;
    }

    for (int i = 0; i < len; i++) {
        pos += arr_dir[dir];
        switch ((*map)[pos.x][pos.y]) {
            case 0:
                break;
            case wall:
                if ((*map)[(pos + arr_dir[dir]).x][(pos + arr_dir[dir]).y] == room)
                    (*map)[pos.x][pos.y] = door;
            case room:
            case door:
            case hall:
                kill();
                return res;
        }

        (*map)[pos.x][pos.y] = hall;
    }

    int prev_dir = rev_dir(dir);
    dir = next_random_direction(dir);

    for (int i = 0; i < 4; i++) {
        if (i == dir || i == prev_dir) continue;

        // here might be more types, such as secret and room
        if (probability_random(0.8)) {
            set_new_architectors(res, pos, i, {4, 40});
        }
    }

    return res;
}

int s_hall_builder::next_random_direction (int curr_dir) {        
    int res_dir = (dir + std::abs(rand()) % 3 + 3) % 4;

    return res_dir;
}
