#include <glm/glm.hpp>
#include <vector>
#include "Utilities.hpp"

#ifndef SpiderBody_hpp
#define SpiderBody_hpp

class SpiderBody
{
    public:
        SpiderBody();
        SpiderBody(const SpiderBody& o) = default;
        SpiderBody(SpiderBody&& o) noexcept = default;
        SpiderBody& operator= (const SpiderBody& o) = default;
        SpiderBody& operator= (SpiderBody&& o) noexcept = default;
        ~SpiderBody() = default;
        
        std::vector<Vertex> getVertices();
        std::vector<uint32_t> getIndices();
    
    private:
    
        glm::vec3 _startPoint{0.0, 0.0,  -43.0};
        glm::vec3 _endPoint  {0.0, 0.0,  0.0  };
        
        glm::vec3 _pointTop0 {0.0, 7.5,  -43.0};
        glm::vec3 _pointTop1 {0.0, 16.5, -30.0};
        glm::vec3 _pointTop2 {0.0, 14.5, -15.0};
        glm::vec3 _pointTop3 {0.0, 7.0,  -5.0 };
        
        //glm::vec3 _pointBot0 {0.0, 10.0, -35.0};
        //glm::vec3 _pointBot1 {0.0, 10.0, -20.0};
        //glm::vec3 _pointBot2 {0.0, 7.5,  -12.5};
        //glm::vec3 _pointBot3 {0.0, 5.0,  -5.0 };
        
        glm::vec3 _pointBot0 {0.0, 7.5, -43.0};
        glm::vec3 _pointBot1 {0.0, 16.5, -30.0};
        glm::vec3 _pointBot2 {0.0, 14.5, -15.0};
        glm::vec3 _pointBot3 {0.0, 7.0,  -5.0 };
        
        
        
        std::vector<Vertex>   _vertices{};
        std::vector<uint32_t> _indices;
        
        // vertices rows are from bottom to top.
        // vertices go in a +y rotation direction
        void populateIndices(int numOfSides, int numOfRows);
};

#endif /* SpiderBody_hpp */
