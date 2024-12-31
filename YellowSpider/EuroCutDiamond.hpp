#include <glm/glm.hpp>
#include <vector>
#include "Utilities.hpp"

#ifndef EuroCutDiamond_hpp
#define EuroCutDiamond_hpp

class EuroCutDiamond
{
    public:
    
    EuroCutDiamond(float y0, float r0,
                   float y1, float r1,
                   float y2, float r2,
                   float y3, float r3,
                   float y4, float r4,
                   float y5, float r5);
    EuroCutDiamond()                                    = delete;
    EuroCutDiamond(const EuroCutDiamond& o)             = default;
    EuroCutDiamond(EuroCutDiamond&& o) noexcept         = default;
    EuroCutDiamond& operator= (const EuroCutDiamond& o) = default;
    EuroCutDiamond& operator= (EuroCutDiamond&& o)      = default;
    ~EuroCutDiamond()                                   = default;
    
    std::vector<Vertex> getVertices();
    std::vector<uint32_t>  getIndices();
        
        
    private:
    
    // TODO set size of _radii and _heights here.
    std::vector<float>     _radii{};
    std::vector<float>     _heights{}; // y values.
    std::vector<glm::vec3> _colors{{1.0f, 0.0f, 0.0f},
                                   {0.0f, 1.0f, 0.0f},
                                   {0.0f, 0.0f, 1.0f},
                                   {1.0f, 0.0f, 0.0f},
                                   {0.0f, 1.0f, 0.0f},
                                   {0.0f, 0.0f, 1.0f}
    };
    
    int _numOfLevels = 6;
    int _numOfSides  = 8;
    std::vector<Vertex>    _vertices{};
    std::vector<uint32_t>  _indices{};
    void populateIndices();
    
};
        
#endif /* EuroCutDiamond_hpp */
