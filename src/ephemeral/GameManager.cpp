
#include <ephemeral/GameManager.h>

GameManager& GameManager::GetInstance() {
    static GameManager gameManager;
    return gameManager;
}

int GameManager::GetCoinCount() {
    return mCoinCount;
}

void GameManager::AddCoin() {
    mCoinCount++;
}

bool GameManager::ShouldEndGame() {
    return mEndGame;
}

void GameManager::EndGame() {
    mEndGame = true;
}
