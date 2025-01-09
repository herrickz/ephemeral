
#include <ephemeral/Player.h>
#include <ephemeral/GameManager.h>
#include <ephemeral/AudioManager.h>

#include <iostream>
#include <fstream>

Player::Player(Camera &camera):
    mTexturedSquare(
        "player",
        SpriteType::PLAYER,
        camera,
        { 0.0f, 0.0f, -5.0f }
    ),
    mCamera(camera),
    mPosition({ 0.0f, -0.01f, -5.0f }) {

}

void Player::Draw(Shader &shader, Texture &texture) {
    mTexturedSquare.Draw(shader, texture);
}

void Player::ProcessKeyboard(int direction, float deltaTime) {
    if(direction == 0) {
        mPosition.x -= mSpeed * deltaTime;
        mTexturedSquare.SetPosition(mPosition);
        mCamera.SetPosition(mPosition.x, mPosition.y);
    }

    if(direction == 1) {
        mPosition.x += mSpeed * deltaTime;
        mTexturedSquare.SetPosition(mPosition);
        mCamera.SetPosition(mPosition.x, mPosition.y);
    }

}

void Player::OnUpdate(float deltaTime) {

    mYVelocity -= mGravity * deltaTime;

    mPosition.y += mYVelocity * deltaTime;
    mTexturedSquare.SetPosition(mPosition);
    mCamera.SetPosition(mPosition.x, mPosition.y);

}

void Player::SetPosition(glm::vec3 position) {
    mPosition = position;
}

void Player::DoCollisions(std::vector<std::unique_ptr<TexturedSquare>> &squares, float deltaTime) {

    std::vector<int> tilesToDelete;

    for(int i = 0; i < squares.size(); i++) {

        // TexturedSquare *square = squares[i];

        glm::vec3 squarePosition = squares[i]->GetPosition();
        glm::vec3 playerPosition = this->GetPosition();

        bool xCollision = playerPosition.x + 1 >= squarePosition.x && squarePosition.x + 1 >= playerPosition.x;
        bool yCollision = playerPosition.y + 1 >= squarePosition.y && squarePosition.y + 1 >= playerPosition.y;

        if (xCollision && yCollision) {

            if(squares[i]->GetSpriteType() == SpriteType::COIN) {
                GameManager::GetInstance().AddCoin();
                tilesToDelete.push_back(i);
                 AudioManager::GetInstance().Play("resources/audio/coin.mp3", 2);
                continue;
            } else if(squares[i]->GetSpriteType() == SpriteType::ENEMY) {
                GameManager::GetInstance().EndGame();
                continue;
            }

            float adjacent = abs(squarePosition.x - playerPosition.x);
            float opposite = abs(squarePosition.y - playerPosition.y);

            float theta = glm::degrees(atan(opposite/adjacent));
            float angle = 90.0f;

            if(playerPosition.x > squarePosition.x) {
                if(playerPosition.y > squarePosition.y) { // q1
                    angle = theta;
                } else { // q4
                    angle = 360.0f - theta;
                }
            } else {
                if(playerPosition.y > squarePosition.y) { // q2
                    angle = 180.0f - theta;
                } else { // q3
                    angle = 180.0f + theta;
                }
            }

            if(angle > 45 && angle < 135) {
                float offset = abs(playerPosition.y - (squarePosition.y + 1));
                this->PushUp(offset);

                if(mYVelocity < 0.0f) {
                    mCanJump = true;
                    mYVelocity = 0.0f;
                }
            } else if (angle > 135 && angle < 225) {
                float offset = abs((playerPosition.x + 1) - squarePosition.x);
                this->PushLeft(offset);
            } else if (angle > 225 && angle < 315) {
                float offset = abs((playerPosition.y + 1) - squarePosition.y);
                this->PushDown(offset);
                mYVelocity = 0.0f;
            } else {
                this->PushRight(deltaTime);
            }
        }

    }

    for(int j = tilesToDelete.size() - 1; j >= 0; j--) {
        squares.erase(squares.begin() + tilesToDelete[j]);
    }

}


void Player::OnJump() {
    if(mCanJump) {
        mCanJump = false;
        mYVelocity = mInitialJumpVelocity;

        AudioManager::GetInstance().Play("resources/audio/powerup.mp3", 1);
    }
}

glm::vec3 Player::GetPosition() {
    return mPosition;
}

void Player::PushLeft(float offset) {
    mPosition.x -= offset;
    mTexturedSquare.SetPosition(mPosition);
    mCamera.SetPosition(mPosition.x, mPosition.y);
}

void Player::PushRight(float deltaTime) {
    mPosition.x += mSpeed * deltaTime;
    mTexturedSquare.SetPosition(mPosition);
    mCamera.SetPosition(mPosition.x, mPosition.y);
}

void Player::PushUp(float offset) {
    mPosition.y += offset;
    mTexturedSquare.SetPosition(mPosition);
    mCamera.SetPosition(mPosition.x, mPosition.y);
}

void Player::PushDown(float deltaTime) {
    mPosition.y -= mSpeed * deltaTime;
    mTexturedSquare.SetPosition(mPosition);
    mCamera.SetPosition(mPosition.x, mPosition.y);
}