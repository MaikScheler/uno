#include "stackmodel.h"

StackModel::StackModel()
{
    char colors[4] = { 'r', 'g', 'b', 'y' };

    int x = 0;
    while(colors[x] != '\0')
    {
        char c = colors[x];
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
}

CardModel* StackModel::getCard()
{
    return cards.at(0);
}
