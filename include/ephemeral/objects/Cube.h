#pragma once

#include <vector>

#include <ephemeral/objects/GameObject.h>
#include <ephemeral/Shader.h>
#include <ephemeral/Camera.h>

class Cube : public GameObject {

public:

    Cube(glm::vec3 initialPosition, glm::vec4 color);

    Cube(
        glm::vec3 initialPosition, 
        glm::vec4 sideOneColor,
        glm::vec4 sideTwoColor,
        glm::vec4 sideThreeColor,
        glm::vec4 sideFourColor,
        glm::vec4 sideFiveColor,
        glm::vec4 sideSixColor
    );

    void Draw(const Shader &shader, Camera &camera);

    glm::vec3 GetFrontFacePosition();

private:
    
    void InitializeData();

    uint32_t mVAO;
    uint32_t mVBO;

    std::vector<glm::vec4> mSideColors;

    float mVertices[108] = {
        -0.5f, -0.5f, 0.0f, // front face
        -0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,

        0.5f, 0.5f, 0.0f, // right side face
        0.5f, 0.5f, -1.0f,
        0.5f, -0.5f, -1.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, -1.0f,

        -0.5f, 0.5f, 0.0f, // left side face
        -0.5f, 0.5f, -1.0f,
        -0.5f, -0.5f, -1.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, -1.0f,

        -0.5f, -0.5f, -1.0f, // back face
        -0.5f, 0.5f, -1.0f,
        0.5f, -0.5f, -1.0f,
        0.5f, -0.5f, -1.0f,
        0.5f, 0.5f, -1.0f,
        -0.5f, 0.5f, -1.0f,

        -0.5f, 0.5f, 0.0f, // top face
        -0.5f, 0.5f, -1.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, -1.0f,
        -0.5f, 0.5f, -1.0f,

        -0.5f, -0.5f, 0.0f, // bottom face
        -0.5f, -0.5f, -1.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, -1.0f,
        -0.5f, -0.5f, -1.0f,
    };

};