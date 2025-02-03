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
    
    float fortyFive_rad     = 45.0f  * PI_F / 180.0f;
    float ninety_rad        = 90.0f  * PI_F / 180.0f;
    float twoSeventy_rad    = 270.0f * PI_F / 180.0f;
    float oneThirtyFive_rad = 135.0f * PI_F / 180.0f;
    
    // Taken from EggShape class. Must get these values in a more modular way.
    float rLarge  = 10.0f;
    float rMedium = rLarge/2.0f;
    float rSmall  = rLarge - (1.414 * rMedium);
    
    float rotation_rad = _eggShape.getRotation(_tempCounter);
    float rotation_deg = rotation_rad * 180.0f/PI_F;
    
    float x = _eggShape.getPos(_tempCounter).x;
    float y = _eggShape.getPos(_tempCounter).y;
    float z = _eggShape.getPos(_tempCounter).z;
    
    float rotationCorrected_rad = rotation_rad - twoSeventy_rad;
    float rotationCorrected_deg = rotationCorrected_rad * 180.0f/ PI_F; 
    std::cout << "rotationCorrected, x, y , z: " << rotationCorrected_deg << ", " << x << ", "<< y << ", " << z << std::endl;
    
    // Translations are from cycloid movement
    float xTranslation0 = (rotationCorrected_rad * rMedium) - (rMedium * sin(rotationCorrected_rad));
    float yTranslation0 = rMedium - (rMedium * cos(rotationCorrected_rad));
    
    float xTranslation90 = (ninety_rad * rMedium) - (rMedium * sin(ninety_rad));
    float yTranslation90 = rMedium - (rMedium * cos(ninety_rad));
    
    float xTranslation10L45Deg = (fortyFive_rad * rLarge) - (rLarge * sin(fortyFive_rad));
    float yTranslation10L45Deg = rLarge - (rLarge * cos(fortyFive_rad));
    
    float xTranslationSmallL45Deg = (fortyFive_rad * rSmall) - (rSmall * sin(fortyFive_rad));
    float yTranslationSmallL45Deg = rSmall - (rSmall * cos(fortyFive_rad));
    
    if(rotationCorrected_deg > 135.0f)
    {

        
        float rotationDiff_rad = rotationCorrected_rad - oneThirtyFive_rad;
        float xTranslationSmR = (rotationDiff_rad * rSmall) - (rSmall * sin(rotationDiff_rad));
        float yTranslationSmR = rSmall - (rSmall * cos(rotationDiff_rad));
        // Rotate up to 180 deg, which is 135 deg plus 45 deg. So only rotate an additional 45 deg.
        // Rotation is about length 2.
        // Translate to correct spot after second rotation (after 135 deg).
        // Translate to account for rotation.
        //model = glm::translate(model, glm::vec3{xTranslationSmallL45Deg, yTranslationSmallL45Deg, 0.0f});
        //model = glm::translate(model, glm::vec3{+(xTranslation90 + 5.0f) + xTranslation10L45Deg, 0.0f, 0.0f});
        //model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
    
        // Translate back to (0, 0)
        //model = glm::translate(model, glm::vec3{-(xTranslation90 + rMedium) - xTranslation10L45Deg, 0.0f, 0.0f});
    
        // Rotate up to 135 deg, which is 90 deg + 45d eg. So only rotate an additional 45 deg.
        // Translate to correct spot.
        //model = glm::translate(model, glm::vec3{+(xTranslation90 + rMedium), 0.0f, 0.0f}); // plus
        //model = glm::translate(model, glm::vec3{xTranslation10L45Deg, yTranslation10L45Deg, 0.0f});
        //model = glm::rotate(model, fortyFive_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Translate back to (0, 0)
        //model = glm::translate(model, glm::vec3{-(xTranslation90 + rMedium), 0.0f, 0.0f}); //minus
        
        // Rotate to 90deg. And translate to correct spot.
        //model = glm::translate(model, glm::vec3{xTranslation90, yTranslation90, 0.0f});
        //model = glm::rotate(model, ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        model = glm::translate(model, glm::vec3{xTranslationSmR, yTranslationSmR, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslation90 + rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslation10L45Deg, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{7.07f, 0.0f, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        //
        // Move back to (0, 0)
        model = glm::translate(model, glm::vec3{-7.07f, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslation10L45Deg, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslation90 + rMedium), 0.0f, 0.0f});
        
        // Rotate 45 deg about larget length. Move to correct spot.
        model = glm::translate(model, glm::vec3{+(xTranslation90 + rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslation10L45Deg, yTranslation10L45Deg, 0.0f});
        model = glm::rotate(model, fortyFive_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0, 0)
        model = glm::translate(model, glm::vec3{-(xTranslation90 + rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg about medium length. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslation90, yTranslation90, 0.0f});
        model = glm::rotate(model, ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
    }
    else if(rotationCorrected_deg > 90.0f)
    {
        float rotationDiff_rad = rotationCorrected_rad - ninety_rad;
        float xTranslation10L = (rotationDiff_rad * rLarge) - (rLarge * sin(rotationDiff_rad));
        float yTranslation10L = rLarge - (rLarge * cos(rotationDiff_rad));
        
        model = glm::translate(model, glm::vec3{+(xTranslation90 + rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslation10L, yTranslation10L, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0, 0)
        model = glm::translate(model, glm::vec3{-(xTranslation90 + rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg about medium length. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslation90, yTranslation90, 0.0f});
        model = glm::rotate(model, ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
    }
    else
    {
        model = glm::translate(model, glm::vec3{xTranslation0, yTranslation0, 0.0f});
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



