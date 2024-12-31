#include "SpiderBody.hpp"
#include "UtilityCircles.hpp"

#include <iostream>
const float  PI_F=3.14159265358979f;
//
using std::vector;

SpiderBody::SpiderBody()
{
    int32_t numOfSides = 8;
    _vertices.reserve(numOfSides * 8);
    
    glm::vec3 point0A{0.0f, 0.0f, 0.0f};
    glm::vec3 point0B{0.0f, 2.0f, 6.0f};
    std::vector<glm::vec4> circle0 = UtilityCircles::getVerticesFromDiameter( point0A, point0B, numOfSides, glm::vec3{0.0f, point0B.z - point0A.z, point0A.y - point0B.y} );
    
    glm::vec3 point1A{0.0f, 1.0f, -4.0f};
    glm::vec3 point1B{0.0f, 3.0f, 7.5f};
    std::vector<glm::vec4> circle1 = UtilityCircles::getVerticesFromDiameter( point1A, point1B, numOfSides, glm::vec3{0.0f, point1B.z - point1A.z, point1A.y - point1B.y} );
    
    glm::vec3 point2A{0.0f, 4.0f, -5.0f};
    glm::vec3 point2B{0.0f, 5.0f, 7.0f};
    std::vector<glm::vec4> circle2 = UtilityCircles::getVerticesFromDiameter( point2A, point2B, numOfSides, glm::vec3{0.0f, point2B.z - point2A.z, point2A.y - point2B.y} );
    
    glm::vec3 point3A{0.0f, 7.0f, -6.5f};
    glm::vec3 point3B{0.0f, 7.0f, 7.0f};
    std::vector<glm::vec4> circle3 = UtilityCircles::getVerticesFromDiameter( point3A, point3B, numOfSides, glm::vec3{0.0f, point3B.z - point3A.z, point3A.y - point3B.y} );
    
    glm::vec3 point4A{0.0f, 8.0f, -5.5f};
    glm::vec3 point4B{0.0f, 9.0f, 2.0f};
    std::vector<glm::vec4> circle4 = UtilityCircles::getVerticesFromDiameter( point4A, point4B, numOfSides, glm::vec3{0.0f, point4B.z - point4A.z, point4A.y - point4B.y} );
    
    glm::vec3 point5A{0.0f, 10.0f, -7.0f};
    glm::vec3 point5B{0.0f, 14.0f, 3.0f};
    std::vector<glm::vec4> circle5 = UtilityCircles::getVerticesFromDiameter( point5A, point5B, numOfSides, glm::vec3{0.0f, point5B.z - point5A.z, point5A.y - point5B.y} );
    
    glm::vec3 point6A{0.0f, 11.0f, -9.0f};
    glm::vec3 point6B{0.0f, 22.0f, -1.0f};
    std::vector<glm::vec4> circle6 = UtilityCircles::getVerticesFromDiameter( point6A, point6B, numOfSides, glm::vec3{0.0f, point6B.z - point6A.z, point6A.y - point6B.y} );
    
    glm::vec3 point7A{0.0f, 16.0f, -12.5f};
    glm::vec3 point7B{0.0f, 23.0f, -7.0f};
    std::vector<glm::vec4> circle7 = UtilityCircles::getVerticesFromDiameter( point7A, point7B, numOfSides, glm::vec3{0.0f, point7B.z - point7A.z, point7A.y - point7B.y} );
    
    
    
    for(int ii=0; ii<numOfSides; ++ii)
    {
        _vertices.emplace_back( glm::vec3{circle0[ii].x, circle0[ii].y, circle0[ii].z}, glm::vec3{1.0f, 0.0f, 0.0f} );
    }
    
    for(int ii=0; ii<numOfSides; ++ii)
    {
        _vertices.emplace_back( glm::vec3{circle1[ii].x, circle1[ii].y, circle1[ii].z}, glm::vec3{0.0f, 1.0f, 0.0f} );
    }
    
    for(int ii=0; ii<numOfSides; ++ii)
    {
        _vertices.emplace_back( glm::vec3{circle2[ii].x, circle2[ii].y, circle2[ii].z}, glm::vec3{0.0f, 0.0f, 1.0f} );
    }
    
    for(int ii=0; ii<numOfSides; ++ii)
    {
        _vertices.emplace_back( glm::vec3{circle3[ii].x, circle3[ii].y, circle3[ii].z}, glm::vec3{0.0f, 1.0f, 0.0f} );
    }
    
    for(int ii=0; ii<numOfSides; ++ii)
    {
        _vertices.emplace_back( glm::vec3{circle4[ii].x, circle4[ii].y, circle4[ii].z}, glm::vec3{0.0f, 0.0f, 1.0f} );
    }
    
    for(int ii=0; ii<numOfSides; ++ii)
    {
        _vertices.emplace_back( glm::vec3{circle5[ii].x, circle5[ii].y, circle5[ii].z}, glm::vec3{1.0f, 0.0f, 0.0f} );
    }
    
    for(int ii=0; ii<numOfSides; ++ii)
    {
        _vertices.emplace_back( glm::vec3{circle6[ii].x, circle6[ii].y, circle6[ii].z}, glm::vec3{0.0f, 1.0f, 0.0f} );
    }
    
    for(int ii=0; ii<numOfSides; ++ii)
    {
        _vertices.emplace_back( glm::vec3{circle7[ii].x, circle7[ii].y, circle7[ii].z}, glm::vec3{0.0f, 0.0f, 1.0f} );
    }
    
    
    populateIndices(numOfSides, 8);
    
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
    for(int rr=1; rr<numOfRows; ++rr)
    {
        for(int jj=0; jj<numOfSides; ++jj)
        {
            if(jj != (numOfSides-1))
            {
                _indices.push_back( (numOfSides*rr)     + (jj)  );
                _indices.push_back( (numOfSides*(rr-1)) + (jj)  );
                _indices.push_back( (numOfSides*rr)     + (jj+1));
                
                _indices.push_back( (numOfSides*rr)     + (jj+1));
                _indices.push_back( (numOfSides*(rr-1)) + (jj)  );
                _indices.push_back( (numOfSides*(rr-1)) + (jj+1));
            }
            else
            {
                _indices.push_back( (numOfSides*rr)     + (jj)  );
                _indices.push_back( (numOfSides*(rr-1)) + (jj)  );
                _indices.push_back( (numOfSides*rr)     + (0)   );
                
                _indices.push_back( (numOfSides*rr)     + (0)   );
                _indices.push_back( (numOfSides*(rr-1)) + (jj)  );
                _indices.push_back( (numOfSides*(rr-1)) + (0)   );
            }
        }
    }
    
    for(int ii=2; ii<_indices.size(); ++ii)
    {
        std::cout << _indices[ii-2] << ", " << _indices[ii-1] << ", " << _indices[ii] << std::endl;
        ++ii;
    }
}
