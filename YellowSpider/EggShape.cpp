#include "EggShape.hpp"
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

EggShape::EggShape(
    uint32_t numOfSectionsAboutY,
    float    angleAboutZ,
    float    mediumRadius
):

    _rMedium{mediumRadius},
    _rLarge {2.0f * mediumRadius},
    _rSmall{_rLarge - (1.414f * _rMedium)},
    _numOfSectionsAboutY{numOfSectionsAboutY},
    _angleAboutZ{angleAboutZ},
    _halfWeight{ (1.0f/8.0f * PI_F * _rLarge * _rLarge) - (0.5f * _rMedium * _rMedium) + (1.0f/8.0f*PI_F*_rSmall * _rSmall) + 0.25f * PI_F * _rMedium * _rMedium}
{
    std::cout << "Create EggShape" << std::endl;
    
    populateReferenceAnglesAboutZ();
    
    std::cout << "eggshape line 25" << std::endl;
    
    outlineVerticesAboutZ();
    
    std::cout << "eggshape line 29" << std::endl;
    
    populateVertAboutYAxis();
    
    std::cout << "eggshape line 33" << std::endl;
    
    populateIndices();
    
    std::cout << "eggshape line 37" << std::endl;
}

void EggShape::populateIndices()
{
    int numVertInRow = _numOfSectionsAboutY + 1;
    for(int ii=1; ii<_vertices.size() - numVertInRow; ++ii)
    {
        if (ii % numVertInRow == 0)
        {
            _indices.push_back(ii);
            _indices.push_back(ii - _numOfSectionsAboutY);
            _indices.push_back(ii + 1);
            _indices.push_back(ii);
            _indices.push_back(ii + 1);
            _indices.push_back(ii + numVertInRow);
        }
        else
        {
            _indices.push_back(ii);
            _indices.push_back(ii+1);
            _indices.push_back(ii+1 + numVertInRow);
            _indices.push_back(ii);
            _indices.push_back(ii+1 + numVertInRow);
            _indices.push_back(ii   + numVertInRow);
        }
        
    }
}

void EggShape::populateReferenceAnglesAboutZ()
{
    float delta = 1.0f * PI_F / 180.0f;

    float angle = 0.0f;
    while(angle < _threeSixty_rad)
    {
        std::cout << "angle: " << angle << std::endl;
        if (angle > _oneEighty_rad - delta && angle < _oneEighty_rad + delta)
        {
            angle = _oneEighty_rad;
        }
        if (angle > _threeSixty_rad - delta)
        {
            break;
        }
        _referenceAnglesAboutZ.push_back(angle);
        angle += _angleAboutZ;
    }
}

void EggShape::outlineVerticesAboutZ()
{
    for(float x : _referenceAnglesAboutZ)
    {
        float radius     = getCorrespondingRadius(x);
        glm::vec2 center = getCorrespondingCenter(x);
        
        _eggOutlineAboutZ.push_back({sin(x) * radius, _rMedium - (cos(x) * radius), 0.0f});
    }
}

void EggShape::populateVertAboutYAxis()
{
    glm::mat4 transformMatrix{1.0f};
    glm::vec3 rotationAxis{0.0f, 1.0f, 0.0f};
    size_t    colorIndex = 0;
    
    for(int ii=0; ii<_referenceAnglesAboutZ.size(); ++ii)
    {
        float angle = _referenceAnglesAboutZ[ii];
        if (angle != _zero_rad && angle != _oneEighty_rad)
        {
            _vertices.push_back(Vertex{ _eggOutlineAboutZ[ii], _colors[colorIndex]});
        }
        else
        {
            for(int jj=0; jj<_numOfSectionsAboutY; ++jj)
            {
                glm::vec3 point = _eggOutlineAboutZ[ii];
                _vertices.push_back(
                Vertex{ glm::rotate(transformMatrix,
                                    glm::radians(360.0f/(_numOfSectionsAboutY)) * ii, rotationAxis ) * glm::vec4(point, 1.0),
                        _colors[colorIndex]
                });
            }
        }
        
        colorIndex = ((colorIndex + 1) % _colors.size());
    }
}

void EggShape::populateVerticesAboutYAxis(glm::vec3 point, glm::vec3 color)
{
    glm::mat4 transformMatrix{1.0f};
    glm::vec3 rotationAxis{0.0f, 1.0f, 0.0f};
    
    for(int ii = 0; ii<_numOfSectionsAboutY; ++ii)
    {
        _vertices.push_back(Vertex{ 
        glm::rotate(transformMatrix,
                    glm::radians(360.0f/(_numOfSectionsAboutY)) * ii, rotationAxis ) * glm::vec4(point, 1.0),
        color});
    }
}

