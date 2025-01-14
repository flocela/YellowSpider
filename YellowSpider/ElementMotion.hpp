#ifndef ElementMotion_hpp
#define ElementMotion_hpp

#include <glm/glm.hpp>
#include <vector>

class ElementMotion
{
    public:
    
    ElementMotion(
        glm::vec3 deltaRotation,
        glm::vec3 deltaPos,
        float ,
        int type
    );
    
    ElementMotion(const ElementMotion& o) = default;
    ElementMotion(ElementMotion&& o) noexcept = default;
    ElementMotion& operator= (const ElementMotion& o) = default;
    ElementMotion& operator= (ElementMotion&& o) noexcept = default;
    ~ElementMotion() = default;
    
    glm::vec3 getDeltaRotation();
    glm::vec3 getDeltaPos();
    float getTotalTime();
    int getType();
    
    private:
    
    glm::vec3 _deltaRotation;
    glm::vec3 _deltaPos;
    float     _deltaTime;
    int       _type;
    
};
#endif /* HeadMotion_hpp */
