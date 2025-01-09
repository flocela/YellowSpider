#ifndef ModelGeometry_hpp
#define ModelGeometry_hpp

#include <vector>
#include "Utilities.hpp"

// TODO Requiring Mesh shouldn't be required. Think of moving Model to Utilities.
#include "Mesh.hpp"

class ModelGeometry
{
    public:
    
    ModelGeometry();
    ModelGeometry(const ModelGeometry& o) = default;
    ModelGeometry(ModelGeometry&& o) noexcept = default;
    ModelGeometry& operator=(const ModelGeometry& o) = default;
    ModelGeometry& operator=(ModelGeometry&& o) noexcept = default;
    ~ModelGeometry() = default;
    
    std::vector<Vertex>   getVertices() const;
    std::vector<uint32_t> getIndices() const;
    glm::mat4             getModel() const;
    
    void setVertices(std::vector<Vertex> vertices);
    void setIndices(std::vector<uint32_t> indices);
    void setModel(glm::mat4 model);
    
    
    private:
    
    std::vector<Vertex>   _vertices{};
    std::vector<uint32_t> _indices{};
    glm::mat4             _model{};

};

#endif /* ModelGeometry_hpp */
