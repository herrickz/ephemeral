#pragma once

#include <stdexcept>
#include <iostream>
#include <string>
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture {

public:

    Texture(
        const std::string &imagePath
    );

    void Use();

private:

    uint32_t mTextureId;
    uint32_t mVAO;

};