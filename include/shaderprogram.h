// Created by Oliver Ramos on 9/9/25

#ifndef DEMO_SHADERPROGRAM_H
#define DEMO_SHADERPROGRAM_H

#include <string>
#include <glad/glad.h>
#include "GLFW/glfw3.h"


class ShaderProgram{
  public:
    ShaderProgram(const std::string& vertex_path, const std::string& fragmentPath); // Constructor must be public
    ~ShaderProgram();

    void use() const;
    GLuint getID() const;

    void set1i(const char* name, int v); // HW2 10/05/25
    void set1f(const char* name, float v);

    void destroy();

    private:
      GLuint ID;
      bool isDeleted = false;
      std::string loadShaderSource(const std::string& filePath); // FIX THIS
      GLuint compileShader(const std::string& source, GLenum shaderType);
      void linkProgram(GLuint vertexShader, GLuint fragmentShader);
};
#endif