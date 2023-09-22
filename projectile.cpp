#include "projectile.h"
#include <cstdlib>

// --- Comment

// --- Constructors
Projectile::Projectile(QRect shooterHitbox)
{
    projectileFrameCounter = 0;
    this->shooterHitbox = shooterHitbox;
    hasShot = false;
}

Projectile::Projectile()
{
    projectileFrameCounter = 0;
    hasShot = false;
}

// --- Functions
QRect Projectile::fire(bool isAlien, bool isPaused, int projectileSpeed) // --- Projectile in motion
{
    if(!isAlien)
    {
        botProjectile = QPoint(shooterHitbox.left() + (shooterHitbox.width() / 2), shooterHitbox.top() - (projectileSpeed * (projectileFrameCounter)));
        // --- QPoint of the lowest pixel of projectile
        // --- (6 * projectileFramecounter) indicates speed of bullet
        // --- The higher the number, the faster it does and vice versa
        topProjectile = QPoint(shooterHitbox.left() + (shooterHitbox.width() / 2), shooterHitbox.top() - 6 - (projectileSpeed * (projectileFrameCounter)));
        // --- QPoint of the highest pixel of projectile
        // --- projectileRect.top() - 8 indicates length of bullet
        // --- The higher the number, the longer it is and vice versa
    }
    else
    {
        botProjectile = QPoint(shooterHitbox.left() + (shooterHitbox.width() / 2), shooterHitbox.bottom() - 8 + (projectileSpeed * (projectileFrameCounter)));
        topProjectile = QPoint(shooterHitbox.left() + (shooterHitbox.width() / 2), shooterHitbox.bottom() + (projectileSpeed * (projectileFrameCounter)));
    }

    if(!isPaused)
        projectileFrameCounter++;

    return QRect(topProjectile.x(), topProjectile.y(), 2, 8);
}

void Projectile::emptyProjectile() // --- Gets rid of the projectile
{
    botProjectile = QPoint();
    topProjectile = QPoint();
    projectileFrameCounter = 0;
}

bool Projectile::isOutOfFrame(bool isAlien)
{
    if(!isAlien)
    {
        if(topProjectile.y() < 20) // --- Indicates the duration of projectile
        // --- The higher the number, the longer it lives and vice versa
        {
            projectileFrameCounter = 0;  // --- The projectile frame count resets on projectile death
                // ### Instead of projectile lifetime, check if projectile is outside the border
            return true;
        }
    }
    else
        if(topProjectile.y() > 720)
        {
            projectileFrameCounter = 0;
            return true;
        }
    return false;
}
