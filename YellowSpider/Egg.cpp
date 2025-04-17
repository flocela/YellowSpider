#include "Egg.hpp"
#include "EggShape.hpp"
#include "ModelGeometry.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <numeric>
#include <algorithm>

Egg::Egg(float time)
: _eggShapeNumOfSectionsAboutY{20},
  _eggShapeAngleIncrementAboutZ_deg{5.0f},
  _eggShapeMediumRadius{5.0f},
  _eggShape{_eggShapeNumOfSectionsAboutY, (_eggShapeAngleIncrementAboutZ_deg* PI_F/180.0f), _eggShapeMediumRadius},
  _deltaTime{0.01},
  _time_sections_s{0.0775f, 0.0605f, .0245f, 0.1025f, 0.1025f, 0.0245f, 0.0605f, 0.0775f},
  _timesSize{53}, // TODO set this somewhere else.
  _times(_timesSize, 0.0f),
  _radians(_timesSize, 0.0f),
  _velocities(_timesSize, 0.0f),
  _acc_rs2(_timesSize, 0.0f),
  _negVelocities(_timesSize, 0.0f)
{
    setTimes();
    ModelGeometry modelGeometry{};
    modelGeometry.setVertices(_eggShape.getVertices());
    modelGeometry.setIndices(_eggShape.getIndices());

    _modelGeometries.push_back(modelGeometry);
}

std::vector<glm::mat4> Egg::getModelsPerRotation(float rotation_rad)
{
    glm::mat4 model{1.0f};
    
    float   circumference        = _eggShape.getEggCircumferenceAboutZ();
    int32_t numOfFullRotations   = rotation_rad/(2.0f*PI_F);
    float   wholeCircumferences  = numOfFullRotations * circumference;
    float   moveLeft             = -100.0f;
    
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
        
        model = glm::translate(model, glm::vec3{moveLeft, 0.0f, 0.0f});
        
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
        
        model = glm::translate(model, glm::vec3{moveLeft, 0.0f, 0.0f});
        
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
        
        model = glm::translate(model, glm::vec3{moveLeft, 0.0f, 0.0f});
        
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
        
        model = glm::translate(model, glm::vec3{moveLeft, 0.0f, 0.0f});
        
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
        
        model = glm::translate(model, glm::vec3{moveLeft, 0.0f, 0.0f});
        
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
    /*
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
    */
    std::vector<glm::mat4> models{};
    models.push_back(model);
    return models;
}

