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
  // Time sections for 1 revolution. [-PI, PI]
  _time_sections_s{0.07725f, 0.0605f, .0245f, 0.10775f,
                   0.10775f, 0.0245f, 0.0605f, 0.07725f}
  
{
    /*
    float accumulateT = (std::accumulate(_time_sections_s.begin(), _time_sections_s.end(), 0.0f))/0.01f;
    std::cout << accumulateT << std::endl;
    std::cout << (accumulateT) << std::endl;
    std::cout << (static_cast<int>(accumulateT)) << std::endl;
    */
    _timesSize  = 55; // TODO calculate this. There seems to be an error
    std::cout << "_timeSizeA: " << _timesSize << std::endl;
    _times      = std::vector<float>(_timesSize, 0.0f);
    _radians    = std::vector<float>(_timesSize, 0.0f);
    _velocities = std::vector<float>(_timesSize, 0.0f);
    _acc_rs2    = std::vector<float>(_timesSize, 0.0f);
    _negVelocities = std::vector<float>(_timesSize, 0.0f);
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

std::vector<glm::mat4> Egg::getModels(float time_s, Direction direction)
{
    time_s = time_s/1.0f;
    if (_lastTime_s == -1.0f)
    {
        _firstTime_s    = time_s;
        _lastTime_s     = 0.0f;
        _lastVelocity_s = 15.0f;
        _lastRadians_r  = -PI_F;
        return getModelsPerRotation(_lastRadians_r);
    }
    else
    {
        float curTime_s  = time_s - _firstTime_s;
        float timeDiff_s = curTime_s - _lastTime_s;

        // finalTime_s returned will equal curTime_s. 
        auto [finalTime_s, curRadians_r, curVelocity_rps] = getTimeRadiansAndVSuper(_lastRadians_r, _lastVelocity_s, curTime_s , timeDiff_s);
    
        _lastRadians_r  = curRadians_r;
        _lastTime_s     = curTime_s;
        _lastVelocity_s = curVelocity_rps;
       
        return getModelsPerRotation(_lastRadians_r);
    }
}

std::tuple<float, float, float> Egg::getTimeRadiansAndVSuper(float radians0_r, float velocity0_rps, float time0_s, float timeDiff_s)
{
    auto [finalTime_s, curRadians_r, curVelocity_rps] = getTimeRadiansAndV(radians0_r, velocity0_rps, time0_s, timeDiff_s);
    
    auto [rotationsLast, radiansLast] = getModRadians(_lastRadians_r);
    
    auto [rotationsCur, radiansCur] = getModRadians(curRadians_r);

    if ( //( between(radians0_r, (76.917   * (PI_F / 180.0f)), radiansLast) ) ||
         //( between(radians0_r, (103.083f * (PI_F / 180.0f)), radiansLast) ) ||
         //( between(radians0_r, (160.685f * (PI_F / 180.0f)), radiansLast) ) ||
         //( between(radians0_r, PI_F,                         radiansLast) ) ||
         //( between(radians0_r, (199.315f * (PI_F / 180.0f)), radiansLast) ) ||
         //( between(radians0_r, (256.917f * (PI_F / 180.0f)), radiansLast) ) ||
         //( between(radians0_r, (283.083f * (PI_F / 180.0f)), radiansLast) )
         true
    )
    {
        std::cout << "done: "  << radiansLast << ", " << radiansCur << ":: ";
        int divisor = 3;
        float timeDiffDiv = timeDiff_s/(static_cast<float>(divisor));
        
        float curTime = time0_s;
        float curRadians = radians0_r;
        float curVelocity = velocity0_rps;
        
        for (int ii=0; ii<divisor; ++ii)
        {
            auto [resultTime, resultRadians, resultVelocity]
                    = getTimeRadiansAndV(curRadians, curVelocity, (curTime + (ii * timeDiffDiv)), timeDiffDiv);
                    
            curTime = resultTime;
            curRadians = resultRadians;
            curVelocity = resultVelocity;
            
            std::cout << curRadians << ", ";
        }
        
        std::cout << "done" << std::endl;
        
        return {curTime, curRadians, curVelocity};
    }
    else
    {
        return {finalTime_s, curRadians_r, curVelocity_rps};
    }
    
}

// Return the state at time1_s. time1_s = time0_s + timeDiff_s.
std::tuple<float, float, float> Egg::getTimeRadiansAndV(float radians0_r, float velocity0_rps, float time0_s, float timeDiff_s)
{
    std::vector<float> acc_rps2(9, 0.0f);
    acc_rps2[0] = 0.0f;
    acc_rps2[1] = 116.2f;
    acc_rps2[2] = 0.0f;
    acc_rps2[3] = -271.4f;
    acc_rps2[4] = 0.0f;
    acc_rps2[5] = -acc_rps2[3];
    acc_rps2[6] = -acc_rps2[2];  
    acc_rps2[7] = -acc_rps2[1]; 
    acc_rps2[8] = -acc_rps2[0];

    std::vector<float> aRadians(9, 0.0f);
    aRadians[0] = 0.0f     * (PI_F / 180.0f);
    aRadians[1] = 76.917f  * (PI_F / 180.0f);
    aRadians[2] = 103.083f * (PI_F / 180.0f);
    aRadians[3] = 160.685f * (PI_F / 180.0f);
    aRadians[4] = PI_F;
    aRadians[5] = 199.315f * (PI_F / 180.0f);
    aRadians[6] = 256.917f * (PI_F / 180.0f);
    aRadians[7] = 283.083f * (PI_F / 180.0f);
    aRadians[8] = 2 * PI_F;

    auto [rotations, radians0] = getModRadians(radians0_r);

    size_t rad0Idx = std::distance( aRadians.begin(),
                                    std::lower_bound(aRadians.begin(), aRadians.end(),
                                    radians0)
                                  );
                        
    float acc0 = acc_rps2[rad0Idx] - ( (aRadians[rad0Idx] - radians0) *
                                       (acc_rps2[rad0Idx] - acc_rps2[rad0Idx-1]) /
                                       (aRadians[rad0Idx] - aRadians[rad0Idx-1]) );
                            
    float radiansTry1 = radians0 +
                        (velocity0_rps * timeDiff_s) +
                        (0.5f * timeDiff_s * timeDiff_s * acc0);

    auto [rotationsT1, radiansT1] = getModRadians(radiansTry1);
                     
    //float velocity1_rps = velocity0_rps + (acc0 * timeDiff_s/2);

    size_t ar1Idx = std::distance( aRadians.begin(),
                                   std::lower_bound(aRadians.begin(), aRadians.end(),
                                   radiansT1)
                                 );
                                 
    float acc1 = acc_rps2[ar1Idx] - ( (aRadians[ar1Idx] - radians0) *
                                      (acc_rps2[ar1Idx] - acc_rps2[ar1Idx-1]) /
                                      (aRadians[ar1Idx] - aRadians[ar1Idx-1]) );
                                
    float aveAcc = (acc1+acc0)/2.0f;
    
    float finalVel = (aveAcc * timeDiff_s/2.0f) + velocity0_rps;
    float aveVel = (finalVel + velocity0_rps)/2.0f;
                                  
    float radiansTry2 = radians0 +
                       (aveVel * timeDiff_s) +
                       (0.5f * timeDiff_s * timeDiff_s * aveAcc);
     
    
    //float percent = std::abs(radiansTry2 - radiansTry1);
                       
    //std::cout << "radians: " << radiansTry1 << ", " << radiansTry2 << ", " << percent << std::endl;

     if (_tempCounter == 100 ||
        _tempCounter == 200 ||
        _tempCounter == 500)
    {
            //velocity1_rps = -velocity1_rps;
            //std::cout << "change in Velocity" << std::endl;
    }
    ++_tempCounter;

    
    //std::cout << "radians: " << radiansTry2 << ", " << finalVel << std::endl;
    return {time0_s + timeDiff_s,((rotations) * 2 * PI_F) + radiansTry2, finalVel};
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
    float numTemp = radians / (2.0f * PI_F);
    float numTemp2 = radians / (2*PI_F);
    int numOfRotations = std::floor(radians / (2*PI_F));
    float modRadians = radians - (numOfRotations * (2*PI_F));
    
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
    std::cout << "getCTimePerVal: " << velocity << ", " << time0_s << ", " << time1_s << std::endl;
    size_t utIdx0 = std::distance(
                    _times.begin(),
                    std::lower_bound(_times.begin(), _times.end(), time0_s));
                    
    std::cout <<"utIdx0: " << utIdx0 << ";  ";
                    
    size_t utIdx1 = std::distance(
                    _times.begin(),
                    std::lower_bound(_times.begin(), _times.end(), time1_s));
    std::cout <<"utIdx1: " << utIdx1 << ";  ";

    size_t uvIdx  = std::distance(
                    _velocities.begin(),
                    std::lower_bound(_velocities.begin()+utIdx0, _velocities.begin()+utIdx1, velocity));
    std::cout <<"uvIdx: " << uvIdx << ";  ";
    
    return (_times[uvIdx]) -
       ( ( _velocities[uvIdx] - velocity) *
         ( (_times[uvIdx] - _times[uvIdx-1]) / (_velocities[uvIdx] - _velocities[uvIdx-1]) )
       );
}

float getDeltaTimeTo(float r0, float r1, float v0, float a0)
{
    float c = -(r1 - r0);
    float b = v0;
    float a = a0;
    
    float sqrtP = sqrt((b * b) - (4.0f * a * c));
    float negB  = -b;
    float bot   = 2.0f * a * c;
    
    return (negB + (sqrtP))/bot;
}

bool between(float first, float mid, float last)
{
    return ( (first <= mid) && (mid <= last) );
}