void EggShape::populateVerticesAboutZAxis()
{

    //std::cout << "68: radius: " << _rSmall << ", " << _rMedium << ", " << _rLarge << std::endl;
    // Egg sits with wide end at bottom. Bottom most point of egg is at 270 deg. Points will be designated
    // about the z-axis (traveling counter clock-wise). Vertices will be populated for each each of these points.
    // Vertices are a circle of points about the y-axis.
    //glm::vec3 bottomPoint = {0.0f, 0.0f, 0.0f};
    
    
    
    // FIRST SECTION
    //   Theta range is [270.0, 360.0). Polar radius is rMedium. Note point at 270.0 deg should be at (0, 0).
    //   So, move center from (0, 0) to (0, rMedium), then bottom of egg is at (0, 0).
    //   Note, range is [inclusive, exclusive).
    populateVerticesAboutZAxis(
        _twoSeventy_rad,
        _ninety_rad + (2 * PI_F));
        
    // SECOND SECTION
    //   Theta range is [0.0, 45.0). Note, range is [inclusive, exclusive).
    populateVerticesAboutZAxis(
        _ninety_rad,
        _twoSeventy_rad);                        
    
    size_t colorCount = 0;
    // Populate the Vertices about the Y axis. Only populate from (270, 0], (0, 90).
    // Point at 270 is singular, doesn't represent a circle about Y.
    // Point at 90 is singular, doesn't represent a circle about Y.
    for(size_t ii = 0; ii<_referenceAnglesAboutZ.size(); ++ii)
    {
        if( !( _referenceAnglesAboutZ[ii] >= _ninety_rad && _referenceAnglesAboutZ[ii] <= _twoSeventy_rad) )
        {
            glm::vec3 point = _eggOutlineAboutZ[ii];
            populateVerticesAboutYAxis(point, _colors[colorCount]);
            
            colorCount = ((colorCount + 1) % _colors.size());
        }
    }
    
    _circumference = _circumferenceTraveledAboutZ[_circumferenceTraveledAboutZ.size()-1] + (sin(_twoSeventy_rad - _referenceAnglesAboutZ[_referenceAnglesAboutZ.size()-1]) * _rMedium);
    
    int _angle0Index = 0; 
    for(; _angle0Index<_referenceAnglesAboutZ.size(); ++_angle0Index)
    {
        if(_referenceAnglesAboutZ[_angle0Index] > _referenceAnglesAboutZ[_angle0Index+1])
        {
            break;
        }
    }
    
    for(int ii=_angle0Index; ii<_referenceAnglesAboutZ.size(); ++ii)
    {
        if(ii == _angle0Index)
        {
            _referenceAnglesForWeight.push_back(0.0f);
        }
        else
        {
            _referenceAnglesForWeight.push_back(_referenceAnglesAboutZ[ii]);
        }
        
    }
    for(int ii=0; ii<_angle0Index; ++ii)
    {
        _referenceAnglesForWeight.push_back(_referenceAnglesAboutZ[ii]);
    }
    
    for(int ii=0; ii<_referenceAnglesForWeight.size(); ++ii)
    {
        //std::cout << "_refWeight: " << ii << ", " << (_referenceAnglesForWeight[ii] * 180.0f/PI_F) << std::endl;
    }
    
    populateForwardAndBackwardsWeights();
    
    //std::cout << "circumference: " << _circumference << std::endl;
    //std::cout << "sizes: " << _circumferenceTraveledAboutZ.size() << ", " << _referenceAnglesAboutZ.size() << std::endl;
    for(int ii=1; ii<_circumferenceTraveledAboutZ.size(); ++ii)
    {
        //std::cout << ii << ", " << (_referenceAnglesAboutZ[ii] * 180.0 / PI_F) << ",  "<< _circumferenceTraveledAboutZ[ii] << ", " << 
       // _circumferenceTraveledAboutZ[ii] - _circumferenceTraveledAboutZ[ii-1] <<  std::endl;
    }
    
    
}

