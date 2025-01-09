#include "SpiderLeg.hpp"
#include <iostream>

// Assumes points wind in a +ve +y direction
// Assumes points all lie on the y = 0 plane.
SpiderLeg::SpiderLeg(float length0, float length1, float length2,
          glm::vec3 point0, glm::vec3 point1, glm::vec3 point2)
{
    std::vector<float> heights(4);
    heights[0] = 0.0f;
    heights[1] = length0;
    heights[2] = length1;
    heights[3] = length2;
    //
    std::vector<glm::vec3> colors(4);
    colors[0] = {1.0, 0.0, 0.0};
    colors[1] = {0.0, 1.0, 0.0};
    colors[2] = {0.0, 0.0, 1.0};
    colors[3] = {0.0, 0.0, 0.0};
    
    for(int ii=0; ii<3; ++ii)
    {
        std::vector<Vertex> segmentVertices{};
        
        point0.y = heights[0];
        point1.y = heights[0];
        point2.y = heights[0];
        segmentVertices.push_back({point0, colors[ii]});
        segmentVertices.push_back({point1, colors[ii]});
        segmentVertices.push_back({point2, colors[ii]});
        
        point0.y = heights[ii+1];
        point1.y = heights[ii+1];
        point2.y = heights[ii+1];
        segmentVertices.push_back({point0, colors[ii+1]});
        segmentVertices.push_back({point1, colors[ii+1]});
        segmentVertices.push_back({point2, colors[ii+1]});
        
        _vertices.push_back(segmentVertices);
    }
    
    std::vector<uint32_t> segmentIndices= { 0, 1, 4,
                                            0, 4, 3,
                                            1, 2, 5,
                                            1, 5, 4,
                                            2, 3, 5,
                                            2, 0, 3
                                          };
    _indices = { segmentIndices,
                 segmentIndices,
                 segmentIndices};
}

std::vector<std::vector<Vertex>> SpiderLeg::getSegmentVertices()
{
    return _vertices;
}

std::vector<std::vector<uint32_t>> SpiderLeg::getSegmentIndices()
{
    return _indices;
}
