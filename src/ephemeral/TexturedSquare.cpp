
#include <ephemeral/TexturedSquare.h>
#include <ephemeral/Settings.h>
#include <ephemeral/Texture.h>

#include <iostream>
#include <stdexcept>
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TexturedSquare::TexturedSquare(
    const std::string &id,
    const SpriteType &spriteType,
    Camera &camera,
    glm::vec3 initialPosition
):
    mId(id),
    mSpriteType(spriteType),
    mCamera(camera),
    mPosition(initialPosition)
    {
}

SpriteType TexturedSquare::GetSpriteType() const {
    return mSpriteType;
}

void TexturedSquare::OnUpdate(float deltaTime) {
    mElapsedAnimationTime += deltaTime;

    const float MAGNITUDE = 0.02f;

    mPosition.x += MAGNITUDE * cos(mElapsedAnimationTime / mAnimationTime);
}

void TexturedSquare::SetPosition(glm::vec3 position) {
    mPosition = position;
}

void TexturedSquare::Draw(Shader &shader, Texture &texture) {

    shader.Use();
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view;

    model = glm::translate(model, mPosition);

    view  = mCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Settings::SCR_WIDTH / (float)Settings::SCR_HEIGHT, 0.1f, 100.0f);

    int modelLocation = glGetUniformLocation(shader.GetShaderProgramId(), "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

    int viewLocation = glGetUniformLocation(shader.GetShaderProgramId(), "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLocation = glGetUniformLocation(shader.GetShaderProgramId(), "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    texture.Use();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

glm::vec3 TexturedSquare::GetPosition() const {
    return mPosition;
}

std::string TexturedSquare::GetId() const {
    return mId;
}

