#pragma once

#define hall 1
#define room 2
#define door 3
#define wall 4

#define NUM_OF_ARCHITECTORS 2

#include <vector>
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>

class architector {

    friend static void set_new_architectors ();

protected:
    std::vector<std::vector<char>>* map;
    sf::Vector2i size;

    sf::Vector2i pos;
    bool _alive;

    const sf::Vector2i arr_dir[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
public:
    virtual ~architector ();

    void kill ();
    bool alive ();

    virtual void spawn (sf::Vector2i pos = {rand(), rand()}, int dir = rand()) = 0;
    virtual std::vector<architector*>* step () = 0;

protected:
    int rev_dir (int dir);
    bool probability_random (double probability);
    void set_new_architectors (std::vector<architector*>*& res, sf::Vector2i pos, int dir, std::vector<double> probabilities = {0.5, 0.5});
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