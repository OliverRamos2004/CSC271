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
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int SCR_WIDTH = 800;
int SCR_LENGTH = 600;
Camera camera;

// Custom LookAt (manual view matrix)
glm::mat4 MyLookAt(const glm::vec3& position,
                   const glm::vec3& target,
                   const glm::vec3& up) {
    glm::vec3 zaxis = glm::normalize(position - target);                 // forward
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(up), zaxis)); // right
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);                          // up

    glm::mat4 view(1.0f);
    view[0][0] = xaxis.x; view[1][0] = xaxis.y; view[2][0] = xaxis.z; view[3][0] = -glm::dot(xaxis, position);
    view[0][1] = yaxis.x; view[1][1] = yaxis.y; view[2][1] = yaxis.z; view[3][1] = -glm::dot(yaxis, position);
    view[0][2] = zaxis.x; view[1][2] = zaxis.y; view[2][2] = zaxis.z; view[3][2] = -glm::dot(zaxis, position);


    return view;
}


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_LENGTH, "Hello CG", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    std::string vertPath = std::string(SHADER_DIR) + "cube_vertex.vert";
    std::string fragPath = std::string(SHADER_DIR) + "container_fragment.frag";
    ShaderProgram containerShaderProgram(vertPath, fragPath);

    vertPath = std::string(SHADER_DIR) + "cube_vertex.vert";
    fragPath = std::string(SHADER_DIR) + "light_fragment.frag";
    ShaderProgram lightShaderProgram(vertPath, fragPath);

    Mesh container(std::string(ASSETS_DIR) + "box.obj", containerShaderProgram.getID());
    Mesh light(std::string(ASSETS_DIR) + "box.obj", lightShaderProgram.getID());


    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.ProcessKeyboard(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        containerShaderProgram.use();
        glm::vec3 lightPos(1.5f, 0.0f, 0.0f);
        glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = MyLookAt(camera.Position, camera.Position + camera.Front, camera.Up); // Changed this
        glm::mat4 projection = camera.GetProjection(SCR_WIDTH/SCR_LENGTH);

        containerShaderProgram.setUniform("lightPos", lightPos);
        //containerShaderProgram.setUniform("objectColor", objectColor);
        containerShaderProgram.setUniform("lightColor", lightColor);
        containerShaderProgram.setUniform("viewPos", camera.Position);

        containerShaderProgram.setUniform("model", model);
        containerShaderProgram.setUniform("view", view);
        containerShaderProgram.setUniform("projection", projection);

        containerShaderProgram.setUniform("material.ambient", glm::vec3(0.1f, 0.05f, 0.031f));
        containerShaderProgram.setUniform("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        containerShaderProgram.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        containerShaderProgram.setUniform("material.shininess", 32.0f);

        container.draw();

        lightShaderProgram.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        lightShaderProgram.setUniform("model", model);
        lightShaderProgram.setUniform("view", view);
        lightShaderProgram.setUniform("projection", projection);


        light.draw();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout<< width << " " << height << std::endl;
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (camera.firstMouse)
    {
        camera.lastX = xpos;
        camera.lastY = ypos;
        camera.firstMouse = false;
    }

    float xoffset = xpos - camera.lastX;
    float yoffset = camera.lastY - ypos; // reversed since y-coordinates go from bottom to top

    camera.lastX = xpos;
    camera.lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}