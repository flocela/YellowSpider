#include "LegMotion.hpp"

LegMotion::LegMotion(
    std::vector<float> deltaRotations,
    glm::vec3 deltaPos,
    float totalTime,
    int type
) : _deltaRotations{deltaRotations},
    _deltaPos{deltaPos},
    _totalTime{totalTime},
    _type{type}
{}

float LegMotion::getDeltaRotation0()
{
    return _deltaRotations[0];
}

float LegMotion::getDeltaRotation1()
{
    return _deltaRotations[1];
}

float LegMotion::getDeltaRotation2()
{
    return _deltaRotations[2];
}

float LegMotion::getDeltaPosX()
{
    return _deltaPos.x;
}

float LegMotion::getDeltaPosY()
{
    return _deltaPos.y;
}

float LegMotion::getDeltaPosZ()
{
    return _deltaPos.z;
}

float LegMotion::getTotalTime()
{
    return _totalTime;
}

float LegMotion::getType()
{
    return _type;
}

