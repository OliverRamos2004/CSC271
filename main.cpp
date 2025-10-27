#include <cmath>
#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderprogram.h"
#include "stb_image.h"
#include "mesh.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);  // 09/04 -- explain

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow *window = glfwCreateWindow(640, 480, "CSC271", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 09/04 -- explain

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // First Shader Program
    std::string vertPath = std::string(SHADER_DIR) + "vertex.vert";
    std::string fragPath = std::string(SHADER_DIR) + "fragment.frag";
    ShaderProgram shaderProgram(vertPath, fragPath);
    shaderProgram.use();

    const std::vector<float> vertices= {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f

    };

    const std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
            };

    const GLuint prog = shaderProgram.getID();
    const GLint posLoc = glGetAttribLocation(prog, "aPOS");
    const GLint colLoc = glGetAttribLocation(prog, "aColor");
    const GLint uvLoc = glGetAttribLocation(prog, "aTexCoord");

    const GLsizei stride = 8 * sizeof(float);
    std::vector<VertexAttribute> attributes = {
        VertexAttribute{static_cast<GLuint>(posLoc), 3, GL_FLOAT, GL_FALSE, stride, 0},
        VertexAttribute{static_cast<GLuint>(colLoc), 3, GL_FLOAT, GL_FALSE, stride, 3 *sizeof(float)},
        VertexAttribute{static_cast<GLuint>(uvLoc), 2, GL_FLOAT, GL_FALSE, stride, 6 *sizeof(float)}
    };

    std::vector<TextureSpec> textures = {
        TextureSpec{"texture1", 0, prog, 0, std::string(ASSETS_DIR) + "wall.jpg", false},
        TextureSpec{"texture2", 0, prog, 1, std::string(ASSETS_DIR) + "awesomeface.png", true}
    };



    Transformation M = {"model", prog, 0, glm::mat4(1.0f)};
    Transformation V = {"view", prog, 0, glm::mat4(1.0f)};
    Transformation P = {"projection", prog, 0, glm::mat4(1.0f)};
    Mesh quad(vertices, indices, attributes, textures, M, V, P); // ADD MOR E TRANSFORMATION MATRIX





    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // First container - rotating
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        quad.draw(model, view, projection);

        // Second container - Top-left corner, scaling over time
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(-0.8f, 0.8f, 0.0f));
        float scaleAmount = sin(glfwGetTime()) * 0.25f + 1.0f;
        model2 = glm::scale(model2, glm::vec3(scaleAmount, scaleAmount, scaleAmount));

        quad.draw(model2, view, projection);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    quad.cleanup();
    shaderProgram.destroy();
    glfwTerminate();
    return 0;


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << width << " " << height << std::endl;
}