#include "rocket.h"

// --- Comment

// --- Constructors
Rocket::Rocket()
{
    right = false;
    left = false;
    up = false;
    down = false;
    energy = 100;
    setColor(8);
    setPositionY(650);
    setTopBorder(500);
    setBottomBorder(700);
    Entity();
}

Rocket::Rocket(int color)
{
    right = false;
    left = false;
    up = false;
    down = false;
    energy = 100;
    setColor(color);
    setPositionX(250);
    setPositionY(650);
    setTopBorder(500);
    setBottomBorder(700);
    Entity();
}

// --- Functions
int Rocket::getSpeed()
{
    if(energy > 50)
        return 3;
    if(energy > 20)
        return 2;
    return 1;
}

void Rocket::regenerateEnergy()
{
    if(energy != 100)
        energy++;
    if(isDead())
        energy = 0;
}

void Rocket::takeDamage(int damage)
{
    energy -= damage;
    if(energy < 0)
        energy = 0;
}

void Rocket::resetRocket()
{
    energy = 100;
    setPositionY(650);
    if(getEntityID() == 0)
        setPositionX(150);
    else
        setPositionX(300);
    revive();
}
