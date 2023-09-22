#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QRectF>
#include <QPoint>
#include <QPolygon>
#include <QKeyEvent>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <QFontDatabase>

// --- Comment
// ### Suggestion
// !!! Bug / Needs Fixing
// $$$ For Debugging
// §§§ Important / Idea

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus); // --- Needed for keyboard input
    this->setFixedSize(QSize(500, 900)); // --- Window size is locked
    mainMenu();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mainMenu()
{
    inMainMenu = true;
    inLeaderboard = false;
    onStart();
    if(!inMainMenu)
        afterMainMenuInitialization();
}

void Widget::afterMainMenuInitialization()
{
    switch(difficulty)
    {
    case Easy:
        frames = 150;
        randomMultiplier = 4;
        projectileSpeed = 3;
        break;
    case Normal:
        frames = 100;
        randomMultiplier = 3;
        projectileSpeed = 4;
        break;
    case Hard:
        frames = 75;
        randomMultiplier = 2;
        projectileSpeed = 4;
        break;
    case Impossible:
        frames = 40;
        randomMultiplier = 1;
        projectileSpeed = 5;
        break;
    }
}

void Widget::onStart()
{
    // --- Initialization
    // --- Int
    frameCounter = 0; // --- Counts frames / Displays current frame
    frames = 150; // --- Used for modifying enemy speed
    deathCounter = 0; // --- Counts the number of enemy deaths
    atBorder = 0; // --- Checks if an enemy has reached the border while moving
    points = 0; // --- Used for displaying player point
    spriteCounterL = 2; // --- Used for displaying 2-Frame sprite animations of large enemies
    spriteCounterM = 4; // --- Used for displaying 2-Frame sprite animations of medium enemies
    spriteCounterS = 6; // --- Used for displaying 2-Frame sprite animations of small enemies
    playerLeaderboard = 0;
    difficultyLeaderboard = 0;

    // --- Double
    fps = 1000/60; // --- 60 Frames per Second

    // --- Bool
    pause = false; // --- Used for pausing the program
    is2Player = false; // --- Used for knowing if Player 2 has been enabled
    hasViewedEndGame = false; // --- Used for knowing if endGame() has been called
    gameOver = false;

    // --- Other classes
    difficulty = Easy; // --- Set difficulty to Easy as default

    QTimer::connect(&timer, SIGNAL(timeout()), this , SLOT(moveEnemies())); // --- calls moveEnemies() every timer cycle
    QTimer::connect(&timer, SIGNAL(timeout()), this , SLOT(moveRocket())); // --- calls moveShip() every timer cycle
    timer.start(fps); // --- Loops the functions at 60 FPS

    // --- Actions
    actions.insert(Qt::Key_5, ChangeDifficulty); // --- Memorizes "ChangeDifficulty" as an action
    actions.insert(Qt::Key_2, Player2); // --- Memorizes "Player2" as an action
    actions.insert(Qt::Key_1, StartGame); // --- Memorizes "StartGame" as an action
    actions.insert(Qt::Key_3, ChangeColor1); // --- Memorizes "ChangeColor1" as an action for Player 1
    actions.insert(Qt::Key_4, ChangeColor2); // --- Memorizes "ChangeColor2" as an action for Player 2
    actions.insert(Qt::Key_6, OpenLeaderboard);

    actions.insert(Qt::Key_1, TogglePlayerLeaderboard);
    actions.insert(Qt::Key_2, ToggleDifficultyLeaderboard);
    actions.insert(Qt::Key_Escape, LeaveLeaderboard);

    actions.insert(Qt::Key_A, MoveLeft); // --- Memorizes "MoveLeft" as an action for Player 1
    actions.insert(Qt::Key_D, MoveRight); // --- Memorizes "MoveRight" as an action for Player 1
    actions.insert(Qt::Key_W, MoveTop); // --- Memorizes "MoveTop" as an action for Player 1
    actions.insert(Qt::Key_S, MoveBot); // --- Memorizes "MoveBot" as an action for Player 1
    actions.insert(Qt::Key_Space, Shoot); // --- Memorizes "Shoot" as an action for Player 1

    actions.insert(Qt::Key_Left, MoveLeft2); // --- Memorizes "MoveLeft" as an action for Player 2
    actions.insert(Qt::Key_Right, MoveRight2); // --- Memorizes "MoveRight" as an action for Player 2
    actions.insert(Qt::Key_Up, MoveTop2); // --- Memorizes "MoveTop" as an action for Player 2
    actions.insert(Qt::Key_Down, MoveBot2); // --- Memorizes "MoveBot" as an action for Player 2
    actions.insert(Qt::Key_Period, Shoot2); // --- Memorizes "Shoot" as an action for Player 2

    actions.insert(Qt::Key_P, Pause); // --- Memorizes "Pause" as an action
    actions.insert(Qt::Key_N, NewGame); // --- Memorizes "NewGame" as an action
    actions.insert(Qt::Key_Escape, EnterMainMenu);
    actions.insert(Qt::Key_7, EnterControls);
    actions.insert(Qt::Key_Escape, LeaveControls);

    rocket.setEntityID(0); // --- Sets rocket's entityID
    if(is2Player)
        rocket2.setEntityID(1); // --- Sets rocket's entityID for Player 2
    rocket.setPositionX(150);
    rocket2.setPositionX(300);

    for(int j = 0; j < ENEMIES_PER_COLUMN; j++)
    {
        for(int i = 0; i < ENEMIES_PER_ROW; i++)
            enemies.push_back(new Enemy(i + (j * ENEMIES_PER_ROW), i, j)); // --- Sets enemy's entityID and initializes enemy position
    }

    // --- Initializes sprites
    sprites[0] = QImage(".\\sprites\\rocket.png");
    sprites[1] = QImage(".\\sprites\\projectile.png");
    sprites[2] = QImage(".\\sprites\\enemyL1.png");
    sprites[3] = QImage(".\\sprites\\enemyL2.png");
    sprites[4] = QImage(".\\sprites\\enemyM1.png");
    sprites[5] = QImage(".\\sprites\\enemyM2.png");
    sprites[6] = QImage(".\\sprites\\enemyS1.png");
    sprites[7] = QImage(".\\sprites\\enemyS2.png");
    sprites[8] = QImage(".\\sprites\\rocketRed.png");
    sprites[9] = QImage(".\\sprites\\rocketOrange.png");
    sprites[10] = QImage(".\\sprites\\rocketYellow.png");
    sprites[11] = QImage(".\\sprites\\rocketGreen.png");
    sprites[12] = QImage(".\\sprites\\rocketBlue.png");
    sprites[13] = QImage(".\\sprites\\projectileRed.png");
    sprites[14] = QImage(".\\sprites\\projectileOrange.png");
    sprites[15] = QImage(".\\sprites\\projectileYellow.png");
    sprites[16] = QImage(".\\sprites\\projectileGreen.png");
    sprites[17] = QImage(".\\sprites\\projectileBlue.png");

    srand(time(0)); // --- The seed for the pseudo-random number generator

    // §§§ Powerups:
    // §§§      Lightspeed - Boosts rocket movement
    // §§§      Quickshot - Boosts rocket projectile
    // §§§      Slow (Needs name) - Lowers enemy speed and projectile speed
    // §§§      Shuttle Cloak - Turns the rocket invisible for enemies, making them less likely to fire
    // §§§      Repairment - Fully heals the rocket
    // §§§ Different Rocket Types / Classes:
    // §§§      Tank - Has 2x energy but only moves at 2 speed
}

