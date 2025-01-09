#pragma once

#include <string>
#include <ostream>
#include <ephemeral/Shader.h>
#include <ephemeral/Camera.h>
#include <ephemeral/Texture.h>

enum class SpriteType {
    PLAYER = 0,
    BRICK = 1,
    COIN = 2,
    ENEMY = 3,
};

class TexturedSquare {

public:

    /*
     * @param imagePath: Path to image data to be loaded
    */
    TexturedSquare(
        const std::string &id,
        const SpriteType &spriteType,
        Camera &camera,
        glm::vec3 initialPosition
    );

    void Draw(Shader &shader, Texture &texture);

    void OnUpdate(float deltaTime);

    void SetPosition(glm::vec3 position);
    glm::vec3 GetPosition() const;

    std::string GetId() const;
    SpriteType GetSpriteType() const;

private:

    std::string mId;
    SpriteType mSpriteType;
    Camera &mCamera;
    glm::vec3 mPosition;

    const float mAnimationTime = 1.0f;
    float mElapsedAnimationTime = 0.0f;
};