float EggShape::getForwardRotationWeight(float angle_rad)
{
    
    size_t size = _referenceAnglesForWeight.size();
    
    //std::cout << "egg shape getForwardRotationWeight for angle: " << (angle_rad * 180.0f/ PI_F) << std::endl;
    
    if (angle_rad >= _referenceAnglesForWeight[size-1] )
    {
        
        float deltaForwardOverAngleDiff   = (_forwardWeights[0] - _forwardWeights[size-1]) /
                                            (_threeSixty_rad - _referenceAnglesForWeight[size-1]);
        
        float deltaBackwardsOverAngleDiff = (_backwardWeights[0] - _backwardWeights[size-1]) /
                                            (_threeSixty_rad - _referenceAnglesForWeight[size-1]);
        
        
        float forwardWeight   = _forwardWeights[size-1] +
                                ( (angle_rad-_referenceAnglesForWeight[size-1]) * deltaForwardOverAngleDiff );
                              
        float backwardsWeight = _backwardWeights[size-1] +
                                ( (angle_rad-_referenceAnglesForWeight[size-1]) * deltaBackwardsOverAngleDiff );
        
        return forwardWeight - backwardsWeight;
    }
    else
    {
        int index = 0;
        while(index < _referenceAnglesForWeight.size())
        {
            if(_referenceAnglesForWeight[index] > angle_rad)
            {
                break;
            }
            ++index;
        }
        //std::cout << "index: " << index << std::endl;
        
        float deltaForwardOverAngleDiff = (_forwardWeights[index] - _forwardWeights[index-1]) /
                                          (_referenceAnglesForWeight[index] - _referenceAnglesForWeight[index-1]);
        
        float deltaBackwardsOverAngleDiff = (_backwardWeights[index] - _backwardWeights[index-1]) /
                                            (_referenceAnglesForWeight[index] - _referenceAnglesForWeight[index-1]);
        
        
        float forwardWeight   = _forwardWeights[index-1] +
                                ( (angle_rad-_referenceAnglesForWeight[index-1]) * deltaForwardOverAngleDiff );
                              
        float backwardsWeight = _backwardWeights[index-1] +
                                ( (angle_rad-_referenceAnglesForWeight[index-1]) * deltaBackwardsOverAngleDiff );
        
        //std::cout << "fW- bW: " << (angle_rad * 180.0f/ PI_F) << ": " << (forwardWeight - backwardsWeight) << std::endl;
        return forwardWeight - backwardsWeight;
    }
    
}

