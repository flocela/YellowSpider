#include "Egg.hpp"
#include "EggShape.hpp"
#include "ModelGeometry.hpp"

Egg::Egg(float time)
{
    EggShape eggShape{};
    ModelGeometry modelGeometry{};
    modelGeometry.setVertices(eggShape.getVertices());
    modelGeometry.setIndices(eggShape.getIndices());
    
    _modelGeometries.push_back(modelGeometry);
}

std::vector<glm::mat4> Egg::getModels(float time, Direction direction)
{
    std::vector<glm::mat4> models{};
    models.push_back(glm::mat4(1.0f));
    return models;
}

std::vector<std::vector<Vertex>> Egg::getVertices()
{
    std::vector<std::vector<Vertex>> vectorOfVectorOfVertices{};
    vectorOfVectorOfVertices.push_back(_eggShape.getVertices());
    
    return vectorOfVectorOfVertices;
} 

std::vector<std::vector<uint32_t>> Egg::getIndices()
{
    std::vector<std::vector<uint32_t>> vectorOfVectorOfIndices{};
    vectorOfVectorOfIndices.push_back(_eggShape.getIndices());
    return vectorOfVectorOfIndices;
    
}

std::vector<ModelGeometry> Egg::getModelGeometries()
{
    return _modelGeometries;
}



