#include "EggShape.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

EggShape::EggShape(
    uint32_t numOfSectionsAboutY,
    float    angleAboutZ,
    float    mediumRadius
):

    _rMedium{mediumRadius},
    _rLarge{2.0f * mediumRadius},
    _rSmall{_rLarge - (1.414f * _rMedium)},
    _numOfSectionsAboutY{numOfSectionsAboutY},
    _angleAboutZ{angleAboutZ}
{
    std::cout << "Create EggShape" << std::endl;
    populateVerticesAboutZAxis();
    
    // Number of levels that use indices. Remove the bottom point. Remove the bottom point.
    // Total number of levels = size/2 + 1. That is the number of reference angles divided by 2.
    //                          0      1      2     3     4      5      6   
    // Say reference angles are 270.0, 360.0, 45.0, 90.0, 135.0, 180.0, 225.0. Notice there's no ending 270.0
    // But don't forget to subtract the top and bottom level, because they only have one point. Here
    // that wouuld be 270.0 and 90.0
    int numOfLevels = (static_cast<int32_t>(_referenceAnglesAboutZ.size())/2 + 1) - 2;
    
    for(int lev=2; lev<numOfLevels; ++lev)
    {
        for(int ii = 0; ii<_numOfSectionsAboutY; ++ii)
        {
            _indices.push_back((lev)   * _numOfSectionsAboutY + (ii)   + 1);
            _indices.push_back((lev)   * _numOfSectionsAboutY + (ii-1) + 1);
            _indices.push_back((lev-1) * _numOfSectionsAboutY + (ii-1) + 1);
            _indices.push_back((lev)   * _numOfSectionsAboutY + (ii)   + 1);
            _indices.push_back((lev-1) * _numOfSectionsAboutY + (ii-1) + 1);
            _indices.push_back((lev-1) * _numOfSectionsAboutY + (ii  ) + 1);
        }
        
        _indices.push_back((lev)   * _numOfSectionsAboutY + (0)                      + 1);
        _indices.push_back((lev)   * _numOfSectionsAboutY + (_numOfSectionsAboutY-1) + 1);
        _indices.push_back((lev-1) * _numOfSectionsAboutY + (_numOfSectionsAboutY-1) + 1);
        _indices.push_back((lev)   * _numOfSectionsAboutY + (0)                      + 1);
        _indices.push_back((lev-1) * _numOfSectionsAboutY + (_numOfSectionsAboutY-1) + 1);
        _indices.push_back((lev-1) * _numOfSectionsAboutY + (0)                      + 1);
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
    
    std::cout << "circumference: " << _circumference << std::endl;
    std::cout << "sizes: " << _circumferenceTraveledAboutZ.size() << ", " << _referenceAnglesAboutZ.size() << std::endl;
    for(int ii=1; ii<_circumferenceTraveledAboutZ.size(); ++ii)
    {
        
        std::cout << ii << ", " << (_referenceAnglesAboutZ[ii] * 180.0 / PI_F) << ",  "<< _circumferenceTraveledAboutZ[ii] << ", " << 
        _circumferenceTraveledAboutZ[ii] - _circumferenceTraveledAboutZ[ii-1] <<  std::endl;
    }

}

void EggShape::populateVerticesAboutZAxis(float startingPolarAngle_rad, float endingPolarAngle_rad)
{
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
            _forwardWeights.push_back(_startWeight);
            _backwardWeights.push_back(_startWeight);
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
                
            _forwardWeights.push_back( _forwardWeights[_forwardWeights.size()-1] -
                                       (0.5f * arcLengthAtAngle * radius) + 
                                       (0.5f * arcLengthAtOppAngle * oppRadius) );
                                       
            _backwardWeights.push_back( _backwardWeights[_backwardWeights.size()-1] +
                                        (0.5f * arcLengthAtAngle * radius) - 
                                        (0.5f * arcLengthAtOppAngle * oppRadius) );
        }
        
        // Push back angle. Push pack point on edge of egg.
        float polarAngleReduced_rad = (polarAngle_rad >= (2 * PI_F)) ? (polarAngle_rad - (2 * PI_F)) : (polarAngle_rad);
        _referenceAnglesAboutZ.push_back(polarAngleReduced_rad);
        _eggOutlineAboutZ.push_back(point);
        //std::cout << "angle, radius: " << (polarAngleReduced_rad * 180.0f/PI_F) << ", " << radius << std::endl;
        polarAngle_rad = polarAngle_rad + (_angleAboutZ * angleMultiplier);
        
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
    std::cout << origDist << ", " << dist << ", " << (finalAngle * 180.0f / PI_F) << std::endl;
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
        
    if( (polarAngle_rad >= _oneEighty_rad) && (polarAngle_rad <_threeSixty_rad))
    {
        return _rMedium;
    }
    else if ( (polarAngle_rad >= _fortyFive_rad) && (polarAngle_rad < _oneThirtyFive_rad) )
    {
        return _rSmall;
    }
    else
    {
        return _rLarge;
    }
}

glm::vec2 EggShape::getCorrespondingCenter(float polarAngle_rad)
{
    polarAngle_rad = (polarAngle_rad >= (2 * PI_F)) ? (polarAngle_rad - (2 * PI_F)) : (polarAngle_rad);
        
    if( (polarAngle_rad >= _oneEighty_rad) && (polarAngle_rad <_threeSixty_rad))
    {
        return glm::vec2{0.0f, _rMedium};
    }
    else if ( (polarAngle_rad >= _fortyFive_rad) && (polarAngle_rad < _oneThirtyFive_rad) )
    {
        return glm::vec2{0.0f, (2.0f * _rMedium)};
    }
    else
    {
        return glm::vec2{-_rMedium, _rMedium};
    }
}

