#ifndef LEVELCONFIGLOADER_H
#define LEVELCONFIGLOADER_H

#include "../../models/GameModel.h"
#include "cocos2d.h"

class LevelConfigLoader {
public:
    static GameModel* loadLevelFromFile(const std::string& filename);
};

#endif // LEVELCONFIGLOADER_H