#ifndef HeadMotion_hpp
#define HeadMotion_hpp

#include <glm/glm.hpp>
#include <vector>

class HeadMotion
{
    public:
    
    HeadMotion(
        glm::vec3 deltaRotation,
        glm::vec3 deltaPos,
        float totalTime,
        int type
    );
    
    HeadMotion(const HeadMotion& o) = default;
    HeadMotion(HeadMotion&& o) noexcept = default;
    HeadMotion& operator= (const HeadMotion& o) = default;
    HeadMotion& operator= (HeadMotion&& o) noexcept = default;
    ~HeadMotion() = default;
    
    glm::vec3 getDeltaRotation();
    glm::vec3 getDeltaPos();
    float getTotalTime();
    int getType();
    
    private:
    
    glm::vec3 _deltaRotation;
    glm::vec3 _deltaPos;
    float     _totalTime;
    int       _type;
    
};
#endif /* HeadMotion_hpp */
