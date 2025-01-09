#ifndef LegMarker_hpp
#define LegMarker_hpp

#include <glm/glm.hpp>
#include <vector>

#include "Direction.hpp"
#include "LegMotion.hpp"

class LegMarker
{

    public:
    
    LegMarker(std::vector<float> startGlobalRotations,
        glm::vec3 startGlobalPos,
        float startGlobalTime,
        Direction direction,
        LegMotion legMotion);
    
    LegMarker(const LegMarker& o) = default;
    LegMarker(LegMarker&& o) noexcept = default;
    LegMarker& operator= (const LegMarker& o) = default;
    LegMarker& operator= (LegMarker&& o) noexcept = default;
    ~LegMarker() = default;
    
    float     getStartRotation0();
    float     getStartRotation1();
    float     getStartRotation2();
    glm::vec3 getStartGlobalPos();
    float     getStartGlobalPosX();
    float     getStartGlobalPosY();
    float     getStartGlobalPosZ();
    float     getStartGlobalTime();
    Direction getDirection();
    float     getMotionTotalTime();
    float     getDeltaXPos();
    float     getDeltaYPos();
    float     getDeltaZPos();
    float     getDeltaRotation0();
    float     getDeltaRotation1();
    float     getDeltaRotation2();
    float     getEndTime();
    int       getMotionType();
    
    private:
    
    std::vector<float> _startGlobalRotations{};
    glm::vec3          _startGlobalPos;
    float              _startGlobalTime;
    Direction          _direction;
    LegMotion          _legMotion;
    
    
    

};

#endif /* LegMarker_hpp */
