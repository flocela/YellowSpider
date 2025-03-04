#include "Egg.hpp"
#include "EggShape.hpp"
#include "ModelGeometry.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


Egg::Egg(float time)
: _eggShape{20, (1.0f * PI_F/180.0f), 5.0f}
{
    ModelGeometry modelGeometry{};
    modelGeometry.setVertices(_eggShape.getVertices());
    modelGeometry.setIndices(_eggShape.getIndices());
    
    _modelGeometries.push_back(modelGeometry);
}

std::vector<glm::mat4> Egg::getModelsPerRotation(float rotation_rad)
{
    glm::mat4 model{1.0f};
    
    float   circumference        = _eggShape.getEggCircumferenceAboutZ();
    int32_t numOfFullRotations   = rotation_rad/(2*PI_F);
    float   wholeCircumferences  = numOfFullRotations * circumference;
    
    float rotationCorrected_rad = rotation_rad - (numOfFullRotations * 2 * PI_F);
    //std::cout << "rotation_rad, rotationCorrected_rad: " << (rotation_rad * 180.0f/PI_F) << ", " << (rotationCorrected_rad * 180.0f/PI_F)  << std::endl;
    //std::cout << "orig, dist, rotations: " << distOrig << ", " << dist << ", " << (rotation_rad * 180.0f / PI_F) << std::endl;
    float rotationCorrected_deg = rotationCorrected_rad * 180.0f/ PI_F;
    
    // Translations are from cycloid movement
    float xTranslationCycloid90MR = (_ninety_rad * _rMedium) - (_rMedium * sin(_ninety_rad));
    float yTranslationCycloid90MR = _rMedium - (_rMedium * cos(_ninety_rad));
    
    float xTranslationCycloid45LR = (_fortyFive_rad * _rLarge) - (_rLarge * sin(_fortyFive_rad));
    float yTranslationCycloid45LR = _rLarge - (_rLarge * cos(_fortyFive_rad));
    
    float xTranslationCycloid45SR = (_fortyFive_rad * _rSmall) - (_rSmall * sin(_fortyFive_rad));
    float yTranslationCycloid45SR = _rSmall - (_rSmall * cos(_fortyFive_rad));
    
    float xTranslateCycloid90SR   = (_ninety_rad * _rSmall) - (_rSmall * sin(_ninety_rad));
    float yTranslationCycloid90SR = _rSmall - (_rSmall * cos(_ninety_rad));
    
    if(rotationCorrected_deg > 270.0f)
    {
        // Intermediate translations are from cycloid movement using small-radius circle.
        float rotationDiff_rad = rotationCorrected_rad - _twoSeventy_rad;
        float xIntermediateTranslationMR1 = (rotationDiff_rad * _rMedium) - (_rMedium* sin(rotationDiff_rad));
        float yIntermediateTranslationMR1 = _rMedium - (_rMedium * cos(rotationDiff_rad));
        
        model = glm::translate(model, glm::vec3{wholeCircumferences, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xIntermediateTranslationMR1, yIntermediateTranslationMR1, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{(xTranslateCycloid90SR + _rSmall), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslateCycloid90SR + _rSmall),0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 45 deg using large-radius.
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, yTranslationCycloid45LR, 0.0f});
        model = glm::translate(model, glm::vec3{(xTranslateCycloid90SR + _rSmall), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::rotate(model, _fortyFive_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslateCycloid90SR + _rSmall),0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg using small-radius.
        model = glm::translate(model, glm::vec3{xTranslateCycloid90SR, yTranslationCycloid90SR, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 45 deg using large-radius. 
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, yTranslationCycloid45LR, 0.0f});
        model = glm::rotate(model, _fortyFive_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg about medium length. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslationCycloid90MR, yTranslationCycloid90MR, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
    }
    else if(rotationCorrected_deg > 225.0f)
    {
        // Intermediate translations are from cycloid movement using small-radius circle.
        float rotationDiff_rad = rotationCorrected_rad - _twoTwentyFive_rad;
        float xIntermediateTranslationLR1 = (rotationDiff_rad * _rLarge) - (_rLarge* sin(rotationDiff_rad));
        float yIntermediateTranslationLR1 = _rLarge - (_rLarge * cos(rotationDiff_rad));
        
        model = glm::translate(model, glm::vec3{wholeCircumferences, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xIntermediateTranslationLR1, yIntermediateTranslationLR1, 0.0f});
        model = glm::translate(model, glm::vec3{(xTranslateCycloid90SR + _rSmall), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslateCycloid90SR + _rSmall),0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg using small-radius.
        model = glm::translate(model, glm::vec3{xTranslateCycloid90SR, yTranslationCycloid90SR, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 45 deg using large-radius. 
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, yTranslationCycloid45LR, 0.0f});
        model = glm::rotate(model, _fortyFive_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg about medium length. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslationCycloid90MR, yTranslationCycloid90MR, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
    }
    else if(rotationCorrected_deg > 135.0f)
    {
        // Intermediate translations are from cycloid movement using small-radius circle.
        float rotationDiff_rad = rotationCorrected_rad - _oneThirtyFive_rad;
        float xIntermediateTranslationSmR = (rotationDiff_rad * _rSmall) - (_rSmall * sin(rotationDiff_rad));
        float yIntermediateTranslationSmR = _rSmall - (_rSmall * cos(rotationDiff_rad));
        
        model = glm::translate(model, glm::vec3{wholeCircumferences, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xIntermediateTranslationSmR, yIntermediateTranslationSmR, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 45 deg using large-radius. 
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, yTranslationCycloid45LR, 0.0f});
        model = glm::rotate(model, _fortyFive_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        //
        // Rotate 90 deg about medium length. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslationCycloid90MR, yTranslationCycloid90MR, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
    }
    else if(rotationCorrected_deg > 90.0f)
    {
        // Intermediate translations are from cycloid movement using large-radius circle.
        float rotationDiff_rad = rotationCorrected_rad - _ninety_rad;
        float xIntermediateTranslationLR0 = (rotationDiff_rad * _rLarge) - (_rLarge * sin(rotationDiff_rad));
        float yIntermediateTranslationLR0 = _rLarge - (_rLarge * cos(rotationDiff_rad));
        
        model = glm::translate(model, glm::vec3{wholeCircumferences, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xIntermediateTranslationLR0, yIntermediateTranslationLR0, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg using medium-radius. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslationCycloid90MR, yTranslationCycloid90MR, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
    }
    else
    {
        // Intermediate translations due to cycloid rotation using medium-radius circle.
        float xIntermediateTranslationMR0 = (rotationCorrected_rad * _rMedium) - (_rMedium * sin(rotationCorrected_rad));
        float yIntermediateTranslationMR0 = _rMedium - (_rMedium * cos(rotationCorrected_rad));
        
        model = glm::translate(model, glm::vec3{wholeCircumferences, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xIntermediateTranslationMR0, yIntermediateTranslationMR0, 0.0f});
        model = glm::rotate(model, rotationCorrected_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
    }
    std::vector<glm::mat4> models{};
    models.push_back(model);
    return models;
}

std::vector<glm::mat4> Egg::getModelsPerDistance(float dist)
{
    glm::mat4 model{1.0f};
    
    float circumference = _eggShape.getEggCircumferenceAboutZ();
    float distOrig = dist;
    dist = distOrig - static_cast<float>(floor(distOrig / circumference) * circumference);
    float wholeCircumferences = static_cast<int>(distOrig/circumference)*circumference;
    
    float rotation_rad = _eggShape.getRotationGivenCircumferenceDistance(dist);
    float rotationCorrected_rad = (rotation_rad >= _twoSeventy_rad) ? (rotation_rad - _twoSeventy_rad) : (rotation_rad + _ninety_rad);
    rotationCorrected_rad = (rotationCorrected_rad > (2*PI_F)) ? (rotationCorrected_rad - (2*PI_F)) : (rotationCorrected_rad);
    std::cout << "rotation_rad, rotationCorrected_rad: " << (rotation_rad * 180.0f/PI_F) << ", " << (rotationCorrected_rad * 180.0f/PI_F)  << std::endl;
    //std::cout << "orig, dist, rotations: " << distOrig << ", " << dist << ", " << (rotation_rad * 180.0f / PI_F) << std::endl;
    float rotationCorrected_deg = rotationCorrected_rad * 180.0f/ PI_F;
    
    // Translations are from cycloid movement
    float xTranslationCycloid90MR = (_ninety_rad * _rMedium) - (_rMedium * sin(_ninety_rad));
    float yTranslationCycloid90MR = _rMedium - (_rMedium * cos(_ninety_rad));
    
    float xTranslationCycloid45LR = (_fortyFive_rad * _rLarge) - (_rLarge * sin(_fortyFive_rad));
    float yTranslationCycloid45LR = _rLarge - (_rLarge * cos(_fortyFive_rad));
    
    float xTranslationCycloid45SR = (_fortyFive_rad * _rSmall) - (_rSmall * sin(_fortyFive_rad));
    float yTranslationCycloid45SR = _rSmall - (_rSmall * cos(_fortyFive_rad));
    
    float xTranslateCycloid90SR   = (_ninety_rad * _rSmall) - (_rSmall * sin(_ninety_rad));
    float yTranslationCycloid90SR = _rSmall - (_rSmall * cos(_ninety_rad));
    
    if(rotationCorrected_deg > 270.0f)
    {
        // Intermediate translations are from cycloid movement using small-radius circle.
        float rotationDiff_rad = rotationCorrected_rad - _twoSeventy_rad;
        float xIntermediateTranslationMR1 = (rotationDiff_rad * _rMedium) - (_rMedium* sin(rotationDiff_rad));
        float yIntermediateTranslationMR1 = _rMedium - (_rMedium * cos(rotationDiff_rad));
        
        model = glm::translate(model, glm::vec3{wholeCircumferences, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xIntermediateTranslationMR1, yIntermediateTranslationMR1, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{(xTranslateCycloid90SR + _rSmall), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslateCycloid90SR + _rSmall),0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 45 deg using large-radius.
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, yTranslationCycloid45LR, 0.0f});
        model = glm::translate(model, glm::vec3{(xTranslateCycloid90SR + _rSmall), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::rotate(model, _fortyFive_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslateCycloid90SR + _rSmall),0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg using small-radius.
        model = glm::translate(model, glm::vec3{xTranslateCycloid90SR, yTranslationCycloid90SR, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 45 deg using large-radius. 
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, yTranslationCycloid45LR, 0.0f});
        model = glm::rotate(model, _fortyFive_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg about medium length. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslationCycloid90MR, yTranslationCycloid90MR, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
    }
    else if(rotationCorrected_deg > 225.0f)
    {
        // Intermediate translations are from cycloid movement using small-radius circle.
        float rotationDiff_rad = rotationCorrected_rad - _twoTwentyFive_rad;
        float xIntermediateTranslationLR1 = (rotationDiff_rad * _rLarge) - (_rLarge* sin(rotationDiff_rad));
        float yIntermediateTranslationLR1 = _rLarge - (_rLarge * cos(rotationDiff_rad));
        
        model = glm::translate(model, glm::vec3{wholeCircumferences, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xIntermediateTranslationLR1, yIntermediateTranslationLR1, 0.0f});
        model = glm::translate(model, glm::vec3{(xTranslateCycloid90SR + _rSmall), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslateCycloid90SR + _rSmall),0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg using small-radius.
        model = glm::translate(model, glm::vec3{xTranslateCycloid90SR, yTranslationCycloid90SR, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 45 deg using large-radius. 
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, yTranslationCycloid45LR, 0.0f});
        model = glm::rotate(model, _fortyFive_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg about medium length. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslationCycloid90MR, yTranslationCycloid90MR, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
    }
    else if(rotationCorrected_deg > 135.0f)
    {
        // Intermediate translations are from cycloid movement using small-radius circle.
        float rotationDiff_rad = rotationCorrected_rad - _oneThirtyFive_rad;
        float xIntermediateTranslationSmR = (rotationDiff_rad * _rSmall) - (_rSmall * sin(rotationDiff_rad));
        float yIntermediateTranslationSmR = _rSmall - (_rSmall * cos(rotationDiff_rad));
        
        model = glm::translate(model, glm::vec3{wholeCircumferences, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xIntermediateTranslationSmR, yIntermediateTranslationSmR, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(cos(_fortyFive_rad)*_rLarge), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-xTranslationCycloid45LR, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 45 deg using large-radius. 
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xTranslationCycloid45LR, yTranslationCycloid45LR, 0.0f});
        model = glm::rotate(model, _fortyFive_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        //
        // Rotate 90 deg about medium length. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslationCycloid90MR, yTranslationCycloid90MR, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
    }
    else if(rotationCorrected_deg > 90.0f)
    {
        // Intermediate translations are from cycloid movement using large-radius circle.
        float rotationDiff_rad = rotationCorrected_rad - _ninety_rad;
        float xIntermediateTranslationLR0 = (rotationDiff_rad * _rLarge) - (_rLarge * sin(rotationDiff_rad));
        float yIntermediateTranslationLR0 = _rLarge - (_rLarge * cos(rotationDiff_rad));
        
        model = glm::translate(model, glm::vec3{wholeCircumferences, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{+(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xIntermediateTranslationLR0, yIntermediateTranslationLR0, 0.0f});
        model = glm::rotate(model, rotationDiff_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
        // Move back to (0,0): Move bottom most point to (0, 0). Note no change in x-direction, just slide left.
        model = glm::translate(model, glm::vec3{-(xTranslationCycloid90MR + _rMedium), 0.0f, 0.0f});
        
        // Rotate 90 deg using medium-radius. Move to correct spot.
        model = glm::translate(model, glm::vec3{xTranslationCycloid90MR, yTranslationCycloid90MR, 0.0f});
        model = glm::rotate(model, _ninety_rad, glm::vec3{0.0f, 0.0f, -1.0f});
    }
    else
    {
        // Intermediate translations due to cycloid rotation using medium-radius circle.
        float xIntermediateTranslationMR0 = (rotationCorrected_rad * _rMedium) - (_rMedium * sin(rotationCorrected_rad));
        float yIntermediateTranslationMR0 = _rMedium - (_rMedium * cos(rotationCorrected_rad));
        
        model = glm::translate(model, glm::vec3{wholeCircumferences, 0.0f, 0.0f});
        model = glm::translate(model, glm::vec3{xIntermediateTranslationMR0, yIntermediateTranslationMR0, 0.0f});
        model = glm::rotate(model, rotationCorrected_rad, glm::vec3{0.0f, 0.0f, -1.0f});
        
    }
    std::vector<glm::mat4> models{};
    models.push_back(model);
    return models;
}
//
std::vector<glm::mat4> Egg::getModels(float time_s, Direction direction)
{
    if (_lastTime_s == -1.0f)
    {
        _lastTime_s = time_s;
        return getModelsPerRotation(_angle0_rad);
    }
    else
    {
        float diffTime_s = time_s - _lastTime_s;
        float angle_rad  = ( 0.5 * (_a0_radPerSec) * (diffTime_s) * (diffTime_s) ) + 
                           ( _v0_radPerSec * diffTime_s) +
                           ( _angle0_rad);
                           
        _lastTime_s = time_s;
        _angle0_rad = angle_rad;
        _a0_radPerSec = _a0_radPerSec + 0.0f;
        _v0_radPerSec = _v0_radPerSec + (_a0_radPerSec * diffTime_s);
        std::cout << "_angle0_rad, diffTime_s, _v0_radPerSec: " << _angle0_rad << ", " << diffTime_s << ", " << _v0_radPerSec << std::endl;
    }
    
    
    return getModelsPerRotation(_angle0_rad);
}

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



