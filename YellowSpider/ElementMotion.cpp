#include "ElementMotion.hpp"

ElementMotion::ElementMotion(
    glm::vec3 deltaRotation,
    glm::vec3 deltaPos,
    float deltaTime,
    int type
)
:   _deltaRotation{deltaRotation},
    _deltaPos{deltaPos},
    _deltaTime{deltaTime},
    _type{type}
{}

glm::vec3 ElementMotion::getDeltaRotation()
{
    return _deltaRotation;
}
glm::vec3 ElementMotion::getDeltaPos()
{
    return _deltaPos;
}

float ElementMotion::getTotalTime()
{
    return _deltaTime;
}

int ElementMotion::getType()
{
    return _type;
}
