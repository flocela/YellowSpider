#include <glm/glm.hpp>
#include <vector>
#include "Utilities.hpp"

#ifndef SpiderHead_hpp
#define SpiderHead_hpp

class SpiderHead
{
    public:
        SpiderHead();
        SpiderHead(const SpiderHead& o) = default;
        SpiderHead(SpiderHead&& o) noexcept = default;
        SpiderHead& operator= (const SpiderHead& o) = default;
        SpiderHead& operator= (SpiderHead&& o) noexcept = default;
        ~SpiderHead() = default;
        
        std::vector<Vertex> getVertices();
        std::vector<uint32_t> getIndices();
    
    private:
    
        std::vector<Vertex>   _vertices{};
        std::vector<uint32_t> _indices;
        
        // vertices rows are from bottom to top.
        // vertices go in a +y rotation direction
        void populateIndices(int numOfSides, int numOfRows);
};

#endif /* SpiderHead_hpp */
