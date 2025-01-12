#include "HeadMotion.hpp"

HeadMotion::HeadMotion(
    glm::vec3 deltaRotation,
    glm::vec3 deltaPos,
    float totalTime,
    int type
)
:   _deltaRotation{deltaRotation},
    _deltaPos{deltaPos},
    _totalTime{totalTime},
    _type{type}
{}

glm::vec3 HeadMotion::getDeltaRotation()
{
    return _deltaRotation;
}
glm::vec3 HeadMotion::getDeltaPos()
{
    return _deltaPos;
}

float HeadMotion::getTotalTime()
{
    return _totalTime;
}

int HeadMotion::getType()
{
    return _type;
}
