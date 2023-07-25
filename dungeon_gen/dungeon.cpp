#include "dungeon.h"

dungeon::dungeon (unsigned int size) {
    field_pic.create(size, size);
    done = false;

    _size = {(int)size, (int)size};

    map.resize(size);
    for (unsigned int i = 0; i < size; i++) {
        map[i].resize(size);
    }

    for (int x = 0; x < this->_size.x; x++) {
        for (int y = 0; y < this->_size.y; y++) {
            map[x][y] = 0;
        }
    }

    builders.clear();
    builders.push_back(new s_hall_builder(map, _size));
    builders[0]->spawn(_size / 2);
    map[_size.x / 2][_size.y / 2] = 1;
}

dungeon::dungeon (unsigned int x_size, unsigned int y_size) {
    field_pic.create(x_size, y_size);
    done = false;

    _size = {(int)x_size, (int)y_size};
    map.resize(x_size);
    for (unsigned int x = 0; x < x_size; x++) {
        map[x].resize(y_size);
    }

    for (int x = 0; x < this->_size.x; x++) {
        for (int y = 0; y < this->_size.y; y++) {
            map[x][y] = 0;
        }
    }

    builders.clear();
    builders.push_back(new room_builder(map, _size));
    builders[0]->spawn(_size / 2);
    map[_size.x / 2][_size.y / 2] = 1;
}

std::vector<field_type>& dungeon::operator[] (unsigned int idx) {
    return this->map[idx];
}

void dungeon::render (sf::RenderWindow& window) {
    if (window.getSize() != field_pic_buf.getSize()) {
        field_pic_buf = field_pic;
        upscale(field_pic_buf, window.getSize());
    }

    sf::Sprite render_buf;
    sf::Texture buf_texture;
    buf_texture.loadFromImage(field_pic_buf);
    render_buf.setTexture(buf_texture);
    window.draw(render_buf);
}

void dungeon::draw () {
    for (unsigned int x = 0; x < (unsigned int)_size.x; x++) {
        for (unsigned int y = 0; y < (unsigned int)_size.y; y++) {
            field_pic.setPixel(x, y, convert(map[x][y]));
        }
    }

    field_pic_buf = field_pic;
}

void dungeon::restart () {
    done = false;

    for (unsigned int x = 0; x < this->_size.x; x++) {
        for (unsigned int y = 0; y < this->_size.y; y++) {
            map[x][y] = 0;
        }
    }

    builders.clear();
    builders.push_back(new s_hall_builder(map, _size));
    builders[0]->spawn(_size / 2);
    map[_size.x / 2][_size.y / 2] = 1;
}

void dungeon::process () {
    if (done) return;

    std::vector<architector*> builders_accum;
    std::vector<architector*>* buf = nullptr;

    for (int i = 0; i < builders.size(); i++) {
        buf = builders[i]->step();
        if (!buf) continue;

        builders_accum.insert(builders_accum.end(), (*buf).begin(), (*buf).end());
        delete buf;
    }

    // delete dead
    int dead_workers = 0;
    for (int i = 0; i < builders.size(); i++) {
        if (!builders[i]->alive()) {
            dead_workers++;
        } else if (dead_workers) {
            std::swap(builders[i - dead_workers], builders[i]);
        }
    }
    builders.resize(builders.size() - dead_workers);

    if (builders_accum.size() > 0) 
        builders.insert(builders.end(), builders_accum.begin(), builders_accum.end());

    if (builders.size() == 0) {
        done = true;
    }

    draw();
}

sf::Color dungeon::convert(field_type cell) {
    const sf::Color arr[] = {sf::Color::Black, sf::Color::Blue, sf::Color::Yellow, sf::Color::Red, sf::Color::Green};

    return arr[cell];
}

