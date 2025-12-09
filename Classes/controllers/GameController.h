#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "models/GameModel.h"
#include "managers/UndoManager.h"
#include "views/GameView.h"

class GameController {
public:
    GameController();
    void startGame(const std::string& levelFile);
    GameModel* getModel();
    UndoManager* getUndoManager();
    GameView* getView();

    void GameController::updateClickableCards();
    std::vector<CardView*> GameController::getSortedCardsByZOrder();
    bool GameController::isCardCovering(CardView* a, CardView* b);
    void GameController::updateCoverRelations();

    //// »ØÍË²Ù×÷
    void onUndoMove(UndoModel* record);

private:
    GameModel* _model;
    GameView* _view;
    UndoManager* _undoManager;
};

#endif 