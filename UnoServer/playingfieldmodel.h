#ifndef PLAYINGFIELDMODEL_H
#define PLAYINGFIELDMODEL_H

#include <QObject>
#include "playermodel.h"
#include "cardmodel.h"
#include "stackmodel.h"

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

signals:

private:
    vector<PlayerModel*> players;
    PlayerModel* currentPlayer;
    CardModel* card;
    StackModel* stack;
    int drawenCards = 0;
    int drawCardLimit = 1;

    void notifyPlayers();
};

#endif // PLAYINGFIELDMODEL_H
