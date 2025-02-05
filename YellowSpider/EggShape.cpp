#include "EggShape.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

EggShape::EggShape(
    uint32_t numOfSectionsAboutY,
    uint32_t numOfSectionsAboutZ,
    float mediumRadius
):
    _numOfSectionsAboutY{numOfSectionsAboutY},
    _numOfSectionsAboutZ{numOfSectionsAboutZ},
    _rLarge{2.0f * mediumRadius},
    _rMedium{mediumRadius},
    _rSmall{_rLarge - (1.414f * _rMedium)}
{
    std::cout << "Create EggShape" << std::endl;
    
    // Total number of sections about z is _numOfSectionsAboutZ.
    // medCount is number of sections using medium radius.
    // lgCount is number of sections using only large radius.
    // smCount is number of sections using only small radius.
    // numOfSectionsAboutZ = medCount + lgCount + smCount.
    // numOfSectionsAboutZ = medCount +
    //                       1/2 * lgRadius/mdRadius * medCount +
    //                       1/2 * smRadius/mdRadius * medCount.
    // There is 1/2 the number of degrees that the lgRadius traverses vs the mdRadius.
    // There is 1/2 the number of degrees that the smRadius traverses vs the mdRadius.
    // That is, the mdRadius traverses [270, 360)deg, lgRadius traverses [0, 45) deg,
    // smRadius traverses [45, 90).
    // The distance the lgRadius traverses is 1/2 * (lgRadius/mdRadius) * (distance mdRadius traverses).
    
    int32_t numOfSectionsUsingMRadius = _numOfSectionsAboutZ / (4.0f + (_rSmall/_rMedium));
    int32_t numOfSectionsUsingLRadius = numOfSectionsUsingMRadius;
    int32_t numOfSectionsUsingSRadius = numOfSectionsUsingMRadius * _rSmall / _rMedium;
    
    std::cout << "numMR, numLR, numSR: " << numOfSectionsUsingMRadius << ", " << numOfSectionsUsingLRadius << ", " << numOfSectionsUsingSRadius << std::endl;
    
    // FIRST SECTION
    //   Theta range is [270.0, 360.0). Polar radius is rMedium. Note point at 270.0 deg should be at (0, 0).
    //   So, move center from (0, 0) to (0, rMedium), then bottom of egg is at (0, 0).
    populateVerticesAboutZAxis(
        0.0f,
        _rMedium,
        _twoSeventy_rad,
        _threeSixty_rad,
        numOfSectionsUsingMRadius,
        _numOfSectionsAboutY,
        _rMedium
    );
    
    // SECOND SECTION
    // Theta is from theta = 0.0 to theta < 45.0 deg.
    populateVerticesAboutZAxis(
        -_rMedium,
        _rMedium,
        _zero_rad,
        _fortyFive_rad,
        numOfSectionsUsingLRadius,
        _numOfSectionsAboutY,
        _rLarge
    );                        
    
    // THIRD SECTION
    // Theta is from 45.0 to 90.0 deg.
    populateVerticesAboutZAxis(
        0.0f,
        2 * _rMedium,
        _fortyFive_rad,
        _ninety_rad,
        numOfSectionsUsingSRadius,
        _numOfSectionsAboutY,
        _rSmall
    );
    
    int numOfLevels = numOfSectionsUsingMRadius + numOfSectionsUsingLRadius + numOfSectionsUsingSRadius;
    
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
    
    for(int ii=0; ii<_azimuths.size(); ++ii)
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

void EggShape::populateVerticesAboutZAxis(
    float centerX,
    float centerY,
    float startingPolarAngle_rad,
    float endingPolarAngle_rad,
    float numOfSectionsAboutZ,
    float numOfSectionsAboutY,
    float radius)
{
    int   colorCount        = 0;
    float polarAngle_rad    = startingPolarAngle_rad;
    float x                 = (cos(polarAngle_rad) * radius);
    float y                 = (sin(polarAngle_rad) * radius) + centerY;
    glm::vec3 point         = {x + centerX, y, 0.0f};
    float deltaPolAngle_rad = (endingPolarAngle_rad - startingPolarAngle_rad) /  numOfSectionsAboutZ;
    
    while(polarAngle_rad < endingPolarAngle_rad)
    {
        _azimuths.push_back(polarAngle_rad);
        _eggOutline.push_back(point);
        
        populateVerticesAboutYAxis(point, _colors[colorCount]);
        
        ++colorCount;
        colorCount = (colorCount % _colors.size());
        polarAngle_rad = polarAngle_rad + deltaPolAngle_rad;
        
        x = cos(polarAngle_rad) * radius;
        y      = (sin(polarAngle_rad) * radius) + centerY;
        point  = {x + centerX, y, 0.0f};
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
    return _eggOutline[index];
}

float EggShape::getRotation(int index)
{
    return _azimuths[index];
}

int EggShape::getNumOfRotations()
{
    return _azimuths.size();
}

