#include "architector.h"

room_builder::room_builder (std::vector<std::vector<char>>& map, sf::Vector2i size) {
    this->map  = &map;
    this->size = size;

    dir = 0;
    _alive = false;
}

room_builder::room_builder (std::vector<std::vector<char>>* map, sf::Vector2i size) {
    this->map  = map;
    this->size = size;

    dir = 0;
    _alive = false;
}

std::vector<architector*>* room_builder::step () {
    using namespace std;

    random_device rd{};
    mt19937 gen{rd()};
    normal_distribution<double> gen_normal{10.0, 2.5};

    std::vector<architector*>* res = nullptr;
    sf::Vector2i room_size(max<int>(5, abs(round(gen_normal(gen)))), max<int>(5, abs(round(gen_normal(gen)))));

    int left_l = 0, right_l = 0;
    int left_dir  = (dir + 3) % 4;
    int right_dir = (dir + 1) % 4;

    sf::Vector2i left_pos  = pos + arr_dir[dir] + arr_dir[left_dir];
    sf::Vector2i right_pos = pos + arr_dir[dir];

    // search for free space

    for (int i = 0; i < room_size.x - 1; i++, left_pos += arr_dir[left_dir]) {
        sf::Vector2i buf = left_pos;
        bool clear = true;

        for (int j = 0; j < room_size.y; j++, buf += arr_dir[dir]) {
            if (buf.x < 0       || buf.y < 0 ||
                buf.x >= size.x || buf.y >= size.y) {
                clear = false;
                break;
            }

            if ((*map)[buf.x][buf.y]) {
                clear = false;
                break;
            }
        }

        if (clear) {
            left_l++;
        } else {
            break;
        }
    }

    for (int i = 0; i < room_size.x; i++, right_pos += arr_dir[right_dir]) {
        sf::Vector2i buf = right_pos;
        bool clear = true;

        for (int j = 0; j < room_size.y; j++, buf += arr_dir[dir]) {
            if (buf.x < 0       || buf.y < 0 ||
                buf.x >= size.x || buf.y >= size.y) {
                clear = false;
                break;
            }

            if ((*map)[buf.x][buf.y]) {
                clear = false;
                break;
            }
        }

        if (clear) {
            right_l++;
        } else {
            break;
        }
    }

    if (!right_l || left_l + right_l < room_size.x) {
        kill();
        return res;
    }

    int d = (left_l + right_l - room_size.x) ? (std::abs(rand()) % (left_l + right_l - room_size.x)) : 0;
    left_pos  = pos + arr_dir[dir] + arr_dir[left_dir];
    right_pos = pos + arr_dir[dir];

    if (left_l - d == 0 || room_size.x + d - left_l == 1) {
        kill();
        return res;
    }

    // draw room

    for (int i = 0; i < left_l - d; i++, left_pos += arr_dir[left_dir]) {
        sf::Vector2i buf = left_pos;

        for (int j = 0; j < room_size.y; j++, buf += arr_dir[dir]) {
            if (i == left_l - d - 1 ||
                j == 0 || j == room_size.y - 1) {
                (*map)[buf.x][buf.y] = wall;
            } else {
                (*map)[buf.x][buf.y] = room;
            }
        }
    }

    for (int i = 0; i < room_size.x + d - left_l; i++, right_pos += arr_dir[right_dir]) {
        sf::Vector2i buf = right_pos;

        for (int j = 0; j < room_size.y; j++, buf += arr_dir[dir]) {
            if (i == room_size.x + d - left_l - 1 ||
                j == 0 || j == room_size.y - 1) {
                (*map)[buf.x][buf.y] = wall;
            } else {
                (*map)[buf.x][buf.y] = room;
            }
        }
    }

    // draw door

    sf::Vector2i door_pos = pos + arr_dir[dir];
    (*map)[door_pos.x][door_pos.y] = door;

    // genere new objects

    sf::Vector2i new_doors_pos = door_pos;
    int new_dors_dir = (dir + 1) % 4;
    for (int i = 0; i < 2 * (room_size.x + room_size.y - 4); i++) {
        int bufdir = (new_dors_dir + 3) % 4; // direction to xenter
        sf::Vector2i tocen = new_doors_pos + arr_dir[bufdir];

        if ((*map)[tocen.x][tocen.y] == wall) {
            new_dors_dir = bufdir;
            new_doors_pos += arr_dir[new_dors_dir];
            bufdir = (new_dors_dir + 3) % 4;
        }

        bufdir = (new_dors_dir + 1) % 4;
        if (probability_random(0.1)) {
            set_new_architectors(res, new_doors_pos, bufdir, {4, 10});
            (*map)[new_doors_pos.x][new_doors_pos.y] = door;
        }

        new_doors_pos += arr_dir[new_dors_dir];
    }

    return res;
}



void room_builder::spawn (sf::Vector2i pos, int dir) {
    if (_alive) return;

    _alive = true;

    pos.x = std::abs(pos.x) % size.x;
    pos.y = std::abs(pos.y) % size.y;

    this->dir = std::abs(dir) % 4;
    this->pos = pos;
}