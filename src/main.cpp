#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ephemeral/Shader.h>
#include <ephemeral/TexturedSquare.h>
#include <ephemeral/Player.h>
#include <ephemeral/Direction.h>
#include <ephemeral/TextRender.h>
#include <ephemeral/Settings.h>
#include <ephemeral/StaticLine.h>
#include <ephemeral/Texture.h>
#include <ephemeral/LevelLoader.h>
#include <ephemeral/GameManager.h>
#include <ephemeral/AudioManager.h>
#include <ephemeral/Logger.h>

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>

std::ostream& operator<< (std::ostream& stream, const TexturedSquare& square) {

    glm::vec3 position = square.GetPosition();

    stream << "(" << square.GetId() << ")" << " " <<  position.x << ", " << position.y << std::endl;

    return stream;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void drawLines(glm::vec3 squarePosition, Shader &lineShader);

Camera camera;
std::unique_ptr<Player> player(std::make_unique<Player>(camera));

float deltaTime = 0.0f;

int main()
{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        LOG_F("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_F("Failed to initialize GLAD");
        return -1;
    }

    // Initialize all textures
    Texture brickTexture("resources/textures/bricks2.jpg");
    Texture playerTexture("resources/textures/matrix.jpg");
    Texture coinTexture("resources/textures/awesomeface.png");
    Texture enemyTexture("resources/textures/enemy.png");

    TextRender textRender("resources/fonts/Antonio-Regular.ttf");

    // Initialize all shaders
    Shader texturedShader("resources/shaders/shader.vs", "resources/shaders/shader.fs");
    Shader lineShader("resources/shaders/line.vs", "resources/shaders/line.fs");

    // Create all the squares using a cube position vector
    std::vector<std::unique_ptr<TexturedSquare>> squares;
    std::vector<glm::vec3> cubePositions;

    LevelLoader levelLoader;

    AudioManager::GetInstance().Play("resources/audio/breakout.mp3", 0);

    if(!levelLoader.Load(player, squares, camera)) {
        LOG_F("Could not load level");
        return -1;
    }

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    std::string framesPerSecond = "";
    glm::vec3 squarePosition = squares[0]->GetPosition();

    float lastShownFps = 0.0f;
    float lastFrameTime = glfwGetTime();

    GameManager &gameManager = GameManager::GetInstance();

    while (!glfwWindowShouldClose(window) && !gameManager.ShouldEndGame())
    {
        processInput(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        char currentFrameCharBuffer[100];
        snprintf(currentFrameCharBuffer, 100, "%.1f", currentFrame);
        std::string temp(currentFrameCharBuffer);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawLines(squarePosition, lineShader);

        for(const auto &square : squares) {
            if(square->GetSpriteType() == SpriteType::COIN) {
                square->Draw(texturedShader, coinTexture);
            } else if(square->GetSpriteType() == SpriteType::BRICK) {
                square->Draw(texturedShader, brickTexture);
            } else if(square->GetSpriteType() == SpriteType::ENEMY) {
                square->Draw(texturedShader, enemyTexture);
                square->OnUpdate(deltaTime);
            }
        }

        player->OnUpdate(deltaTime);
        player->Draw(texturedShader, playerTexture);

        textRender.Render(framesPerSecond, SCR_WIDTH * 0.03, SCR_HEIGHT * 0.9, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        textRender.Render("Coins: " + std::to_string(GameManager::GetInstance().GetCoinCount()), SCR_WIDTH * 0.03, SCR_HEIGHT * 0.1, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

        if(lastShownFps < (currentFrame - 0.5f)) {
            framesPerSecond = std::to_string(static_cast<int>(1.0f / deltaTime));
            lastShownFps = currentFrame;
        }

        player->DoCollisions(squares, deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    AudioManager::GetInstance().StopAll();

    return 0;
}

void drawLines(glm::vec3 squarePosition, Shader &lineShader) {

    float alpha = abs(cos(glfwGetTime()));

    glm::vec3 position = player->GetPosition();

    position.x -= 0.5;
    position.y -= 0.5;

    glm::vec4 color = { 0.0f, 1.0f, 1.0f, alpha };

    StaticLine line1(
        position,
        squarePosition,
        color,
        camera
    );

    position.x += 1.0;

    StaticLine line2(
        position,
        squarePosition,
        color,
        camera
    );

    position.y += 1.0;

    StaticLine line3(
        position,
        squarePosition,
        color,
        camera
    );

    position.x -= 1.0;

    StaticLine line4(
        position,
        squarePosition,
        color,
        camera
    );

    line1.Draw(lineShader);
    line2.Draw(lineShader);
    line3.Draw(lineShader);
    line4.Draw(lineShader);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_A)) {
        player->ProcessKeyboard(0, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        player->ProcessKeyboard(1, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE)) {
        player->OnJump();
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    printf("Resize: %d, %d\n", width, height);
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}