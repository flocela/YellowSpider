#include "EggShape.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

const float  PI_F=3.14159265358979f;

EggShape::EggShape()
{
    _numOfSectionsAboutY = 20;
    _numOfSectionsAboutZ = 10;
    _rLarge              = 10.0f;
    _rMedium             = _rLarge/2.0f;
    _rSmall              = _rLarge - (1.414 * _rMedium);
    
    std::vector<glm::vec3> colors{{1.0f, 0.0f, 0.0f},
                                   {0.0f, 1.0f, 0.0f},
                                   {0.0f, 0.0f, 1.0f},
                                   {1.0f, 0.0f, 0.0f},
                                   {0.0f, 1.0f, 0.0f},
                                   {0.0f, 0.0f, 1.0f}
    };
    
    glm::vec3 rotationAxis{0.0f, 1.0f, 0.0f};
    glm::mat4 transformMatrix{1.0f};
    
    // FIRST SECTION
    // Theta is from theta = 270.0 to theta < 360.0. But there are no points at theta = 270. There's really only one point there
    // because the radius is zero.
    
    float deltaTheta_deg = (360.0f - 270.0f)/_numOfSectionsAboutZ;
    float deltaTheta_rad = deltaTheta_deg * PI_F / 180.0f;
    float theta_rad = (270.0f * PI_F / 180.0f) + deltaTheta_rad; // bottom level of points
    
    int level = 0;
    // Bottom circle of points.
    float radius    = (cos(theta_rad) * _rMedium);
    float y         = (sin(theta_rad) * _rMedium) + _rMedium;
    glm::vec3 point = {radius, y, 0.0f};
    
    for(int ii = 0; ii<_numOfSectionsAboutY; ++ii)
    {
        _vertices.push_back(Vertex{ 
            glm::rotate(transformMatrix,
                        glm::radians(360.0f/(_numOfSectionsAboutY)) * ii, rotationAxis ) * glm::vec4(point, 1.0),
            colors[colors.size()-1]});
    
    }
    
    // Second level of points to theta < 360.0 deg.
    level = 1;
    theta_rad = theta_rad + deltaTheta_rad;
    int colorCount = 0;
    while(theta_rad < (360.0f * PI_F / 180.0f))
    {
        radius = cos(theta_rad) * _rMedium;
        y      = (sin(theta_rad) * _rMedium) + _rMedium;
        point  = {radius, y, 0.0f};
        
        for(int ii = 0; ii<_numOfSectionsAboutY; ++ii)
        {
            _vertices.push_back(Vertex{ 
            glm::rotate(transformMatrix,
                        glm::radians(360.0f/(_numOfSectionsAboutY)) * ii, rotationAxis ) * glm::vec4(point, 1.0),
            colors[colorCount]});
            
            _indices.push_back((level)   * _numOfSectionsAboutY + (ii)  );
            _indices.push_back((level)   * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((level-1) * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((level)   * _numOfSectionsAboutY + (ii)  );
            _indices.push_back((level-1) * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((level-1) * _numOfSectionsAboutY + (ii  ));
        }
        
        _indices.push_back((level)   * _numOfSectionsAboutY + (0));
        _indices.push_back((level)   * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((level-1) * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((level)   * _numOfSectionsAboutY + (0));
        _indices.push_back((level-1) * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((level-1) * _numOfSectionsAboutY + (0));
        
        
        ++colorCount;
        colorCount = (colorCount % colors.size());
        theta_rad = theta_rad + deltaTheta_rad;
        ++level;
    }
        
    // SECOND SECTION
    // Theta is from theta = 0.0 to theta < 45.0 deg.
    deltaTheta_rad = (deltaTheta_rad) * _rMedium / _rLarge;
    colorCount = 0;
    theta_rad = 0.0f; // 360.0 deg equals 0.0 deg.
    while(theta_rad < (45.0f * PI_F / 180.0f))
    {
        radius = cos(theta_rad) * _rLarge - _rMedium;
        y      = (sin(theta_rad) * _rLarge) + _rMedium;
        point = {radius, y, 0.0f};
        
        for(int ii = 0; ii<_numOfSectionsAboutY; ++ii)
        {
            _vertices.push_back(Vertex{ 
            glm::rotate(transformMatrix,
                        glm::radians(360.0f/(_numOfSectionsAboutY)) * ii, rotationAxis ) * glm::vec4(point, 1.0),
            colors[colorCount]});
            
            _indices.push_back((level)   * _numOfSectionsAboutY + (ii)  );
            _indices.push_back((level)   * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((level-1) * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((level)   * _numOfSectionsAboutY + (ii)  );
            _indices.push_back((level-1) * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((level-1) * _numOfSectionsAboutY + (ii  ));
        }
        
        _indices.push_back((level)   * _numOfSectionsAboutY + (0));
        _indices.push_back((level)   * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((level-1) * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((level)   * _numOfSectionsAboutY + (0));
        _indices.push_back((level-1) * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((level-1) * _numOfSectionsAboutY + (0));
        
        ++colorCount;
        colorCount = (colorCount % colors.size());
        theta_rad = theta_rad + deltaTheta_rad;
        ++level;
    }
    
    // THIRD SECTION
    // Theta is from 45.0 to 90.0 deg.
    deltaTheta_rad = ((deltaTheta_rad) * _rLarge / _rMedium) * (_rSmall / _rMedium);
    colorCount = 0;
    while(theta_rad < (90.0f * PI_F / 180.0f))
    {
        radius = cos(theta_rad) * _rSmall;
        y      = (sin(theta_rad) * _rSmall) + (2 * _rMedium);
        point = {radius, y, 0.0f};
        
        for(int ii = 0; ii<_numOfSectionsAboutY; ++ii)
        {
            _vertices.push_back(Vertex{ 
            glm::rotate(transformMatrix,
                        glm::radians(360.0f/(_numOfSectionsAboutY)) * ii, rotationAxis ) * glm::vec4(point, 1.0),
            colors[colorCount]});
            
            _indices.push_back((level)   * _numOfSectionsAboutY + (ii)  );
            _indices.push_back((level)   * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((level-1) * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((level)   * _numOfSectionsAboutY + (ii)  );
            _indices.push_back((level-1) * _numOfSectionsAboutY + (ii-1));
            _indices.push_back((level-1) * _numOfSectionsAboutY + (ii  ));
        }
        
        _indices.push_back((level)   * _numOfSectionsAboutY + (0));
        _indices.push_back((level)   * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((level-1) * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((level)   * _numOfSectionsAboutY + (0));
        _indices.push_back((level-1) * _numOfSectionsAboutY + (_numOfSectionsAboutY-1));
        _indices.push_back((level-1) * _numOfSectionsAboutY + (0));
        
        ++colorCount;
        colorCount = (colorCount % colors.size());
        theta_rad = theta_rad + deltaTheta_rad;
        ++level;
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

