import <random>;
import constants;
import vec;

class Body {

public:
    Body() = default;
    virtual ~Body() = default;

    void inline step(float dt) { m_c = m_c + m_dir * m_speed * dt; }

    auto inline left() const -> float { return m_c.x - m_hw; }
    auto inline right() const -> float { return m_c.x + m_hw; }
    auto inline top() const -> float { return m_c.y + m_hw; }
    auto inline bottom() const -> float { return m_c.y - m_hw; }

    vec2f m_c;
    vec2f m_dir;
    float m_hw;
    float m_speed;
};


class Clock final : public Body {

public:
    Clock() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0, 1.0);

        vec2f dir1(1, 1);
        dir1.transform(vec2f(0, 0), dis(gen) * 2.f * math::pi);

        m_dir = dir1;
        m_c = vec2f((dis(gen) + 1) * sim::screen_w / 2.5f, (dis(gen) + 1) * sim::screen_h / 2.5f);
        m_speed = (dis(gen) + 1 )* sim::proj_speed/10;
        m_hw = sim::max_clk_size;
    }

    void split_clock(std::vector<Clock>& clocks, auto pos) {
        Clock c1, c2;
        c1.m_hw = c2.m_hw = this->m_hw /= 2;
        c1.m_c = this->m_c + c1.m_dir * c1.m_hw;
        c2.m_c = this->m_c + c2.m_dir * c2.m_hw;
        clocks.erase(std::next(std::begin(clocks), pos));
        clocks.push_back(c1);
        clocks.push_back(c2);
    }
};


class Projectile final : public Body {

public:
    Projectile(vec2f c, vec2f dir, const int hw, const int speed) {
        m_c = c;
        m_dir = dir;
        m_hw = static_cast<float>(hw);
        m_speed = static_cast<float>(speed);
    }
};


static auto bodies_overlap(Body& b1, Body& b2) -> bool {
    return (std::max(b1.left(), b2.left()) < std::min(b1.right(), b2.right())) &&
        (std::max(b1.bottom(), b2.bottom()) < std::min(b1.top(), b2.top()));
};