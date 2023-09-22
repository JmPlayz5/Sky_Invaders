#ifndef ENEMY_H
#define ENEMY_H

#include <QRect>
#include <QLine>
#include <entity.h>
#include <projectile.h>

// --- Comment

// --- Enum
enum Size
{
    Small,
    Medium,
    Large
};

class Enemy : public Entity
{
public:
    // --- Constructors
    Enemy(int entityID, int posX, int posY);
    Enemy();

private:

    // --- Variables
    // --- Int
    int cooldownTimer;

    // --- Bool
    bool directionL2R;
    bool isSideAlien;
    bool canShoot;

    // --- Other classes
    QRect areaControl;
    Size size;

public:
    // --- Functions
    // --- Int
    int moveX();
    int moveX(int pixels);

    // --- Bool
    bool isSide(){return isSideAlien;} // --- Returns true if enemy is the farthest left or right
    bool getCanShoot(){return canShoot;}
    bool isOutOfGame();

    // --- Void
    void moveY();
    void setCanShoot(bool canShoot){this->canShoot = canShoot;}
    void setAreaControl(QRect areaControl){this->areaControl = areaControl;}
    void setSize(Size size){this->size = size;}

    // --- Other classes
    QRect generateHitboxS();
    QRect generateHitboxM();
    QRect generateHitboxL();
    QRect getAreaControl(){return areaControl;}
    Size getSize(){return size;}


};

#endif // ENEMY_H
