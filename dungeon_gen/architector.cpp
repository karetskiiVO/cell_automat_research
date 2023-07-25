#include "architector.h"

bool architector::alive () {
    return _alive;
}

void architector::kill () {
    _alive = false;
}

int architector::rev_dir (int dir) {
    return (dir + 2) % 4;
}

bool architector::probability_random (double probability) {
    return (1.0 * std::abs(rand()) < RAND_MAX * probability);
}

architector::~architector () {

}

void architector::set_new_architectors (std::vector<architector*>*& res, sf::Vector2i pos, int dir, std::vector<double> probabilities) {
    double sum = 0;
    for (int i = 0; i < NUM_OF_ARCHITECTORS; i++) {
        sum += probabilities[i];
    }

    for (int i = 0; i < NUM_OF_ARCHITECTORS; i++) {
        probabilities[i] /= sum;

        if (i) probabilities[i] += probabilities[i - 1];
    }

    double prob = (std::abs(rand()) * 1.0) / (RAND_MAX * 1.0);

    if (prob < probabilities[0]) {
        if (!res) res = new std::vector<architector*> ();

        architector* buf_architector = new s_hall_builder(map, size);
        buf_architector->spawn(pos, dir);
        res->push_back(buf_architector);
        return;
    }

    if (prob < probabilities[1]) {
        if (!res) res = new std::vector<architector*> ();

        architector* buf_architector = new room_builder(map, size);
        buf_architector->spawn(pos, dir);

        res->push_back(buf_architector);
        return;
    }


    if (!res) res = new std::vector<architector*> ();

    architector* buf_architector = new room_builder(map, size);
    buf_architector->spawn(pos, dir);

    res->push_back(buf_architector);
    return;
}