void dungeon::upscale (sf::Image& img, sf::Vector2u newres) {
    unsigned int scale = std::min(newres.x / img.getSize().x, newres.y / img.getSize().y);

    if (!scale) return;

    unsigned int cur_x = (newres.x - scale * img.getSize().x) / 2;
    unsigned int cur_y = (newres.y - scale * img.getSize().y) / 2;

    unsigned int start_y = cur_y;

    sf::Image buf(img);
    img.create(newres.x, newres.y, sf::Color(0x49423d));

    for (unsigned int x = 0; x < buf.getSize().x; x++) {
        for (unsigned int y = 0; y < buf.getSize().y; y++) {
            sf::Color buf_color = buf.getPixel(x, y);

            for (unsigned int dx = 0; dx < scale; dx++) {
                for (unsigned int  dy = 0; dy < scale; dy++) {
                    img.setPixel(cur_x + dx, cur_y + dy, buf_color);
                }
            }

            cur_y += scale;
        }

        cur_y = start_y;
        cur_x += scale;
    }
}

static bool on_field (sf::Vector2i pos, sf::Vector2i field_size) {
    return !(pos.x < 0 || pos.x >= field_size.x ||
             pos.y < 0 || pos.y >= field_size.y);
}

int dungeon::hall_caherency (sf::Vector2i pos) {
    int res = 0;

    for (int i = 0; i < 4; i++) {
        sf::Vector2i bufpos = pos + arr_dir[i];
        if (!on_field(bufpos, _size)) continue;

        switch (map[bufpos.x][bufpos.y]) {
            case door:
            case hall:
                res++;
                break;
            default:
                break;
        }
    }

    return res;
}

void dungeon::post_processing () {
    if (!done) return;

    for (int x = 0; x < _size.x; x++) {
        for (int y = 0; y < _size.y; y++) {
            switch (map[x][y]) {
                case hall:
                    if (hall_caherency({x, y}) < 2)
                        hall_dtr({x, y});
                    break;
            }
        }
    }

    for (int x = 0; x < _size.x; x++) {
        for (int y = 0; y < _size.y; y++) {
            switch (map[x][y]) {
                case door:
                    door_dtr({x, y});
                break;
            }
        }
    }
    draw();
}

void dungeon::hall_dtr (sf::Vector2i pos) {
    if (!on_field(pos, _size))     return;
    if (map[pos.x][pos.y] != hall) return;
    if (hall_caherency(pos) > 1)   return;

    map[pos.x][pos.y] = 0;
    for (int i = 0; i < 4; i++) {
        hall_dtr(pos + arr_dir[i]);
    }
}

static bool noonord_eq_pair (sf::Vector2i& fst, sf::Vector2i& snd) {
    return fst == snd || sf::Vector2i(fst.y, fst.x) == snd;
}

static bool pair_in_pairs (sf::Vector2i& pair, std::vector<sf::Vector2i> pairs) {
    for (int i = 0; i < pairs.size(); i++)
        if (noonord_eq_pair(pair, pairs[i])) 
            return true;

    return false;
}

void dungeon::door_dtr (sf::Vector2i pos) {
    for (int i = 0; i < 4; i++) {
        sf::Vector2i buf_pos = pos + arr_dir[i];

        if (!on_field(buf_pos, _size)) {
            map[pos.x][pos.y] = wall;
            return;
        }
    }

    const std::vector<sf::Vector2i> acceptable = {sf::Vector2i(wall, wall), sf::Vector2i(room, door), sf::Vector2i(room, hall), sf::Vector2i(door, wall)};

    for (int i = 0; i < 4; i++) {
        sf::Vector2i front_pos = pos + arr_dir[i];
        sf::Vector2i back_pos  = pos - arr_dir[i];
        sf::Vector2i types(map[front_pos.x][front_pos.y], map[back_pos.x][back_pos.y]);
        if (!pair_in_pairs(types, acceptable)) {
            map[pos.x][pos.y] = wall;
            return;
        }        
    }
}

int dungeon::rev_dir (int dir) {
    return (dir + 2) % 4;
}