void Widget::moveEnemies()
{
    if(!pause && !inMainMenu) // --- Stops when paused
    {
        frameCounter++;

        /*  deathCounter / frames

            Easy:
            0 150   11 96   22 57   33 30   44 12
            1 145   12 92   23 54   34 28   45 11
            2 140   13 88   24 51   35 26   46 10
            3 135   14 84   25 48   36 24   47 9
            4 130   15 80   26 45   37 22   48 8
            5 125   16 76   27 42   38 20   49 7
            6 120   17 72   28 40   39 18   50 6
            7 115   18 69   29 38   40 16   51 5
            8 110   19 66   30 36   41 15   52 4
            9 105   20 63   31 34   42 14   53 3
            10 100  21 60   32 32   43 13   54 1

            if(deathCounter > 0 && deathCounter < 11)
                frames -= 5;
            else if(deathCounter >= 11 && deathCounter < 17)
                frames -= 4;
            else if(deathCounter >= 17 && deathCounter < 27)
                frames -= 3;
            else if(deathCounter >= 27 && deathCounter < 38)
                frames -= 2;
            else if(deathCounter >= 38 && deathCounter < 54)
                frames -= 1;
            else
                frames = 1;

            Medium:
            0 100   11 68   22 46   33 24   44 10
            1 97    12 66   23 44   34 22   45 9
            2 94    13 64   24 42   35 20   46 8
            3 91    14 62   25 40   36 18   47 7
            4 88    15 60   26 38   37 17   48 6
            5 85    16 58   27 36   38 16   49 5
            6 82    17 56   28 34   39 15   50 4
            7 79    18 54   29 32   40 14   51 3
            8 76    19 52   30 30   41 13   52 2
            9 73    20 50   31 28   42 12   53 1
            10 70   21 48   32 26   43 11   54 1

            if(deathCounter > 0 && deathCounter < 10)
                frames -= 3;
            else if(deathCounter >= 10 && deathCounter < 36)
                frames -= 2;
            else if(deathCounter >= 36 && deathCounter < 53)
                frames -= 1;
            else
                frames = 1;

            Hard:
            0 75    11 60   22 40   33 20   44 8
            1 75    12 55   23 40   34 20   45 8
            2 75    13 55   24 35   35 20   46 7
            3 70    14 55   25 35   36 15   47 7
            4 70    15 50   26 35   37 15   48 6
            5 70    16 50   27 30   38 15   49 5
            6 65    17 50   28 30   39 10   50 4
            7 65    18 45   29 30   40 10   51 3
            8 65    19 45   30 25   41 10   52 2
            9 60    20 45   31 25   42 9    53 1
            10 60   21 40   32 25   43 9    54 1

            if(deathCounter % 3 == 0 && deathCounter <= 41 && deathCounter > 0)
                frames -= 5;
            else if(deathCounter % 2 == 0 && deathCounter <= 47 && deathCounter > 42)
                frames -= 1;
            else if(deathCounter >= 47 && deathCounter < 53)
                frames -= 1;
            else if(deathCounter == 53 || deathCounter == 54)
                frames = 1;

            Impossible:
            0 40    11 30   22 18   33 9    44 4
            1 40    12 28   23 18   34 8    45 4
            2 38    13 28   24 16   35 8    46 3
            3 38    14 26   25 16   36 7    47 3
            4 36    15 26   26 14   37 7    48 3
            5 36    16 24   27 14   38 6    49 2
            6 34    17 24   28 12   39 6    50 2
            7 34    18 22   29 12   40 5    51 2
            8 32    19 22   30 10   41 5    52 1
            9 32    20 20   31 10   42 5    53 1
            10 30   21 20   32 9    43 4    54 1

            if(deathCounter % 2 == 0 && deathCounter > 0 && deathCounter <= 30)
                frames -= 2;
            else if(deathCounter % 2 == 0 && deathCounter > 30 && deathCounter <= 40)
                frames -= 1;
            else if(deathCounter % 3 == 1 && deathCounter > 40 && deathCounter <= 52)
                frames -= 1;
            else if(deathCounter == 52 || deathCounter == 53 || deathCounter == 54)
                frames = 1;

        */

        if(frameCounter % frames == 0)
        {
            if(deathCounter % 2 == 1)
            {
                if(spriteCounterL == 2)
                    spriteCounterL = 3;
                else if(spriteCounterL == 3)
                    spriteCounterL = 2;

                if(spriteCounterM == 4)
                    spriteCounterM = 5;
                else if(spriteCounterM == 5)
                    spriteCounterM = 4;

                if(spriteCounterS == 6)
                    spriteCounterS = 7;
                else if(spriteCounterS == 7)
                    spriteCounterS = 6;
            }
            atBorder = 0;
            for(int i = 0; i < enemies.size(); i++)
            {
                if(deathCounter != 54)
                    atBorder += enemies.at(i)->moveX();
                else
                {
                    int random = rand();
                    if(difficulty == Easy)
                        atBorder += enemies.at(i)->moveX();
                    else if(difficulty == Normal)
                        atBorder += enemies.at(i)->moveX(6);
                    else if(difficulty == Hard)
                        atBorder += enemies.at(i)->moveX((random % 8) + 1);
                    else
                        atBorder += enemies.at(i)->moveX((random % 15) + 1);
                }
                if(spriteCounterL == 2)
                    spriteCounterL = 3;
                else if(spriteCounterL == 3)
                    spriteCounterL = 2;

                if(spriteCounterM == 4)
                    spriteCounterM = 5;
                else if(spriteCounterM == 5)
                    spriteCounterM = 4;

                if(spriteCounterS == 6)
                    spriteCounterS = 7;
                else if(spriteCounterS == 7)
                    spriteCounterS = 6;
            }

            if(atBorder > 0)
            {
                for(int i = 0; i < enemies.size(); i++)
                {
                    enemies.at(i)->moveY();
                    if(!enemies.at(i)->isSide())
                    {
                        if(deathCounter != 54)
                            enemies.at(i)->moveX();
                        else
                        {
                            int random = rand();
                            if(!enemies.at(i)->isDead())
                            {
                                if(difficulty == Easy)
                                    atBorder += enemies.at(i)->moveX();
                                else if(difficulty == Normal)
                                    atBorder += enemies.at(i)->moveX(6);
                                else if(difficulty == Hard)
                                    atBorder += enemies.at(i)->moveX((random % 8) + 1);
                                else
                                    atBorder += enemies.at(i)->moveX((random % 15) + 1);
                            }
                        }
                    }
                }
            }
        }
    }

    repaint(); // --- Calls paintEvent(QPaintEvent *)
}

