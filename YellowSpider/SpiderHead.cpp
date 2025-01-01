#include "SpiderHead.hpp"
#include "UtilityCircles.hpp"
#include "EuroCutDiamond.hpp"

#include <iostream>
const float  PI_F=3.14159265358979f;
//
using std::vector;

SpiderHead::SpiderHead()
{
    
    EuroCutDiamond head{0.0f, 3.8f,
                        1.6f, 4.8f,
                        3.2f, 6.0f,
                        5.6f, 6.8f,
                        7.2f, 6.3f,
                        8.8f, 4.3f};
                        
    _vertices = head.getVertices();
    _indices = head.getIndices();

}

vector<Vertex> SpiderHead::getVertices()
{
    return _vertices;
}

std::vector<uint32_t> SpiderHead::getIndices()
{
    return _indices;
}

void SpiderHead::populateIndices(int numOfSides, int numOfRows)
{
}
