#include "RodTriangular.hpp"

using std::vector;

RodTriangular::RodTriangular(
    glm::vec3 point0,
    glm::vec3 point1,
    glm::vec3 point2,
    glm::vec3 point3,
    glm::vec3 point4,
    glm::vec3 point5
    )
: _point0{point0},
  _point1{point1},
  _point2{point2},
  _point3{point3},
  _point4{point4},
  _point5{point5}
{
    _vertices = { { _point0, {0.0f, 0.0f, 0.0f}},
                  { _point1, {0.0f, 0.0f, 0.0f}},
                  { _point2, {0.0f, 0.0f, 0.0f}},
                  { _point3, {1.0f, 0.0f, 0.0f}},
                  { _point4, {0.0f, 1.0f, 0.0f}},
                  { _point5, {0.0f, 0.0f, 1.0f}}
                };
                 
    _indices = { 0, 1, 4,
                 0, 4, 3,
                 1, 2, 5,
                 1, 5, 4,
                 2, 3, 5,
                 2, 0, 3
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
