#include "stackmodel.h"

// Initalisiert das StackModel und legt alle Karten im kartenstappel an
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

// Legt eine neue Karte an
void StackModel::createCard(QString cardName) {
    CardModel *card = new CardModel(cardName);
    cards.push_back(card);
}

CardModel* StackModel::getCard()
{
    CardModel* card = cards.at(0);
    cards.erase(cards.begin());
    return card;
}
