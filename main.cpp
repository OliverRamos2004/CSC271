#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shaderprogram.h"
#include "stb_image.h"
#include "mesh.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int SCR_WIDTH = 1000;
int SCR_LENGTH = 800;
Camera camera;

// Custom LookAt
glm::mat4 MyLookAt(const glm::vec3& position,
                   const glm::vec3& target,
                   const glm::vec3& up) {
    glm::vec3 zaxis = glm::normalize(position - target);
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(up), zaxis));
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);

    glm::mat4 view(1.0f);
    view[0][0] = xaxis.x; view[1][0] = xaxis.y; view[2][0] = xaxis.z; view[3][0] = -glm::dot(xaxis, position);
    view[0][1] = yaxis.x; view[1][1] = yaxis.y; view[2][1] = yaxis.z; view[3][1] = -glm::dot(yaxis, position);
    view[0][2] = zaxis.x; view[1][2] = zaxis.y; view[2][2] = zaxis.z; view[3][2] = -glm::dot(zaxis, position);

    return view;
}

// Material struct used in C++
struct Mat {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};


// Mat materialsSet1[8] = {
//     {{0.0215f, 0.1745f, 0.0215f}, {0.07568f, 0.61424f, 0.07568f}, {0.633f, 0.727811f, 0.633f}, 0.6f},
//
//     {{0.25f, 0.20725f, 0.20725f}, {1.0f, 0.829f, 0.829f}, {0.296648f, 0.296648f, 0.296648f}, 0.088f},
//
//     {{0.2125f, 0.1275f, 0.054f}, {0.714f, 0.4284f, 0.18144f}, {0.393548f, 0.271906f, 0.166721f}, 0.2f},
//
//     {{0.24725f, 0.1995f, 0.0745f}, {0.75164f, 0.60648f, 0.22648f}, {0.628281f, 0.555802f, 0.366065f}, 0.4f},
//
//     {{0.0f, 0.1f, 0.06f}, {0.0f, 0.509804f, 0.509804f}, {0.501961f, 0.501961f, 0.501961f}, 0.25f},
//
//     {{0.0f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f}, {0.7f, 0.6f, 0.6f}, 0.25f},
//
//     {{0.0f, 0.05f, 0.0f}, {0.4f, 0.5f, 0.4f}, {0.04f, 0.7f, 0.04f}, 0.0781f},
//
//     {{0.05f, 0.05f, 0.0f}, {0.5f, 0.5f, 0.4f}, {0.7f, 0.7f, 0.04f}, 0.0781f}
// };
//
//

Mat materialsSet1[8] = {
    // Jade
    {{0.135f, 0.2225f, 0.1575f}, {0.54f, 0.89f, 0.63f}, {0.316228f, 0.316228f, 0.316228f}, 0.1f},

    // Obsidian
    {{0.05375f, 0.05f, 0.06625f}, {0.18275f, 0.17f, 0.22525f}, {0.332741f, 0.328634f, 0.346435f}, 0.3f},

    // Ruby
    {{0.1745f, 0.01175f, 0.01175f}, {0.61424f, 0.04136f, 0.04136f}, {0.727811f, 0.626959f, 0.626959f}, 0.6f},

    // Turquoise
    {{0.1f, 0.18725f, 0.1745f}, {0.396f, 0.74151f, 0.69102f}, {0.297254f, 0.30829f, 0.306678f}, 0.1f},

    // Brass
    {{0.329412f, 0.223529f, 0.027451f}, {0.780392f, 0.568627f, 0.113725f}, {0.992157f, 0.941176f, 0.807843f}, 0.217949f},

    // Chrome
    {{0.25f, 0.25f, 0.25f}, {0.4f, 0.4f, 0.4f}, {0.774597f, 0.774597f, 0.774597f}, 0.6f},

    // Copper
    {{0.19125f, 0.0735f, 0.0225f}, {0.7038f, 0.27048f, 0.0828f}, {0.256777f, 0.137622f, 0.086014f}, 0.1f},

    // Silver
    {{0.19225f, 0.19225f, 0.19225f}, {0.50754f, 0.50754f, 0.50754f}, {0.508273f, 0.508273f, 0.508273f}, 0.4f}
};


// 8 cube positions
glm::vec3 cubePositions[8] = {
    {-3.0f,  1.5f, 0.0f},
    {-1.0f,  1.5f, 0.0f},
     {1.0f,  1.5f, 0.0f},
     {3.0f,  1.5f, 0.0f},
    {-3.0f, -1.5f, 0.0f},
    {-1.0f, -1.5f, 0.0f},
     {1.0f, -1.5f, 0.0f},
     {3.0f, -1.5f, 0.0f}
};

void applyMaterial(ShaderProgram &shader, const Mat &m) {
    shader.setUniform("material.ambient",  m.ambient);
    shader.setUniform("material.diffuse",  m.diffuse);
    shader.setUniform("material.specular", m.specular);
    shader.setUniform("material.shininess", m.shininess);
}


int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_LENGTH, "Materials Display", nullptr, nullptr);
    if (!window) return -1;

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    ShaderProgram containerShaderProgram(std::string(SHADER_DIR) + "cube_vertex.vert",
                                         std::string(SHADER_DIR) + "container_fragment.frag");

    Mesh container(std::string(ASSETS_DIR) + "box.obj", containerShaderProgram.getID());

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.ProcessKeyboard(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        containerShaderProgram.use();

        glm::vec3 lightPos(4.0f, 3.0f, 3.0f);
        containerShaderProgram.setUniform("viewPos", camera.Position);
        containerShaderProgram.setUniform("pointLight.position", lightPos);
        containerShaderProgram.setUniform("pointLight.ambient",  glm::vec3(0.2f));
        containerShaderProgram.setUniform("pointLight.diffuse",  glm::vec3(0.8f));
        containerShaderProgram.setUniform("pointLight.specular", glm::vec3(1.0f));
        containerShaderProgram.setUniform("pointLight.constant", 1.0f);
        containerShaderProgram.setUniform("pointLight.linear", 0.09f);
        containerShaderProgram.setUniform("pointLight.quadratic", 0.032f);

        glm::mat4 projection = camera.GetProjection((float)SCR_WIDTH / SCR_LENGTH);
        glm::mat4 view = MyLookAt(camera.Position, camera.Position + camera.Front, camera.Up);
        containerShaderProgram.setUniform("view", view);
        containerShaderProgram.setUniform("projection", projection);

        // Bind diffuse texture
        // containerShaderProgram.bindTexture2D("material.diffuse",
        //     std::string(ASSETS_DIR) + "container2.png", 0, false);

        // Draw 8 cubes
        for (int i = 0; i < 8; i++) {
            applyMaterial(containerShaderProgram, materialsSet1[i]);
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            containerShaderProgram.setUniform("model", model);
            container.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (camera.firstMouse) {
        camera.lastX = xpos;
        camera.lastY = ypos;
        camera.firstMouse = false;
    }
    float xoffset = xpos - camera.lastX;
    float yoffset = camera.lastY - ypos;
    camera.lastX = xpos;
    camera.lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
