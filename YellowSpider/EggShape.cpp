#include "EggShape.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

EggShape::EggShape(
    uint32_t numOfSectionsAboutY,
    uint32_t numOfSectionsAboutZ, // medium radius, other radii will be sectioned accordingly
    float mediumRadius
):
    _numOfSectionsAboutY{numOfSectionsAboutY},
    _numOfSectionsAboutZ{numOfSectionsAboutZ},
    _rLarge{2.0f * mediumRadius},
    _rMedium{mediumRadius},
    _rSmall{_rLarge - (1.414f * _rMedium)}
{
    std::cout << "Create EggShape" << std::endl;
    
    glm::vec3 rotationAxis{0.0f, 1.0f, 0.0f};
    glm::mat4 transformMatrix{1.0f};
    
    float deltaPolAng_deg = 0.0f;
    float deltaPol_rad    = 0.0f;
    float polarAngle_rad  = 0.0f + deltaPol_rad;
    int level = 0;
    int colorCount = 0;
    
    // FIRST SECTION
    //   Theta range is [270.0, 360.0).
    
    deltaPolAng_deg = (360.0f - 270.0f)/_numOfSectionsAboutZ;
    deltaPol_rad    = deltaPolAng_deg * PI_F / 180.0f;
    polarAngle_rad  = (270.0f * PI_F / 180.0f) + deltaPol_rad; // bottom level of points
    level = 0;
    
    
    //   Bottom circle of points.
    //   TODO there is no tippy tippy bottom of the egg. The bottom keystone cap doesn't exist.
    float radius    = (cos(polarAngle_rad) * _rMedium);
    float y         = (sin(polarAngle_rad) * _rMedium) + _rMedium;
    glm::vec3 point = {radius, y, 0.0f};
    colorCount = 0;
    
    populateVerticesAboutZAxis(0.0f, _rMedium, polarAngle_rad, _threeSixty_rad, deltaPol_rad, 0, _rMedium, level);
    
    // SECOND SECTION
    // Theta is from theta = 0.0 to theta < 45.0 deg.
    deltaPol_rad = (deltaPol_rad) * _rMedium / _rLarge;
    colorCount = 0;
    radius = cos(polarAngle_rad) * _rLarge - _rMedium;
    y      = (sin(polarAngle_rad) * _rLarge) + _rMedium;
    point = {radius, y, 0.0f};
    
    populateVerticesAboutZAxis(-_rMedium, _rMedium, polarAngle_rad,  (405.0f * PI_F / 180.0f), deltaPol_rad, 0, _rLarge, level);
    
    // THIRD SECTION
    // Theta is from 45.0 to 90.0 deg.
    deltaPol_rad = ((deltaPol_rad) * _rLarge / _rMedium) * (_rSmall / _rMedium);
    colorCount = 0;
    radius = cos(polarAngle_rad) * _rSmall;
    y      = (sin(polarAngle_rad) * _rSmall) + (2 * _rMedium);
    point = {radius, y, 0.0f};
    
    populateVerticesAboutZAxis(0.0f, 2*_rMedium, polarAngle_rad,  (450.0f * PI_F / 180.0f), deltaPol_rad, 0, _rSmall, level);
    
    for(int lev=1; lev<=level; ++lev)
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
    float& polarAngle_rad,
    float polarAngleEnd_rad,
    float deltaPol_rad,
    int colorCount,
    float radius,
    int& level)
{
    float radiusMinor = (cos(polarAngle_rad) * radius);
    float y           = (sin(polarAngle_rad) * radius) + centerY;
    glm::vec3 point   = {radiusMinor + centerX, y, 0.0f};
    
    while(polarAngle_rad < polarAngleEnd_rad)
    {
        
        _azimuths.push_back(polarAngle_rad);
        _eggOutline.push_back(point);
        std::cout << "_rotations: " << _azimuths[_azimuths.size()-1] * 180.0f/PI_F << std::endl;
        
        populateVerticesAboutYAxis(point, _colors[colorCount]);
        
        ++colorCount;
        colorCount = (colorCount % _colors.size());
        polarAngle_rad = polarAngle_rad + deltaPol_rad;
        ++level;
        
        radiusMinor = cos(polarAngle_rad) * radius;
        y      = (sin(polarAngle_rad) * radius) + centerY;
        point  = {radiusMinor + centerX, y, 0.0f};
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

