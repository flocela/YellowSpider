#include <glm/glm.hpp>
#include <vector>
#include "Utilities.hpp"

#ifndef Leg_hpp
#define Leg_hpp

class Leg
{
    public:
    
    Leg() = delete;
    Leg(float seg0, float seg1, float seg2);
    Leg(const Leg& o) = default;
    Leg(Leg&& o) noexcept = default;
    Leg& operator= (const Leg& o) = default;
    Leg& operator= (Leg&& o) noexcept = default;
    ~Leg() = default;
    
    std::vector<glm::mat4> getModels();
    std::vector<std::vector<uint32_t>> getIndices();
    std::vector<std::vector<Vertex>> getVertices();
    
    private:
    
    float _seg0Length = 0.0;
    float _seg1Length = 0.0;
    float _seg2Length = 0.0;

};

#endif /* Leg_hpp */
