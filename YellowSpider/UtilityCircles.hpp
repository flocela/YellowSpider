#ifndef UtilityCircles_hpp
#define UtilityCircles_hpp

#include "Utilities.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

class UtilityCircles
{
    public:
    
    static std::vector<glm::vec4> getVerticesFromDiameter(glm::vec3 point1, glm::vec3 point2,
                                                          uint32_t numOfSides, glm::vec3 rotationAxis)
    {
        std::vector<glm::vec4> vertices{};
        
        // TODO This won't work if floats are too big.
        glm::vec3 origCenter = { (point1.x + point2.x)/2,
                                 (point1.y + point2.y)/2,
                                 (point1.z + point2.z)/2
                               };
                               
        // Ring is moved so that center is at (0, 0, 0). This moves point1 to translatedPoint1
        glm::vec3 translatedPoint1 = { (point1.x - origCenter.x),
                                       (point1.y - origCenter.y),
                                       (point1.z - origCenter.z)
                                     };
        
        std::cout << "translatedAboutCenter: ";
        std::cout << ", (" << translatedPoint1.x  << ", " << translatedPoint1.y << ", " << translatedPoint1.z << ")" <<std::endl;
        
        glm::mat4 transformMatrix{1.0f};
        transformMatrix = glm::translate(transformMatrix, origCenter);
        for(int ii=0; ii<numOfSides; ++ii)
        {
            vertices.push_back( glm::rotate(transformMatrix, glm::radians(360.0f/numOfSides) * ii, rotationAxis) *
            glm::vec4(translatedPoint1, 1.0) );
        }
        
        std::cout << "vertices: " << std::endl;
        for(int ii=0; ii<vertices.size(); ++ii)
        {
            std::cout << vertices[ii].x << ", " << vertices[ii].y << ", " << vertices[ii].z << std::endl;
        }
        
        return vertices;
    }

};

#endif /* UtilityCircles_hpp */
