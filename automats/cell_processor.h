#pragma once

#include "field.h"

void antLR_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed);
void antLRL_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed);
void LR4_uniersal_processor (sf::Vector2u size, std::vector<std::vector<field_type>>& map, int seed);
