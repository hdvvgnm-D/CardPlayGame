#include "GameModel.h"

void GameModel::addPileCard(CardModel* card)
{
    _pileCards.push_back(card);
}

void GameModel::addMainCard(CardModel* card)
{
    _mainCards.push_back(card);
}

std::vector<CardModel*> GameModel::getPileCards() { return _pileCards; }

std::vector<CardModel*> GameModel::getMainCards() { return _mainCards; }

void GameModel::addPileCardToHand(CardModel* card)
{
    _handCards.push_back(card);
    _pileCards.erase(std::remove(_pileCards.begin(), _pileCards.end(), card), _pileCards.end());
}

void GameModel::addMainCardToHand(CardModel* card)
{
    _handCards.push_back(card);
    _mainCards.erase(std::remove(_mainCards.begin(), _mainCards.end(), card), _mainCards.end());
}

void GameModel::moveHandCardToPile()
{
    _pileCards.push_back(_handCards.back());
    _handCards.pop_back();
}

void GameModel::moveHandCardToMain()
{
    _mainCards.push_back(_handCards.back());
    _handCards.pop_back();
}

CardModel* GameModel::getTopHandCard() { return _handCards.back(); }