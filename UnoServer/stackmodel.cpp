#include "stackmodel.h"

/*
* Initalisiert das StackModel und legt alle Karten im kartenstappel an
*/
StackModel::StackModel()
{
    char colors[4] = { 'r', 'g', 'b', 'y' };

    for(char c : colors)
    {
        QString cardName = c + QString("0");
        CardModel *card = new CardModel(cardName);
        cards.push_back(card);

        for(int i = 1; i <= 2; i++)
        {
            for(int j = 1; j <= 9; j++)
            {
                createCard(c + QString::number(j));
            }

            createCard(c + QString("o"));
            createCard(c + QString("u"));
            createCard(c + QString("+"));
        }
    }

    for (int i = 1; i <= 4; i++) {
        createCard(QString("s+"));
        createCard(QString("sc"));
    }

    random_shuffle(begin(cards), end(cards));
}

/*
* Legt eine neue Karte an
*/
void StackModel::createCard(QString cardName) {
    CardModel *card = new CardModel(cardName);
    cards.push_back(card);
}

/*
* Gibt die oberste Karte zurück
*/
CardModel* StackModel::getCard()
{
    CardModel* card = cards.at(0);
    cards.erase(cards.begin());
    return card;
}

/*
* Gibt die erste Karte zurück die keine plus, aussetzten oder Retour karte ist
*/
CardModel* StackModel::getFirstCard() {
    CardModel* card = NULL;
    int i = 0;
    QRegExp regex = QRegExp("\\+|u|o");

    for (;i < this->cards.size();) {
        card = cards.at(i);
        if (!(card->getName().contains(regex))) {
            break;
        }

        ++i;
    }

    cards.erase(cards.begin() + i);
    return card;
}
