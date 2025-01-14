
#include "HeadMarker.hpp"

HeadMarker::HeadMarker(
    glm::vec3 startGlobalRotation,
    glm::vec3 startGlobalPos,
    float startGlobalTime,
    Direction direction,
    ElementMotion headMotion)
:   _startGlobalRotation{startGlobalRotation},
    _startGlobalPos{startGlobalPos},
    _startGlobalTime{startGlobalTime},
    _direction{direction},
    _headMotion{headMotion}
{}

glm::vec3 HeadMarker::getStartGlobalRotation()
{
    return _startGlobalRotation;
}

float HeadMarker::getStartGlobalRotationX()
{
    return _startGlobalRotation.x;
}

float HeadMarker:: getStartGlobalRotationY()
{
    return _startGlobalRotation.y;
}

float HeadMarker::getStartGlobalRotationZ()
{
    return _startGlobalRotation.z;
}

glm::vec3 HeadMarker::getStartGlobalPos()
{
    return _startGlobalPos;
}

float HeadMarker::getStartGlobalPosX()
{
    return _startGlobalPos.x;
}

float HeadMarker::getStartGlobalPosY()
{
    return _startGlobalPos.y;
}

float HeadMarker::getStartGlobalPosZ()
{
    return _startGlobalPos.z;
}

Direction HeadMarker::getDirection()
{
    return _direction;
}

float HeadMarker::getMotionTotalTime()
{
    return _headMotion.getTotalTime();
}

float HeadMarker::getStartGlobalTime()
{
    return _startGlobalTime;
}

float HeadMarker::getEndTime()
{
    return _startGlobalTime + _headMotion.getTotalTime();
}

int HeadMarker::getMotionType()
{
    return _headMotion.getType();
}

float HeadMarker::getDeltaXPos()
{
    return _headMotion.getDeltaPos().x;
}

float HeadMarker::getDeltaYPos()
{
    return _headMotion.getDeltaPos().y;
}

float HeadMarker::getDeltaZPos()
{
    return _headMotion.getDeltaPos().z;
}

float HeadMarker::getDeltaRotationX()
{
    return _headMotion.getDeltaRotation().x;
}

float HeadMarker::getDeltaRotationY()
{
    return _headMotion.getDeltaRotation().y;
}

float HeadMarker::getDeltaRotationZ()
{
    return _headMotion.getDeltaRotation().z;
}
