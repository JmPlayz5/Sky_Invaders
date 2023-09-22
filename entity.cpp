#include "entity.h"

// --- Comment

// --- Constructors
Entity::Entity()
{
    topBorder = 20;
    leftBorder = 20;
    rightBorder = 452;
    bottomBorder = 715;
    positionX = 36;
    positionY = 36;
    dead = false;
    shooting = false;
}

// --- Functions
void Entity::pullFromOutOfBounds(bool isAlien) // --- Pulls entities from out of bounds
{
    if(!isAlien)
    {
        if(positionX < leftBorder)
            positionX = leftBorder;
        else if(positionX > rightBorder)
            positionX = rightBorder;
        if(positionY < topBorder)
            positionY = topBorder;
        else if(positionY > bottomBorder)
            positionY = bottomBorder;
        isOutOfBounds(false);
    }
    else
    {
        if(positionX < leftBorder)
            positionX = leftBorder;
        else if(positionX > rightBorder)
            positionX = rightBorder;
        if(positionY < topBorder)
            positionY = topBorder;
        isOutOfBounds(true);
    }
}

bool Entity::isOutOfBounds(bool isAlien) // --- Checks if entity is out of bounds
{
    if(!isAlien)
    {
        if(positionX < leftBorder ||
            positionX > rightBorder ||
            positionY < topBorder ||
            positionY > bottomBorder)
        {
            pullFromOutOfBounds(false);
            return true;
        }
        return false;
    }
    else
    {
        if(positionX < leftBorder ||
            positionX > rightBorder ||
            positionY < topBorder)
        {
            pullFromOutOfBounds(true);
            return true;
        }
        return false;
    }
}

bool Entity::checkForCollision(QPoint enemyProjectile) // --- Checks if projectile has hit an entity
{
    if(hitbox.contains(enemyProjectile))
        return true;
    return false;
}

void Entity::emptyHitbox() // --- Gets rid of entity hitbox
{
    hitbox = QRect();
}
