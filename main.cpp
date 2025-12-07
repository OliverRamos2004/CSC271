#include <iostream>
#include <random>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shaderprogram.h"
#include "stb_image.h"
#include "mesh.h"
#include "camera.h"
#include "light_config.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
Camera camera;

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

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello CG", nullptr, nullptr);
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

    std::vector<std::string>faces{
        std::string(ASSETS_DIR) + "skybox/right.jpg",
        std::string(ASSETS_DIR) + "skybox/left.jpg",
        std::string(ASSETS_DIR) + "skybox/top.jpg",
        std::string(ASSETS_DIR) + "skybox/bottom.jpg",
        std::string(ASSETS_DIR) + "skybox/front.jpg",
        std::string(ASSETS_DIR) + "skybox/back.jpg"
    };

    std::string vertPath = std::string(SHADER_DIR) + "cube_vertex.vert";
    std::string fragPath = std::string(SHADER_DIR) + "container_fragment.frag";
    ShaderProgram containerShaderProgram(vertPath, fragPath);
    GLuint cube_diffuse = containerShaderProgram.bindTexture2D("material.diffuse", std::string(ASSETS_DIR) + "container2.png", 0, false);
    GLuint cube_specular = containerShaderProgram.bindTexture2D("material.specular", std::string(ASSETS_DIR) + "container2_specular.png", 1, false);
    containerShaderProgram.bindCubeMap("skybox", faces, 2);
    containerShaderProgram.setUniform("material.shininess", 32.0f);
    containerShaderProgram.setUniform("material.alpha",     1.0f);

    vertPath = std::string(SHADER_DIR) + "cube_vertex.vert";
    fragPath = std::string(SHADER_DIR) + "light_fragment.frag";
    ShaderProgram lightShaderProgram(vertPath, fragPath);

    vertPath = std::string(SHADER_DIR) + "skybox_vertex.vert";
    fragPath = std::string(SHADER_DIR) + "skybox_fragment.frag";
    ShaderProgram skyboxShaderProgram(vertPath, fragPath);
    skyboxShaderProgram.bindCubeMap("skybox", faces, 0);

    Mesh container(std::string(ASSETS_DIR) + "box.obj", containerShaderProgram.getID());
    Mesh light(std::string(ASSETS_DIR) + "box.obj", lightShaderProgram.getID());
    Mesh skybox(std::string(ASSETS_DIR) + "skybox.obj", skyboxShaderProgram.getID());

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    gfx::SceneConfig cfg = gfx::makeDefaultSceneConfig();

    cfg.dirLights.resize(1);
    cfg.pointLights.clear();
    cfg.spotLights.clear();
    gfx::applyDirLights(containerShaderProgram,   cfg.dirLights);
    gfx::applyPointLights(containerShaderProgram, cfg.pointLights);
    gfx::applySpotLights(containerShaderProgram,  cfg.spotLights);


    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.ProcessKeyboard(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //objects
        containerShaderProgram.use();
        glm::mat4 view       = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjection((float)SCR_WIDTH / SCR_HEIGHT);

        containerShaderProgram.setUniform("view",       view);
        containerShaderProgram.setUniform("projection", projection);
        containerShaderProgram.setUniform("viewPos",    camera.Position);


        for (int i = 0; i < 10; i++) {
            containerShaderProgram.setUniform("material.alpha", 1.0f);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i),
                                glm::vec3(1.0f, 0.3f, 0.5f));
            containerShaderProgram.setUniform("model", model);

            container.draw();
        }

        glDepthFunc(GL_LEQUAL);
        skyboxShaderProgram.use();

        glm::mat4 viewNoTrans = glm::mat4(glm::mat3(view));
        skyboxShaderProgram.setUniform("view", viewNoTrans);
        skyboxShaderProgram.setUniform("projection", projection);
        skybox.draw();

        glDepthFunc(GL_LESS);

        //just for point lights
//        lightShaderProgram.use();
//        lightShaderProgram.setUniform("view",       view);
//        lightShaderProgram.setUniform("projection", projection);
//
//        for (int i = 0; i < (int)cfg.pointLights.size(); ++i)
//        {
//            glm::vec3 pos  = cfg.pointLights[i].position;
//
//            // Build model matrix for the small light cube
//            glm::mat4 model = glm::mat4(1.0f);
//            model = glm::translate(model, pos);
//            model = glm::scale(model, glm::vec3(0.2f));   // small cube size
//
//            lightShaderProgram.setUniform("model", model);
//            container.draw();
//        }

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