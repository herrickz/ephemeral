#pragma once


class GameManager {

public:

    static GameManager& GetInstance();

    int GetCoinCount();
    void AddCoin();

    bool ShouldEndGame();
    void EndGame();

private:

    int mCoinCount = 0;
    bool mEndGame = false;

    GameManager() { };
};