#include "LegMarker.hpp"

LegMarker::LegMarker(
    std::vector<float> startGlobalRotations,
    glm::vec3 startGlobalPos,
    float startGlobalTime,
    Direction direction,
    LegMotion legMotion)
:   _startGlobalRotations{startGlobalRotations},
    _startGlobalPos{startGlobalPos},
    _startGlobalTime{startGlobalTime},
    _direction{direction},
    _legMotion{legMotion}
{}

float LegMarker::getStartRotation0()
{
    return _startGlobalRotations[0];
}

float LegMarker::getStartRotation1()
{
    return _startGlobalRotations[1];
}

float LegMarker:: getStartRotation2()
{
    return _startGlobalRotations[2];
}

float LegMarker::getDeltaRotation0()
{
    return _legMotion.getDeltaRotation0();
}

float LegMarker::getDeltaRotation1()
{
    return _legMotion.getDeltaRotation1();
}

float LegMarker::getDeltaRotation2()
{
    return _legMotion.getDeltaRotation2();
}

glm::vec3 LegMarker::getStartGlobalPos()
{
    return _startGlobalPos;
}

float LegMarker::getStartGlobalPosX()
{
    return _startGlobalPos.x;
}

float LegMarker::getStartGlobalPosY()
{
    return _startGlobalPos.y;
}

float LegMarker::getStartGlobalPosZ()
{
    return _startGlobalPos.z;
}

Direction LegMarker::getDirection()
{
    return _direction;
}

float LegMarker::getMotionTotalTime()
{
    return _legMotion.getTotalTime();
}

float LegMarker::getDeltaXPos()
{
    return _legMotion.getDeltaPosX();
}

float LegMarker::getDeltaYPos()
{
    return _legMotion.getDeltaPosY();
}

float LegMarker::getDeltaZPos()
{
    return _legMotion.getDeltaPosZ();
}

float LegMarker::getStartGlobalTime()
{
    return _startGlobalTime;
}

float LegMarker::getEndTime()
{
    return _startGlobalTime + _legMotion.getTotalTime();
}


int LegMarker::getMotionType()
{
    return _legMotion.getType();
}

