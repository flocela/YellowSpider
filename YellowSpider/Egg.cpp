#include "Egg.hpp"
#include "EggShape.hpp"
#include "ModelGeometry.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <numeric>
#include <algorithm>

Egg::Egg(float time)
: 
  _eggShape{_eggShapeNumOfSectionsAboutY, (_eggShapeAngleIncrementAboutZ_deg* PI_F/180.0f), _eggShapeMediumRadius}
  
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


std::vector<glm::mat4> Egg::getModels(float time_s, Direction direction)
{
    time_s = time_s/1.0f;
    
    if (_lastTime_s == -1.0f)
    {
        _firstTime_s    = time_s;
        _lastTime_s     = 0.0f;
        _lastVelocity_s = 2.0f;
        _lastRadians_r  = -PI_F;
        return getModelsPerRotation(_lastRadians_r);
    }
    else
    {
        float curTime_s  = time_s    - _firstTime_s;
        float timeDiff_s = curTime_s - _lastTime_s;

        // finalTime_s returned will equal curTime_s. 
        auto [finalTime_s, curRadians_r, curVelocity_rps] = getNextEggStateSuper(_lastRadians_r, _lastVelocity_s, curTime_s , timeDiff_s);
    
        _lastRadians_r  = curRadians_r;
        _lastTime_s     = curTime_s;
        _lastVelocity_s = curVelocity_rps;
       
        return getModelsPerRotation(_lastRadians_r);
    }
}

std::tuple<float, float, float> Egg::getNextEggStateSuper(float radians0_r, float velocity0_rps, float time0_s, float timeDiff_s)
{
    int divisor = 5;
    float timeDiffDiv = timeDiff_s/(static_cast<float>(divisor));
    
    float curTime     = time0_s;
    float curRadians  = radians0_r;
    float curVelocity = velocity0_rps;
    
    for (int ii=0; ii<divisor; ++ii)
    {
        auto [resultTime, resultRadians, resultVelocity]
                = getEggState(curRadians, curVelocity, (curTime + (ii * timeDiffDiv)), timeDiffDiv);
                
        curTime = resultTime;
        curRadians = resultRadians;
        curVelocity = resultVelocity;
    }
    
    return {curTime, curRadians, curVelocity};
}

// Return the state at time1_s. time1_s = time0_s + timeDiff_s.
std::tuple<float, float, float> Egg::getEggState(float radians0_r, float velocity0_rps, float time0_s, float timeDiff_s)
{

    auto [rotations, radians0] = abbrRadians(radians0_r);

    size_t rad0Idx = std::distance( _aPoints_r.begin(),
                                    std::lower_bound(_aPoints_r.begin(), _aPoints_r.end(),
                                    radians0)
                                  );
                        
    float acc0 = _acc_rps2[rad0Idx] - ( (_aPoints_r[rad0Idx] - radians0) *
                                        (_acc_rps2[rad0Idx] - _acc_rps2[rad0Idx-1]) /
                                        (_aPoints_r[rad0Idx] - _aPoints_r[rad0Idx-1]) );
                            
    float radiansTry1 = radians0 +
                        (velocity0_rps * timeDiff_s) +
                        (0.5f * timeDiff_s * timeDiff_s * acc0);

    auto [rotationsT1, radiansT1] = abbrRadians(radiansTry1);

    size_t ar1Idx = std::distance( _aPoints_r.begin(),
                                   std::lower_bound(_aPoints_r.begin(), _aPoints_r.end(),
                                   radiansT1)
                                 );
                                 
    float acc1 = _acc_rps2[ar1Idx] - ( (_aPoints_r[ar1Idx] - radians0) *
                                       (_acc_rps2[ar1Idx] - _acc_rps2[ar1Idx-1]) /
                                       (_aPoints_r[ar1Idx] - _aPoints_r[ar1Idx-1]) );
                                
    float aveAcc = (acc1+acc0)/2.0f;
    
    float finalVel = (aveAcc * timeDiff_s/2.0f) + velocity0_rps;
    float aveVel = (finalVel + velocity0_rps)/2.0f;
                                  
    float radiansTry2 = radians0 +
                       (aveVel * timeDiff_s) +
                       (0.5f * timeDiff_s * timeDiff_s * aveAcc);

     if (_tempCounter == 100 ||
        _tempCounter == 200 ||
        _tempCounter == 500)
    {
            //velocity1_rps = -velocity1_rps;
            //std::cout << "change in Velocity" << std::endl;
    }
    ++_tempCounter;

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

std::tuple<int, float> Egg::abbrRadians(float radians)
{
    // TODO combine into one line.
    float numTemp = radians / (2.0f * PI_F);
    float numTemp2 = radians / (2*PI_F);
    int numOfRotations = std::floor(radians / (2*PI_F));
    float modRadians = radians - (numOfRotations * (2*PI_F));
    
    return {numOfRotations, modRadians};
}

