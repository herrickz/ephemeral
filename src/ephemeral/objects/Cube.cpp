
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ephemeral/objects/Cube.h>
#include <ephemeral/InputManager.h>
#include <ephemeral/Settings.h>

const glm::vec3 FORWARD_VECTOR = { -1.0f, 0.0f, 0.0f };

Cube::Cube(glm::vec3 initialPosition, glm::vec4 color):
    GameObject::GameObject(initialPosition, FORWARD_VECTOR) {

    mSideColors.push_back(color);
    InitializeData();
}

void Cube::InitializeData() {

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertices) * sizeof(float), mVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
}

Cube::Cube(
        glm::vec3 initialPosition, 
        glm::vec4 sideOneColor,
        glm::vec4 sideTwoColor,
        glm::vec4 sideThreeColor,
        glm::vec4 sideFourColor,
        glm::vec4 sideFiveColor,
        glm::vec4 sideSixColor
): 
    GameObject::GameObject(initialPosition, FORWARD_VECTOR) {

    mSideColors.push_back(sideOneColor);
    mSideColors.push_back(sideTwoColor);
    mSideColors.push_back(sideThreeColor);
    mSideColors.push_back(sideFourColor);
    mSideColors.push_back(sideFiveColor);
    mSideColors.push_back(sideSixColor);

    InitializeData();
}

void Cube::Draw(const Shader &shader, Camera &camera) {

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view;

    // float currentTime = glfwGetTime();
    // deltaTime = currentTime - lastTime;
    // lastTime = currentTime;

    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 actualCubePosition = { -0.1f, -0.1f, 0.0f };
    glm::vec3 normalizedDirectionVector = glm::normalize(InputManager::normalizedMousePosition - actualCubePosition);
    glm::vec3 rotationVector = glm::cross(GetForwardVector(), normalizedDirectionVector);

    glm::quat result;

    rotationVector = glm::normalize(rotationVector);

    float dotProduct = glm::clamp(glm::dot(GetForwardVector(), normalizedDirectionVector), -1.0f, 1.0f);
    float angle = glm::acos(dotProduct);

    glm::vec3 scaledRotationVector = glm::sin(angle / 2) * rotationVector;

    result = glm::quat(glm::cos(angle / 2),
                    scaledRotationVector.x,
                    scaledRotationVector.y,
                    scaledRotationVector.z);

    // LOG_I("Quaternion: %f,%f,%f,%f",
    //     result.x,
    //     result.y,
    //     result.z,
    //     result.w
    // );

    glm::quat finalRotation;

    if ( !InputManager::firstMouse ) {
        finalRotation = glm::slerp(GetRotation(), result, 0.005f);
    } else {
        finalRotation = GetRotation();
    }

    glm::mat4 rotationMatrix = glm::mat4_cast(finalRotation); // Alternative to glm::toMat4

    SetRotation(finalRotation);

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), GetPosition());
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    view  = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    int modelLocation = glGetUniformLocation(shader.GetShaderProgramId(), "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int viewLocation = glGetUniformLocation(shader.GetShaderProgramId(), "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLocation = glGetUniformLocation(shader.GetShaderProgramId(), "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    int colorLocation = glGetUniformLocation(shader.GetShaderProgramId(), "inputColor");
    glUniform4f(colorLocation, 1.0f, 0.0f, 0.0f, 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUniform4f(colorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 6, 6);

    glUniform4f(colorLocation, 1.0f, 0.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 12, 6);

    glUniform4f(colorLocation, 0.5f, 0.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 18, 6);

    glUniform4f(colorLocation, 0.0f, 0.5f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 24, 6);

    glUniform4f(colorLocation, 0.5f, 0.5f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 30, 6);

}