void Egg::setTimes()
{
    // acceleration radians per second squared
    //std::vector<float> acc_rs2{44.67f, -30.712f, 30.712f, -44.67f};
    //std::vector<float> time_sections_s{0.0f, 0.25f, 0.75f, 1.25f, 1.5f};
    
    float time1HalfRev = accumulate(_time_sections_s.begin(), _time_sections_s.end(), 0.0f)/2.0f;
    std::cout << "time1Rev: " << time1HalfRev << std::endl;
    // TODO don't change _time_sections_s like this. Maybe make a copy.
    // Insert -time1Rev at beginning of time_sections_s, so that cum_time_s will have the start and end times of each section.
    _time_sections_s.insert(_time_sections_s.begin(), -time1HalfRev);
    std::vector<float> cum_time_s(_time_sections_s.size(), 0.0f);
    
    std::partial_sum(_time_sections_s.begin(), _time_sections_s.end(), cum_time_s.begin());
    std::cout << "_timeSize: " << _timesSize << std::endl;
    
    std::cout << "Egg 455" << std::endl;
    std::vector<float> acc_rs2(_time_sections_s.size(), 0.0f);
    acc_rs2[0]  = 0.0f;
    acc_rs2[1]  = 271.4f;
    acc_rs2[2]  = 0.0f;
    acc_rs2[3]  = -116.2f;
    acc_rs2[4]  = 0.0f;
    acc_rs2[5]  = -acc_rs2[3];  // 1.162
    acc_rs2[6]  = -acc_rs2[2];  // 0.0f
    acc_rs2[7]  = -acc_rs2[1];  // -2.714
    acc_rs2[8]  = -acc_rs2[0];  // 0.0f
    /*
    acc_rs2[9]  = -acc_rs2[7];  // 2.714
    acc_rs2[10] = -acc_rs2[6];  // 0.0f
    acc_rs2[11] = -acc_rs2[5];  // -1.162
    acc_rs2[12] = -acc_rs2[4];  // 0.0f
    acc_rs2[13] = -acc_rs2[3]; // 1.162
    acc_rs2[14] = -acc_rs2[2]; // 0.0f
    acc_rs2[15] = -acc_rs2[1]; // -2.714
    acc_rs2[16] = -acc_rs2[0]; // 0.0f
    */
    
    _times[0]      = -time1HalfRev;
    _radians[0]    = -PI_F;
    _velocities[0] = 0.0f;
    _acc_rs2[0]    = 0.0f;
    
    for(int ts=1; ts<_times.size(); ++ts)
    {
        _times[ts] = _times[ts-1] + _deltaTime;
    }
    
    std::cout << "Egg 484" << std::endl;
    
    for(int ts=1; ts<_times.size(); ++ts)
    {
        if (ts == 53)
        {
            std::cout << "ts: " << ts << std::endl;
        }
        float targetTime  = _times[ts];
        size_t cumTimeIdx = std::distance(cum_time_s.begin(),
                                          std::lower_bound(cum_time_s.begin(), cum_time_s.end(),
                                          targetTime));
        float time2 = cum_time_s[cumTimeIdx];
        float acc2  = acc_rs2[cumTimeIdx];
        float time1 = cum_time_s[cumTimeIdx-1];
        float acc1  = acc_rs2[cumTimeIdx-1];
        
        // Line formula (y2-y1)/(x2-x1).
        _acc_rs2[ts] = acc2 - ((time2-targetTime) * (acc2-acc1) / (time2-time1));
    }
    
    std::cout << "Egg 501" << std::endl;
    
    for(int idx=1; idx<_times.size(); ++idx)
    {
        float tDiff = _times[idx] - _times[idx-1];
        _radians[idx] = (_radians[idx-1]) +
                        (_velocities[idx-1] * tDiff) +
                        (0.5 * _acc_rs2[idx] * tDiff * tDiff);
                        
        _velocities[idx] = _velocities[idx-1] + (_acc_rs2[idx] * tDiff);
        if (abs(_velocities[idx]) < .0001)
        {
            _velocities[idx] = 0.0f;
        }
    }
    
    float minVal = *(std::min_element(_velocities.begin(), _velocities.end()));
    std::cout << "minVal: " << minVal << std::endl;
    if (minVal < 0)
    {
        for_each(_velocities.begin(), _velocities.end(), [minVal](float& n) {n -= minVal;});
    }
    
    std::cout << "Egg 516" << std::endl;
    
    for(int ii=0; ii<_times.size(); ++ii)
    {
        std::cout << ii << ":: " << (_times[ii] - _times[0]) << ", " << (PI_F + _radians[ii]) << std::endl;
    }
}

