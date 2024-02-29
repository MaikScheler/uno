#include "playingfieldmodel.h"

PlayingFieldModel::PlayingFieldModel()
{
    stack = new StackModel();
}

void PlayingFieldModel::addPlayer(PlayerModel *player) {
    this->players.push_back(player);
}

PlayerModel* PlayingFieldModel::getPlayer(int clientId) {
    auto it = find_if(this->players.begin(), this->players.end(), [&clientId](PlayerModel *player) {return player->getId() == clientId;});

    if (it != this->players.end())
    {
        // found element. it is an iterator to the first matching element.
        // if you really need the index, you can also get it:
        auto index = std::distance(this->players.begin(), it);

        return this->players.at(index);
    }

}


bool PlayingFieldModel::hasSpace()
{
    return players.size() < 2;
}

void PlayingFieldModel::drawCard(PlayerModel *player)
{

    //player->addCard();
    stack->getCard();

    for(PlayerModel* p : players)
    {
        QTcpSocket* pSocket = p->getSocket();
        QTextStream os(pSocket);

        if(player == p)
        {
            os << "card::r+";
        } else {
            os << "card::back";
        }
    }
}


