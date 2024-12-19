#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

#include "VulkanRenderer.hpp"

GLFWwindow* window;
VulkanRenderer vulkanRenderer;

void initWindow(std::string wName = "Test Window", const int width = 800, const int height = 600)
{
    // Initialise GLFW
    glfwInit();

    // Set GLFW to NOT work with OpenGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
}
//
int main()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // Create Window
    initWindow("Test Window", 800, 600);

    // Create Vulkan Renderer instance
    if (vulkanRenderer.init(window) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    float angle     = 0.0f;
    float deltaTime = 0.0f;
    float lastTime  = 0.0f;
    
    // Loop until closed
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        float now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;
        
        angle += 2.0f * deltaTime;
        if(angle > 360.0f)
        {
            angle -= 360.0f;
        }
        
        glm::mat4 modelA(1.0f);
        glm::mat4 modelB(1.0f);
        glm::mat4 modelC(1.0f);
        
        if(angle >= 30.0f)
        {
            angle = 30.0f;
        }

        modelA = glm::translate(modelA, glm::vec3(3.0f, 0.0f, 0.0f)); // red
        //modelB = glm::rotate(modelB, glm::radians(angle), glm::vec3(1.0f, 0.0f, 1.0f));
        //modelC = glm::rotate(modelC, glm::radians(angle), glm::vec3(1.0f, 0.0f, 1.0f));
        
        //secondModel = glm::translate(secondModel, glm::vec3(0.0f, 0.0f, -3.0f)); // blue
        //secondModel = glm::rotate(secondModel, glm::radians(-angle * 25), glm::vec3(0.0f, 0.0f, 1.0f));

        vulkanRenderer.updateModel(0, modelA);
        //vulkanRenderer.updateModel(1, modelB);
        //vulkanRenderer.updateModel(2, modelC);

        vulkanRenderer.draw();
    }

    vulkanRenderer.cleanup();

    // Destroy GLFW window and stop GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
