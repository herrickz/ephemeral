
#include <ephemeral/objects/GameObject.h>

GameObject::GameObject(glm::vec3 initialPosition, glm::vec3 forwardVector) {
    mPosition = initialPosition;
    mForwardVector = forwardVector;
}

glm::vec3 GameObject::GetPosition() {
    return mPosition;
}

void GameObject::SetPosition(glm::vec3 position) {
    mPosition = position;
}

glm::vec3 GameObject::GetForwardVector() {
    return mForwardVector;
}

void GameObject::SetForwardVector(glm::vec3 forwardVector) {
    mForwardVector = forwardVector;
}

glm::quat GameObject::GetRotation() {
    return mRotation;
}

void GameObject::SetRotation(glm::quat rotation) {
    mRotation = rotation;
}