std::vector<glm::mat4> Egg::getModels(float time_s, Direction direction)
{
    if (_lastTime_s == -1.0f)
    {
        _firstTime_s    = time_s;
        _lastTime_s     = 0.0f;
        //_lastVelocity_s = _velocities[50];
        _lastVelocity_s = 0.2f;
        _lastRadians_r  = -2*PI_F;
        std::cout << "545 _lastRadians_r: " << _lastRadians_r << std::endl;
        return getModelsPerRotation(_lastRadians_r);
    }
    else
    {
        std::cout << "time: " << time_s << std::endl;
        if (_lastVelocity_s == -0.02f)
        {
            std::cout << "-0.02 *******************************" << std::endl;
        }
        
        if (_lastVelocity_s == 0.02f)
        {
            std::cout << "0.02 *******************************" << std::endl;
        }
        
        float curTime_s   = time_s - _firstTime_s;
        float timeDiff_s  = curTime_s - _lastTime_s;
        auto [numRotations, baseRadians0_r] = getModRadians(_lastRadians_r);
        float baseTime0_s    = getModTime(_lastTime_s);

        auto [time1, radians1_r, velocity1_rps] = getTimeRadiansAndV(_lastVelocity_s, baseTime0_s, timeDiff_s);
        
        // TODO DOn't add to last radians. add whole radians. Add whole revolutions.
        _lastRadians_r = numRotations * (2*PI_F) + radians1_r;
        std::cout << "lastRadians: " << _lastRadians_r << " = " << numRotations << " + " << radians1_r << std::endl;
        //_lastRadians_r += ( (radians1_r >=  baseRadians0_r) ? (radians1_r -  baseRadians0_r) : ((2*PI_F) - baseRadians0_r + radians1_r) );
        //_lastRadians_r = radians1_r;
        _lastTime_s = curTime_s;
    
        if (std::abs(velocity1_rps) < 0.0001f)
        {
            std::cout << "changed to +-0.02" << std::endl;
            _lastVelocity_s = (_lastVelocity_s > 0.0f) ? (-0.02f) : (0.02f);
        }
        else
        {
            _lastVelocity_s = velocity1_rps;
        }
        
        std::cout << "_lastRadians_r" << _lastRadians_r << std::endl;
        return getModelsPerRotation(_lastRadians_r);
    }
}

 // time0_s is less than time for one rotation.
 std::tuple<float, float, float> Egg::getTimeRadiansAndV(float velocity_rps, float time0_s, float timeDiffRT_s)
 {
        ++_tempCounter;
        
        if (_tempCounter == 100 ||
            _tempCounter == 200 ||
            _tempCounter == 500)
        {
            //velocity_rps = -velocity_rps;
        }
        
        float origVelocity0_rps = velocity_rps;
        int   origDir  = (origVelocity0_rps > 0.0f) ? 1 : -1;
        if (origDir == -1)
        {
            //std::cout << "negative" << std::endl;
            //std::cout << _times[0] << std::endl;
        }
        auto [rotations, lastRadians] = getModRadians(_lastRadians_r);
        std::cout << "Egg 610" << std::endl;
        float baseTime0_s       = getCorrespondingTimePerRadians(lastRadians);
        std::cout << "Egg 612" << std::endl;
        float baseVelocity0_rps = getCorrespondingVelocity(baseTime0_s, _velocities);
        std::cout << "Egg 614" << std::endl;
        float baseTimeDiff      = timeDiffRT_s * ((_lastVelocity_s+1)/(baseVelocity0_rps+1)); // TODO baseTimeDiff < 1 rotation.
        float baseTime1_s       = baseTime0_s + baseTimeDiff; // TODO may have rotated to index 0
        
        float dirbaseVelocity0_rps = origDir * baseVelocity0_rps;
        std::cout << "Egg 616" << std::endl;
        float baseVelocity1_rps    = getCorrespondingVelocity(baseTime1_s, _velocities);
        float dirBaseVelocity1_rps = origDir * baseVelocity1_rps;
        float diffTemp = (origVelocity0_rps - dirbaseVelocity0_rps);
        float realVelocity1_rps    = dirBaseVelocity1_rps + diffTemp;
        
        if ((origVelocity0_rps * realVelocity1_rps) >= 0)
        {
            std::cout << "653: " << realVelocity1_rps << ", " <<  getCorrespondingRadians(baseTime1_s) << ", " << baseTime0_s << "::  ";
            return {baseTime1_s, getCorrespondingRadians(baseTime1_s), realVelocity1_rps};
        }
        else
        {
            // The base velocity when the current velocity is zero.
            float baseVelocityAtRealZero = baseVelocity0_rps - origVelocity0_rps + 0.0f;
            float zeroBaseTime           = getCorrespondingTimePerVelocity(baseVelocityAtRealZero, time0_s, baseTime1_s);
            
            float finalBaseTime = zeroBaseTime + ((origDir == 1) ?
                                                  ((-1) * std::abs(zeroBaseTime - baseTime1_s)) :
                                                  (( 1) * std::abs(zeroBaseTime - baseTime1_s)) );
            float finalBaseVelocity = getCorrespondingVelocity(finalBaseTime, _velocities);
            float finalRealVelocity = finalBaseVelocity + (velocity_rps - baseVelocity0_rps);
            finalRealVelocity = (origDir == 1) ? (-finalRealVelocity) : (finalRealVelocity);
            std::cout << "666: " << finalRealVelocity << std::endl;
            return {finalBaseTime,
                    getCorrespondingRadians(finalBaseTime),
                    finalRealVelocity};
        }
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

 float Egg::moduloRotationsAsPositive(float rotation_rad)
 {
    float moduloRotations = rotation_rad - ( (2 * PI_F) * ( static_cast<int>(rotation_rad/(2*PI_F)) ) );
     
    return (moduloRotations < 0) ? ((2*PI_F) + moduloRotations) : (moduloRotations);
 }

 float Egg::getCorrespondingRadians(float targetTime)
 {
    size_t utIdx  = std::distance(_times.begin(),
                                  std::lower_bound(_times.begin(), _times.end(),
                                  targetTime));
    return (_radians[utIdx]) -
           ( (_times[utIdx] - targetTime) *
             ( (_radians[utIdx] - _radians[utIdx-1]) / (_times[utIdx] - _times[utIdx-1]) )
           );
 }
 
float Egg::getCorrespondingTimePerRadians(float targetRadians)
{
    // urvidx is index at targetRadians, or index before targetRadians.
    size_t urIdx  = std::distance(_radians.begin(),
                                  std::lower_bound(_radians.begin(), _radians.end(), targetRadians));
    return (_times[urIdx]) -
           ( (_radians[urIdx] - targetRadians) *
             ( (_times[urIdx] - _times[urIdx-1]) / (_radians[urIdx] - _radians[urIdx-1]) )
           );
           
}

 float Egg::getCorrespondingVelocity(float time, const std::vector<float>& velocities)
 {
    size_t utIdx  = std::distance(_times.begin(),
                                  std::lower_bound(_times.begin(), _times.end(),
                                  time));
                    
    return (velocities[utIdx]) -
           ( (_times[utIdx] - time) *
             ( (velocities[utIdx] - velocities[utIdx-1]) / (_times[utIdx] - _times[utIdx-1]) )
           );
 }
 std::tuple<int, float> Egg::getModRadians(float radians)
 {
    // TODO combine into one line.
    int numOfRotations = std::floor(_lastRadians_r / (2*PI_F));
    float modRadians = _lastRadians_r - (numOfRotations * (2*PI_F));
    return {numOfRotations, modRadians};
 }
 
 float Egg::getModTime(float time)
 {
    // TODO combine into one line.
    float numRotations = std::floor(time / _times[_times.size()-1]);
    float returnValue = time - (numRotations * _times[_times.size()-1]);
    return returnValue;
 }



 float Egg::getCorrespondingTimePerVelocity(float velocity, float time0_s, float time1_s)
 {
    size_t utIdx0 = std::distance(
                        _times.begin(),
                        std::lower_bound(_times.begin(), _times.end(), time0_s));
                        
    size_t utIdx1 = std::distance(
                        _times.begin(),
                        std::lower_bound(_times.begin(), _times.end(), time1_s));                     
    
    size_t uvIdx  = std::distance(
                        _velocities.begin(),
                        std::lower_bound(_velocities.begin()+utIdx0, _velocities.begin()+utIdx1, velocity));
    
    return (_times[uvIdx]) -
           ( ( _velocities[uvIdx] - velocity) *
             ( (_times[uvIdx] - _times[uvIdx-1]) / (_velocities[uvIdx] - _velocities[uvIdx-1]) )
           );
 }

