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
    _polarAngleDeltaForMRadius_rad{(angleRangeMediumRadius.second - angleRangeMediumRadius.first)/_numOfSectionsUsingMRadius},
    _polarAngleDeltaForLRadius_rad{ (angleRangeLargeRadius.second - angleRangeLargeRadius.first) /_numOfSectionsUsingLRadius},
    _polarAngleDeltaForSRadius_rad{ (angleRangeSmallRadius.second - angleRangeSmallRadius.first) /_numOfSectionsUsingSRadius}
{
    std::cout << "Create EggShape" << std::endl;
    
    populateVerticesAboutZAxis();
    
    int numOfLevels = _numOfSectionsUsingMRadius + _numOfSectionsUsingLRadius + _numOfSectionsUsingSRadius;
    
    for(int lev=1; lev<=numOfLevels; ++lev)
    {
        for(int ii = 0; ii<_numOfSectionsAboutY; ++ii)
        {
            _indices.push_back((lev)   * _numOfSectionsAboutY + (ii)  );
            _indices.push_back((lev)   * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((lev-1) * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((lev)   * _numOfSectionsAboutY + (ii)  );
            _indices.push_back((lev-1) * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((lev-1) * _numOfSectionsAboutY + (ii  ));
        }
        
        _indices.push_back((lev)   * _numOfSectionsAboutY + (0));
        _indices.push_back((lev)   * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((lev-1) * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((lev)   * _numOfSectionsAboutY + (0));
        _indices.push_back((lev-1) * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((lev-1) * _numOfSectionsAboutY + (0));
    }
    
    for(int ii=0; ii<_referenceAnglesAboutZ.size(); ++ii)
    {
        //std::cout << "rotations: " << (_rotations[ii] * 180.0f / PI_F) << std::endl;
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
    std::cout << "EggShape 94" << std::endl;
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
    
    std::cout << "end of populate vertices about Z." << std::endl;
}

void EggShape::populateVerticesAboutZAxis(
    float centerX,
    float centerY,
    float startingPolarAngle_rad,
    float endingPolarAngle_rad,
    float polarAngleDelta_rad,
    float numOfSectionsAboutY,
    float radius)
{
    std::cout << "polarAngleDelta_rad: " << polarAngleDelta_rad << std::endl;
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
            _circumferenceTraveledAboutZ[_circumferenceTraveledAboutZ.size()-1] + sin(polarAngleDelta_rad) * radius);
        
        populateVerticesAboutYAxis(point, _colors[colorCount]);
        
        colorCount = (colorCount + 1) % _colors.size();
        
        polarAngle_rad = polarAngle_rad + polarAngleDelta_rad;
        x      = cos(polarAngle_rad) * radius;
        y      = (sin(polarAngle_rad) * radius) + centerY;
        point  = {x + centerX, y, 0.0f};
        std::cout << "polarAngle: " << (polarAngle_rad * 180.0f / PI_F);
    }
    
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
    return _circumferenceTraveledAboutZ[_circumferenceTraveledAboutZ.size()-1];
}

