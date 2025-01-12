#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

const float CAMERA_INITIAL_X = 0.0f;
const float CAMERA_INITIAL_Y = 0.0f;
const float CAMERA_INITIAL_Z = 40.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {

public:

    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // euler Angles
    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(
        glm::vec3 position = glm::vec3(CAMERA_INITIAL_X, CAMERA_INITIAL_Y, CAMERA_INITIAL_Z),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH
    );

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 GetViewMatrix() const;

    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);

    glm::vec3 GetPosition();

    void SetPosition(float x, float y);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
