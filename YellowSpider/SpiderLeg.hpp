#include <glm/glm.hpp>
#include <vector>
#include "Utilities.hpp"

#ifndef SpiderLeg_hpp
#define SpiderLeg_hpp

// TODO set number of leg segments
class SpiderLeg
{
    public:
        SpiderLeg(float length0, float length1, float length2,
                  glm::vec3 point0, glm::vec3 point1, glm::vec3 point2);
        SpiderLeg(const SpiderLeg& o) = default;
        SpiderLeg(SpiderLeg&& o) noexcept = default;
        SpiderLeg& operator= (const SpiderLeg& o) = default;
        SpiderLeg& operator= (SpiderLeg&& o) noexcept = default;
        ~SpiderLeg() = default;
        
        std::vector<std::vector<Vertex>> getSegmentVertices();
        std::vector<std::vector<uint32_t>> getSegmentIndices();
        
    
    private:
    
        std::vector<std::vector<Vertex>>   _vertices{};
        std::vector<std::vector<uint32_t>> _indices;
        
        // vertices rows are from bottom to top.
        // vertices go in a +y rotation direction
        void populateIndices(int numOfSides, int numOfRows);
};

#endif
