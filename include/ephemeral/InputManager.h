#pragma once

#include <glm/glm.hpp>

namespace InputManager {

static bool firstMouse = true;
static float mouseX = 0.0f;
static float mouseY = 0.0f;
static glm::vec3 normalizedMousePosition;

void Hello();

void SetMouse(double mouseX, double mouseY);

} // namespace InputManager
