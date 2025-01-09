#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ephemeral/Shader.h>
#include <ephemeral/Settings.h>
#include <ephemeral/Camera.h>

class StaticLine {

public:

    StaticLine(glm::vec3 start, glm::vec3 end, glm::vec4 color, const Camera &camera);
    ~StaticLine();

    void Draw(Shader &shader);

private:

    glm::vec4 mColor;

    uint32_t mVAO;
    uint32_t mVBO;

    const Camera &mCamera;
};