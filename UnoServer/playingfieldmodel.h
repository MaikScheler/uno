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

signals:

private:
    vector<PlayerModel*> players;
    CardModel* card;
    StackModel* stack;

    void notifyPlayers();
};

#endif // PLAYINGFIELDMODEL_H
