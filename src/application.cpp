#include "application.hpp"

Application::Application(float screen_width, float screen_height) {
    m_screenw = screen_width;
    m_screenh = screen_height;
    m_cannon_x = screen_width * .5f;
    m_cannon_y = screen_height * .9f;
    m_cannon_tilt = 0.f;
}

void Application::reset_if_empty(std::vector<Clock>& m_clocks) {
    if (!m_clocks.empty())
        return;
    Clock c1, c2;
    while (bodies_overlap(c1 = Clock(), c2 = Clock()));
    m_clocks.push_back(c1);
    m_clocks.push_back(c2);
}

void Application::insert_projectile() {
    vec2f head(0, -sim::cannon_h);
    head.transform(vec2f(m_cannon_x, m_cannon_y), m_cannon_tilt);
    vec2f dir(head.x - m_cannon_x, head.y - m_cannon_y);
    dir.normalize();
    m_projs.emplace_back(head, dir, sim::proj_size, sim::proj_speed);
}

void Application::process_keys(t_pressed_key& pressed_keys) {
    if (pressed_keys & s_key_left)
        m_cannon_tilt = std::clamp(m_cannon_tilt += sim::tilt_amount, -math::pi / 2, math::pi / 2);
    if (pressed_keys & s_key_right)
        m_cannon_tilt = std::clamp(m_cannon_tilt -= sim::tilt_amount, -math::pi / 2, math::pi / 2);
    if(pressed_keys & s_key_space)
        insert_projectile();
}
 
void Application::solve_collisions() {
    auto out = [&](auto it){ return it->m_c.x < 0 || it->m_c.x > m_screenw || it->m_c.y < 0 || it->m_c.y > m_screenh; };
    auto intersect_verticaly = [&](auto it){ return (it->bottom() < 0) || (it->top() > m_screenh); };
    auto intersect_horizontaly = [&](auto it){ return (it->left() < 0) || (it->right() > m_screenw); };

    for (auto iprj = m_projs.begin(); iprj != m_projs.end(); ) {
        for (auto iclk = m_clocks.begin(); iclk != m_clocks.end(); ++iclk) {
            if (bodies_overlap(*iprj, *iclk)) {
                if (iclk->m_hw < sim::min_clk_size) 
                    m_clocks.erase(iclk);
                else 
                    iclk->split_clock(m_clocks, iclk - m_clocks.begin());
                break;
            }
        }

        if (out(iprj))
            iprj = m_projs.erase(iprj);
        else 
            ++iprj;
    }

    for (auto ita = m_clocks.begin(); ita != m_clocks.end(); ) {
        if (out(ita))
            ita = m_clocks.erase(ita);
        else {
            if (intersect_verticaly(ita)) 
                ita->m_dir.y *= -1;
            else if (intersect_horizontaly(ita)) 
                ita->m_dir.x *= -1;
            else {
                for (auto itb = m_clocks.begin(); itb != m_clocks.end(); ++itb) {
                    if (ita != itb && bodies_overlap(*ita, *itb)) {
                        ita->m_dir *= -1;
                        itb->m_dir *= -1;
                        while (bodies_overlap(*ita, *itb)) {
                            ita->step(sim::step);
                            itb->step(sim::step);
                        }   
                    }
                }
            }
            ita++;
        }  
    }
}

void Application::tick(t_pressed_key& pressed_keys) {
    auto simulate = [](auto& b) { b.step(sim::step); };
    auto draw_clocks = [&](auto& c) { draw_clock(c, m_time); };
    auto draw_projs = [](auto& p) { draw_proj(p); };
    
    get_time(m_time);
    reset_if_empty(m_clocks);
    process_keys(pressed_keys);
    solve_collisions();

    ranges::for_each(m_clocks, simulate);
    ranges::for_each(m_projs, simulate);

    clear_screen();
    ranges::for_each(m_clocks, draw_clocks);
    ranges::for_each(m_projs, draw_projs);
    draw_cannon(m_cannon_x, m_cannon_y, m_cannon_tilt);
}