#include "stackmodel.h"

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
                QString cardName = c + QString::number(j);
                CardModel *card = new CardModel(cardName);
                cards.push_back(card);
            }
        }
    }

    random_shuffle(begin(cards), end(cards));
}

CardModel* StackModel::getCard()
{
    CardModel* card = cards.at(0);
    cards.erase(cards.begin());
    return card;
}
