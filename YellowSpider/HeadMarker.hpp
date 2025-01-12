#ifndef HeadMarker_hpp
#define HeadMarker_hpp

#include "HeadMarker.hpp"

#include <glm/glm.hpp>
#include <vector>
#include "Direction.hpp"
#include "HeadMotion.hpp"

class HeadMarker
{
    public:
    
    HeadMarker(
        glm::vec3 startGlobalRotation,
        glm::vec3 startGlobalPos,
        float startGlobalTime,
        Direction direction,
        HeadMotion headMotion);
        
    HeadMarker(const HeadMarker& o) = default;
    HeadMarker(HeadMarker&& o) noexcept = default;
    HeadMarker& operator= (const HeadMarker& o) = default;
    HeadMarker& operator= (HeadMarker&& o) noexcept = default;
    ~HeadMarker() = default;
    
    glm::vec3 getStartGlobalRotation();
    float     getStartGlobalRotationX(); // about X
    float     getStartGlobalRotationY(); // about Y
    float     getStartGlobalRotationZ(); // about Z
    glm::vec3 getStartGlobalPos();
    float     getStartGlobalPosX();
    float     getStartGlobalPosY();
    float     getStartGlobalPosZ();
    Direction getDirection();
    float     getMotionTotalTime();
    float     getStartGlobalTime();
    float     getEndTime();
    int       getMotionType();
    float     getDeltaXPos();
    float     getDeltaYPos();
    float     getDeltaZPos();
    float     getDeltaRotationX();
    float     getDeltaRotationY();
    float     getDeltaRotationZ();
    
    private:
    
    glm::vec3  _startGlobalRotation;
    glm::vec3  _startGlobalPos;
    float      _startGlobalTime;
    Direction  _direction;
    HeadMotion _headMotion;

};

#endif /* HeadMarker_hpp */

