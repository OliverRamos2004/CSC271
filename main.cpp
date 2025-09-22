#include <cmath>
#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderprogram.h"

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
    // std::string vertPath = std::string(SHADER_DIR) + "vertex.vert";
    // std::string fragPath = std::string(SHADER_DIR) + "fragment.frag";
    // ShaderProgram shaderProgram(vertPath, fragPath);
    // shaderProgram.use();

    // ------------------- TWO SHADER PROGRAM ----------

    std::string vertPath = std::string(SHADER_DIR) + "vertex.vert";

    std::string fragPathRed   = std::string(SHADER_DIR) + "fragment1.frag";
    std::string fragPathBlue  = std::string(SHADER_DIR) + "fragment2.frag";

    // Create two shader programs
    ShaderProgram progRed(vertPath, fragPathRed);
    ShaderProgram progBlue(vertPath, fragPathBlue);

    // progRed.use();
    // progBlue.use();


    // float vertices[] = {
    //     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    //     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    //     -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f
    // };

    float triA[] = {
        -0.8f, -0.2f, 0.0f,
        -0.2f, -0.2f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    unsigned int triA_indices[] = { 0, 1, 2 };

    float triB[] = {
        0.2f, -0.1f, 0.0f,
        0.6f, -0.1f, 0.0f,
        0.4f,  0.25f, 0.0f


   };
    unsigned int triB_indices[] = { 0, 1, 2 };


    //
    // unsigned int indices[] = {
    // 0, 1, 2,
    // 0, 2, 3
    //     };



    //      ------------ BUFFERS ------------
    // Vertex buffer object
    // Vertex array buffer object


    // Triangle A buffers
    unsigned int VAO_A, VBO_A, EBO_A;
    glGenVertexArrays(1, &VAO_A);
    glGenBuffers(1, &VBO_A);
    glGenBuffers(1, &EBO_A);

    glBindVertexArray(VAO_A);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_A);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triA), triA, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_A);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triA), triA_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Triangle B buffers
    unsigned int VAO_B, VBO_B, EBO_B;
    glGenVertexArrays(1, &VAO_B);
    glGenBuffers(1, &VBO_B);
    glGenBuffers(1, &EBO_B);

    glBindVertexArray(VAO_B);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_B);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triB), triB, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_B);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triB), triB_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


        //      --------------- RENDERING LOOP ------------------

    while (!glfwWindowShouldClose(window)) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw A in red

        glBindVertexArray(VAO_A);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        progRed.use();

        // draw B in blue

        glBindVertexArray(VAO_B);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        progBlue.use();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO_A);
    glDeleteBuffers(1, &VBO_A);
    glDeleteBuffers(1, &EBO_A);

    glDeleteVertexArrays(1, &VAO_B);
    glDeleteBuffers(1, &VBO_B);
    glDeleteBuffers(1, &EBO_B);

    // shaderProgram.destroy();
    // glfwTerminate();
    // return 0;

    progRed.destroy();
    progBlue.destroy();

}

// 09/04 -- This is a function

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << width << " " << height << std::endl;
}