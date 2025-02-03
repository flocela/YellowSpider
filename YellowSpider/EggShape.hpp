#include <glm/glm.hpp>
#include <vector>
#include "Utilities.hpp"

#ifndef EggShape_hpp
#define EggShape_hpp

class EggShape
{

    public:

    EggShape();
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
    
    
};

#endif /* EggShape_hpp */
