#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class GameObject {

public:

    GameObject(glm::vec3 initialPosition, glm::vec3 forwardVector);

    glm::vec3 GetForwardVector();
    void SetForwardVector(glm::vec3 forwardVector);

    glm::vec3 GetPosition();
    void SetPosition(glm::vec3 position);

    glm::quat GetRotation();
    void SetRotation(glm::quat rotation);

private:

    glm::vec3 mPosition;
    glm::vec3 mForwardVector;
    glm::quat mRotation = { 0.0f, 0.0f, 0.0f, 0.0f };

};