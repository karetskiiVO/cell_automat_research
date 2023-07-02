#include "cell_processor.h"
#include <iostream>
#include <string>

#define marked     ((char)0xff)
#define not_marked ((char)0x00)

void clear (sf::Vector2u size, std::vector<std::vector<field_type>>& map) {
    for (unsigned int x = 0; x < size.x; x++) {
        for (unsigned int y = 0; y < size.y; y++) {
            map[x][y] = not_marked;
        }
    }
}

void normalized (sf::Vector2u size, std::vector<std::vector<field_type>>& map) {
    for (unsigned int x = 0; x < size.x; x++) {
        for (unsigned int y = 0; y < size.y; y++) {
            if (map[x][y]) {
                map[x][y] = marked;
            } else {
                map[x][y] = not_marked;
            }
        }
    }
}

class turemite {
private:
    sf::Vector2u size;
    std::vector<std::vector<field_type>>* map;

    sf::Vector2u position;
    sf::Vector2i direction;
public:
    turemite () = delete;

    turemite (sf::Vector2u size, std::vector<std::vector<field_type>>& map) {
        this->map  = &map;
        this->size = size;

        position = {0, 0};
    }
    
    void spawn () {
        position = {std::abs(rand()) % size.x, std::abs(rand()) % size.y};
    }

    void spawn (sf::Vector2u pos) {
        position = pos;
    }

};

sf::Vector2i four_dir[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

class antLR {
private:
    char colors;
    sf::Vector2u size;
    std::vector<std::vector<field_type>>* map;

    sf::Vector2i pos;
    unsigned int cur_dir;
public:
    antLR () = delete;

    antLR (sf::Vector2u size, std::vector<std::vector<field_type>>& map) {
        this->map  = &map;
        this->size = size;

        pos = {0, 0};
        cur_dir = 0;
        colors = 2;
    }
    
    void spawn () {
        pos = {(int)(std::abs(rand()) % size.x), (int)(std::abs(rand()) % size.y)};
        cur_dir = std::abs(rand()) % 4;
    }

    void spawn (sf::Vector2u pos, int dir = std::abs(rand()) % 4) {
        this->pos.x = pos.x;
        this->pos.y = pos.y;
        cur_dir = dir;
    }

    void move () {
        switch ((*map)[pos.x][pos.y]) {
            case 0:
                left();
                next_color();
                step();
                break;
            case 1:
                right();
                next_color();
                step();
                break;
        }
    }
private:
    void left  () {
        cur_dir = (cur_dir + 1) % 4;
    }

    void right () {
        cur_dir = (cur_dir + 4 - 1) % 4;
    }

    void step () {
        pos.x = (pos.x + four_dir[cur_dir].x + size.x) % size.x;
        pos.y = (pos.y + four_dir[cur_dir].y + size.y) % size.y;
    }

    void next_color () {
        (*map)[pos.x][pos.y] = ((*map)[pos.x][pos.y] + 1) % colors;
    }
};

void antLR_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed) {
    clear(size, map);
    srand(seed);
    const unsigned int automats_size = 6;
    antLR* automats[automats_size];

    for (unsigned int i = 0; i < automats_size; i++) {
        automats[i] = new antLR(size, map);
        automats[i]->spawn();
    }

    unsigned int num = 3254 + std::abs(rand()) % 8751;
    for (unsigned int i = 0; i < num; i++) {
        for (unsigned int i = 0; i < automats_size; i++) {
            automats[i]->move();
        }
    }

    for (unsigned int i = 0; i < automats_size; i++) {
        delete automats[i];
    }

    normalized(size, map);
}

class antLRL {
private:
    char colors;
    sf::Vector2u size;
    std::vector<std::vector<field_type>>* map;

    sf::Vector2i pos;
    unsigned int cur_dir;
public:
    antLRL () = delete;

    antLRL (sf::Vector2u size, std::vector<std::vector<field_type>>& map) {
        this->map  = &map;
        this->size = size;

        pos = {0, 0};
        cur_dir = 0;
        colors = 3;
    }
    
    void spawn () {
        pos = {(int)(std::abs(rand()) % size.x), (int)(std::abs(rand()) % size.y)};
        cur_dir = std::abs(rand()) % 4;
    }

