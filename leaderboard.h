#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QFile>
#include <QTextStream>
#include <math.h>
#include <QList>

class Leaderboard
{
public:
    Leaderboard();

    QList<int> pointsRanking;

    void saveScore(double points, QString filename);
};

#endif // LEADERBOARD_H
