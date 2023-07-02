#pragma once

#include "field.h"

#include <string>

void antLR_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed);
void antLRL_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed);
void LR4_uniersal_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed);

class LR4_universal;

class LR4_universal_online {
private:
    std::vector<LR4_universal*> automats;
    field* fld;
public:
    LR4_universal_online () = delete;

    LR4_universal_online (field& fld, std::vector<const char*> instr);
    void start ();
    void restart ();
    void step ();
};

