//
// Created by Oliver Ramos on 9/9/2025.
//

#include "shaderprogram.h"
#include <fstream>
#include <sstream>
#include <iostream>



std::string ShaderProgram::loadShaderSource(const std::string &filePath) {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        shaderStream <<shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();

    }catch(std::ifstream::failure& e){
    std::cerr << "ERROR:: Unable to successfully read file " << filePath << std::endl;
    return "";
    }


}

GLuint ShaderProgram::compileShader(const std::string &source, GLenum shaderType) {
    const char* shaderCode = source.c_str();
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error compiling shader: " << infoLog << std::endl;
    }
     return shader;

}

void ShaderProgram::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "Error linking program: " << infoLog << std::endl;
    }
}

ShaderProgram::ShaderProgram(const std::string &vertex_path, const std::string &fragmentPath) {
    std::string vertexCode = loadShaderSource(vertex_path);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    GLuint vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

    linkProgram(vertexShader, fragmentShader);
}

ShaderProgram::~ShaderProgram() {
    destroy();
}

void ShaderProgram::use() const {
    glUseProgram(ID);
}

GLuint ShaderProgram::getID() const {
    return ID;
}

void ShaderProgram::destroy() {
    if(!isDeleted && ID != 0) {
        glDeleteProgram(ID);
        ID = 0;
        isDeleted = true;
    }
}