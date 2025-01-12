#ifndef LegMotion_hpp
#define LegMotion_hpp

#include <glm/glm.hpp>
#include <vector>

class LegMotion
{

    public:
    
    LegMotion(
        std::vector<float> deltaRotations,
        glm::vec3 deltaPos,
        float totalTime,
        int type
    );
        
    LegMotion(const LegMotion& o) = default;
    LegMotion(LegMotion&& o) noexcept = default;
    LegMotion& operator= (const LegMotion& o) = default;
    LegMotion& operator= (LegMotion&& o) noexcept = default;
    ~LegMotion() = default;
    
    // Rotation0 is the rotation at the leg to body joint.
    float getDeltaRotation0();
    
    // Rotation1 can be thought of as the knee joint. (The next joint after the leg to body joint.)
    float getDeltaRotation1();
    
    // Rotation2 can be thought of as the ankle joint. (The lowest joint on the leg.)
    float getDeltaRotation2();
    float getDeltaPosX();
    float getDeltaPosY();
    float getDeltaPosZ();
    float getTotalTime();
    float getType();
    
    private:
    
    std::vector<float> _deltaRotations{};
    glm::vec3          _deltaPos;
    float              _totalTime;
    int                _type;
    
};

#endif /* LegMotion_hpp */