void Widget::moveRocket()
{
    if(!pause && !inMainMenu) // --- Stops when paused
    {
        if(frameCounter % (300 - 60 * randomMultiplier) == 0)
        {
            rocket.regenerateEnergy();
            rocket2.regenerateEnergy();
        }

        if(frameCounter % 1 == 0) // --- Moves at 60 FPS
        {
            if(rocket.getLeft()) // --- Ship moves by 3px to the left every frame
                rocket.modifyPositionX(-rocket.getSpeed());
            if(rocket.getRight()) // --- Ship moves by 3px to the right every frame
                rocket.modifyPositionX(rocket.getSpeed());
            if(rocket.getUp())
                rocket.modifyPositionY(-rocket.getSpeed());
            if(rocket.getDown())
                rocket.modifyPositionY(rocket.getSpeed());
            rocket.isOutOfBounds(false); // --- Checks if rocket would end up outside the border

            if(is2Player)
            {
                if(rocket2.getLeft()) // --- Ship moves by 3px to the left every frame
                    rocket2.modifyPositionX(-rocket2.getSpeed());
                if(rocket2.getRight()) // --- Ship moves by 3px to the right every frame
                    rocket2.modifyPositionX(rocket2.getSpeed());
                if(rocket2.getUp())
                    rocket2.modifyPositionY(-rocket2.getSpeed());
                if(rocket2.getDown())
                    rocket2.modifyPositionY(rocket2.getSpeed());
                rocket2.isOutOfBounds(false); // --- Checks if rocket would end up outside the border
            }
        }
    }

    repaint(); // --- Calls paintEvent(QPaintEvent *)
}

void Widget::keyPressEvent(QKeyEvent *event) // --- Function gets called on key press
{
    if (event->isAutoRepeat() || !actions.contains(event->key()))
    // --- event->isAutoRepeat() only accepts first key input
    // --- actions.contains(event->key()) checks if key input is an action
    {
        event->ignore(); // --- If conditions are met, the key is ignored
        return;
    }

    if(!inMainMenu)
    {
        if(event->key() == Qt::Key_A)
            rocket.setLeft(true); // --- If key press is 'A', ship starts moving to the left
        if(event->key() == Qt::Key_D)
            rocket.setRight(true); // --- If key press is 'D', ship starts moving to the right
        if(event->key() == Qt::Key_W)
            rocket.setUp(true);
        if(event->key() == Qt::Key_S)
            rocket.setDown(true);
        if(event->key() == Qt::Key_Space && !pause)
            rocket.shoot(); // --- If key press is "Space", ship starts "shooting"

        if(is2Player)
        {
            if(event->key() == Qt::Key_Left)
                rocket2.setLeft(true); // --- If key press is 'A', ship starts moving to the left
            if(event->key() == Qt::Key_Right)
                rocket2.setRight(true); // --- If key press is 'D', ship starts moving to the right
            if(event->key() == Qt::Key_Up)
                rocket2.setUp(true);
            if(event->key() == Qt::Key_Down)
                rocket2.setDown(true);
            if(event->key() == Qt::Key_Period && !pause)
                rocket2.shoot(); // --- If key press is "Space", ship starts "shooting"
        }

        if(event->key() == Qt::Key_P && !hasViewedEndGame) // !!! Needs adjusting
        {
            if(pause) // --- if key press is 'P' and game is not paused, the game pauses
                // --- otherwise the game unpauses
                pause = false;
            else
                pause = true;
        }
        if(event->key() == Qt::Key_N)
            newGame();

        if(event->key() == Qt::Key_Escape && gameOver)
            inMainMenu = true;
    }
    else
    {
        if(!inControls)
        {
            if(!inLeaderboard)
            {
                if(event->key() == Qt::Key_5)
                {
                    if(difficulty == Easy)
                        difficulty = Normal;
                    else if(difficulty == Normal)
                        difficulty = Hard;
                    else if(difficulty == Hard)
                        difficulty = Impossible;
                    else
                        difficulty = Easy;
                }
                if(event->key() == Qt::Key_2)
                {
                    if(is2Player)
                        is2Player = false;
                    else
                        is2Player = true;
                }
                if(event->key() == Qt::Key_1)
                {
                    inMainMenu = false;
                    afterMainMenuInitialization();
                }
                if(event->key() == Qt::Key_3) // §§§ Only 1 color per rocket
                {
                    if(rocket.getColor() >= 8 && rocket.getColor() < 12)
                        rocket.addColor();
                    else if(rocket.getColor() == 12)
                        rocket.setColor(8);
                }
                if(event->key() == Qt::Key_4) // §§§ Only 1 color per rocket
                {
                    if(rocket2.getColor() >= 8 && rocket2.getColor() < 12)
                        rocket2.addColor();
                    else if(rocket2.getColor() == 12)
                        rocket2.setColor(8);
                }
                if(event->key() == Qt::Key_6)
                    inLeaderboard = true;
                if(event->key() == Qt::Key_7)
                    inControls = true;
            }
            else
            {
                if(event->key() == Qt::Key_1)
                {
                    if(playerLeaderboard == 0)
                        playerLeaderboard = 1;
                    else
                        playerLeaderboard = 0;
                }
                if(event->key() == Qt::Key_2)
                {
                    if(difficultyLeaderboard >= 0 && difficultyLeaderboard < 3)
                        difficultyLeaderboard++;
                    else if(difficultyLeaderboard == 3)
                        difficultyLeaderboard = 0;
                }
                if(event->key() == Qt::Key_Escape)
                    inLeaderboard = false;
            }
        }
        else
        {
            if(event->key() == Qt::Key_Escape)
                inControls = false;
        }
    }
}

