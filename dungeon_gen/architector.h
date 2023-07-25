#pragma once

#define hall 1
#define room 2
#define door 3

#include <vector>
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>

class architector {
    
    friend class room_builder;
    friend class s_hall_builder;

private:
    std::vector<std::vector<char>>* map;
    sf::Vector2i size;

    sf::Vector2i pos;
    bool _alive;

    const sf::Vector2i arr_dir[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
public:
    
    void kill ();
    bool alive ();

    virtual void spawn (sf::Vector2i pos = {rand(), rand()}, int dir = rand()) = 0;
    virtual std::vector<architector*>* step () = 0;

private:
    int rev_dir (int dir);
};

class s_hall_builder : public architector {
private:
    int dir;
public:
    s_hall_builder () = delete; 
    s_hall_builder (std::vector<std::vector<char>>& map, sf::Vector2i size);
    s_hall_builder (std::vector<std::vector<char>>* map, sf::Vector2i size);

    void spawn (sf::Vector2i pos = {rand(), rand()}, int dir = rand());
    std::vector<architector*>* step ();

private:
    int next_random_direction (int curr_dir);
    bool probability_random (double probability);
};

class room_builder : public architector {
private:
    int dir;
public:
    room_builder () = delete;
    room_builder (std::vector<std::vector<char>>& map, sf::Vector2i size);
    room_builder (std::vector<std::vector<char>>* map, sf::Vector2i size);

    std::vector<architector*>* step ();
    void spawn (sf::Vector2i pos = {rand(), rand()}, int dir = rand());
};