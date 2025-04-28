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
    float timeHalfRev = accumulate(_time_sections_s.begin(), _time_sections_s.end(), 0.0f)/2.0f;
    float timeOffset  = -timeHalfRev;
    std::cout << "timeHalfRev: " << timeHalfRev << std::endl;
    
    // Insert -time1Rev at beginning of time_sections_s, so that cum_time_s will have the start and end times of each section.
    std::vector<float> copyTimeSections{_time_sections_s.begin(), _time_sections_s.end()};
    copyTimeSections.insert(copyTimeSections.begin(), 0.0f);  // TODO should be timeOffset.
    
    std::vector<float> cum_time_s(copyTimeSections.size(), 0.0f);
    
    std::partial_sum(copyTimeSections.begin(), copyTimeSections.end(), cum_time_s.begin());
    std::cout << "_timeSize: " << _timesSize << std::endl;

    std::cout << "Egg 455" << std::endl;
    std::vector<float> acc_rs2(_time_sections_s.size() + 1, 0.0f);
    acc_rs2[0]   = 0.0f;
    acc_rs2[1]   = 271.4f;
    acc_rs2[2]   = 0.0f;
    acc_rs2[3]   = -116.2f;
    acc_rs2[4]   = 0.0f;
    acc_rs2[5]   = -acc_rs2[3];
    acc_rs2[6]   = -acc_rs2[2];  
    acc_rs2[7]   = -acc_rs2[1]; 
    acc_rs2[8]  = -acc_rs2[0]; 

    _times[0]      = 0.0f;
    _radians[0]    = -PI_F;
    _velocities[0] = 0.0f;
    _acc_rs2[0]    = 0.0f;

    for(int ts=1; ts<_times.size(); ++ts)
    {
        _times[ts] = _times[ts-1] + _deltaTime;
    }
    
    for(int ts=1; ts<_times.size(); ++ts)
    {
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
        _velocities[idx] = _velocities[idx-1] + (tDiff * (_acc_rs2[idx-1] + _acc_rs2[idx])/2.0f);
        _radians[idx] = (_radians[idx-1]) +
                        (tDiff * (_velocities[idx-1] + _velocities[idx])/2.0f) +
                        (0.5 * tDiff * tDiff * (_acc_rs2[idx-1] + _acc_rs2[idx])/2.0f);
        if (std::abs(_radians[idx]) < .005f)
        {
            _radians[idx] = 0.0f;
            float fudge = 1.035f;
            _velocities[idx] *= fudge;
        }
        else if (std::abs(_radians[idx] - (PI_F)) < .011f)
        {
            _radians[idx] = (PI_F);
            float fudge = 1.04f;
            _velocities[idx] = 0.0f;
        } 
    }


    float minVal = *(std::min_element(_velocities.begin(), _velocities.end()));
    std::cout << "minVal: " << minVal << std::endl;

    std::cout << "Egg 516" << std::endl;

    size_t origTimeSize = _times.size();

    _times = std::vector<float>((_times.size() * 3) - 2);
    _times[0] = 2 * timeOffset;
    for(int ts=1; ts<_times.size(); ++ts)
    {
        _times[ts] = _times[ts-1] + _deltaTime;
    }

    std::vector<float> origRadians{_radians.begin(), _radians.end()};
    std::vector<float> origVelocity{_velocities.begin(), _velocities.end()};
    
    auto posR = std::next(_radians.begin(), 0);
    auto posV = std::next(_velocities.begin(), 0);
    
    _radians.insert   (posR, origRadians.begin(),  origRadians.begin()+54);
    _velocities.insert(posV, origVelocity.begin(), origVelocity.begin()+54);
    
    posR = std::next(_radians.end(), 0);
    posV = std::next(_velocities.end(), 0);

    _radians.insert   (posR, origRadians.begin()+1, origRadians.end());
    _velocities.insert(posV, origVelocity.begin()+1, origVelocity.end());

    float offsetRadians = 2 * PI_F;
    for(int ii=0; ii<_times.size(); ++ii)
    { 
        if (ii == 108)
        {
            std::cout << "Stop" << std::endl;
        }
        if (ii < 54)
        {
            _radians[ii] -= offsetRadians;
        }
        else if (ii > 108)
        {
            _radians[ii] += offsetRadians;
        }
    }
    
    for(int ii=1; ii<_times.size(); ++ii)
    {
        std::cout << ii << ":: " << (_times[ii]) << ", " << (_radians[ii]) << ", " << _velocities[ii] << ", " << (_radians[ii] - _radians[ii-1]) << std::endl;
    }

    std::cout << std::endl;
}

