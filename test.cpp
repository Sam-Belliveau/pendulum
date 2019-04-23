#include <SFML/Graphics.hpp>

#include "Pendulum.hpp"

static constexpr int WIDTH = 600;
static constexpr int HEIGHT = 600;

static constexpr int PEN_COUNT = 256;
static constexpr int PEN_MULT = 64;

using UInt8 = unsigned char;

sf::Color hueToColor(UInt8 hue) 
{
    sf::Color out(0,0,0);
    const UInt8 rem = (hue % 43) * 6;

    const UInt8 q = (255 * (0xff ^ ((255 * rem) >> 8))) >> 8;
    const UInt8 t = (255 * (0xff ^ ((255 * (0xff ^ rem)) >> 8))) >> 8;

    switch (hue / 43)
    {
        case 0:  out.r = 255; out.g = t; out.b = 0; break;
        case 1:  out.r = q; out.g = 255; out.b = 0; break;
        case 2:  out.r = 0; out.g = 255; out.b = t; break;
        case 3:  out.r = 0; out.g = q; out.b = 255; break;
        case 4:  out.r = t; out.g = 0; out.b = 255; break;
        default: out.r = 255; out.g = 0; out.b = q; break;
    }

    return out;
}

void reset(Pendulum* pen)
{
    for(int i = 0; i < PEN_COUNT * PEN_MULT; ++i)
    {
        pen[i].setAngle(1);
        pen[i].setVel(0);
        pen[i].len = FLOAT(i)/FLOAT(PEN_MULT)+1.0;
    }
}

int main()
{
    Pendulum pen[PEN_COUNT * PEN_MULT];
    reset(pen);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;

    sf::RenderWindow app(sf::VideoMode(WIDTH, HEIGHT), "Pendulum", 7, settings);
    app.setFramerateLimit(60);
    
    while(app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event))
            if (event.type == sf::Event::Closed) app.close();

        app.clear();

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        { reset(pen); }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        { app.setFramerateLimit(0); }
        else { app.setFramerateLimit(60); }

        sf::VertexArray lines(sf::Points, PEN_COUNT*PEN_MULT);

        FLOAT STICK_LENGTH = 0;

        const bool LEFT = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        const bool RIGHT = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

        for(int i = 0; i < PEN_COUNT*PEN_MULT; ++i)
        {
            lines[i].position = sf::Vector2f(WIDTH/2.f, HEIGHT/2.f);
            lines[i].position.x -= STICK_LENGTH * std::sin(pen[i].angle);
            lines[i].position.y -= STICK_LENGTH * std::cos(pen[i].angle);
            lines[i].color = hueToColor(i*256/(PEN_COUNT*PEN_MULT));
            lines[i].color.a = 64;

            if(LEFT) pen[i].addVel(-0.03125);
            if(RIGHT) pen[i].addVel(0.03125);
                
            pen[i].applyGravity();
            STICK_LENGTH += 1.0 / PEN_MULT;
        }

        app.draw(lines);
        app.display();
    }
}