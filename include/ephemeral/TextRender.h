#pragma once

#include <map>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <ephemeral/Shader.h>

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextRender {

public:
    TextRender(const std::string &fontPath);

    void Render(std::string text, float x, float y, float scale, glm::vec3 color);


private:

    std::map<GLchar, Character> Characters;
    Shader mShader;

    uint32_t mVao;
    uint32_t mVbo;

};