void EggShape::populateForwardAndBackwardsWeights()
{
    std::vector<float> addForwardWeights(_referenceAnglesForWeight.size(), 0.0f);
    std::vector<float> subtrForwardWeights(_referenceAnglesForWeight.size(), 0.0f);
    
    float betaCutOff_rad = atan( (cos(_fortyFive_rad)*_rSmall) / ( (cos(_fortyFive_rad)*_rSmall) + (_rMedium) ) );
    //std::cout << "eggshape 127  " << _referenceAnglesAboutZ.size() << std::endl;
    for(int ii=1; ii<_referenceAnglesForWeight.size(); ++ii)
    {
        float orig_rad = _referenceAnglesForWeight[ii];
        float deltaBeta_rad = orig_rad - _referenceAnglesForWeight[ii-1];
        //std::cout << "eggshape 132" << std::endl;
        if(orig_rad == _twoSeventy_rad)
        {
            addForwardWeights[ii] = 0;
            subtrForwardWeights[ii] = 0;
        }
        else if ( (orig_rad >= _twoSeventy_rad) && (orig_rad < _twoSeventy_rad + betaCutOff_rad) )
        {
            float beta_rad = orig_rad - _twoSeventy_rad;
            float theta_rad = beta_rad * _fortyFive_rad / betaCutOff_rad;
            float radius = (_rMedium + (cos(theta_rad)*_rSmall)) / cos(beta_rad);
            
            addForwardWeights[ii] = 0.5f * sin(deltaBeta_rad) * radius * radius;
            subtrForwardWeights[ii] = 0.5f * sin(deltaBeta_rad) * _rMedium * _rMedium;
        }
        else if ( orig_rad >= _twoSeventy_rad + betaCutOff_rad && orig_rad < _threeSixty_rad)
        {
            float beta_rad = orig_rad - (_twoSeventy_rad + betaCutOff_rad);
            float theta_rad = _fortyFive_rad - (beta_rad * _fortyFive_rad / (_ninety_rad - betaCutOff_rad));
            float radius = sqrt(
                                ( ((cos(theta_rad)*_rLarge) - _rMedium) * ((cos(theta_rad)*_rLarge) - _rMedium) ) +
                                ( sin(theta_rad) * _rLarge * (sin(theta_rad) * _rLarge) )
                            );
            //std::cout << "eggshape 161 beta_rad, r: " << (beta_rad * 180.0f/PI_F) << ", "<< radius << std::endl;
            addForwardWeights[ii] = 0.5f * sin(deltaBeta_rad) * radius * radius;
            subtrForwardWeights[ii] = 0.5f * sin(deltaBeta_rad) * _rMedium * _rMedium;             
        }
        else if (orig_rad >= 0.0f && orig_rad < _fortyFive_rad)
        {
            addForwardWeights[ii] = 0.0f;
            subtrForwardWeights[ii] = 0.5f * sin(deltaBeta_rad) * _rLarge * _rLarge;
        }
        else if (orig_rad >= _fortyFive_rad && orig_rad < _ninety_rad)
        {   //std:: cout << "EggShape 176" << std::endl;
            float beta_rad = orig_rad - _fortyFive_rad;
            float rest_rad = (_fortyFive_rad - beta_rad);
            float radius = 2 * cos(rest_rad) * _rMedium;
            //std::cout << "eggshape 189 angle, r: " << (beta_rad * 180.0f/ PI_F) << ", " << radius << std::endl;
            addForwardWeights[ii] = 0.5 * sin(deltaBeta_rad) * radius * radius;
            subtrForwardWeights[ii] = 0.5 * sin(deltaBeta_rad) * _rSmall * _rSmall; 
        }
        else if (orig_rad >= _ninety_rad && orig_rad < _oneThirtyFive_rad)
        {
            float beta_rad = orig_rad - _ninety_rad;
            float radius = cos(beta_rad) * 2 * _rMedium;
            
            addForwardWeights[ii] = 0.5 * sin(deltaBeta_rad) * radius * radius;
            subtrForwardWeights[ii] = 0.5 * sin(deltaBeta_rad) * _rSmall * _rSmall; 
        }
        else if (orig_rad >= _oneThirtyFive_rad && orig_rad < _oneEighty_rad)
        {
            addForwardWeights[ii] = 0.0f;
            subtrForwardWeights[ii] = 0.5f * sin(deltaBeta_rad) * _rLarge * _rLarge; 
        }
        else if (orig_rad >= _oneEighty_rad && orig_rad < (_twoSeventy_rad - betaCutOff_rad))
        {
            float beta_rad = orig_rad - _oneEighty_rad;
            float theta_rad = beta_rad * _fortyFive_rad / (_ninety_rad - betaCutOff_rad);
            float radius = sqrt(
                            ( ( (cos(theta_rad) * _rLarge) - _rMedium ) * ((cos(theta_rad) * _rLarge) - _rMedium ) ) +
                            ( ( sin(theta_rad)*_rLarge) * (sin(theta_rad) * _rLarge) )
            );
            //std::cout <<"egg shape 229: r: " << (theta_rad * 180.0f/ PI_F) << ", " << (beta_rad * 180.0f/PI_F) << ", " << radius  << std::endl;
            addForwardWeights[ii] = 0.5f * sin(deltaBeta_rad) * radius * radius;
            subtrForwardWeights[ii] = 0.5f * sin(deltaBeta_rad) * _rMedium * _rMedium;
        }
        else
        {  
            float beta_rad = orig_rad - (_twoSeventy_rad - betaCutOff_rad);
            float theta_rad = _fortyFive_rad - (beta_rad * _fortyFive_rad/betaCutOff_rad);
            float radius = (_rMedium + (cos(theta_rad) * _rSmall)) / cos(beta_rad);
            
            addForwardWeights[ii] = 0.5f * sin(deltaBeta_rad) * radius * radius;
            subtrForwardWeights[ii] = 0.5f * sin(deltaBeta_rad) * _rMedium * _rMedium; 
        }
    }
    
    //std:: cout << "EggShape 230" << std::endl;
    float totalForwardArea  = 0.0f;
    float totalBackwardArea = 0.0f;
    
    for(int ii=0; ii<_referenceAnglesForWeight.size(); ++ii)
    {
        totalForwardArea  += addForwardWeights[ii];
        totalBackwardArea += subtrForwardWeights[ii];
        
        //std::cout << "245: totalForwardArea, totalBackwardArea: " << (_referenceAnglesAboutZ[ii]*180.0f/PI_F) << ", " << totalForwardArea << ", " << totalBackwardArea << std::endl;
    }
    
    totalForwardArea = 60.28f;
    totalBackwardArea = 39.27f;
    //std::cout << "totalForwardA, totalBackwardA: " << totalForwardArea << ", " << totalBackwardArea << std::endl;
    std::cout << "line 313: " << "_referenceAnglesForWeight.size(): " << _referenceAnglesForWeight.size() << std::endl;
    for(int ii=0; ii<_referenceAnglesForWeight.size(); ++ii)
    {
        totalForwardArea += (addForwardWeights[ii] - subtrForwardWeights[ii]);
        _forwardWeights.push_back(totalForwardArea);
        
        totalBackwardArea += (subtrForwardWeights[ii] - addForwardWeights[ii]);
        _backwardWeights.push_back(totalBackwardArea);
    }
    
    for(int ii=0; ii<_referenceAnglesForWeight.size(); ++ii)
    {
        std::cout << "325: refAngle, forward, backward: " << (_referenceAnglesForWeight[ii] * 180.0f / PI_F) << ", " << _forwardWeights[ii] << ", " << _backwardWeights[ii] << " = " << (_forwardWeights[ii] - _backwardWeights[ii]) << std::endl;
    }
    
}

