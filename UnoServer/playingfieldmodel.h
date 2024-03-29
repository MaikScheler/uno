#ifndef PLAYINGFIELDMODEL_H
#define PLAYINGFIELDMODEL_H

#include <QObject>
#include "playermodel.h"
#include "cardmodel.h"
#include "stackmodel.h"
#include "logger.h"

using namespace std;

class PlayingFieldModel
{
public:
    PlayingFieldModel();
    void addPlayer(PlayerModel *player);
    PlayerModel* getPlayer(int clientId);
    bool hasSpace();

    void drawCard(PlayerModel* player);

    void start();

    void playCard(QString cardName, PlayerModel* player);

    void skip(PlayerModel* player);

    void switchPlayer(PlayerModel* currentPlayer);

    void pickColor(QString color, PlayerModel *player);

    void chatMessage(QString message, PlayerModel *player);

signals:

private:
    vector<PlayerModel*> players;
    PlayerModel* currentPlayer;
    CardModel* card;
    StackModel* stack;
    Logger* logger;
    bool isPicking = false;
    QChar currentColor = QChar();
    int drawenCards = 0;
    int drawCardLimit = 1;

    void notifyDrawCards(PlayerModel *player, int toDraw);

    void notifyPlayers();

    void notifyTurn();
    void checkWin(PlayerModel *player);
};

#endif // PLAYINGFIELDMODEL_H
