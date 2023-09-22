#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QRect>
#include <QLine>

// --- Comment

class Projectile
{
public:
    // --- Constructors
    Projectile(QRect shooterHitbox);
    Projectile();

private:
    // --- Variables
    // --- Int
    int projectileFrameCounter;
    int projectileID;

    // --- Bool
    bool hasShot;

    // --- Other classes
    QPoint topProjectile;
    QPoint botProjectile;
    QRect shooterHitbox;

public:
    // --- Functions
    // --- Int
    int getProjectileFrameCounter(){return projectileFrameCounter;}
    int getProjectileID(){return projectileID;}

    // --- Bool
    bool getHasShot(){return hasShot;}
    bool isOutOfFrame(bool isAlien);

    // --- Void
    void setHasShot(bool hasShot){this->hasShot = hasShot;}
    void setProjectileFrameCounter(int projectileFrameCounter){this->projectileFrameCounter = projectileFrameCounter;}
    void setShooterHitbox(QRect shooterHitbox){this->shooterHitbox = shooterHitbox;}
    void addProjectileFrameCounter(){projectileFrameCounter++;}
    void emptyProjectile();
    void setProjectileID(int projectileID){this->projectileID = projectileID;}

    // --- Other classes
    QPoint getTopProjectile(){return topProjectile;}
    QPoint getBotProjectile(){return botProjectile;}
    QRect getShooterHitbox(){return shooterHitbox;}
    QRect fire(bool isAlien, bool isPaused, int projectileSpeed);
};

#endif // PROJECTILE_H
