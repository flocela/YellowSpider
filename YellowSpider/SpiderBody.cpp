#include "SpiderBody.hpp"
#include "UtilityCircles.hpp"
#include "EuroCutDiamond.hpp"

#include <iostream>
const float  PI_F=3.14159265358979f;
//
using std::vector;

SpiderBody::SpiderBody()
{
/*
    EuroCutDiamond body{8.0f,  4.0f,
                        13.0f, 8.0f,
                        19.0f, 11.5f,
                        23.0f, 12.0f,
                        30.0f, 11.0f,
                        34.0f, 6.0f};
*/
    EuroCutDiamond body{0.0f,  2.5f,
                        2.0f, 3.5f,
                        6.0f, 5.5f,
                        10.0f, 7.0f,
                        16.5f, 7.5f,
                        20.0f, 5.5};
                        
    _vertices = body.getVertices();
    _indices = body.getIndices();

}

vector<Vertex> SpiderBody::getVertices()
{
    return _vertices;
}

    std::vector<uint32_t> SpiderBody::getIndices()
{
    return _indices;
}

void SpiderBody::populateIndices(int numOfSides, int numOfRows)
{
}
