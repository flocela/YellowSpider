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
#include "Egg.hpp"
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
    initWindow("Test Window", 1600, 600);
    std::cout << "main 36" << std::endl;
    float now = static_cast<float>(glfwGetTime());
    //Spider spider{now};
    Egg egg{now};
    
    //std::vector<ModelGeometry> spiderGeometries = spider.getModelGeometries();
    std::vector<ModelGeometry> eggGeometries = egg.getModelGeometries();
    
    // Create Vulkan Renderer instance
    //if (vulkanRenderer.init(window, spiderGeometries) == EXIT_FAILURE)
    if (vulkanRenderer.init(window) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    
    vulkanRenderer.addGeometries(eggGeometries);

    float deltaTime = 0.0f;
    float lastTime  = 0.0f;
    //
    // Loop until closed
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        //std::vector<glm::mat4> models = spider.getModels(now, Direction::Forward);
        std::vector<glm::mat4> models = egg.getModels(now, Direction::Forward);
        for(int ii=0; ii<models.size(); ++ii)
        {
            vulkanRenderer.updateModel(ii, models[ii]);
        }
        vulkanRenderer.draw();
        //std::cout << now << std::endl;
    }

    vulkanRenderer.cleanup();
    //
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
