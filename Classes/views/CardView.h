#ifndef CARDVIEW_H
#define CARDVIEW_H

#include "cocos2d.h"
#include "../models/CardModel.h"

class CardView : public cocos2d::Sprite {
public:
    static CardView* create(CardModel* model);
    bool init(CardModel* model);
    void moveToPosition(cocos2d::Vec2 pos, float duration = 0.3f);
    std::string CardView::getSuitString(CardSuitType suit);
    std::string CardView::getFaceString(CardFaceType face);
    std::string CardView::getColorString(CardSuitType suit);
    CardModel* getModel() const;
    void setClickable(bool clickable);
    void setCoveredBy(CardView* coveredBy);
    CardView* getCoveredBy();
    bool isClickable();

private:
    CardModel* _model;
    CardView* _coveredBy;
    bool _isClickable;

};

#endif // CARDVIEW_H