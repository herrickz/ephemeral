
#include <ephemeral/LevelLoader.h>
#include <ephemeral/Logger.h>

#include <fstream>
#include <iostream>

LevelLoader::LevelLoader() {

}

bool LevelLoader::Load(std::unique_ptr<Player> &player, std::vector<std::unique_ptr<TexturedSquare>> &tiles, Camera &camera) {

    // open the file
    std::fstream file(mLevelFilePath);

    if(file.is_open()) {
        // loop over the lines
        std::string lineContents;

        int xCoordinate = 0;
        int yCoordinate = 0;

        int brickCount = 0;
        int coinCount = 0;
        int enemyCount = 0;

        bool playerLoaded = false;

        while(std::getline(file, lineContents)) {

            xCoordinate = 0;

            for(char c : lineContents) {

                if(c == TileType::BRICK) {

                    glm::vec3 position = { xCoordinate, -yCoordinate, -5.0f};

                    tiles.push_back(
                        std::make_unique<TexturedSquare>(
                            "Brick " + std::to_string(brickCount),
                            SpriteType::BRICK,
                            camera,
                            position
                        )
                    );

                    brickCount++;

                } else if(c == TileType::PLAYER) {

                    if(playerLoaded) {
                        LOG_F("Player already loaded");
                        return false;
                    }

                    player->SetPosition({ xCoordinate, -yCoordinate, -5.0f});

                    playerLoaded = true;

                } else if(c == ' ') {

                } else if(c == TileType::COIN) {

                    glm::vec3 position = { xCoordinate, -yCoordinate, -5.0f};

                    tiles.push_back(
                        std::make_unique<TexturedSquare>(
                            "Coin " + std::to_string(coinCount),
                            SpriteType::COIN,
                            camera,
                            position
                        )
                    );
                } else if(c == TileType::ENEMY) {

                    glm::vec3 position = { xCoordinate, -yCoordinate, -5.0f};

                    tiles.push_back(
                        std::make_unique<TexturedSquare>(
                            "Enemy " + std::to_string(enemyCount),
                            SpriteType::ENEMY,
                            camera,
                            position
                        )
                    );

                    enemyCount++;

                } else {
                    LOG_F("Unrecognized character: %c", c);
                    return false;
                }
                xCoordinate++;
            }
            yCoordinate++;
        }

    } else {
        LOG_I("Could not load level: %s", mLevelFilePath.c_str());
    }

    return true;


}