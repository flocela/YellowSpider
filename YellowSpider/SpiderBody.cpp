#include "SpiderBody.hpp"
#include <iostream>
const float  PI_F=3.14159265358979f;

using std::vector;

SpiderBody::SpiderBody()
{//
    // TODO declare _vertices as _vertices(26, Vertex{_point0,     {1.0f, 0.0f, 0.0f}})
    _vertices.reserve(26);
    for(int ii=0; ii<26; ++ii)
    {
        _vertices.push_back({ _pointTop0,     {1.0f, 0.0f, 0.0f}});
    }
    _vertices[0]  = { _pointTop0,     {1.0f, 0.0f, 0.0f} };
    _vertices[6]  = { _pointTop1,     {0.0f,  1.0f, 0.0f}};
    _vertices[12] = { _pointTop2,     {0.0f, 0.0f, 1.0f} };
    _vertices[18] = { _pointTop3,     {1.0f, 0.0f, 0.0f} };
    _vertices[24] = { _startPoint, {0.0f, 0.0f, 0.0f} };
    _vertices[25] = { _endPoint,   {1.0f, 1.0f, 1.0f} };
    uint32_t numOfSections = 6;
    float radiansPerSection = 180.0f/numOfSections * PI_F/180.0f;
    for(int ii=0; ii<4; ++ii)
    {
        float radius      = _vertices[ii*numOfSections].pos.y;
        float zVal        = _vertices[ii*numOfSections].pos.z;
        glm::vec3 ringCol = _vertices[ii*numOfSections].col;
        for(int jj=0; jj<numOfSections; ++jj)
        {
            _vertices[(ii*numOfSections)+jj] = 
            { glm::vec3{radius * cos(radiansPerSection * jj),
                        radius * sin(radiansPerSection * jj),
                        zVal},
              ringCol
            };
        }
    }
    //
    for(int ii=0; ii<4; ++ii)
    {
        for(int jj=0; jj<numOfSections; ++jj)
        {
            if(jj != numOfSections - 1)
            {
                _indices.push_back(( (ii)   * numOfSections ) + jj    );
                _indices.push_back(( (ii+1) * numOfSections ) + jj    );
                _indices.push_back(( (ii)   * numOfSections ) + jj + 1);
                
                _indices.push_back(( (ii)   * numOfSections ) + jj + 1);
                _indices.push_back(( (ii+1) * numOfSections ) + jj    );
                _indices.push_back(( (ii+1) * numOfSections ) + jj + 1);
            }
            else
            {
                _indices.push_back(( (ii)   * numOfSections ) + jj    );
                _indices.push_back(( (ii+1) * numOfSections ) + jj    );
                _indices.push_back(( (ii)   * numOfSections ) + 0     );
                
                _indices.push_back(( (ii)   * numOfSections ) + 0     );
                _indices.push_back(( (ii+1) * numOfSections ) + jj    );
                _indices.push_back(( (ii+1) * numOfSections ) + 0     );
            }
        }
    }
    
    for(int ii=0; ii<_vertices.size(); ++ii)
    {
        std::cout << ii << ":: " << _vertices[ii].pos.x << ", " << _vertices[ii].pos.y << ", " << _vertices[ii].pos.z << std::endl;
    }
    
    for(int ii=0; ii<numOfSections; ++ii)
    {
        if(ii != numOfSections - 1)
        {
            _indices.push_back(24);
            _indices.push_back(ii);
            _indices.push_back(ii+1);
        }
        else
        {
            _indices.push_back(24);
            _indices.push_back(ii);
            _indices.push_back(0);
        }
    }
    
    for(int ii=0; ii<numOfSections; ++ii)
    {
        if(ii != numOfSections - 1)
        {
            _indices.push_back(3*numOfSections + ii);
            _indices.push_back(25);
            _indices.push_back(3*numOfSections + ii+1);
        }
        else
        {
            _indices.push_back(3*numOfSections + ii);
            _indices.push_back(25);
            _indices.push_back(3*numOfSections);
        }
    }
}

vector<Vertex> SpiderBody::getVertices()
{
    return _vertices;
}

    std::vector<uint32_t> SpiderBody::getIndices()
{
    return _indices;
}