void Widget::keyReleaseEvent(QKeyEvent *event) // --- Function gets called on key release
{
    if ( event->isAutoRepeat() || !actions.contains( event->key() ) )
    // --- event->isAutoRepeat() only accepts first key input
    // --- actions.contains(event->key()) checks if key input is an action
    {
        event->ignore(); // --- If conditions are met, the key is ignored
        return;
    }
    if(event->key() == Qt::Key_A)
        rocket.setLeft(false); // --- On key release the ship stops moving to the left
    if(event->key() == Qt::Key_D)
        rocket.setRight(false); // --- On key release the ship stops moving to the right
    if(event->key() == Qt::Key_W)
        rocket.setUp(false);
    if(event->key() == Qt::Key_S)
        rocket.setDown(false);

    if(is2Player)
    {
        if(event->key() == Qt::Key_Left)
            rocket2.setLeft(false); // --- If key press is 'A', ship starts moving to the left
        if(event->key() == Qt::Key_Right)
            rocket2.setRight(false); // --- If key press is 'D', ship starts moving to the right
        if(event->key() == Qt::Key_Up)
            rocket2.setUp(false);
        if(event->key() == Qt::Key_Down)
            rocket2.setDown(false);
    }
}

void Widget::paintEvent(QPaintEvent *) // --- Function mainly for drawing on Widget
{
    QPainter painter(this); // --- Assigns the Widget to the painter
    QPen pen(Qt::white);
    painter.setPen(pen);

    if(!inMainMenu)
    {
        for(int i = 0; i <= 500; i++)
        {
            painter.drawPoint(i, 730);
        }

        if(!rocket.isDead())
        {
            rocket.setHitbox(QRect(rocket.getPositionX(), rocket.getPositionY(), 25, 24));
            painter.drawImage(rocket.getHitbox(), sprites[rocket.getColor()]);
        }

        if(!rocket2.isDead() && is2Player)
        {
            rocket2.setHitbox(QRect(rocket2.getPositionX(), rocket2.getPositionY(), 25, 24));
            painter.drawImage(rocket2.getHitbox(), sprites[rocket2.getColor()]);
        }

        for(int i = enemies.size() - 1; i >= 0; i--)
        {
            if(enemies.at(i)->getEntityID() < 11)
            {
                QRect rect(enemies.at(i)->generateHitboxS());
                painter.drawImage(rect, sprites[spriteCounterS]);
            }
            else if(enemies.at(i)->getEntityID() >= 11 && enemies.at(i)->getEntityID() < 33)
            {
                QRect rect(enemies.at(i)->generateHitboxM());
                painter.drawImage(rect, sprites[spriteCounterM]);
            }
            else
            {
                QRect rect(enemies.at(i)->generateHitboxL());
                painter.drawImage(rect, sprites[spriteCounterL]);
            }

            enemies.at(i)->setAreaControl(QRect(enemies.at(i)->getPositionX(), enemies.at(i)->getPositionY(), 10, 200));

        }

        // §§§ if(ssIsShooting) toggles auto fire
        if(rocket.isShooting() && !pause) // --- Enters when ship is shooting and stops when paused
        {
            if(projectileRocket.getProjectileFrameCounter() == 0)
                projectileRocket.setShooterHitbox(rocket.getHitbox());
            painter.drawImage(projectileRocket.fire(false, pause, 6), sprites[rocket.getColor() + 5]);
            if(projectileRocket.isOutOfFrame(false))
                rocket.stopShooting();

            for(int i = enemies.size() - 1; i >= 0; i--)
            {
                if(enemies.at(i)->checkForCollision(projectileRocket.getTopProjectile()))
                {
                    switch(enemies.at(i)->getSize())
                    {
                    case Small:
                        points += 9 * (((double)rocket.getBottomBorder() - (double)enemies.at(i)->getHitbox().y() + 200) / 200);
                        break;
                    case Medium:
                        points += 5 * (((double)rocket.getBottomBorder() - (double)enemies.at(i)->getHitbox().y() + 200) / 200);
                        break;
                    case Large:
                        points += 2 * (((double)rocket.getBottomBorder() - (double)enemies.at(i)->getHitbox().y() + 200) / 200);
                        break;
                        // ### Points system:
                        // ### Small enemy: 30 Points
                        // ### Medium enemy: 15 Points
                        // ### Big enemy: 5 Points
                        // ### Distance from border: (Distance + 200) / 200
                        // ### Multipliers:
                        // ### Difficulty: Easy 1.0x, Medium 1.5x, Hard 2.0x
                        // ### Energy level at finish: 1x + 0.1x per 10%
                        // ### 0-9 1.1x   10-19 1.2x   20-29 1.3x   30-39 1.4x   40-49 1.5x   50-59 1.6x
                        // ### 60-69x 1.7x   70-79 1.8x   80-89 1.9x   90-99 2.0x   100 2.5x
                    }
                    enemies.removeAt(i);
                    rocket.stopShooting();
                    projectileRocket.emptyProjectile();
                    deathCounter++;
                    switch(difficulty)
                    {
                    case Easy:
                        if(deathCounter > 0 && deathCounter < 11)
                            frames -= 5;
                        else if(deathCounter >= 11 && deathCounter < 17)
                            frames -= 4;
                        else if(deathCounter >= 17 && deathCounter < 27)
                            frames -= 3;
                        else if(deathCounter >= 27 && deathCounter < 38)
                            frames -= 2;
                        else if(deathCounter >= 38 && deathCounter < 54)
                            frames -= 1;
                        else
                            frames = 1;
                        break;
                    case Normal:
                        if(deathCounter > 0 && deathCounter < 10)
                            frames -= 3;
                        else if(deathCounter >= 10 && deathCounter < 36)
                            frames -= 2;
                        else if(deathCounter >= 36 && deathCounter < 53)
                            frames -= 1;
                        else
                            frames = 1;
                        break;
                    case Hard:
                        if(deathCounter % 3 == 0 && deathCounter <= 41 && deathCounter > 0)
                            frames -= 5;
                        else if(deathCounter % 2 == 0 && deathCounter <= 47 && deathCounter > 42)
                            frames -= 1;
                        else if(deathCounter >= 47 && deathCounter < 53)
                            frames -= 1;
                        else if(deathCounter == 53 || deathCounter == 54)
                            frames = 1;
                        break;
                    case Impossible:
                        if(deathCounter % 2 == 0 && deathCounter > 0 && deathCounter <= 30)
                            frames -= 2;
                        else if(deathCounter % 2 == 0 && deathCounter > 30 && deathCounter <= 40)
                            frames -= 1;
                        else if(deathCounter % 3 == 1 && deathCounter > 40 && deathCounter <= 52)
                            frames -= 1;
                        else if(deathCounter == 52 || deathCounter == 53 || deathCounter == 54)
                            frames = 1;
                        break;
                    }
                }
            }
        }
        else if(rocket.isShooting() && pause)
        {
            painter.drawImage(projectileRocket.fire(false, pause, 6), sprites[rocket.getColor() + 5]);
        }

        if(rocket2.isShooting() && !pause && is2Player) // --- Enters when ship is shooting and stops when paused
        {
            if(projectileRocket2.getProjectileFrameCounter() == 0)
                projectileRocket2.setShooterHitbox(rocket2.getHitbox());
            painter.drawImage(projectileRocket2.fire(false, pause, 6), sprites[rocket2.getColor() + 5]);
            if(projectileRocket2.isOutOfFrame(false))
                rocket2.stopShooting();

            for(int i = enemies.size() - 1; i >= 0; i--)
            {
                if(enemies.at(i)->checkForCollision(projectileRocket2.getTopProjectile()))
                {
                    switch(enemies.at(i)->getSize())
                    {
                    case Small:
                        points2 += 9 * (((double)rocket2.getBottomBorder() - (double)enemies.at(i)->getHitbox().y() + 200) / 200);
                        break;
                    case Medium:
                        points2 += 5 * (((double)rocket2.getBottomBorder() - (double)enemies.at(i)->getHitbox().y() + 200) / 200);
                        break;
                    case Large:
                        points2 += 2 * (((double)rocket2.getBottomBorder() - (double)enemies.at(i)->getHitbox().y() + 200) / 200);
                        break;
                    }
                    enemies.removeAt(i);
                    rocket2.stopShooting();
                    projectileRocket2.emptyProjectile();
                    deathCounter++;
                    switch(difficulty)
                    {
                    case Easy:
                        if(deathCounter > 0 && deathCounter < 11)
                            frames -= 5;
                        else if(deathCounter >= 11 && deathCounter < 17)
                            frames -= 4;
                        else if(deathCounter >= 17 && deathCounter < 27)
                            frames -= 3;
                        else if(deathCounter >= 27 && deathCounter < 38)
                            frames -= 2;
                        else if(deathCounter >= 38 && deathCounter < 54)
                            frames -= 1;
                        else
                            frames = 1;
                        break;
                    case Normal:
                        if(deathCounter > 0 && deathCounter < 10)
                            frames -= 3;
                        else if(deathCounter >= 10 && deathCounter < 36)
                            frames -= 2;
                        else if(deathCounter >= 36 && deathCounter < 53)
                            frames -= 1;
                        else
                            frames = 1;
                        break;
                    case Hard:
                        if(deathCounter % 3 == 0 && deathCounter <= 41 && deathCounter > 0)
                            frames -= 5;
                        else if(deathCounter % 2 == 0 && deathCounter <= 47 && deathCounter > 42)
                            frames -= 1;
                        else if(deathCounter < 53)
                            frames -= 1;
                        break;
                    case Impossible:
                        if(deathCounter % 2 == 0 && deathCounter > 0 && deathCounter <= 30)
                            frames -= 2;
                        else if(deathCounter % 2 == 0 && deathCounter > 30 && deathCounter <= 40)
                            frames -= 1;
                        else if(deathCounter % 3 == 1 && deathCounter > 40 && deathCounter <= 52)
                            frames -= 1;
                        else if(deathCounter == 52 || deathCounter == 53 || deathCounter == 54)
                            frames = 1;
                        break;
                    }
                }
            }
        }
        else if(rocket2.isShooting() && pause)
        {
            painter.drawImage(projectileRocket2.fire(false, pause, 6), sprites[rocket2.getColor() + 5]);
        }

        for(int i = 0; i < enemies.size(); i++)
        {
            intersects = 0;
            for(int j = 0; j < enemies.size(); j++)
            {
                if(i < j)
                {
                    if(enemies.at(i)->getAreaControl().intersects(enemies.at(j)->getHitbox()))
                        intersects++;
                }
            }
            if(intersects == 0)
                enemies.at(i)->setCanShoot(true);
        }

        if(!pause)
        {
            for(int i = enemies.size() - 1; i >= 0; i--)
            {
                int random = rand();

                if(random % ((600 + 400 * randomMultiplier) - (13 + 5 * randomMultiplier) * deathCounter) == 0 && deathCounter != 54)
                {
                    if(enemies.at(i)->getCanShoot())
                        projectilesEnemies.push_back(new Projectile(enemies.at(i)->getHitbox()));
                }
                if(deathCounter == 54)
                {
                    if(random % (15 * randomMultiplier) == 0)
                    {
                        if(enemies.at(i)->getCanShoot())
                            projectilesEnemies.push_back(new Projectile(enemies.at(i)->getHitbox()));
                    }
                }
                if(is2Player)
                {
                    if((rocket.getHitbox().x() < enemies.at(i)->getHitbox().x() + 60 &&
                         rocket.getHitbox().x() > enemies.at(i)->getHitbox().x() - 60) ||
                        (rocket2.getHitbox().x() < enemies.at(i) ->getHitbox().x() + 60 &&
                         rocket2.getHitbox().x() > enemies.at(i)->getHitbox().x() - 60))
                    {
                        if(random % (125 * randomMultiplier) == 0)
                        {
                            if(enemies.at(i)->getCanShoot())
                                projectilesEnemies.push_back(new Projectile(enemies.at(i)->getHitbox()));
                        }
                    }
                }
                else
                {
                    if(rocket.getHitbox().x() < enemies.at(i)->getHitbox().x() + 60 &&
                        rocket.getHitbox().x() > enemies.at(i)->getHitbox().x() - 60)
                    {
                        if(random % (125 * randomMultiplier) == 0)
                        {
                            if(enemies.at(i)->getCanShoot())
                                projectilesEnemies.push_back(new Projectile(enemies.at(i)->getHitbox()));
                        }
                    }
                }
            }
        }
        for(int i = projectilesEnemies.size() - 1; i >= 0; i--)
        {
            painter.drawImage(projectilesEnemies.at(i)->fire(true, pause, projectileSpeed), sprites[1]);
            if(projectilesEnemies.at(i)->isOutOfFrame(true))
            {
                projectilesEnemies.removeAt(i);
            }
        }

        for(int i = projectilesEnemies.size() - 1; i >= 0; i--)
        {
            if(rocket.checkForCollision(projectilesEnemies.at(i)->getTopProjectile()))
            {
                projectilesEnemies.removeAt(i);
                switch(difficulty)
                {
                case Easy:
                    rocket.takeDamage(15);
                    break;
                case Normal:
                    rocket.takeDamage(25);
                    break;
                case Hard:
                    rocket.takeDamage(40);
                    break;
                case Impossible:
                    rocket.takeDamage(80);
                    break;
                }
            }
        }

        QFont font;

        for(int i = projectilesEnemies.size() - 1; i >= 0; i--)
        {
            if(rocket2.checkForCollision(projectilesEnemies.at(i)->getTopProjectile()) && is2Player)
            {
                projectilesEnemies.removeAt(i);
                switch(difficulty)
                {
                case Easy:
                    rocket2.takeDamage(15);
                    break;
                case Normal:
                    rocket2.takeDamage(25);
                    break;
                case Hard:
                    rocket2.takeDamage(40);
                    break;
                case Impossible:
                    rocket2.takeDamage(80);
                    break;
                }
            }
        }

        for(int i = enemies.size() - 1; i >= 0; i--)
        {
            if(enemies.at(i)->isOutOfGame())
            {
                gameOver = true;
                enemies.removeAt(i);
                if(!is2Player)
                {
                    rocket.die();
                    rocket.emptyHitbox();
                }
                else
                {
                    rocket2.die();
                    rocket2.emptyHitbox();
                }

            }
        }
        if(is2Player)
        {
            if(rocket.getEnergy() == 0)
            {
                rocket.die();
                rocket.emptyHitbox();
            }

            if(rocket2.getEnergy() == 0)
            {
                rocket2.die();
                rocket2.emptyHitbox();
            }

            if(rocket.isDead() && rocket2.isDead())
            {
                gameOver = true;
            }
        }

        else
        {
            if(rocket.getEnergy() == 0)
            {
                gameOver = true;
                rocket.die();
                rocket.emptyHitbox();
            }
        }

        if(gameOver)
        {
            font.setPixelSize(50);
            painter.setFont(font);
            painter.drawText(150, 350, 200, 75, Qt::AlignCenter, QString("You lost!"));
            font.setPixelSize(20);
            painter.setFont(font);
            painter.drawText(100, 400, 300, 75, Qt::AlignCenter, QString("Press 'N' to start a new game"));
            painter.drawText(50, 430, 400, 75, Qt::AlignCenter, QString("Or press 'Esc' to go back to main menu"));
            frames = 4;
            if(!hasSetLeaderboard)
            {
                QString filename = "leaderboard";
                if(!is2Player)
                    filename += "1Player";
                else
                    filename += "2Players";
                if(difficulty == Easy)
                    filename += "Easy";
                else if(difficulty == Normal)
                    filename += "Normal";
                else if(difficulty == Hard)
                    filename += "Hard";
                else
                    filename += "Impossible";
                filename += ".txt";
                if(rocket.getPoints() + points != 0 || rocket2.getPoints() + points2 != 0)
                {
                    leaderboard.saveScore(rocket.getPoints() + points, filename);
                    if(is2Player)
                        leaderboard.saveScore(rocket2.getPoints() + points2 + rocket.getPoints() + points, filename);
                }

                hasSetLeaderboard = true;
            }

        }

        font.setPixelSize(30);
        painter.setFont(font);
        painter.drawText(20, 740, 250, 40, Qt::AlignLeft, QString("Player 1"));
        painter.drawText(20, 790, 250, 40, Qt::AlignLeft, QString("Points: %1").arg(floor(rocket.getPoints() + points)));
        painter.drawText(20, 840, 250, 40, Qt::AlignLeft, QString("Energy: %1%").arg(rocket.getEnergy()));
        if(is2Player)
        {
            painter.drawText(270, 740, 250, 40, Qt::AlignLeft, QString("Player 2"));
            painter.drawText(270, 790, 250, 40, Qt::AlignLeft, QString("Points: %1").arg(floor(rocket2.getPoints() + points2)));
            painter.drawText(270, 840, 250, 40, Qt::AlignLeft, QString("Energy: %1%").arg(rocket2.getEnergy()));
        }
        if(deathCounter == 55)
        {
            font.setPixelSize(50);
            painter.setFont(font);
            painter.drawText(150, 350, 200, 75, Qt::AlignCenter, QString("You won!"));
            font.setPixelSize(20);
            painter.setFont(font);
            painter.drawText(100, 400, 300, 75, Qt::AlignCenter, QString("Press 'N' to start a new game"));
            if(!hasViewedEndGame)
            {
                endGame();
                hasViewedEndGame = true;
            }
        }
    }
    else
    {
        if(!inControls)
        {
            if(!inLeaderboard)
            {
                QFont font;
                font.setPixelSize(25);
                painter.setFont(font);
                painter.drawText(20, 50, 300, 50, Qt::AlignLeft, QString("Press '1' to start game"));

                painter.drawText(20, 150, 300, 50, Qt::AlignLeft, QString("Press '2' to toggle Player 2"));
                font.setBold(false);
                if(is2Player)
                    font.setBold(true);
                painter.setFont(font);
                painter.drawText(20, 200, 300, 50, Qt::AlignLeft, QString("Player 2 On"));
                font.setBold(false);
                if(!is2Player)
                    font.setBold(true);
                painter.setFont(font);
                painter.drawText(220, 200, 300, 50, Qt::AlignLeft, QString("Player 2 Off"));

                font.setBold(false);
                painter.setFont(font);
                painter.drawText(20, 300, 400, 50, Qt::AlignLeft, QString("Press '3' to toggle Player 1 colors"));
                for(int i = 0; i < 5; i++)
                {
                    painter.drawImage(QRect(30 + (i * 50), 350, 25, 24), sprites[i + 8]);
                    if(rocket.getColor() == i + 8)
                    {
                        painter.drawRect(QRect(25 + (i * 50), 345, 35, 34));
                    }
                }

                if(is2Player)
                {
                    painter.drawText(20, 430, 400, 50, Qt::AlignLeft, QString("Press '4' to toggle Player 2 colors"));
                    for(int i = 0; i < 5; i++)
                    {
                        painter.drawImage(QRect(30 + (i * 50), 480, 25, 24), sprites[i + 8]);
                        if(rocket2.getColor() == i + 8)
                        {
                            painter.drawRect(QRect(25 + (i * 50), 475, 35, 34));
                        }
                    }
                }

                font.setBold(false);
                painter.setFont(font);
                painter.drawText(20, 555, 400, 50, Qt::AlignLeft, QString("Press '5' to change difficulty"));

                font.setBold(false);
                if(difficulty == Easy)
                    font.setBold(true);
                painter.setFont(font);
                painter.drawText(20, 605, 300, 50, Qt::AlignLeft, QString("Easy"));
                font.setBold(false);
                if(difficulty == Normal)
                    font.setBold(true);
                painter.setFont(font);
                painter.drawText(110, 605, 300, 50, Qt::AlignLeft, QString("Normal"));
                font.setBold(false);
                if(difficulty == Hard)
                    font.setBold(true);
                painter.setFont(font);
                painter.drawText(230, 605, 300, 50, Qt::AlignLeft, QString("Hard"));
                font.setBold(false);
                if(difficulty == Impossible)
                    font.setBold(true);
                painter.setFont(font);
                painter.drawText(320, 605, 300, 50, Qt::AlignLeft, QString("Impossible"));

                font.setBold(false);
                painter.setFont(font);
                painter.drawText(20, 705, 400, 50, Qt::AlignLeft, QString("Press '6' to view leaderboard"));
                painter.drawText(20, 755, 400, 50, Qt::AlignLeft, QString("Press '7' to view controls"));
            }
            else
            {
                QFont font;
                QString filename = "leaderboard";
                font.setPixelSize(25);
                painter.setFont(font);
                painter.drawText(20, 50, 400, 50, Qt::AlignLeft, QString("Press 'Esc' to go back to main menu"));

                painter.drawText(20, 100, 450, 50, Qt::AlignLeft, QString("Press '1' to toggle Player leaderboard"));
                painter.drawText(20, 150, 450, 50, Qt::AlignLeft, QString("Press '2' to toggle Difficulty leaderboard"));
                if(playerLeaderboard == 0)
                {
                    filename += "1Player";
                    font.setBold(true);
                }
                painter.setFont(font);
                painter.drawText(80, 220, 400, 50, Qt::AlignLeft, QString("1 Player"));
                font.setBold(false);
                if(playerLeaderboard == 1)
                {
                    filename += "2Players";
                    font.setBold(true);
                }
                painter.setFont(font);
                painter.drawText(280, 220, 400, 50, Qt::AlignLeft, QString("2 Players"));

                font.setBold(false);
                if(difficultyLeaderboard == 0)
                {
                    filename += "Easy";
                    font.setBold(true);
                }
                painter.setFont(font);
                painter.drawText(20, 270, 300, 50, Qt::AlignLeft, QString("Easy"));
                font.setBold(false);
                if(difficultyLeaderboard == 1)
                {
                    filename += "Normal";
                    font.setBold(true);
                }
                painter.setFont(font);
                painter.drawText(110, 270, 300, 50, Qt::AlignLeft, QString("Normal"));
                font.setBold(false);
                if(difficultyLeaderboard == 2)
                {
                    filename += "Hard";
                    font.setBold(true);
                }
                painter.setFont(font);
                painter.drawText(230, 270, 300, 50, Qt::AlignLeft, QString("Hard"));
                font.setBold(false);
                if(difficultyLeaderboard == 3)
                {
                    filename += "Impossible";
                    font.setBold(true);
                }
                painter.setFont(font);
                painter.drawText(320, 270, 300, 50, Qt::AlignLeft, QString("Impossible"));
                font.setBold(false);
                font.setPixelSize(20);
                painter.setFont(font);
                filename += ".txt";

                QString inputLeaderboard(".\\leaderboards\\" + filename);
                QFile leaderboardFile(inputLeaderboard);
                if(leaderboardFile.open(QIODevice::ReadOnly | QFile::Text))
                {
                    QTextStream leaderboardReader(&leaderboardFile);
                    int lineCounter = 0;

                    while(!leaderboardReader.atEnd() && lineCounter < 10)
                    {
                        painter.drawText(20, 350 + (35 * lineCounter), 300, 50, Qt::AlignLeft, QString("%1. ").arg(lineCounter + 1) + leaderboardReader.readLine() + " Points");
                        lineCounter++;
                    }
                }
                leaderboardFile.close();
            }
        }
        else
        {
            QFont font;
            font.setPixelSize(25);
            painter.setFont(font);
            painter.drawText(20, 10, 400, 50, Qt::AlignLeft, QString("Press 'Esc' to go back to main menu"));

            painter.drawText(20, 50, 400, 50, Qt::AlignLeft, QString("Player 1: Up = 'W'"));
            painter.drawText(20, 90, 400, 50, Qt::AlignLeft, QString("Player 1: Left = 'A'"));
            painter.drawText(20, 130, 400, 50, Qt::AlignLeft, QString("Player 1: Down = 'S'"));
            painter.drawText(20, 170, 400, 50, Qt::AlignLeft, QString("Player 1: Right = 'D'"));
            painter.drawText(20, 210, 400, 50, Qt::AlignLeft, QString("Player 1: Shoot = 'Space'"));

            painter.drawText(20, 250, 400, 50, Qt::AlignLeft, QString("Player 2: Up = 'Arrow_Up'"));
            painter.drawText(20, 290, 400, 50, Qt::AlignLeft, QString("Player 2: Left = 'Arrow_Left'"));
            painter.drawText(20, 330, 400, 50, Qt::AlignLeft, QString("Player 2: Down = 'Arrow_Down'"));
            painter.drawText(20, 370, 400, 50, Qt::AlignLeft, QString("Player 2: Right = 'Arrow_Right'"));
            painter.drawText(20, 410, 400, 50, Qt::AlignLeft, QString("Player 2: Space = 'Period'"));

            painter.drawText(20, 450, 400, 50, Qt::AlignLeft, QString("Pause = 'P'"));
            painter.drawText(20, 490, 400, 50, Qt::AlignLeft, QString("New Game = 'N'"));
        }
    }
}
void Widget::newGame()
{
    afterMainMenuInitialization();
    frameCounter = 0;
    deathCounter = 0;
    atBorder = 0;
    if(gameOver)
    {
        if(!is2Player)
        {
            rocket.resetPoints();
            points = 0;
        }
        else
        {
            rocket.resetPoints();
            points = 0;
            rocket2.resetPoints();
            points2 = 0;
        }
    }
    gameOver = false;
    hasSetLeaderboard = false;
    spriteCounterL = 2;
    spriteCounterM = 4;
    spriteCounterS = 6;
    hasViewedEndGame = false;
    pause = false;

    rocket.resetRocket();
    if(is2Player)
        rocket2.resetRocket();

    projectileRocket.emptyProjectile();
    projectilesEnemies.clear();
    enemies.clear();
    for(int j = 0; j < ENEMIES_PER_COLUMN; j++)
    {
        for(int i = 0; i < ENEMIES_PER_ROW; i++)
            enemies.push_back(new Enemy(i + (j * ENEMIES_PER_ROW), i, j));
    }
}

