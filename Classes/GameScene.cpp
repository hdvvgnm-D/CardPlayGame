#include "GameScene.h"
USING_NS_CC;

bool GameScene::init() {
    // 先调用父类的init函数
    if (!Scene::init()) {
        return false;
    }

    // 创建并初始化游戏控制器
    _gameController = new GameController();
    _gameController->startGame("level_1.json");
    this->addChild(_gameController->getView());

    return true;
}

GameScene::~GameScene() {
    CC_SAFE_DELETE(_gameController);
}