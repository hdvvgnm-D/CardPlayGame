#include"undoModel.h"


void UndoModel::setZone(const std::string& zone) {
    _zone = zone;
}
std::string& UndoModel::getZone() {
    return _zone;
}
void UndoModel::setView(CardView* view) {
    _view = view;
}
CardView* UndoModel::getView() {
    return _view;
}
void UndoModel::setPosition(Vec2 position) {
    _position = position;
}
Vec2 UndoModel::getPosition() {
    return _position;
}
void UndoModel::setZOrder(int zOrder) {
    this->zOrder = zOrder;
}
int UndoModel::getZOrder() {
    return zOrder;
}
UndoModel::UndoModel(CardView* view, const std::string& zone, Vec2 position, int zOrder)
    : _view(view), _zone(zone), _position(position), zOrder(zOrder) {
}