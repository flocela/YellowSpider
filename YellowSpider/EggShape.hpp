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
    std::vector<float>     _rotations{};
    
    
    const float _fortyFive_rad     = 45.0f  * PI_F / 180.0f;
    const float _ninety_rad        = 90.0f  * PI_F / 180.0f;
    const float _twoSeventy_rad    = 270.0f * PI_F / 180.0f;
    const float _oneThirtyFive_rad = 135.0f * PI_F / 180.0f;
    
};

#endif /* EggShape_hpp */
