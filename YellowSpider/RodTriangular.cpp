#include "RodTriangular.hpp"

using std::vector;

RodTriangular::RodTriangular(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, float height)
: _point0{point0},
  _point1{point1},
  _point2{point2},
  _height{height}
{
    _point3 = glm::vec3(_point0.x + 0.1f, _point0.y, _point0.z + 0.1f);
    _point4 = glm::vec3(_point3.x, _point3.y + _height, _point3.z);
    _point5 = glm::vec3(_point2.x, _point2.y + _height, _point2.z);
    _point6 = glm::vec3(_point1.x, _point1.y + _height, _point1.z);
    _point7 = glm::vec3(_point0.x, _point0.y + _height, _point0.z);
    
    _vertices = { { _point0, {1.0f, 0.0f, 0.0f}},
                  { _point1, {0.0f, 1.0f, 0.0f}},
                  { _point2, {0.0f, 0.0f, 1.0f}},
                  { _point3, {1.0f, 0.0f, 0.0f}},
                  { _point4, {1.0f, 0.0f, 0.0f}},
                  { _point5, {0.0f, 0.0f, 1.0f}},
                  { _point6, {0.0f, 1.0f, 0.0f}},
                  { _point7, {1.0f, 0.0f, 0.0f}}
                };
                 
    _indices = { 0, 1, 6,
                 0, 6, 7,
                 1, 2, 5,
                 1, 5, 6,
                 2, 3, 4,
                 2, 4, 5,
                };
}

vector<Vertex> RodTriangular::getVertices()
{
    return _vertices;
}

std::vector<uint32_t> RodTriangular::getIndices()
{
    return _indices;
}
