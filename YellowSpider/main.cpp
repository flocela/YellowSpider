#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

#include "VulkanRenderer.hpp"
#include "Spider.hpp"
#include "Direction.hpp"
//
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
    
    Spider spider{static_cast<float>(glfwGetTime())};
    std::vector<ModelGeometry> spiderGeometries = spider.getModelGeometries();
    
    std::cout << "main line 40 spiderGeometries.size(): " << spiderGeometries.size() << std::endl;

    // Create Vulkan Renderer instance
    if (vulkanRenderer.init(window, spiderGeometries) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    float angle     = 0.0f;
    float deltaTime = 0.0f;
    float lastTime  = 0.0f;
    
    float rotateA = 30.0f;
    float rotateB = 4.0 * rotateA;
    float rotateC = 2.0 * rotateA;
    
    // Loop until closed
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        float now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;
        
        std::vector<glm::mat4> models = spider.getModels(now, Direction::Forward);
        for(int ii=0; ii<models.size(); ++ii)
        {
            vulkanRenderer.updateModel(ii, models[ii]);
        }
        
        vulkanRenderer.draw();
    }

    vulkanRenderer.cleanup();
    
    /*
    modelB = glm::rotate   (modelB, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelB = glm::translate(modelB, glm::vec3(0.0f, 5.0f, 0.0f));
    modelB = glm::rotate   (modelB, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // b rotate
        
    
    modelA = glm::rotate   (modelA, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // a rotate
    */
    
    
    
    

    // Destroy GLFW window and stop GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
