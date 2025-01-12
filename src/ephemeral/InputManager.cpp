

#include <ephemeral/InputManager.h>
#include <ephemeral/Settings.h>
#include <ephemeral/Logger.h>

const float MOUSE_Z = 0.0f;

void InputManager::SetMouse(double mouseX, double mouseY) {

    InputManager::mouseX = static_cast<float>(mouseX);
    InputManager::mouseY = static_cast<float>(mouseY);

    float xNormalized = (InputManager::mouseX / Settings::SCR_WIDTH) * 2 - 1;
    float yNormalized = (InputManager::mouseY / Settings::SCR_HEIGHT) * 2 - 1;

    InputManager::normalizedMousePosition  = { xNormalized, -yNormalized, MOUSE_Z };

    if (InputManager::firstMouse)
    {
        // lastX = xpos;
        // lastY = ypos;
        InputManager::firstMouse = false;

        LOG_I("Setting firstMouse false");
    }

    // float xoffset = xpos - lastX;
    // float yoffset = lastY - ypos;

    // lastX = xpos;
    // lastY = ypos;

}