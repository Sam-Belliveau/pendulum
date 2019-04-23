#include <cmath>

using FLOAT = double;

static constexpr FLOAT PI = 3.1415926535897932384;
static constexpr FLOAT HALF_PI = PI * 0.5;
static constexpr FLOAT TWO_PI = PI * 2;

static constexpr FLOAT GRAVITY = PI/24.0;
static constexpr FLOAT RESISTANCE = PI/3000.0;

struct Pendulum
{
    FLOAT angle = 0, vel = 0;
    FLOAT len = 1;

    void reset() { angle = 0; vel = 0; }

    void setAngle(FLOAT in) 
    { angle = in * PI / 180.0; }

    void setVel(FLOAT in) 
    { vel = in * PI / 180.0; }

    void addVel(FLOAT in) 
    { vel += in * PI / 180.0; }

    void loop()
    {
        while(angle > PI)  angle -= TWO_PI;
        while(angle < -PI) angle += TWO_PI;
    }

    void applyGravity(FLOAT gravity = GRAVITY) 
    {
        loop();
        vel += GRAVITY * std::sin(angle) / len;
        vel *= 1.0 - RESISTANCE;
        angle += vel;
    }
};