std::vector<glm::mat4> Egg::getModels(float time_s, Direction direction)
{
    time_s = time_s/3.0f;
    if (_lastTime_s == -1.0f)
    {
        _firstTime_s    = time_s;
        _lastTime_s     = 0.0f;
        //_lastVelocity_s = _velocities[50];
        _lastVelocity_s = 0.5f;
        _lastRadians_r  = -PI_F;
        std::cout << "545 _lastRadians_r: " << _lastRadians_r << std::endl;
        return getModelsPerRotation(_lastRadians_r);
    }
    else
    {
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
        //std::cout << "lastRadians: " << _lastRadians_r << " = " << numRotations << " + " << radians1_r << std::endl;
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
        
        auto [temp_numRotations, temp_baseRadians0_r] = getModRadians(_lastRadians_r);
        if ( (std::abs(temp_baseRadians0_r - PI_F) < 0.01f) && (_lastVelocity_s > 0.0f))
        {
            //_lastVelocity_s += 0.05f;
        }
       // std::cout << "_lastRadians_r" << _lastRadians_r << std::endl;
        return getModelsPerRotation(_lastRadians_r);
    }
}

 // time0_s is less than time for one rotation.
 std::tuple<float, float, float> Egg::getTimeRadiansAndV(float velocity0_rps, float time0_s, float timeDiffRT_s)
 {
    /*
    std::vector<float> acc_rps2(11, 0.0f);
    acc_rps2[0]   = 0.0f;
    acc_rps2[1]   = 116.2f;
    acc_rps2[2]   = 0.0f;
    acc_rps2[3]   = -5000.4f;
    acc_rps2[4]   = -3000.4f;
    acc_rps2[5]   = 0.0f;
    acc_rps2[6]   = -acc_rps2[4];
    acc_rps2[7]   = -acc_rps2[3];
    acc_rps2[8]   = -acc_rps2[2];  
    acc_rps2[9]   = -acc_rps2[1]; 
    acc_rps2[10]  = -acc_rps2[0];
    
    std::vector<float> aRadians(11, 0.0f);
    aRadians[0] = 0.0f;
    aRadians[1] = 76.917f;
    aRadians[2] = 103.083f;
    aRadians[3] = 160.685f;
    aRadians[4] = 179.99f;
    aRadians[5] = PI_F;
    aRadians[6] = 180.01f;
    aRadians[7] = 199.315f;
    aRadians[8] = 256.917f;
    aRadians[9] = 283.083f;
    aRadians[10] = 2 * PI_F;
    */
    
    std::vector<float> acc_rps2(9, 0.0f);
    acc_rps2[0]   = 0.0f;
    acc_rps2[1]   = 116.2f;
    acc_rps2[2]   = 0.0f;
    acc_rps2[3]   = -271.4f;
    acc_rps2[4]   = 0.0f;
    acc_rps2[5]   = -acc_rps2[3];
    acc_rps2[6]   = -acc_rps2[2];  
    acc_rps2[7]   = -acc_rps2[1]; 
    acc_rps2[8]  = -acc_rps2[0];
    
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
    
    auto [rotations, radians0] = getModRadians(_lastRadians_r);
    if (radians0 > 6.24f)
    {
        std::cout << "stop" << std::endl;
    }
    size_t ar0Idx = std::distance(aRadians.begin(),
                                  std::lower_bound(aRadians.begin(), aRadians.end(),
                                  radians0)
                                 );
                        
    float acc0 = acc_rps2[ar0Idx] - (
                                   (aRadians[ar0Idx] - radians0) *
                                   (acc_rps2[ar0Idx] - acc_rps2[ar0Idx-1]) /
                                   (aRadians[ar0Idx] - aRadians[ar0Idx-1])
                                  );
                                  
    float radiansTry1 = radians0 +
                       (velocity0_rps * timeDiffRT_s) +
                       (0.5f * timeDiffRT_s * timeDiffRT_s * acc0);
                       
    auto [rotationsT1, radiansT1] = getModRadians(radiansTry1);
                     
    float velocity1_rps = velocity0_rps + (acc0 * timeDiffRT_s);
    
    size_t ar1Idx = std::distance(aRadians.begin(),
                                  std::lower_bound(aRadians.begin(), aRadians.end(),
                                  radiansT1)
                                 );
                                 
    float acc1 = acc_rps2[ar1Idx] - (
                                   (aRadians[ar1Idx] - radians0) *
                                   (acc_rps2[ar1Idx] - acc_rps2[ar1Idx-1]) /
                                   (aRadians[ar1Idx] - aRadians[ar1Idx-1])
                                  );
                                  
    float aveVel = (velocity1_rps + velocity0_rps)/2.0f;
    float aveAcc = (acc1+acc0)/2.0f;
                                  
    float radiansTry2 = radians0 +
                       (aveVel * timeDiffRT_s) +
                       (0.5f * timeDiffRT_s * timeDiffRT_s * aveAcc);
    
    std::cout << "radians: " << radiansTry2 << ", " << radiansT1 << ", " << (radiansTry2 - radiansTry1) << ", " << aveAcc << std::endl;
    
    return {time0_s + timeDiffRT_s, radiansTry2, velocity1_rps};
                     
    
        
 
        /*++_tempCounter;
        
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
        float baseTime0_s       = getCorrespondingTimePerRadians(lastRadians);
        float baseVelocity0_rps = getCorrespondingVelocity(baseTime0_s, _velocities); // velocity is based on baseTime0_s, so it's always positive here.
        float baseTimeDiff      = timeDiffRT_s * ((_lastVelocity_s+1)/(baseVelocity0_rps+1)); // TODO baseTimeDiff < 1 rotation.
        float baseTime1_s       = baseTime0_s + baseTimeDiff; // TODO may have rotated to index 0
        
        float dirbaseVelocity0_rps = origDir * baseVelocity0_rps;
        float baseVelocity1_rps    = getCorrespondingVelocity(baseTime1_s, _velocities);
        float dirBaseVelocity1_rps = origDir * baseVelocity1_rps;
        float diffTemp = (origVelocity0_rps - dirbaseVelocity0_rps);
        float realVelocity1_rps    = dirBaseVelocity1_rps + diffTemp;
        
        if ((origVelocity0_rps * realVelocity1_rps) >= 0)
        {
            std::cout << "653: ";
            std::cout << realVelocity1_rps << ", " <<  getCorrespondingRadians(baseTime1_s) << ", " << baseTime0_s << "::  ";
            return {baseTime1_s, getCorrespondingRadians(baseTime1_s), realVelocity1_rps};
        }
        else
        {
            std::cout << "679: ";
            // The base velocity when the current velocity is zero.
            float baseVelocityAtRealZero = baseVelocity0_rps - origVelocity0_rps + 0.0f;
            float zeroBaseTime           = getCorrespondingTimePerVelocity(baseVelocityAtRealZero, time0_s, baseTime1_s);
            // TODO don't use an if statement, it's -1 * origDir * ....
            float finalBaseTime = zeroBaseTime + ((origDir == 1) ?
                                                  ((-1) * std::abs(zeroBaseTime - baseTime1_s)) :
                                                  (( 1) * std::abs(zeroBaseTime - baseTime1_s)) );                                 
            float finalBaseVelocity = getCorrespondingVelocity(finalBaseTime, _velocities);
            float finalRealVelocity = finalBaseVelocity + (velocity_rps - baseVelocity0_rps);
            finalRealVelocity = (origDir == 1) ? (-finalRealVelocity) : (finalRealVelocity);
            std::cout << "690: " << finalRealVelocity << std::endl;
            return {finalBaseTime,
                    getCorrespondingRadians(finalBaseTime),
                    finalRealVelocity};
        }*/
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
    
    if (numOfRotations == -1 && modRadians == (PI_F))
    {
        //return {0, 0.0f};
    }
    
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

