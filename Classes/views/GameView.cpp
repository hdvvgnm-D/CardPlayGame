#include "GameView.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "managers/UndoManager.h"
#include "controllers/GameController.h"
USING_NS_CC;

GameView* GameView::create(GameModel* model, GameController* controller) {
    GameView* node = new GameView();
    if (node && node->init(model, controller)) {
        node->autorelease();
        return node;
    }
    delete node;
    return nullptr;
}

bool GameView::init(GameModel* model, GameController* controller) {
    if (!LayerColor::initWithColor(cocos2d::Color4B(255, 255, 255, 0))) return false;


    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // ====================== 主牌区背景 ======================
    auto mainArea = LayerColor::create(
        Color4B(170, 121, 66, 255),
        1080, 1500
    );
    mainArea->setPosition(
        origin.x,
        origin.y + 580
    );
    mainArea->setZOrder(-1);
    this->addChild(mainArea);

    // ====================== 堆牌区背景 ======================
    auto pileArea = LayerColor::create(
        Color4B(148, 33, 146, 255),
        1080, 580
    );
    pileArea->setPosition(origin.x, origin.y);
    pileArea->setZOrder(-100);
    this->addChild(pileArea);

    _model = model;
    _controller = controller;

    auto pileCards = _model->getPileCards();
    for (const auto& card : pileCards) {
        CardView* cardView = CardView::create(card);
        _pileCards.push_back(cardView);
    }
    auto mainCards = _model->getMainCards();
    for (const auto& card : mainCards) {
        CardView* cardView = CardView::create(card);
        _mainCards.push_back(cardView);
    }

    _cardWidth = _pileCards.empty() ? 0 : _pileCards[0]->getContentSize().width;
    initUndoButton();
    return true;
}

void GameView::initPileZone() {
    int cardIndex = 0;
    float pileZoneHeight = 580.0f;
    float pileZoneWidth = 1080.0f;
    float spacing = _cardWidth * 0.7f;
    for (auto& cardView : _pileCards) {
        Vec2 cardPosition = Vec2(0.7f * (cardIndex + 1) * _cardWidth, pileZoneHeight / 2);
        cardView->setPosition(cardPosition);
        if (!cardView->getParent())
            this->addChild(cardView, cardView->getModel()->getId());
        enablePileCardTouch(cardView);
        cardIndex++;
    }
}

// 初始化主牌区
void GameView::initMainZone() {
    const auto& mainCards = _model->getMainCards();
    for (const auto& cardView : _mainCards) {
        cardView->setPosition(cardView->getModel()->getPosition());
        if (!cardView->getParent())
            this->addChild(cardView, cardView->getModel()->getId());
        enableMainCardTouch(cardView);
    }
    _controller->updateCoverRelations();
}

// 初始化撤销按钮
void GameView::initUndoButton() {
    auto undoButton = ui::Button::create();
    undoButton->setTitleText("Undo");
    undoButton->setTitleFontSize(50);
    undoButton->setTitleColor(Color3B::WHITE);
    undoButton->setContentSize(Size(80, 40));


    float buttonX = 1080 - undoButton->getContentSize().width / 2 - 10;
    float buttonY = 580 / 2;

    undoButton->setPosition(Vec2(buttonX, buttonY));
    undoButton->setZOrder(100);

    this->addChild(undoButton);
    undoButton->addClickEventListener([this](Ref*) {
        if (_controller && _controller->getUndoManager()) {
            _controller->getUndoManager()->undoLastMove();
        }
        });
}

void GameView::showAllCards() {
    initMainZone();
    initPileZone();
    CardView* firstCardView = _pileCards[0];
    moveCardToHandZone(firstCardView);
    leftMove();
}

// 将卡牌从牌堆区或主牌区移动到手牌区
void GameView::moveCardToHandZone(CardView* cardView, bool isPileZone) {
    auto position = cardView->getPosition();
    int zOrder = cardView->getLocalZOrder();
    Vec2 handZonePos = Vec2(810, 290);
    cardView->setLocalZOrder(_handCards.size() + 1);

    auto moveAction = MoveTo::create(0.3f, handZonePos);

    auto callFunc = CallFunc::create([this, cardView, isPileZone, position, zOrder]() {

        _controller->getUndoManager()->recordCardMove(cardView, isPileZone ? "pile" : "main", position, zOrder);
        _eventDispatcher->removeEventListenersForTarget(cardView);
        if (isPileZone) {
            _model->addPileCardToHand(cardView->getModel());
            _pileCards.erase(std::remove(_pileCards.begin(), _pileCards.end(), cardView), _pileCards.end());
            rightMove();
        }
        else {
            _model->addMainCardToHand(cardView->getModel());
            _mainCards.erase(std::remove(_mainCards.begin(), _mainCards.end(), cardView), _mainCards.end()); // 从主牌区缓存中移除
            _controller->updateCoverRelations();
        }
        _handCards.push_back(cardView);
        });

    auto sequence = Sequence::create(moveAction, callFunc, nullptr);
    cardView->runAction(sequence);
}
// 绑定监听事件 牌堆和主牌
void GameView::enablePileCardTouch(CardView* cardView) {
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, cardView](Touch* touch, Event* event) {
        // 检测是否牌堆顶部
        auto parent = cardView->getParent();
        if (_pileCards.empty() || cardView->getModel()->getId() != _pileCards.back()->getModel()->getId()) {
            return false;
        }
        // 检测点击区域
        Rect rect = cardView->getBoundingBox();
        Vec2 touchPos = cardView->getParent()->convertToNodeSpace(touch->getLocation());
        if (rect.containsPoint(touchPos)) {
            // 调用移动函数
            moveCardToHandZone(cardView);
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardView);
}
void GameView::enableMainCardTouch(CardView* cardView) {
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, cardView](Touch* touch, Event* event) {
        // 检测是否可以点击
        if (!cardView->isClickable()) return false;
        // 检测点击区域
        Rect rect = cardView->getBoundingBox();
        Vec2 touchPos = cardView->getParent()->convertToNodeSpace(touch->getLocation());
        if (!rect.containsPoint(touchPos)) {
            return false;
        }

        CardModel* mainCardModel = cardView->getModel();
        CardModel* topHandCard = _model->getTopHandCard();
        if (!topHandCard || std::abs(mainCardModel->getFace() - topHandCard->getFace()) != 1) {
            return false;
        }
        // 调用移动函数
        moveCardToHandZone(cardView, false);
        return true;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardView);
}
// 堆牌区左右移动
void GameView::leftMove() {
    float spacing = _cardWidth * 0.7f;
    for (auto& cardView : _pileCards) {
        Vec2 currentPos = cardView->getPosition();
        Vec2 newPos = Vec2(currentPos.x - spacing, currentPos.y);
        cardView->setPosition(newPos);
    }
}
void GameView::rightMove() {
    float spacing = _cardWidth * 0.7f;
    for (auto& cardView : _pileCards) {
        Vec2 currentPos = cardView->getPosition();
        Vec2 newPos = Vec2(currentPos.x + spacing, currentPos.y);
        cardView->setPosition(newPos);
    }
}

void GameView::moveHandCardToMain() {
    _mainCards.push_back(_handCards.back());
    _handCards.pop_back();
}
void GameView::moveHandCardToPile() {
    _pileCards.push_back(_handCards.back());
    _handCards.pop_back();
}
GameController* GameView::getGameController() {
    return _controller;
}

std::vector<CardView*> GameView::getMainCards() {
    return _mainCards;
}