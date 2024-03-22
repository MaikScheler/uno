#include "playermodel.h"

/*
* Setz den Socket und gibt den Spieler eine Unique Id
*/
PlayerModel::PlayerModel(QTcpSocket *socket)
{
    id = rand() % 100;
    this->socket = socket;
}

/*
* Getter für Id
*/
int PlayerModel::getId()
{
    return id;
}

/*
* Getter für Name
*/
QString PlayerModel::getName()
{
    return name;
}

/*
* Getter für Socket
*/
QTcpSocket* PlayerModel::getSocket()
{
    return socket;
}

/*
* Fügt dem Spieler eine weitere Karte zu
*/
void PlayerModel::addCard(CardModel *card)
{
    cards.push_back(card);
}

/*
* Getter für Karte
*/
CardModel* PlayerModel::getCard(QString cardId)
{
    auto isValueMatch = [&cardId](const CardModel* c) {
        return c->getId() == cardId.toInt();
    };

    auto it = find_if(cards.begin(), cards.end(), isValueMatch);
    auto index = std::distance(this->cards.begin(), it);

    return cards.at(index);
}

/*
* Löscht Karte vom Spieler
*/
void PlayerModel::removeCard(QString cardId)
{
    auto it = find_if(this->cards.begin(), this->cards.end(), [&cardId](CardModel *card) {return card->getId() == cardId.toInt();});

    if (it != this->cards.end())
    {
        // found element. it is an iterator to the first matching element.
        // if you really need the index, you can also get it:
        auto index = std::distance(this->cards.begin(), it);

        cards.erase(this->cards.begin() + index);
    }
}

/*
* Gibt die Anzahl der Karten vom Spieler zurück
*/
int PlayerModel::getCardCount() {
    return this->cards.size();
}
