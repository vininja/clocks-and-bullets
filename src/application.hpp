#pragma once

#include "body.hpp"

import constants;
import time;
import graphics;

namespace ranges = std::ranges;

class Application {

public:

    typedef unsigned int t_pressed_key;

    Application(float screen_width, float screen_height);
    ~Application() = default;

    void tick(t_pressed_key& pressed_keys);
    void process_keys(t_pressed_key& pressed_keys);
    void reset_if_empty(std::vector<Clock>& m_clocks);
    void insert_projectile();
    void solve_collisions();

    static const t_pressed_key s_key_left  = 0x01;
    static const t_pressed_key s_key_up    = 0x02;
    static const t_pressed_key s_key_right = 0x04;
    static const t_pressed_key s_key_down  = 0x08;
    static const t_pressed_key s_key_space = 0x10;

private:
    
    float m_cannon_x;
    float m_cannon_y;
    float m_cannon_tilt;
    float m_screenw;
    float m_screenh;

    std::vector<Clock> m_clocks;
    std::vector<Projectile> m_projs;
    t_time m_time;
};