void EggShape::populateVerticesAboutZAxis(float startingPolarAngle_rad, float endingPolarAngle_rad)
{
    //std::cout << "EggShape 266" << std::endl;
    float polarAngle_rad  = startingPolarAngle_rad;
    
    while(polarAngle_rad < endingPolarAngle_rad)
    {
        float radius          = getCorrespondingRadius(polarAngle_rad);
        glm::vec2 center      = getCorrespondingCenter(polarAngle_rad);
        float angleMultiplier = _rMedium/radius;
        float x               = center.x + (cos(polarAngle_rad) * radius);
        float y               = center.y + (sin(polarAngle_rad) * radius);
        glm::vec3 point       = {x, y, 0.0f};
    
        // If this is the first angle, then circumference traveled is zero.
        // Else take the difference between current polar angle and the last polar angle and find the circumference.
        if (_circumferenceTraveledAboutZ.size() == 0)
        {
            _circumferenceTraveledAboutZ.push_back(0.0f);
        }
        else
        {   
            float diffAngle = polarAngle_rad - _referenceAnglesAboutZ[_referenceAnglesAboutZ.size()-1];
            
            float arcLengthAtAngle    = std::abs(sin(diffAngle) * radius);
            
            float oppRadius           = getCorrespondingRadius(polarAngle_rad + PI_F);
            float arcLengthAtOppAngle = std::abs(sin(diffAngle) * oppRadius);
            
            // Circumference at this angle is the last circumference plus (sin(difference in angle) * radius).
            _circumferenceTraveledAboutZ.push_back(_circumferenceTraveledAboutZ[_circumferenceTraveledAboutZ.size()-1] +
                                                   arcLengthAtAngle);
        }
        
        // Push back angle. Push pack point on edge of egg.
        float polarAngleReduced_rad = (polarAngle_rad >= (2 * PI_F)) ? (polarAngle_rad - (2 * PI_F)) : (polarAngle_rad);
        _referenceAnglesAboutZ.push_back(polarAngleReduced_rad);
        _eggOutlineAboutZ.push_back(point);
        //std::cout << "angle, radius: " << (polarAngleReduced_rad * 180.0f/PI_F) << ", " << radius << std::endl;
        polarAngle_rad = polarAngle_rad + (_angleAboutZ * angleMultiplier);
        
        if(abs(polarAngle_rad - (286.3f * PI_F/180.0f)) < (0.4f * PI_F/180.0f))
        {
            polarAngle_rad = (286.3f * PI_F/180.0f);
        }
        
        if(_referenceAnglesAboutZ[_referenceAnglesAboutZ.size()-1] == (286.3f * PI_F/180.0f))
        {
            polarAngle_rad = 287.0f * PI_F/180.0f;
        }
        
        if(abs(polarAngle_rad - endingPolarAngle_rad) < (0.1 * PI_F/180.0f))
        {
            polarAngle_rad = endingPolarAngle_rad;
        }
    }
}

