#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include "CardModel.h"

class GameModel
{
public:
    void addPileCard(CardModel* card);
    void addMainCard(CardModel* card);
    std::vector<CardModel*> getPileCards();
    std::vector<CardModel*> getMainCards();
    void addPileCardToHand(CardModel* card);
    void addMainCardToHand(CardModel* card);
    void moveHandCardToPile();
    void moveHandCardToMain();
    CardModel* getTopHandCard();

private:
    std::vector<CardModel*> _pileCards;
    std::vector<CardModel*> _mainCards;
    std::vector<CardModel*> _handCards;

};

#endif // GAMEMODEL_H