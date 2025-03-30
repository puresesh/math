#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

void runComputeShader(const std::string& shaderSource, GLuint workGroupSizeX, GLuint workGroupSizeY, GLuint workGroupSizeZ) {

    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    
    const char* source = shaderSource.c_str();
    glShaderSource(computeShader, 1, &source, nullptr);
    
    glCompileShader(computeShader);
    
    GLint success;
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(computeShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
        glDeleteShader(computeShader);
        return;
    }
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, computeShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        glDeleteProgram(shaderProgram);
        glDeleteShader(computeShader);
        return;
    }
    
    glUseProgram(shaderProgram);
    
    glDispatchCompute(workGroupSizeX, workGroupSizeY, workGroupSizeZ);
    
    // wait for completion
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    
    glDeleteProgram(shaderProgram);
    glDeleteShader(computeShader);
}

int main() {
    std::string computeShaderSource = R"(
        #version 430
        layout (local_size_x = 16, local_size_y = 16) in;

        void main() {
        }
    )";

    runComputeShader(computeShaderSource, 1, 1, 1);

    return 0;
}
