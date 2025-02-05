#include <glm/glm.hpp>
#include <vector>
#include "Utilities.hpp"

#ifndef EggShape_hpp
#define EggShape_hpp

const float  PI_F=3.14159265358979f;

class EggShape
{

    public:

    // Creates an egg whose small end is in the positive y direction.
    // It's bottom larger end is centered at (0, 0, 0).
    EggShape(
        uint32_t numOfSectionsAboutY,
        uint32_t numOfSectionsAboutZ, // medium radius, other radii will be sectioned accordingly
        float mediumRadius
    );
    EggShape(const EggShape& o)                 = default;
    EggShape(EggShape&& o) noexcept             = default;
    EggShape& operator= (const EggShape& o)     = default;
    EggShape& operator= (EggShape&& o) noexcept = default;
    
    std::vector<Vertex>    getVertices();
    std::vector<uint32_t>  getIndices();
    std::vector<float>     getRotations();
    glm::vec3              getPos(int index);
    float                  getRotation(int index);
    int                    getNumOfRotations();
    std::vector<glm::vec3> getEggOutline();
    
    private:
    
    uint32_t _numOfSectionsAboutZ = 0.0f;
    uint32_t _numOfSectionsAboutY = 0.0f;
    
    // radii
    float _rLarge           = 0.0f;
    float _rMedium          = 0.0f;
    float _rSmall           = 0.0f;
    
    std::vector<glm::vec3> _eggOutline{};
    std::vector<Vertex>    _vertices{};
    std::vector<uint32_t>  _indices{};
    
    // Think of the egg as being described by a form of polar coordinates.
    // The bottom circular part of the egg is a radius rotating about the center of the circle. It
    // creates an arc range [270, 360). The azimuths are the digital representation of those angles.
    // The azimuths continue, but this time rotating about the center of the larger circle
    // in the range [360, 45).
    // Then finally from [45, 90) using the smallest radius.
    // TODO create this in the initializer list to the correct size using _azimuths{correct size}.
    std::vector<float>     _azimuths{};
    
    
    const float _fortyFive_rad     = 45.0f  * PI_F / 180.0f;
    const float _ninety_rad        = 90.0f  * PI_F / 180.0f;
    const float _oneThirtyFive_rad = 135.0f * PI_F / 180.0f;
    const float _twoSeventy_rad    = 270.0f * PI_F / 180.0f;
    const float _threeSixty_rad    = 360.0f * PI_F / 180.0f;
    
    
    std::vector<glm::vec3> _colors{{1.0f, 0.0f, 0.0f},
                                  {0.0f, 1.0f, 0.0f},
                                  {0.0f, 0.0f, 1.0f},
                                  {1.0f, 0.0f, 0.0f},
                                  {0.0f, 1.0f, 0.0f},
                                  {0.0f, 0.0f, 1.0f}
    };
    
    void populateVerticesAboutZAxis(float centerX, float centerY, float& polarAngle_rad, float polarAngleEnd_rad, float deltaPol_rad, int colorCount, float _radius, int& level);
    void populateVerticesAboutYAxis(glm::vec3 point, glm::vec3 color);
    
};

#endif /* EggShape_hpp */

