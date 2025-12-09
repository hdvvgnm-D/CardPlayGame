#include "UndoManager.h"
#include "controllers/GameController.h"
USING_NS_CC;
bool UndoManager::isEmpty() const {
    return _history.size() <= 1;
}

void UndoManager::setGameView(GameView* gameView) {
    _gameView = gameView;
}

//记录回退消息
void UndoManager::recordCardMove(CardView* cardView,
    const std::string& zone, Vec2 position, int zOrder) {
    UndoModel* record = new UndoModel(cardView, zone, position, zOrder);
    _history.push(record);
}


//回退操作
bool UndoManager::undoLastMove() {
    // 取出栈顶操作
    if (isEmpty()) return false;
    UndoModel* lastRecord = _history.top();
    _history.pop();


    CardView* cardView = lastRecord->getView();
    if (!cardView) return false;

    if (lastRecord->getZone() == "pile") {
        _gameView->leftMove();
    }
    cardView->setZOrder(lastRecord->getZOrder());
    auto moveAction = MoveTo::create(0.5f, lastRecord->getPosition());

    auto callFunc = CallFunc::create([this, cardView, lastRecord]() {
        // 重新绑定事件监听器
        if (lastRecord->getZone() == "pile") {
            _gameView->enablePileCardTouch(cardView);
        }
        else {
            _gameView->enableMainCardTouch(cardView);
            _gameView->getGameController()->updateCoverRelations();
        }
        // 数据恢复
        _gameView->getGameController()->onUndoMove(lastRecord);

        });
    auto sequence = Sequence::create(moveAction, callFunc, nullptr);
    cardView->runAction(sequence);

    return true;
}