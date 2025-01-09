
#include <ephemeral/StaticLine.h>
#include <iostream>

StaticLine::StaticLine(glm::vec3 start, glm::vec3 end, glm::vec4 color, const Camera &camera):
    mColor(color),
    mCamera(camera) {

    float vertices[] = {
        start.x, start.y, start.z,
        end.x, end.y, end.z
    };

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

StaticLine::~StaticLine() {
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}

void StaticLine::Draw(Shader &shader) {
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view  = mCamera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    shader.Use();

    glBindVertexArray(mVAO);

    int colorLocation = glGetUniformLocation(shader.GetShaderProgramId(), "ourColor");
    glUniform4f(colorLocation, mColor.r, mColor.g, mColor.b, mColor.a);

    int modelLocation = glGetUniformLocation(shader.GetShaderProgramId(), "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

    int viewLocation = glGetUniformLocation(shader.GetShaderProgramId(), "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLocation = glGetUniformLocation(shader.GetShaderProgramId(), "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);
}