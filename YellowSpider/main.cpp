#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

#include "VulkanRenderer.hpp"
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

    // Create Vulkan Renderer instance
    if (vulkanRenderer.init(window) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    float angle     = 0.0f;
    float deltaTime = 0.0f;
    float lastTime  = 0.0f;
    
    float rotateA = 30.0f;
    float rotateB = 4.0 * rotateA;
    float rotateC = 2.0 * rotateA;
    //
    // Loop until closed
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        float now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;
        
        angle += 10.0f * deltaTime;
        float updatedARotation = angle * 1.0f;
        float updatedBRotation = angle * 2.0f;
        float updatedCRotation = angle * 5.0f;
    
        if(updatedARotation > 360.0f)
        {
            updatedARotation -= 360.0f;
        }
        
        if(updatedBRotation > 360.0f)
        {
            updatedBRotation -= 360.0f;
        }
        
        if(updatedCRotation > 360.0f)
        {
            updatedCRotation -= 360.0f;
        }
        
        //updatedARotation = (updatedARotation > rotateA) ? (rotateA) : (updatedARotation);
        //updatedBRotation = (updatedBRotation > rotateB) ? (rotateB) : (updatedBRotation);
        //updatedCRotation = (updatedCRotation > rotateC) ? (rotateC) : (updatedCRotation);
        //
        //glm::mat4 modelA(1.0f);
        //modelA = glm::rotate(modelA, glm::radians(updatedARotation), glm::vec3(1.0f, 0.0f, 0.0f)); // a rotate
        //modelA= glm::translate(modelA, glm::vec3(3.0f, 0.0f, 0.0f));
        /*
        glm::mat4 modelB(1.0f);
        glm::mat4 modelC(1.0f);
        
        //modelB= glm::translate(modelB, glm::vec3(3.0f, 0.0f, 0.0f));
        //modelB = glm::rotate(modelB, glm::radians(angle), glm::vec3(1.0f, 0.0f, 1.0f));
        //modelC = glm::translate(modelC, glm::vec3(10.0f, 10.0f, 5.0f));
        modelC = glm::rotate   (modelC, glm::radians(updatedARotation), glm::vec3(1.0f, 0.0f, 1.0f));
        modelC = glm::translate(modelC, glm::vec3(0.0f, 5.0f, 0.0f));
        modelC = glm::rotate   (modelC, glm::radians(updatedBRotation), glm::vec3(1.0f, 0.0f, 1.0f));
        modelC = glm::translate(modelC, glm::vec3(0.0f, 8.0f, 0.0f));
        modelC = glm::rotate   (modelC, glm::radians(updatedCRotation), glm::vec3(1.0f, 0.0f, 1.0f)); // c rotate
       
        //modelB = glm::translate(modelB, glm::vec3(10.0f, 10.0f, 5.0f));
        modelB = glm::rotate   (modelB, glm::radians(updatedARotation), glm::vec3(1.0f, 0.0f, 1.0f));
        modelB = glm::translate(modelB, glm::vec3(0.0f, 5.0f, 0.0f));
        modelB = glm::rotate   (modelB, glm::radians(updatedBRotation), glm::vec3(1.0f, 0.0f, 1.0f)); // b rotate
        
        
        //modelA = glm::translate(modelA, glm::vec3(10.0f, 10.0f, 5.0f));
        modelA = glm::rotate   (modelA, glm::radians(updatedARotation), glm::vec3(1.0f, 0.0f, 1.0f)); // a rotate
        
        
        
        //modelA = glm::translate(modelA, glm::vec3(7.0f, 0.0f, 0.0f));
        //modelB = glm::translate(modelB, glm::vec3(7.0f, 0.0f, 0.0f));
        //modelC = glm::translate(modelC, glm::vec3(7.0f, 0.0f, 0.0f));
        
        //
        //
        
        */
        
        glm::mat4 modelBody(1.0f);
        modelBody = glm::translate(modelBody, glm::vec3(0.0f, 2.0f,2.0f));
        modelBody = glm::rotate(modelBody, glm::radians(-28.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
        glm::mat4 modelHead(1.0f);
        modelHead = glm::translate(modelHead, glm::vec3(0.0f, 0.2f, 0.0f));
        modelHead = glm::rotate(modelHead, glm::radians(-4.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
        
        vulkanRenderer.updateModel(0, modelBody);
        vulkanRenderer.updateModel(1, modelHead);
        //vulkanRenderer.updateModel(1, modelB);
        //vulkanRenderer.updateModel(2, modelC);
        

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
