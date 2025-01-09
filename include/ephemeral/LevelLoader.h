#pragma once

#include <ephemeral/Player.h>
#include <ephemeral/TexturedSquare.h>
#include <ephemeral/Camera.h>
#include <vector>

namespace TileType {

    const char BRICK = 'b';
    const char PLAYER = 'p';
    const char COIN = 'c';
    const char ENEMY = 'e';

} // namespace TileType

class LevelLoader {

public:

    LevelLoader();

    bool Load(std::unique_ptr<Player> &player, std::vector<std::unique_ptr<TexturedSquare>> &tiles, Camera &camera);

private:

    std::string mLevelFilePath = "resources/levels/main.lvl";

};