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
