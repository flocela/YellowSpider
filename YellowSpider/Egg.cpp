#include "Egg.hpp"
#include "EggShape.hpp"
#include "ModelGeometry.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


const float  PI_F=3.14159265358979f;

Egg::Egg(float time)
{
    EggShape eggShape{};
    ModelGeometry modelGeometry{};
    modelGeometry.setVertices(eggShape.getVertices());
    modelGeometry.setIndices(eggShape.getIndices());
    
    _modelGeometries.push_back(modelGeometry);
}

std::vector<glm::mat4> Egg::getModels(float time, Direction direction)
{
    glm::mat4 model{1.0f};
    
    float fortyFive_rad = 45.0f * PI_F / 180.0f;
    float ninety_rad = 90.0f * PI_F / 180.0f;
    float twoSeventy_rad = 270.0f * PI_F / 180.0f;
    
    float rotation_rad = _eggShape.getRotation(_tempCounter);
    float rotation_deg = rotation_rad * 180.0f/PI_F;
    
    float x = _eggShape.getPos(_tempCounter).x;
    float y = _eggShape.getPos(_tempCounter).y;
    float z = _eggShape.getPos(_tempCounter).z;
    
    float rotationCorrected_rad = rotation_rad - twoSeventy_rad;
    float rotationCorrected_deg = rotationCorrected_rad * 180.0f/ PI_F; 
    std::cout << "rotationCorrected, x, y , z: " << rotationCorrected_deg << ", " << x << ", "<< y << ", " << z << std::endl;
    
    float xTranslation0 = (rotationCorrected_rad * 5.0f) - (5.0f * sin(rotationCorrected_rad));
    float yTranslation0 = 5.0f - (5.0f * cos(rotationCorrected_rad));
    
    float xTranslation90 = (ninety_rad * 5.0f) - (5.0f * sin(ninety_rad));
    float yTranslation90 = 5.0f - (5.0f * cos(ninety_rad));
    
    if(rotationCorrected_deg > 90.0f)
    {
        float rotationDiff_rad = rotationCorrected_rad - ninety_rad;
        float xTranslation10 = (rotationDiff_rad * 10.0f) - (10.0f * sin(rotationDiff_rad));
        float yTranslation10 = 10.0f - (10.0f * cos(rotationDiff_rad));
    /*
        
        std::cout << "rotationDiff_rad: " << rotationDiff_rad << std::endl;
        
        
        model = glm::translate(model, glm::vec3{-((ninety_rad * 5.0f - 5.0f) - 5.0f), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslation1, yTranslation1, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        model = glm::translate(model, glm::vec3{(ninety_rad * 5.0f - 5.0f) - 5.0f, 0.0f, 0.0f});
        
        model = glm::translate(model, glm::vec3{(ninety_rad * 5.0f) - 5.0f, 5.0f, 0.0f});
        model = glm::rotate(model, ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        */
        
        model = glm::translate(model, glm::vec3{+(xTranslation90 + 5.0f), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslation10, yTranslation10, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        model = glm::translate(model, glm::vec3{-(xTranslation90 + 5.0f), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslation90, yTranslation90, 0.0f});
        model = glm::rotate(model, ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
    }
    else
    {
        model = glm::translate(model, glm::vec3{xTranslation0, yTranslation0, 0.0f});
        model = glm::rotate(model, rotationCorrected_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        //
    }
    
    /*
    (std::cout << "getRotation: " << _tempCounter << ", " << (180.0f/PI_F)*(-(_eggShape.getRotation(_tempCounter)-(270.0f * PI_F / 180.0f))) << std::endl;
    model = glm::rotate(model, (-(_eggShape.getRotation(_tempCounter)-(270.0f * PI_F / 180.0f))), glm::vec3{0.0f, 0.0f, 1.0f});
    float x = _eggShape.getPos(_tempCounter).x;
    float y = _eggShape.getPos(_tempCounter).y;
    float z = _eggShape.getPos(_tempCounter).z;
    if ( (-(_eggShape.getRotation(_tempCounter)-(270.0f * PI_F / 180.0f))) > (PI_F/4.0f))
    {
        model = glm::translate(model, 5.0f + glm::vec3{0.0f, x, 0.0f});
    }
    else
    {
        model = glm::translate(model, glm::vec3{0.0f, y, 0.0f});
    }
    
    std::cout << _eggShape.getPos(_tempCounter).x << ", " << _eggShape.getPos(_tempCounter).y << ", " << _eggShape.getPos(_tempCounter).z << std::endl;
    */
    ++_tempCounter;
    _tempCounter = _tempCounter % (_eggShape.getNumOfRotations());
    
    std::vector<glm::mat4> models{};
    models.push_back(model);
    
    return models;
}
//
std::vector<std::vector<Vertex>> Egg::getVertices()
{
    std::vector<std::vector<Vertex>> vectorOfVectorOfVertices{};
    vectorOfVectorOfVertices.push_back(_eggShape.getVertices());
    
    return vectorOfVectorOfVertices;
} 

std::vector<std::vector<uint32_t>> Egg::getIndices()
{
    std::vector<std::vector<uint32_t>> vectorOfVectorOfIndices{};
    vectorOfVectorOfIndices.push_back(_eggShape.getIndices());
    return vectorOfVectorOfIndices;
    
}

std::vector<ModelGeometry> Egg::getModelGeometries()
{
    return _modelGeometries;
}



