#include "Leg.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

Leg::Leg(float seg0, float seg1, float seg2)
:   _seg0Length{seg0},
    _seg1Length{seg1},
    _seg2Length{seg2}
{}
//
std::vector<glm::mat4> Leg::getModels()
{
    std::vector<glm::mat4> models{};
    //
    glm::mat4 model0(1.0f);
    glm::mat4 model1(1.0f);
    glm::mat4 model2(1.0f);
    
    float rotate0_deg = 65.0f;
    float rotate1_deg = 60.0f;
    float rotate2_deg = 90.0f;
    
    model2 = glm::translate(model2, glm::vec3(-2.3f, 7.2f, -5.84f)); 
    model2 = glm::rotate   (model2, glm::radians(rotate0_deg), glm::vec3(-5.84f, 0.0f, 3.3f)); 
    model2 = glm::translate(model2, glm::vec3(0.0f, _seg0Length, 0.0f));
    model2 = glm::rotate   (model2, glm::radians(rotate1_deg), glm::vec3(-5.84f, 0.0f, 3.3f)); 
    model2 = glm::translate(model2, glm::vec3(0.0f, _seg1Length, 0.0f));
    model2 = glm::rotate   (model2, glm::radians(rotate2_deg), glm::vec3(-5.84f, 0.0f, 3.3f));  // c rotate
   
    model1 = glm::translate(model1, glm::vec3(-2.3f, 7.2f, -5.84f));
    model1 = glm::rotate   (model1, glm::radians(rotate0_deg), glm::vec3(-5.84f, 0.0f, 3.3f));
    model1 = glm::translate(model1, glm::vec3(0.0f, _seg0Length, 0.0f));
    
    model1 = glm::rotate   (model1, glm::radians(rotate1_deg), glm::vec3(-5.84f, 0.0f, 3.3f));  // b rotate
    
    model0 = glm::translate(model0, glm::vec3(-2.3f, 7.2f, -5.84f));
    model0 = glm::rotate   (model0, glm::radians(rotate0_deg), glm::vec3(-5.84f, 0.0f, 3.3f)); 
    
    
    models.push_back(model0);
    models.push_back(model1);
    models.push_back(model2);
    
    return models;
}
