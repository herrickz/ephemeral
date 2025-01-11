

#include <ephemeral/InputManager.h>
#include <ephemeral/Settings.h>
#include <ephemeral/Logger.h>

void InputManager::Hello() {

}

void InputManager::SetMouse(double mouseX, double mouseY) {

    InputManager::mouseX = static_cast<float>(mouseX);
    InputManager::mouseY = static_cast<float>(mouseY);

    float xNormalized = (InputManager::mouseX / SCR_WIDTH) * 2 - 1;
    float yNormalized = (InputManager::mouseY / SCR_HEIGHT) * 2 - 1;

    InputManager::normalizedMousePosition  = { xNormalized, -yNormalized, 0.0f };

    LOG_I("mouse: %f, %f", InputManager::normalizedMousePosition.x, InputManager::normalizedMousePosition.y);

    // if (firstMouse)
    // {
    //     lastX = xpos;
    //     lastY = ypos;
    //     firstMouse = false;
    // }

    // float xoffset = xpos - lastX;
    // float yoffset = lastY - ypos;

    // lastX = xpos;
    // lastY = ypos;

}