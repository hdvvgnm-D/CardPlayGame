#include "LevelConfigLoader.h"
#include "json/document.h"
#include "cocos2d.h"
using namespace rapidjson;

//解析关卡配置文件
GameModel* LevelConfigLoader::loadLevelFromFile(const std::string& filename) {
    GameModel* gameModel = new GameModel();

    std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
    Document doc;
    doc.Parse(content.c_str());
    int id = 0;
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        for (auto& obj : doc["Playfield"].GetArray()) {
            int face = obj["CardFace"].GetInt();
            int suit = obj["CardSuit"].GetInt();
            float x = obj["Position"]["x"].GetFloat();
            float y = obj["Position"]["y"].GetFloat();

            CardModel* card = new CardModel(
                id++,
                static_cast<CardFaceType>(face),
                static_cast<CardSuitType>(suit),
                cocos2d::Vec2(x, y + 580)
            );
            gameModel->addMainCard(card);
        }
    }

    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        for (auto& obj : doc["Stack"].GetArray()) {
            int face = obj["CardFace"].GetInt();
            int suit = obj["CardSuit"].GetInt();

            CardModel* card = new CardModel(
                id++,
                static_cast<CardFaceType>(face),
                static_cast<CardSuitType>(suit),
                cocos2d::Vec2(0, 0)
            );
            gameModel->addPileCard(card);
        }
    }
    return gameModel;
}