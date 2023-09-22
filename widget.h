#ifndef WIDGET_H
#define WIDGET_H
#define MAX_ENEMIES 55
#define ENEMIES_PER_ROW 11
#define ENEMIES_PER_COLUMN 5

#include <QWidget>
#include <QTimer>
#include <QMap>
#include <iostream>
#include <QList>
#include "rocket.h"
#include "enemy.h"
#include "entity.h"
#include "projectile.h"
#include "leaderboard.h"
#include <QImage>
#include <QList>
#include <QFile>
#include <QTextStream>

// --- Comment

// --- Enum
enum Action
{
    Player2,
    ChangeDifficulty,
    ChangeColor1,
    ChangeColor2,
    StartGame,
    OpenLeaderboard,
    TogglePlayerLeaderboard,
    ToggleDifficultyLeaderboard,
    LeaveLeaderboard,
    MoveLeft,
    MoveRight,
    MoveTop,
    MoveBot,
    Shoot,
    MoveLeft2,
    MoveRight2,
    MoveTop2,
    MoveBot2,
    Shoot2,
    Pause,
    NewGame,
    EnterMainMenu,
    EnterControls,
    LeaveControls
};

enum Difficulty
{
    Easy,
    Normal,
    Hard,
    Impossible
};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    // --- Constructors
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    // --- Variables
    // --- Int
    unsigned long int frameCounter;
    int deathCounter;
    int frames;
    int atBorder;
    int spriteCounterL;
    int spriteCounterM;
    int spriteCounterS;
    int intersects;
    int randomMultiplier;
    int projectileSpeed;
    int playerLeaderboard;
    int difficultyLeaderboard;

    // --- Double
    double fps;
    double points;
    double points2;

    // --- Bool
    bool pause;
    bool is2Player;
    bool hasViewedEndGame;
    bool inMainMenu;
    bool inLeaderboard;
    bool gameOver;
    bool hasSetLeaderboard;
    bool inControls;

    // --- Other classes
    QTimer timer;
    Rocket rocket;
    Rocket rocket2;
    QMap<int, Action> actions;
    QList<Enemy*> enemies;
    QList<Projectile*> projectilesEnemies;
    Projectile projectileRocket;
    Projectile projectileRocket2;
    QImage sprites[18];
    Difficulty difficulty;
    Leaderboard leaderboard;

private slots:
    // --- Slots
    void moveEnemies();
    void moveRocket();

private:
    // --- Functions
    void newGame();
    void endGame();
    void addPoints(Enemy* enemy);
    void paintEvent(QPaintEvent *);
    void onStart();
    void mainMenu();
    void afterMainMenuInitialization();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    Ui::Widget *ui;
};
#endif // WIDGET_H
