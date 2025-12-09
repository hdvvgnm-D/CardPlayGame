// GameView.h
#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include <utility>
#include "cocos2d.h"
#include "models/GameModel.h"
#include "CardView.h"

class GameController;
class GameView : public cocos2d::LayerColor {
public:
    static GameView* create(GameModel* model, GameController* controller);
    virtual bool init(GameModel* model, GameController* controller);
    GameController* getGameController();
    void initUndoButton();
    void initPileZone();
    void initMainZone();
    void showAllCards();
    void leftMove();
    void rightMove();
    void enablePileCardTouch(CardView* cardView);
    void enableMainCardTouch(CardView* cardView);
    void moveCardToHandZone(CardView* cardView, bool isPileZone = true);

    std::vector<CardView*> getMainCards();
    void moveHandCardToMain();
    void moveHandCardToPile();

protected:
    GameModel* _model;
    GameController* _controller;
    std::vector<CardView*> _pileCards;
    std::vector<CardView*> _mainCards;
    std::vector<CardView*> _handCards;
    std::map<int, CardView*> _cardViews; // À˘”–ø®≈∆ ”Õº
    float _cardWidth;
};

#endif