    void spawn (sf::Vector2u pos, int dir = std::abs(rand()) % 4) {
        this->pos.x = pos.x;
        this->pos.y = pos.y;
        cur_dir = dir;
    }

    void move () {
        switch ((*map)[pos.x][pos.y]) {
            case 0:
                left();
                next_color();
                step();
                break;
            case 1:
                right();
                next_color();
                step();
                break;
            case 2:
                left();
                next_color();
                step();
                break;
        }
    }
private:
    void left  () {
        cur_dir = (cur_dir + 1) % 4;
    }

    void right () {
        cur_dir = (cur_dir + 4 - 1) % 4;
    }

    void step () {
        pos.x = (pos.x + four_dir[cur_dir].x + size.x) % size.x;
        pos.y = (pos.y + four_dir[cur_dir].y + size.y) % size.y;
    }

    void next_color () {
        (*map)[pos.x][pos.y] = ((*map)[pos.x][pos.y] + 1) % colors;
    }
};

void antLRL_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed) {
    clear(size, map);
    srand(seed);
    unsigned int automats_size = 2 + std::abs(rand()) % 31;
    antLRL** automats = new antLRL* [automats_size];

    for (unsigned int i = 0; i < automats_size; i++) {
        automats[i] = new antLRL(size, map);
        automats[i]->spawn();
    }

    unsigned int num = 23254 + std::abs(rand()) % 78751;
    for (unsigned int i = 0; i < num; i++) {
        for (unsigned int i = 0; i < automats_size; i++) {
            automats[i]->move();
        }
    }

    for (unsigned int i = 0; i < automats_size; i++) {
        delete automats[i];
    }
    delete[] automats;

    normalized(size, map);
}

class LR4_universal {
private:
    char colors;
    std::string instr;
    sf::Vector2u size;
    std::vector<std::vector<field_type>>* map;

    sf::Vector2i pos;
    unsigned int cur_dir;
public:
    LR4_universal () = delete;

    LR4_universal (sf::Vector2u size, std::vector<std::vector<field_type>>& map, std::string& instr) {
        this->map   = &map;
        this->size  = size;
        this->instr = instr;

        pos = {0, 0};
        cur_dir = 0;
        colors = instr.length();
    }
    
    void spawn () {
        pos = {(int)(std::abs(rand()) % size.x), (int)(std::abs(rand()) % size.y)};
        cur_dir = std::abs(rand()) % 4;
    }

    void spawn (sf::Vector2u pos, int dir = std::abs(rand()) % 4) {
        this->pos.x = pos.x;
        this->pos.y = pos.y;
        cur_dir = dir;
    }

    void move () {
        switch (instr[(*map)[pos.x][pos.y]]) {
            case 'R':
                left();
                next_color();
                step();
                break;
            case 'L':
                right();
                next_color();
                step();
                break;
            case 'N':
                next_color();
                step();
                break;
        }
    }
private:
    void left  () {
        cur_dir = (cur_dir + 1) % 4;
    }

    void right () {
        cur_dir = (cur_dir + 4 - 1) % 4;
    }

    void step () {
        pos.x = (pos.x + four_dir[cur_dir].x + size.x) % size.x;
        pos.y = (pos.y + four_dir[cur_dir].y + size.y) % size.y;
    }

    void next_color () {
        (*map)[pos.x][pos.y] = ((*map)[pos.x][pos.y] + 1) % colors;
    }
};

void LR4_uniersal_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed) {
    clear(size, map);
    srand(seed);
    unsigned int automats_size = 1;
    LR4_universal** automats = new LR4_universal* [automats_size];

    for (unsigned int i = 0; i < automats_size; i++) {
        automats[i] = new LR4_universal(size, map, std::string("LR"));
        automats[i]->spawn();
    }

    unsigned int num = 23254 + std::abs(rand()) % 78751;
    for (unsigned int i = 0; i < num; i++) {
        for (unsigned int i = 0; i < automats_size; i++) {
            automats[i]->move();
        }
    }

    for (unsigned int i = 0; i < automats_size; i++) {
        delete automats[i];
    }
    delete[] automats;

    normalized(size, map);
}
