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
    
    std::vector<Vertex> getVertices();
    std::vector<uint32_t> getIndices();
    
    private:
    
    uint32_t _numOfSectionsAboutZ = 0.0f;
    uint32_t _numOfSectionsAboutY = 0.0f;
    
    // radii
    float _rLarge           = 0.0f;
    float _rMedium          = 0.0f;
    float _rSmall           = 0.0f;
    
    std::vector<Vertex>   _vertices{};
    std::vector<uint32_t> _indices{};
    
};

#endif /* EggShape_hpp */