void Widget::endGame()
{
    rocket.setPoints(rocket.getPoints() - points);
    if(is2Player)
        rocket2.setPoints(rocket2.getPoints() - points2);
    if(rocket.getEnergy() != 100)
    {
        switch(difficulty)
        {
        case Easy:
            points *= (2 + (0.1 * (1 + (double)rocket.getEnergy() / 10)));
            break;
        case Normal:
            points *= (2.5 + (0.1 * (1 + (double)rocket.getEnergy() / 10)));
            break;
        case Hard:
            points *= (3 + (0.1 * (1 + (double)rocket.getEnergy() / 10)));
            break;
        case Impossible:
            points *= (5 + (0.1 * (1 + (double)rocket.getEnergy() / 10)));
        }
    }
    else
    {
        switch(difficulty)
        {
        case Easy:
            points *= (3.5);
            break;
        case Medium:
            points *= (4);
            break;
        case Hard:
            points *= (4.5);
            break;
        case Impossible:
            points *= (6);
        }
    }
    if(rocket2.getEnergy() != 100)
    {
        switch(difficulty)
        {
        case Easy:
            points2 *= (2 + (0.1 * (1 + (double)rocket2.getEnergy() / 10)));
            break;
        case Normal:
            points2 *= (2.5 + (0.1 * (1 + (double)rocket2.getEnergy() / 10)));
            break;
        case Hard:
            points2 *= (3 + (0.1 * (1 + (double)rocket2.getEnergy() / 10)));
            break;
        case Impossible:
            points2 *= (5 + (0.1 * (1 + (double)rocket2.getEnergy() / 10)));
        }
    }
    else
    {
        switch(difficulty)
        {
        case Easy:
            points2 *= (3.5);
            break;
        case Medium:
            points2 *= (4);
            break;
        case Hard:
            points2 *= (4.5);
            break;
        case Impossible:
            points2 *= (6);
        }
    }
    rocket.addPoints(points);
    if(is2Player)
        rocket2.addPoints(points2);
    points = 0;
    if(is2Player)
        points2 = 0;
}
