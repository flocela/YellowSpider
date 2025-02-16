#include "EggShape.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

EggShape::EggShape(
    uint32_t numOfSectionsAboutY,
    uint32_t numOfSectionsAboutZ,
    float mediumRadius
):

    _rMedium{mediumRadius},
    _rLarge{2.0f * mediumRadius},
    _rSmall{_rLarge - (1.414f * _rMedium)},
    _numOfSectionsAboutY{numOfSectionsAboutY},
    _numOfSectionsAboutZ{numOfSectionsAboutZ},
    _numOfSectionsUsingMRadius{_numOfSectionsAboutZ / (4 + static_cast<uint32_t>(_rSmall/_rMedium))},
    _numOfSectionsUsingLRadius{_numOfSectionsUsingMRadius},
    _numOfSectionsUsingSRadius{_numOfSectionsUsingMRadius * static_cast<uint32_t>(_rSmall / _rMedium)},
    _polarAngleDeltaForMRadius_rad{
        (_angleRangeMediumRadius[0].second - _angleRangeMediumRadius[0].first)/_numOfSectionsUsingMRadius},
    _polarAngleDeltaForLRadius_rad{
        (_angleRangeLargeRadius[0].second  - _angleRangeLargeRadius[0].first) /_numOfSectionsUsingLRadius},
    _polarAngleDeltaForSRadius_rad{
        (_angleRangeSmallRadius[0].second  - _angleRangeSmallRadius[0].first) /_numOfSectionsUsingSRadius}
{
    std::cout << "Create EggShape" << std::endl;
    
    populateVerticesAboutZAxis();
    
    int numOfLevels = _numOfSectionsUsingMRadius + _numOfSectionsUsingLRadius + _numOfSectionsUsingSRadius + 1;
    
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
    glm::vec3 bottomPoint = {0.0f, 0.0f, 0.0f};
    
    _referenceAnglesAboutZ      .push_back(_twoSeventy_rad);
    _eggOutlineAboutZ           .push_back(bottomPoint);
    _vertices                   .push_back(Vertex{ bottomPoint, _colors[_colors.size()-1]});
    _circumferenceTraveledAboutZ.push_back(0.0f);
    
    // FIRST SECTION
    //   Theta range is (270.0, 360.0). Polar radius is rMedium. Note point at 270.0 deg should be at (0, 0).
    //   So, move center from (0, 0) to (0, rMedium), then bottom of egg is at (0, 0).
    //   Note, range is exclusive.
    populateVerticesAboutZAxis(
        0.0f,
        _rMedium,
        _twoSeventy_rad,
        _threeSixty_rad,
        _polarAngleDeltaForMRadius_rad,
        _numOfSectionsAboutY,
        _rMedium
    );
    // SECOND SECTION
    //   Theta range is (0.0, 45.0). Note, range is exclusive.
    populateVerticesAboutZAxis(
        -_rMedium,
        _rMedium,
        _zero_rad,
        _fortyFive_rad,
        _polarAngleDeltaForMRadius_rad,
        _numOfSectionsAboutY,
        _rLarge
    );                        
    
    // THIRD SECTION
    //   Theta range is (45.0, 90.0). Note, range is exclusive.
    populateVerticesAboutZAxis(
        0.0f,
        2 * _rMedium,
        _fortyFive_rad,
        _ninety_rad,
        _polarAngleDeltaForMRadius_rad,
        _numOfSectionsAboutY,
        _rSmall
    );
    
    // POPULATE angles about all of z.
    int halfNumOfAngles = _referenceAnglesAboutZ.size();
    //
    for(int ii=0; ii<halfNumOfAngles; ++ii)
    {
        float nextPolarAngle_rad = 
            ( (_referenceAnglesAboutZ[ii] + PI_F) > (2*PI_F) ) ?
            ( (_referenceAnglesAboutZ[ii] + PI_F) - (2*PI_F) ):
            ( _referenceAnglesAboutZ[ii] + PI_F );
        
        float deltaPolarAngle = nextPolarAngle_rad - _referenceAnglesAboutZ[_referenceAnglesAboutZ.size()-1];
        float radius = 0.0f;
        float centerX = 0.0f;
        float centerY = 0.0f;
        
        if((nextPolarAngle_rad > _angleRangeMediumRadius[1].first) && (nextPolarAngle_rad <= _angleRangeMediumRadius[1].second))
        {
            radius = _rMedium;
            centerX = 0.0f;
            centerY = _rMedium;
        }
        else if ((nextPolarAngle_rad > _angleRangeLargeRadius[1].first) && (nextPolarAngle_rad <= _angleRangeLargeRadius[1].second))
        {
            radius = _rLarge;
            centerX = 2 * _rMedium;
            centerY = _rMedium;
        }
        else
        {
            radius = _rSmall;
            centerX = 0.0f;
            centerY = 2 * _rMedium;
        }
        
        float x         = (cos(nextPolarAngle_rad) * radius);
        float y         = (sin(nextPolarAngle_rad) * radius) + centerY;
        glm::vec3 point = {x + centerX, y, 0.0f};
        
        _referenceAnglesAboutZ.push_back(nextPolarAngle_rad);
        _eggOutlineAboutZ.push_back(point);
        _circumferenceTraveledAboutZ.push_back(_circumferenceTraveledAboutZ[_circumferenceTraveledAboutZ.size()-1] +
                                               std::abs(sin(deltaPolarAngle)) * radius);
    }
    //
    //_circumference = _circumferenceTraveledAboutZ[_circumferenceTraveledAboutZ.size()-1] + (std::abs(sin( _twoSeventy_rad - _referenceAnglesAboutZ[_referenceAnglesAboutZ.size()-1] ) * _rMedium));
    
    _circumference = _circumferenceTraveledAboutZ[_circumferenceTraveledAboutZ.size()-1] + _circumferenceTraveledAboutZ[_circumferenceTraveledAboutZ.size()-1] - _circumferenceTraveledAboutZ[_circumferenceTraveledAboutZ.size()-2] + 0.4;
    std::cout << "circumference: " << _circumference << std::endl;
    std::cout << "sizes: " << _circumferenceTraveledAboutZ.size() << ", " << _referenceAnglesAboutZ.size() << std::endl;
    for(int ii=0; ii<_circumferenceTraveledAboutZ.size(); ++ii)
    {
        
        std::cout << ii << ", " << (_referenceAnglesAboutZ[ii] * 180.0 / PI_F) << ",  "<< _circumferenceTraveledAboutZ[ii] <<  std::endl;
    }

}

