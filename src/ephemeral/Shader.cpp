
#include <ephemeral/Shader.h>
#include <ephemeral/Logger.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(
    const std::string &vertexShaderSourcePath,
    const std::string &fragmentShaderSourcePath
) {

    std::ifstream vertexShaderFileStream(vertexShaderSourcePath.c_str());
    std::ifstream fragmentShaderFileStream(fragmentShaderSourcePath.c_str());

    if(!vertexShaderFileStream) {
        LOG_I("Could not find: %s", vertexShaderSourcePath.c_str());
        return;
    }

    if(!fragmentShaderFileStream) {
        LOG_I("Could not find: %s", fragmentShaderSourcePath.c_str());
        return;
    }

    std::stringstream ss;

    ss << vertexShaderFileStream.rdbuf();
    vertexShaderFileStream.close();

    const std::string vertexShaderSourceString = ss.str();
    const char *vertexShaderSource = vertexShaderSourceString.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LOG_I("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
        LOG_I("%s", vertexShaderSourceString.c_str());
    }

    ss.str("");
    ss << fragmentShaderFileStream.rdbuf();

    const std::string fragmentShaderSourceString = ss.str();
    const char *fragmentShaderSource = fragmentShaderSourceString.c_str();

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        LOG_I("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
        LOG_I("%s", fragmentShaderSourceString.c_str());
    }

    mShaderProgramId = glCreateProgram();
    glAttachShader(mShaderProgramId, vertexShader);
    glAttachShader(mShaderProgramId, fragmentShader);
    glLinkProgram(mShaderProgramId);

    glGetProgramiv(mShaderProgramId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(mShaderProgramId, 512, NULL, infoLog);
        LOG_I("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(mShaderProgramId);
}

void Shader::Use() {
    glUseProgram(mShaderProgramId);
}

uint32_t Shader::GetShaderProgramId() {
    return mShaderProgramId;
}
