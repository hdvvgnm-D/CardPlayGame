#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "cocos2d.h"
#include "controllers/GameController.h"
#include "views/GameView.h"

class GameScene : public cocos2d::Scene {
public:
    CREATE_FUNC(GameScene);
    virtual bool init() override;
    virtual ~GameScene();

private:
    GameController* _gameController;
};

#endif // GAME_SCENE_H