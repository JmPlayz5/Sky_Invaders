#include "enemy.h"
#include "widget.h"

// --- Comment

// --- Constructors
Enemy::Enemy(int entityID, int posX, int posY)
{
    setEntityID(entityID);
    setPositionX(36 + posX * 40);
    setPositionY(36 + posY * 40);
    directionL2R = true;
    cooldownTimer = 10;
    if(entityID >= (ENEMIES_PER_ROW * (ENEMIES_PER_COLUMN - 1)))
        canShoot = true;
    else
        canShoot = false;
    Entity();
}
Enemy::Enemy()
{
    Entity();
}

// --- Functions
QRect Enemy::generateHitboxS() // --- Generates enemy hitbox
{
    setHitbox(QRect(getPositionX() + 4, getPositionY(), 16, 16));
    setSize(Small);
    return getHitbox();
}

QRect Enemy::generateHitboxM() // --- Generates enemy hitbox
{
    setHitbox(QRect(getPositionX() + 2, getPositionY(), 20, 20));
    setSize(Medium);
    return getHitbox();
}

QRect Enemy::generateHitboxL() // --- Generates enemy hitbox
{
    setHitbox(QRect(getPositionX(), getPositionY(), 24, 24));
    setSize(Large);
    return getHitbox();
}

int Enemy::moveX() // --- Moves enemy horizontally
{
    if(directionL2R)
        modifyPositionX(4);
    else
        modifyPositionX(-4);
    if(isOutOfBounds(true))
    {
        isSideAlien = true;
        return 1;
    }
    isSideAlien = false;
    return 0;
}

int Enemy::moveX(int pixels)
{
    if(directionL2R)
        modifyPositionX(pixels);
    else
        modifyPositionX(-pixels);
    if(isOutOfBounds(true))
    {
        isSideAlien = true;
        return 1;
    }
    isSideAlien = false;
    return 0;
}

void Enemy::moveY() // --- Moves enemy vertically
{
    directionL2R =! directionL2R;
    modifyPositionY(36);
}

bool Enemy::isOutOfGame()
{
    if(getPositionY() > getBottomBorder())
        return true;
    return false;
}
