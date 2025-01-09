#pragma once

#include <vector>

#include <ephemeral/TexturedSquare.h>
#include <ephemeral/Camera.h>
#include <ephemeral/Shader.h>
#include <ephemeral/Direction.h>
#include <ephemeral/Texture.h>


class Player {

public:
    Player(Camera &camera);

    void Draw(Shader &shader, Texture &texture);

    void OnUpdate(float deltaTime);
    void OnJump();

    void ProcessKeyboard(int direction, float deltaTime);

    void DoCollisions(std::vector<std::unique_ptr<TexturedSquare>> &squares, float deltaTime);

    glm::vec3 GetPosition();
    void SetPosition(glm::vec3 position);

    void PushLeft(float deltaTime);
    void PushRight(float deltaTime);
    void PushUp(float deltaTime);
    void PushDown(float deltaTime);

private:

    TexturedSquare mTexturedSquare;
    bool mCanJump = true;
    Camera &mCamera;

    glm::vec3 mPosition;

    float mSpeed = 5.0f;
    float mInitialJumpVelocity = 10.0f;
    float mGravity = 10.0f;
    float mYVelocity = 0.0f;
};