#include "CardView.h"
#include "cocos2d.h"
using namespace cocos2d;

std::string CardView::getFaceString(CardFaceType face) {
    switch (face) {
    case CFT_ACE: return "A";
    case CFT_TWO: return "2";
    case CFT_THREE: return "3";
    case CFT_FOUR: return "4";
    case CFT_FIVE: return "5";
    case CFT_SIX: return "6";
    case CFT_SEVEN: return "7";
    case CFT_EIGHT: return "8";
    case CFT_NINE: return "9";
    case CFT_TEN: return "10";
    case CFT_JACK: return "J";
    case CFT_QUEEN: return "Q";
    case CFT_KING: return "K";
    default: return "unknown";
    }
}

std::string CardView::getSuitString(CardSuitType suit) {
    switch (suit) {
    case CST_CLUBS: return "club";
    case CST_DIAMONDS: return "diamond";
    case CST_HEARTS: return "heart";
    case CST_SPADES: return "spade";
    default: return "unknown";
    }
}

std::string CardView::getColorString(CardSuitType suit) {
    switch (suit) {
    case CST_CLUBS:
    case CST_SPADES:
        return "black";
    case CST_HEARTS:
    case CST_DIAMONDS:
        return "red";
    default:
        return "black";
    }
}

CardView* CardView::create(CardModel* model) {
    CardView* node = new CardView();
    if (node && node->init(model)) {
        node->autorelease();
        return node;
    }
    delete node;
    return nullptr;
}

bool CardView::init(CardModel* model) {
    if (!Sprite::init()) return false;

    _model = model;

    // 创建卡面
    std::string filename = "res/card_general.png";
    auto cardFront = Sprite::create(filename);
    if (!cardFront) {
        CCLOGERROR("Failed to load card base: %s", filename.c_str());
        return false;
    }

    this->setContentSize(cardFront->getContentSize());
    cardFront->setPosition(getContentSize() / 2);
    this->addChild(cardFront, 0);  // Z轴设为0（底层）

    std::string colorStr = getColorString(model->getSuit());

    // 添加大数字
    std::string faceStr = getFaceString(model->getFace());
    auto bigFaceSprite = Sprite::create("res/number/big_" + colorStr + "_" + faceStr + ".png");
    if (bigFaceSprite) {
        bigFaceSprite->setPosition(
            getContentSize().width * 0.5f,
            getContentSize().height * 0.4f
        );
        this->addChild(bigFaceSprite, 1);
    }

    // 添加小数字
    auto smallFaceSprite = Sprite::create("res/number/small_" + colorStr + "_" + faceStr + ".png");
    if (smallFaceSprite) {
        smallFaceSprite->setPosition(
            getContentSize().width * 0.25f,
            getContentSize().height * 0.85f
        );
        this->addChild(smallFaceSprite, 1);
    }

    // 添加花色
    std::string suitStr = getSuitString(model->getSuit());
    auto suitSprite = Sprite::create("res/suits/" + suitStr + ".png");
    if (suitSprite) {
        suitSprite->setPosition(
            getContentSize().width * 0.75f,
            getContentSize().height * 0.85f
        );
        this->addChild(suitSprite, 1);
    }

    this->setPosition(model->getPosition());

    return true;
}

void CardView::moveToPosition(cocos2d::Vec2 pos, float duration) {
    this->runAction(cocos2d::MoveTo::create(duration, pos));
}

CardModel* CardView::getModel() const { return _model; }

void CardView::setClickable(bool clickable) {
    _isClickable = clickable;
}

void CardView::setCoveredBy(CardView* coveredBy) {
    _coveredBy = coveredBy;
}
CardView* CardView::getCoveredBy() {
    return _coveredBy;
}

bool CardView::isClickable() {
    return _isClickable;
}