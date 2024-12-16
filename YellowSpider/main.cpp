#define STB_IMAGE_IMPLEMENTATION
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <chrono>
#include <thread>

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>

#include "VulkanRenderer.hpp"
#include "Circle.hpp"

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
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    
    // Create Window
    initWindow("Test Window", 1366, 768);

    // Create Vulkan Renderer instance
    if (vulkanRenderer.init(window) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    float angle     = 0.0f;
    float twiceAngle = 0.0f;
    float deltaTime = 0.0f;
    float lastTime  = 0.0f;
    
    Circle outlineCircle{4.0f, 0.0f, 0.0f, 2.5, 720};
    
    std::vector<glm::vec3> outlineVertices = outlineCircle.getOutlineVertices();
    std::cout << "outlineVertices size: " << outlineVertices.size() << std::endl;
    bool stop = false;
    
    // Loop until closed
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        if (!stop)
        {
            float now = glfwGetTime();
            deltaTime = now - lastTime;
            lastTime = now;
            
            angle += 30.0f * deltaTime;
            angle = std::fmod(angle, 360.0f);
            twiceAngle = angle * 3.0f;
            twiceAngle = std::fmod(twiceAngle, 360.0f);
            
            if(angle > 179.5f && angle <180.5f && twiceAngle > 179.5f && twiceAngle < 180.5f)
            {
                angle = 180.0f;
                twiceAngle = 180.0f;
                stop = true;
            }
            
            glm::mat4 model_0(1.0f);
            model_0 = glm::translate(model_0, glm::vec3(0.0f, 0.0f, 0.0f));
            //model_0 = glm::rotate(model_0, glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f));
            
            vulkanRenderer.updateModel(0, model_0);

            vulkanRenderer.draw();
        }
    }
    
    vulkanRenderer.cleanup();

    // Destroy GLFW window and stop GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
