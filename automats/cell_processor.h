#pragma once

#include "field.h"

#include <string>

void antLR_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed);
void antLRL_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed);
void LR4_uniersal_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed);

class LR4_universal;
class LR8_universal;

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

class LR8_universal_online {
private:
    std::vector<LR8_universal*> automats;
    field* fld;
public:
    LR8_universal_online () = delete;

    LR8_universal_online (field& fld, std::vector<const char*> instr);
    void start ();
    void restart ();
    void step ();
};

class fractal_draw {
private:
    size_t num;
    size_t curr_size;
    field* fld;
    sf::Vector2u pos;
    bool diag_mode;
    bool step_by_steps;

public:
    fractal_draw () = delete;
    fractal_draw (field& fld, bool diag, bool step_by_step);

    void start ();
    void restart ();
    void spawn ();
    void spawn (sf::Vector2u pos);
    void step ();

private:
    bool isaround (sf::Vector2u curpos);
};

