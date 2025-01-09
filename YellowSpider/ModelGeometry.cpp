#include "ModelGeometry.hpp"

ModelGeometry::ModelGeometry()
{
    // TODO Should be able to set _model in attributes list.
    _model = glm::mat4(1.0f);
}

std::vector<Vertex> ModelGeometry::getVertices() const
{
    return _vertices;
}

std::vector<uint32_t> ModelGeometry::getIndices() const
{
    return _indices;
}

glm::mat4 ModelGeometry::getModel() const
{
    return _model;
}

void ModelGeometry::setVertices(std::vector<Vertex> vertices)
{
    _vertices = vertices;
}

void ModelGeometry::setIndices(std::vector<uint32_t> indices)
{
    _indices = indices;
}

void ModelGeometry::setModel(glm::mat4 model)
{
    _model = model;
}
