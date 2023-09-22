#ifndef ENTITY_H
#define ENTITY_H


#include <QRect>
#include <QPoint>
#include <stdio.h>
#include <iostream>
#include <projectile.h>

// --- Comment

class Entity
{
public:
    // --- Constructors
    Entity();

private:
    // --- Variables
    // --- Int
    int positionX;
    int positionY;
    int topBorder;
    int leftBorder;
    int rightBorder;
    int bottomBorder;
    int entityID;

    // --- Bool
    bool dead;
    bool shooting;

    // --- Other classes
    QRect hitbox;

public:
    // --- Functions
    // --- Int
    int getPositionX(){return positionX;}
    int getPositionY(){return positionY;}
    int getTopBorder(){return topBorder;}
    int getLeftBorder(){return leftBorder;}
    int getRightBorder(){return rightBorder;}
    int getBottomBorder(){return bottomBorder;}
    int getEntityID(){return entityID;}

    // --- Bool
    bool isDead(){return dead;}
    bool isShooting(){return shooting;}
    bool checkForCollision(QPoint enemyProjectile);
    bool isOutOfBounds(bool isAlien);

    // --- Void
    void setPositionX(int positionX){this->positionX = positionX;}
    void setPositionY(int positionY){this->positionY = positionY;}
    void modifyPositionX(int offsetX){positionX += offsetX;}
    void modifyPositionY(int offsetY){positionY += offsetY;}
    void setHitbox(QRect hitbox){this->hitbox = hitbox;}
    void die(){dead = true;}
    void revive(){dead = false;}
    void shoot(){shooting = true;}
    void stopShooting(){shooting = false;}
    void setEntityID(int entityID){this->entityID = entityID;}
    void emptyHitbox();
    void getShooterHitbox(bool isAlien);
    void setBottomBorder(int bottomBorder){this->bottomBorder = bottomBorder;}
    void setTopBorder(int topBorder){this->topBorder = topBorder;}

    // --- Other classes
    QRect getHitbox(){return hitbox;}

private:
    // --- Functions
    void pullFromOutOfBounds(bool isAlien);
};

#endif // ENTITY_H
