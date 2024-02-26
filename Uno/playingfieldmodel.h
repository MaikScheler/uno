#ifndef PLAYINGFIELDMODEL_H
#define PLAYINGFIELDMODEL_H

#include <QObject>
#include "playermodel.h"
#include "stackmodel.h"

using namespace std;

class PlayingFieldModel
{
public:
    PlayingFieldModel();
    void addPlayer(int clientId, bool primary = false);
    PlayerModel* getPlayer(int clientId);

signals:

private:
    vector<PlayerModel*> players;
    StackModel *stack;

};

#endif // PLAYINGFIELDMODEL_H
