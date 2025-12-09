#ifndef CARDMODEL_H
#define CARDMODEL_H

#include "cocos2d.h"

// 花色类型
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,    // 梅花
    CST_DIAMONDS, // 方块
    CST_HEARTS,   // 红桃
    CST_SPADES,   // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

// 正面类型
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

class CardModel
{
public:
    CardModel();
    CardModel(int id, CardFaceType face, CardSuitType suit, cocos2d::Vec2 position);
    int getId() const;
    CardFaceType getFace() const;
    CardSuitType getSuit() const;
    cocos2d::Vec2 getPosition() const;
    void setPosition(cocos2d::Vec2 pos);

private:
    int _id;                 // 卡牌ID
    CardSuitType _suit;      // 花色
    CardFaceType _face;      // 正面
    cocos2d::Vec2 _position; // 卡牌位置

};

#endif