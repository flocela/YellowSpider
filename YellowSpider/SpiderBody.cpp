#include "SpiderBody.hpp"
#include <iostream>
const float  PI_F=3.14159265358979f;

using std::vector;

SpiderBody::SpiderBody()
{
    // TODO declare _vertices as _vertices(26, Vertex{_point0,     {1.0f, 0.0f, 0.0f}})
    _vertices.reserve(42);
    for(int ii=0; ii<42; ++ii)
    {
        _vertices.push_back({ _pointTop0,     {1.0f, 0.0f, 0.0f}});
    }
    
    int startIdx = 40;
    int endIdx   = 41;
    _vertices[startIdx] = { _startPoint, {0.0f, 0.0f, 0.0f} };
    _vertices[endIdx]   = { _endPoint,   {0.0f, 0.0f, 0.0f} };
    
    /*****     TOP      *****/
    _vertices[0]  = { _pointTop0, {1.0f, 0.0f, 0.0f} };
    _vertices[10] = { _pointTop1, {0.0f,  1.0f, 0.0f}};
    _vertices[20] = { _pointTop2, {0.0f, 0.0f, 1.0f} };
    _vertices[30] = { _pointTop3, {1.0f, 0.0f, 0.0f} };
    
    int offset = 6;
    _vertices[6]  = { _pointBot0, {1.0f, 0.0f, 0.0f} };
    _vertices[16] = { _pointBot1, {0.0f, 1.0f, 0.0f}};
    _vertices[26] = { _pointBot2, {0.0f, 0.0f, 1.0f} };
    _vertices[36] = { _pointBot3, {1.0f, 0.0f, 0.0f} };
    
    uint32_t numOfNodes = 10;
    float radiansPerSection = 2*180.0f/(numOfNodes) * PI_F/180.0f;
    for(int ii=0; ii<4; ++ii)
    {
        float radiusTop   = _vertices[ii*numOfNodes].pos.y;
        float radiusBot   = _vertices[(ii*numOfNodes) + offset].pos.y;
        float zValTop     = _vertices[ii*numOfNodes].pos.z;
        float zValBot     = _vertices[(ii*numOfNodes) + offset].pos.z;
        glm::vec3 ringCol = _vertices[ii*numOfNodes].col;
        for(int jj=0; jj<numOfNodes; ++jj)
        {
            if(jj <= 5)
            {
                std::cout << "jj: " << jj << "angle: " << "radiansPerSection * jj:" << (radiansPerSection * jj * 180.0f/PI_F) << std::endl;
                _vertices[(ii*numOfNodes)+jj] = 
                { glm::vec3{radiusTop * cos(radiansPerSection * jj),
                            radiusTop * sin(radiansPerSection * jj),
                            zValTop},
                  ringCol
                };
            }
            else
            {
                std::cout << "jj: " << jj << "angle: " << "radiansPerSection * jj: " << (radiansPerSection * jj * 180.0f/PI_F) << std::endl;
                _vertices[(ii*numOfNodes)+jj] = 
                { glm::vec3{radiusBot * cos(radiansPerSection * jj),
                            radiusBot * sin(radiansPerSection * jj),
                            zValBot},
                  ringCol
                };
            }
        }
    }
    
        
    for(int ii=0; ii<_vertices.size(); ++ii)
    {
        std::cout << ii << ":: " << _vertices[ii].pos.x << ", " << _vertices[ii].pos.y << ", " << _vertices[ii].pos.z << std::endl;
    }

    
    for(int ii=0; ii<3; ++ii)
    {
        for(int jj=0; jj<numOfNodes; ++jj)
        {
            if(jj != numOfNodes - 1)
            {
                _indices.push_back(( (ii)   * numOfNodes ) + jj    );
                _indices.push_back(( (ii+1) * numOfNodes ) + jj    );
                _indices.push_back(( (ii)   * numOfNodes ) + jj + 1);
                
                _indices.push_back(( (ii)   * numOfNodes ) + jj + 1);
                _indices.push_back(( (ii+1) * numOfNodes ) + jj    );
                _indices.push_back(( (ii+1) * numOfNodes ) + jj + 1);
            }
            else
            {
                _indices.push_back(( (ii)   * numOfNodes ) + jj    );
                _indices.push_back(( (ii+1) * numOfNodes ) + jj    );
                _indices.push_back(( (ii)   * numOfNodes ) + 0     );
                
                _indices.push_back(( (ii)   * numOfNodes ) + 0     );
                _indices.push_back(( (ii+1) * numOfNodes ) + jj    );
                _indices.push_back(( (ii+1) * numOfNodes ) + 0     );
            }
        }
    }

    for(int ii=0; ii<numOfNodes; ++ii)
    {
        if(ii != numOfNodes - 1)
        {
            _indices.push_back(startIdx);
            _indices.push_back(ii);
            _indices.push_back(ii+1);
        }
        else
        {
            _indices.push_back(startIdx);
            _indices.push_back(ii);
            _indices.push_back(0);
        }
    }
    
    for(int ii=0; ii<numOfNodes; ++ii)
    {
        if(ii != numOfNodes - 1)
        {
            _indices.push_back((3*numOfNodes) + ii);
            _indices.push_back(endIdx);
            _indices.push_back((3*numOfNodes) + ii+1);
        }
        else
        {
            _indices.push_back((3*numOfNodes) + ii);
            _indices.push_back(endIdx);
            _indices.push_back(3*numOfNodes);
        }
    }
    
    for(int ii=0; ii<_indices.size(); ii+=3)
    {
        int one = _indices[ii];
        int two = _indices[ii+1];
        int three = _indices[ii+2];
        
        if (one == 40 || one == 41 ||
            two == 40 || two == 41 ||
            three == 40 || three == 41)
        {
            std::cout << "indices: " << one << ", " << two << ", " << three << std::endl;
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
