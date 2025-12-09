#include "GameController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "cocos2d.h"

GameController::GameController() {
    _undoManager = new UndoManager();
}

void GameController::startGame(const std::string& levelFile) {
    // 加载关卡配置
    _model = LevelConfigLoader::loadLevelFromFile(levelFile);
    _view = GameView::create(_model, this);
    _undoManager->setGameView(_view);
    _view->showAllCards();
}

//数据恢复
void GameController::onUndoMove(UndoModel* record) {
    if (record->getZone() == "main") {
        _model->moveHandCardToMain();
        _view->moveHandCardToMain();
    }
    else {
        _model->moveHandCardToPile();
        _view->moveHandCardToPile();
    }
}

GameModel* GameController::getModel() { return _model; }

UndoManager* GameController::getUndoManager() { return _undoManager; }

GameView* GameController::getView() { return _view; }

void GameController::updateCoverRelations() {

    auto mainCards = _view->getMainCards();
    for (auto card : mainCards) {
        card->setCoveredBy(nullptr);
    }
    // 获得排序后的主牌区卡牌视图容器
    auto cards = getSortedCardsByZOrder();

    for (size_t i = 0; i < cards.size(); ++i) {
        for (size_t j = i + 1; j < cards.size(); ++j) {
            if (isCardCovering(cards[i], cards[j])) {
                cards[j]->setCoveredBy(cards[i]);
                break;
            }
        }
    }

    updateClickableCards();
}
bool GameController::isCardCovering(CardView* a, CardView* b) {
    Rect rectA = a->getBoundingBox();
    Rect rectB = b->getBoundingBox();
    return rectA.intersectsRect(rectB);
}

// 按层级对主牌区卡牌进行排序
std::vector<CardView*> GameController::getSortedCardsByZOrder() {
    std::vector<CardView*> cards;
    auto mainCards = _view->getMainCards();
    for (auto card : mainCards) {
        cards.push_back(card);
    }
    std::sort(cards.begin(), cards.end(),
        [](CardView* a, CardView* b) {
            return a->getLocalZOrder() > b->getLocalZOrder();
        });

    return cards;
}

void GameController::updateClickableCards() {
    auto mainCards = _view->getMainCards();
    for (auto card : mainCards) {
        bool isCovered = (card->getCoveredBy() != nullptr);
        card->setClickable(!isCovered);
    }
}