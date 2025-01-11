#pragma once

#include <string>


class Shader {

public:

    /**
     * @param vertexShaderSourcePath: Path to the shader source file
     * @param fragmentShaderSourcePath: Path to the shader source file
     */
    Shader(
        const std::string &vertexShaderSourcePath,
        const std::string &fragmentShaderSourcePath
    );
    ~Shader();

    void Use();

    uint32_t GetShaderProgramId() const;

private:

    uint32_t mShaderProgramId;

};