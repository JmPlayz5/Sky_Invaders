#ifndef ROCKET_H
#define ROCKET_H

#include <QRect>
#include <QPainter>
#include <entity.h>
#include <enemy.h>

// --- Comment

class Rocket : public Entity
{
public:
    // --- Constructors
    Rocket();
    Rocket(int color);

    // --- Variables
    // --- Int
    int energy;
    int speed;
    int color;

    // --- Double
    double points;

    // --- Bool
    bool right;
    bool left;
    bool up;
    bool down;

    // --- Functions
    // --- Int
    int getEnergy(){return energy;}
    int getSpeed();
    int getColor(){return color;}

    // --- Double
    double getPoints(){return points;}

    // --- Bool
    bool getRight(){return right;}
    bool getLeft(){return left;}
    bool getUp(){return up;}
    bool getDown(){return down;}

    // --- Void
    void setRight(bool right){this->right = right;}
    void setLeft(bool left){this->left = left;}
    void setUp(bool up){this->up = up;}
    void setDown(bool down){this->down = down;}
    void setEnergy(int energy){this->energy = energy;}
    void regenerateEnergy();
    void takeDamage(int damage);
    void addPoints(double points){this->points += points;}
    void setPoints(double points){this->points = points;}
    void multiplyPoints(double multiplier){points *= multiplier;}
    void setColor(int color){this->color = color;}
    void addColor(){color++;}
    void resetPoints(){points = 0;}
    void resetRocket();
};
#endif // ROCKET_H
