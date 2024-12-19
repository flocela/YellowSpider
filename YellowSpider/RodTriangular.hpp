#ifndef RodTriangular_hpp
#define RodTriangular_hpp

#include <glm/glm.hpp>
#include <vector>
#include "Utilities.hpp"


class RodTriangular
{
    public:
        RodTriangular() = delete;
        RodTriangular(
            glm::vec3 point0,
            glm::vec3 point1,
            glm::vec3 point2,
            glm::vec3 point3,
            glm::vec3 point4,
            glm::vec3 point5
        );
        RodTriangular(const RodTriangular& o) = default;
        RodTriangular(RodTriangular&& o) noexcept = default;
        RodTriangular& operator=(const RodTriangular& o) = default;
        RodTriangular& operator=(RodTriangular&& o) noexcept = default;
        
        std::vector<Vertex> getVertices();
        std::vector<uint32_t> getIndices();
        
    private:
        glm::vec3 _point0;
        glm::vec3 _point1;
        glm::vec3 _point2;
        glm::vec3 _point3;
        glm::vec3 _point4;
        glm::vec3 _point5;
        float     _height;
        
        std::vector<Vertex>   _vertices{};
        std::vector<uint32_t> _indices;
};
        
        
        

#endif