float EggShape::getRotationGivenCircumferenceDistance(float dist)
{
    int sizeCircVec = _circumferenceTraveledAboutZ.size();
    float origDist  = dist;
    dist = dist - static_cast<float>(floor(dist / _circumference) * _circumference);
    
    float lowerDist  = -1.0f;
    float upperDist  = -1.0f;
    float lowerAngle = -1.0f;
    float upperAngle = -1.0f;
    
    if(dist > _circumferenceTraveledAboutZ[sizeCircVec-1])
    {
        lowerDist = _circumferenceTraveledAboutZ[sizeCircVec-1];
        upperDist = _circumference;
        lowerAngle = _referenceAnglesAboutZ[sizeCircVec-1];
        upperAngle = _twoSeventy_rad;
    }
    else
    {
        auto lowerBoundDist_ptr = std::lower_bound(_circumferenceTraveledAboutZ.begin(), _circumferenceTraveledAboutZ.end(), dist);
        int largerThanDistIdx = lowerBoundDist_ptr - _circumferenceTraveledAboutZ.begin();
        
        lowerDist = _circumferenceTraveledAboutZ[largerThanDistIdx-1];
        upperDist = _circumferenceTraveledAboutZ[largerThanDistIdx];
        lowerAngle = _referenceAnglesAboutZ[largerThanDistIdx-1];
        upperAngle = _referenceAnglesAboutZ[largerThanDistIdx];
        upperAngle = (lowerAngle > upperAngle) ? (upperAngle + _threeSixty_rad) : (upperAngle);
    }
    
    float fractionalPartOfDistance = (dist - lowerDist)/(upperDist - lowerDist);
    
    float finalAngle = lowerAngle + ( fractionalPartOfDistance * (upperAngle - lowerAngle));
    //std::cout << "finalAngle: " << (finalAngle * 180.0f / PI_F) << std::endl;
    //std::cout << origDist << ", " << dist << ", " << (finalAngle * 180.0f / PI_F) << std::endl;
    return (finalAngle > _threeSixty_rad) ? (finalAngle - _threeSixty_rad) : (finalAngle);
    
}

std::vector<Vertex> EggShape::getVertices()
{
    return _vertices;
}
std::vector<uint32_t> EggShape::getIndices()
{
    return _indices;
}

glm::vec3 EggShape::getPos(int index)
{
    return _eggOutlineAboutZ[index];
}

float EggShape::getRotation(int index)
{
    return _referenceAnglesAboutZ[index];
}

int EggShape::getNumOfRotations()
{
    return _referenceAnglesAboutZ.size();
}

float EggShape::getEggCircumferenceAboutZ()
{
    return _circumference;
}

float EggShape::getCorrespondingRadius(float polarAngle_rad)
{
    polarAngle_rad = (polarAngle_rad >= (2 * PI_F)) ? (polarAngle_rad - (2 * PI_F)) : (polarAngle_rad);
        
    if( ( polarAngle_rad == _zero_rad ) ||
        ((polarAngle_rad >= _twoSeventy_rad) && (polarAngle_rad <=_threeSixty_rad)) ||
        ((polarAngle_rad >= _zero_rad)       && (polarAngle_rad <=_ninety_rad))
      )
    {
        return _rMedium;
    }
    else if ( ((polarAngle_rad >= _ninety_rad)        && (polarAngle_rad >= _oneThirtyFive_rad)) ||
              ((polarAngle_rad >= _twoTwentyFive_rad) && (polarAngle_rad >= _twoSeventy_rad))
    )
    {
        return _rLarge;
    }
    else
    {
        return _rSmall;
    }
}

glm::vec2 EggShape::getCorrespondingCenter(float polarAngle_rad)
{

    polarAngle_rad = (polarAngle_rad >= (2 * PI_F)) ? (polarAngle_rad - (2 * PI_F)) : (polarAngle_rad);
        
    if( ( polarAngle_rad == _zero_rad ) ||
        ((polarAngle_rad >= _twoSeventy_rad) && (polarAngle_rad <=_threeSixty_rad)) ||
        ((polarAngle_rad >= _zero_rad)       && (polarAngle_rad <=_ninety_rad))
      )
    {
        return glm::vec2{0.0f, _rMedium};
    }
    else if ( (polarAngle_rad >= _ninety_rad) && (polarAngle_rad >= _oneThirtyFive_rad))
    {
        return glm::vec2{-_rMedium, _rMedium};
    }
    else if ( (polarAngle_rad >= _twoTwentyFive_rad) && (polarAngle_rad >= _twoSeventy_rad))
    {
        return glm::vec2{_rMedium, _rMedium};
    }
    else
    {
        return glm::vec2{0.0f, (2.0f * _rMedium)};
    }
}

