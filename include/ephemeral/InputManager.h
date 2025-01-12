#pragma once

#include <glm/glm.hpp>

namespace InputManager {

inline bool firstMouse = true;
inline float mouseX = 0.0f;
inline float mouseY = 0.0f;
inline glm::vec3 normalizedMousePosition;

void SetMouse(double mouseX, double mouseY);

} // namespace InputManager