// first polar angle is startingPolarAngle_rad plus polarAngleDelta_rad
void EggShape::populateVerticesAboutZAxis(
    float centerX,
    float centerY,
    float startingPolarAngle_rad,
    float endingPolarAngle_rad,
    float polarAngleDelta_rad,
    float numOfSectionsAboutY,
    float radius)
{
    int   colorCount        = 0;
    
    float polarAngle_rad    = startingPolarAngle_rad + polarAngleDelta_rad;
    float x                 = (cos(polarAngle_rad) * radius);
    float y                 = (sin(polarAngle_rad) * radius) + centerY;
    glm::vec3 point         = {x + centerX, y, 0.0f};
    
    while(polarAngle_rad <= endingPolarAngle_rad)
    {
        _referenceAnglesAboutZ      .push_back(polarAngle_rad);
        _eggOutlineAboutZ           .push_back(point);
        _circumferenceTraveledAboutZ.push_back(
            _circumferenceTraveledAboutZ[_circumferenceTraveledAboutZ.size()-1] + std::abs(sin(polarAngleDelta_rad)) * radius);
        
        populateVerticesAboutYAxis(point, _colors[colorCount]);
        
        colorCount = (colorCount + 1) % _colors.size();
        
        polarAngle_rad = polarAngle_rad + polarAngleDelta_rad;
        x      = cos(polarAngle_rad) * radius;
        y      = (sin(polarAngle_rad) * radius) + centerY;
        point  = {x + centerX, y, 0.0f};
    }
    std::cout << "last angle: " << (_referenceAnglesAboutZ[_referenceAnglesAboutZ.size()-1] * 180.0f / PI_F)<< std::endl;
}

float EggShape::getRotationGivenCircumferenceDistance(float dist)
{
    int sizeCircVec = _circumferenceTraveledAboutZ.size();
    float origDist = dist;
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

