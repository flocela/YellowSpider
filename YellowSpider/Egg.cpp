#include "Egg.hpp"
#include "EggShape.hpp"
#include "ModelGeometry.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


Egg::Egg(float time)
: _eggShape{20, 100, 5.0f}
{
    ModelGeometry modelGeometry{};
    modelGeometry.setVertices(_eggShape.getVertices());
    modelGeometry.setIndices(_eggShape.getIndices());
    
    _modelGeometries.push_back(modelGeometry);
}

std::vector<glm::mat4> Egg::getModels(float time, Direction direction)
{
    glm::mat4 model{1.0f};
    
    float fortyFive_rad     = 45.0f  * PI_F / 180.0f;
    float ninety_rad        = 90.0f  * PI_F / 180.0f;
    float twoSeventy_rad    = 270.0f * PI_F / 180.0f;
    float oneThirtyFive_rad = 135.0f * PI_F / 180.0f;
    
    // Taken from EggShape class. Must get these values in a more modular way.
    float rLarge  = 10.0f;
    float rMedium = rLarge/2.0f;
    float rSmall  = rLarge - (1.414f * rMedium);
    
    float rotation_rad = _eggShape.getRotation(_tempCounter);
    std::cout << "rotations_rad: " << rotation_rad << ", " << (rotation_rad * 180.0f / PI_F) << std::endl;
    
    float x = _eggShape.getPos(_tempCounter).x;
    float y = _eggShape.getPos(_tempCounter).y;
    float z = _eggShape.getPos(_tempCounter).z;
    
    float rotationCorrected_rad = (rotation_rad >= twoSeventy_rad) ? (rotation_rad - twoSeventy_rad) : (rotation_rad + ninety_rad);
    float rotationCorrected_deg = rotationCorrected_rad * 180.0f/ PI_F; 
    std::cout << "rotationCorrected, x, y , z: " << rotationCorrected_deg << ", " << x << ", "<< y << ", " << z << std::endl;
    
    // Translations are from cycloid movement
    float xTranslationCycloid90MR = (ninety_rad * rMedium) - (rMedium * sin(ninety_rad));
    float yTranslationCycloid90MR = rMedium - (rMedium * cos(ninety_rad));
    
    float xTranslationCycloid45LR = (fortyFive_rad * rLarge) - (rLarge * sin(fortyFive_rad));
    float yTranslationCycloid45LR = rLarge - (rLarge * cos(fortyFive_rad));
    
    float xTranslationCycloid45SR = (fortyFive_rad * rSmall) - (rSmall * sin(fortyFive_rad));
    float yTranslationCycloid45SR = rSmall - (rSmall * cos(fortyFive_rad));
    
    if(rotationCorrected_deg > 135.0f)
    {
        // Intermediate translations are from cycloid movement using small-radius circle.
        float rotationDiff_rad = rotationCorrected_rad - oneThirtyFive_rad;
        float xIntermediateTranslationSmR = (rotationDiff_rad * rSmall) - (rSmall * sin(rotationDiff_rad));
        float yIntermediateTranslationSmR = rSmall - (rSmall * cos(rotationDiff_rad));
        
        model = glm::translate(model, glm::vec3{xIntermediateTranslationSmR, yIntermediateTranslationSmR, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{7.07f, 0.0f, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-7.07f, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + rMedium), 0.0f, 0.0f});
        
        // Rotate 45 deg using large-radius. 
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, yTranslationCycloid45LR, 0.0f});
        model = glm::rotate(model, fortyFive_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg about medium length. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslationCycloid90MR, yTranslationCycloid90MR, 0.0f});
        model = glm::rotate(model, ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
    }
    
    else if(rotationCorrected_deg > 90.0f)
    {
    
        // Intermediate translations are from cycloid movement using large-radius circle.
        float rotationDiff_rad = rotationCorrected_rad - ninety_rad;
        float xIntermediateTranslationLR = (rotationDiff_rad * rLarge) - (rLarge * sin(rotationDiff_rad));
        float yIntermediateTranslationLR = rLarge - (rLarge * cos(rotationDiff_rad));
        
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xIntermediateTranslationLR, yIntermediateTranslationLR, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg using medium-radius. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslationCycloid90MR, yTranslationCycloid90MR, 0.0f});
        model = glm::rotate(model, ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
    }
    else
    {
        
        // Intermediate translations due to cycloid rotation using medium-radius circle.
        float xIntermediateTranslationMR = (rotationCorrected_rad * rMedium) - (rMedium * sin(rotationCorrected_rad));
        float yIntermediateTranslationMR = rMedium - (rMedium * cos(rotationCorrected_rad));
        
        model = glm::translate(model, glm::vec3{xIntermediateTranslationMR, yIntermediateTranslationMR, 0.0f});
        model = glm::rotate(model, rotationCorrected_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
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



