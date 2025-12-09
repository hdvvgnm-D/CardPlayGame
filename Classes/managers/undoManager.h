#ifndef UNDO_MANAGER_H
#define UNDO_MANAGER_H

#include "models/UndoModel.h"
#include "views/CardView.h"
#include "views/GameView.h"

class UndoManager {
public:
    void recordCardMove(CardView* cardView,
        const std::string& zone, Vec2 position, int zOrder);
    bool undoLastMove();
    bool isEmpty()const;
    void setGameView(GameView* gameView);
private:
    std::stack<UndoModel*> _history;
    GameView* _gameView = nullptr;
};

#endif 