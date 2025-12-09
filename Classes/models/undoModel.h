#ifndef UNDO_MODEL_H
#define UNDO_MODEL_H

#include "cocos2d.h"
#include "views/CardView.h"

USING_NS_CC;

class UndoModel
{
public:
    UndoModel(CardView* view, const std::string& zone, Vec2 position, int zOrder);
    void setZone(const std::string& zone);
    std::string& getZone();
    void setView(CardView* view);
    CardView* getView();
    void setPosition(Vec2 position);
    Vec2 getPosition();
    void setZOrder(int zOrder);
    int getZOrder();



private:
    CardView* _view;
    std::string _zone;
    Vec2 _position;
    int zOrder;
};

#endif