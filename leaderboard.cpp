#include "leaderboard.h"

Leaderboard::Leaderboard()
{

}

void Leaderboard::saveScore(double points, QString filename)
{
    pointsRanking.clear();
    QString inputLeaderboard(QString(".\\leaderboards\\") + filename);
    QString line;
    QFile leaderboardFile(inputLeaderboard);
    if(leaderboardFile.open(QIODevice::ReadWrite | QFile::Text))
    {
        QTextStream leaderboardReader(&leaderboardFile);

        while(!leaderboardReader.atEnd())
        {
            line = leaderboardReader.readLine();
            if(line.toInt() != 0)
                pointsRanking.push_back(line.toInt());
        }
    }

    pointsRanking.push_front(floor(points));
    std::sort(pointsRanking.begin(), pointsRanking.end());

    QTextStream leaderboardReader(&leaderboardFile);

    leaderboardFile.resize(0);
    for(int i = pointsRanking.size() - 1; i >= 0 ; i--)
        leaderboardReader << pointsRanking.at(i) << '\n';

    leaderboardFile.close();
}
