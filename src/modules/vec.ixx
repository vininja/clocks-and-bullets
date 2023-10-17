export module vec;

import <corecrt_math.h>;

export struct vec2f {
    vec2f() {};

    vec2f(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void rotate(float r) {
        project();
        *this = vec2f(x * cosf(r) - y * sinf(r), x * sinf(r) + y * cosf(r));
        project();
    }

    inline void translate(vec2f t) {
        x += t.x;
        y += t.y;
    }

    inline void transform(vec2f t, float r) {
        rotate(r);
        translate(t);
    }
    inline void normalize() {
        float mag = sqrtf(x * x + y * y);
        x /= mag;
        y /= mag;
    }

    inline void project() {
        this->y *= -1;
    }

    auto operator*(float rhs) const -> vec2f {
        return vec2f(x * rhs, y * rhs);
    }

    vec2f operator*=(float rhs) {
        x *= rhs; y *= rhs;
        return *this;
    }

    vec2f operator+(vec2f rhs) {
        x += rhs.x; y += rhs.y;
        return *this;
    }
    
    bool operator==(vec2f& rhs)
    {
        return (x == rhs.x) && (y == rhs.y);
    }

    float x, y;
};