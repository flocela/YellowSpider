#include "Spider.hpp"

#include "SpiderHead.hpp"
#include "SpiderBody.hpp"
#include "SpiderLeg.hpp"
#include "Leg.hpp"
#include "Utilities.hpp"
#include "LegMarker.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Spider::Spider(float time)
:   
    _headStateMap{
        {0, HeadMotion{glm::vec3{0.0f, 0.0f, 0.0f},
                       glm::vec3{0.0f, 0.0f, 0.0f},
                       3.0f,
                       0}},
        {1, HeadMotion{glm::vec3{0.0f, 0.0f, 0.0f},
                       glm::vec3{6.0f, 0.0f, 0.0f},
                       3.0f,
                       1}},
        {2, HeadMotion{glm::vec3{0.0f, 0.0f, 0.0f},
                       glm::vec3{0.0f, 0.0f, 0.0f},
                       3.0f,
                       2}},
        {3, HeadMotion{glm::vec3{0.0f, 0.0f, 0.0f},
                       glm::vec3{0.0f, 0.0f, 0.0f},
                       3.0f,
                       3}},
        {4, HeadMotion{glm::vec3{0.0f, 0.0f, 0.0f},
                       glm::vec3{0.0f, 0.0f, 0.0f},
                       3.0f,
                       4}},
        {5, HeadMotion{glm::vec3{0.0f, 0.0f, 0.0f},
                       glm::vec3{0.0f, 0.0f, 0.0f},
                       3.0f,
                       5}},
        {6, HeadMotion{glm::vec3{0.0f, 0.0f, 0.0f},
                       glm::vec3{6.0f, 0.0f, 0.0f},
                       3.0f,
                       6}},
        {7, HeadMotion{glm::vec3{0.0f, 0.0f, 0.0f},
                       glm::vec3{0.0f, 0.0f, 0.0f},
                       3.0f,
                       7}},
        {8, HeadMotion{glm::vec3{0.0f, 0.0f, 0.0f},
                       glm::vec3{0.0f, 0.0f, 0.0f},
                       3.0f,
                       8}},
        {9, HeadMotion{glm::vec3{0.0f, 0.0f, 0.0f},
                       glm::vec3{0.0f, 0.0f, 0.0f},
                       3.0f,
                       9}}
    },
    _headMarker{ 
        glm::vec3{10.0f, 0.0f, 0.0f},
        glm::vec3{0.0f, 4.4f, 0.0f}, 
        time,
        Direction::None,
        _headStateMap.at(0)}
{

    SpiderHead head{};
    ModelGeometry headMG{};
    headMG.setVertices(head.getVertices());
    headMG.setIndices(head.getIndices());
    _modelGeometries.push_back(headMG);
    _headModel = &_modelGeometries[_modelGeometries.size()-1];
    
    SpiderBody body{};
    ModelGeometry bodyMG{};
    bodyMG.setVertices(body.getVertices());
    bodyMG.setIndices(body.getIndices());
    //_modelGeometries.push_back(bodyMG);
    _bodyModel = &_modelGeometries[_modelGeometries.size()-1];
    
    for(int ii=0; ii<8; ++ii)
    {
        _legLengths.push_back(std::vector<float>{10.0f, 12.0f, 15.0f});
    }
    
    // Leg (per index) rotate about these axes.
    _legRotations = {
        glm::vec3{-2.0f, 0.0f, 3.0f}, // 0
        glm::vec3{ 0.0f, 0.0f, 1.0f}, // 1
        glm::vec3{ 1.0f, 0.0f, 3.0f}, // 2
        glm::vec3{ 3.0f, 0.0f, 1.0f}, // 3
        glm::vec3{ -1.0f, 0.0f, -3.0f}, // 4
        glm::vec3{ 1.0f, 0.0f, -3.0f}, // 5
        glm::vec3{ 0.0f, 0.0f, -1.0f}, // 6
        glm::vec3{ -2.0f, 0.0f,  -3.0f}, // 7
    };
    
    // generic spider leg size
    SpiderLeg spiderLeg{_legLengths[0][0], _legLengths[0][1], _legLengths[0][2],
                         glm::vec3{-0.5, 0.0, -0.33}, glm::vec3{0.0, 0.0, 0.66}, glm::vec3{0.5, 0.0, -0.33}};
    std::vector<std::vector<Vertex>>   leg0Vertices = spiderLeg.getSegmentVertices();
    std::vector<std::vector<uint32_t>> leg0Indices  = spiderLeg.getSegmentIndices();
    for(int ii=0; ii<8; ++ii)
    {
        for(int segment=0; segment<3; ++segment)
        {
            ModelGeometry legModelGeometry{};
            legModelGeometry.setVertices(leg0Vertices[segment]);;
            legModelGeometry.setIndices(leg0Indices[segment]);
            _modelGeometries.push_back(legModelGeometry);
        }
    }
    
    
    
    _legStateMap.insert({ 0, LegMotion{std::vector<float>{0.0f, 0.0f, 0.0f},
                                       glm::vec3{0.0f, 0.0f, 0.0f},
                                       3.0f,
                                       0}
    });
    
    _legStateMap.insert({ 1, LegMotion{std::vector<float>{-15.0f, 10.45f, -26.52f},
                                       glm::vec3{6.0f, 0.0f, 0.0f},
                                       3.0f,
                                       1}
    });
    
    _legStateMap.insert({ 2, LegMotion{std::vector<float>{0.0f, 0.0f, 0.0f},
                                       glm::vec3{0.0f, 0.0f, 0.0f},
                                       3.0f,
                                       2}
    });
    
    _legStateMap.insert({ 3, LegMotion{std::vector<float>{-3.0f, -2.0f, -1.0f},
                                       glm::vec3{0.0f, 0.0f, 0.0f},
                                       3.0f,
                                       3}
    });
    
    _legStateMap.insert({ 4, LegMotion{std::vector<float>{18.0f, -8.45f, 27.22f},
                                       glm::vec3{0.0f, 0.0f, 0.0f},
                                       3.0f,
                                       4}
    });
    
    _legStateMap.insert({ 5, LegMotion{std::vector<float>{-6.0f, -6.0f, -6.0f},
                                       glm::vec3{0.0f, 0.0f, 0.0f},
                                       3.0f,
                                       5}
    });
    
    _legStateMap.insert({ 6, LegMotion{std::vector<float>{0.0f, 0.0f, 0.0f},
                                       glm::vec3{6.0f, 0.0f, 0.0f},
                                       3.0f,
                                       6}
    });
    
    _legStateMap.insert({ 7, LegMotion{std::vector<float>{6.0f, 6.0f, 6.0f},
                                       glm::vec3{0.0f, 0.0f, 0.0f},
                                       3.0f,
                                       7}
    });
    //
    _legStateMap.insert({ 8, LegMotion{std::vector<float>{0.0f, 0.0f, 0.0f},
                                       glm::vec3{0.0f, 0.0f, 0.0f},
                                       3.0f,
                                       8}
    });
    
    _legStateMap.insert({ 9, LegMotion{std::vector<float>{0.0f, 0.0f, 0.0f},
                                       glm::vec3{0.0f, 0.0f, 0.0f},
                                       3.0f,
                                       9}
    });
    //
    _subsequentStatesPerState.insert({0,  {1,  10}});
    _subsequentStatesPerState.insert({1,  {2,  0 }});
    _subsequentStatesPerState.insert({2,  {3,  1 }});
    _subsequentStatesPerState.insert({3,  {4,  2 }});
    _subsequentStatesPerState.insert({4,  {5,  3 }});
    _subsequentStatesPerState.insert({5,  {6,  4 }});
    _subsequentStatesPerState.insert({6,  {7,  5 }});
    _subsequentStatesPerState.insert({7,  {8,  6 }});
    _subsequentStatesPerState.insert({8,  {9,  7 }});
    _subsequentStatesPerState.insert({9,  {0,  8 }});
    
    // TODO emplace_back
    _legMarkers.push_back(LegMarker{
        std::vector<float>{45.0f, 78.0f, 90.0f},
        glm::vec3(-4.57f, 7.2f, -3.05f),
        time,
        Direction::None,
        _legStateMap.at(0)});
    
    _legMarkers.push_back(LegMarker{
        std::vector<float>{45.0f, 78.0f, 90.0f},
        glm::vec3(-5.5f, 7.2f, 0.0f),
        time,
        Direction::None,
        _legStateMap.at(5)});
        
    _legMarkers.push_back(LegMarker{
        std::vector<float>{45.0f, 78.0f, 90.0f},
        glm::vec3(-5.21f, 7.2f, 1.74f),
        time,
        Direction::None,
        _legStateMap.at(0)});
        
    _legMarkers.push_back(LegMarker{
    std::vector<float>{45.0f, 78.0f, 90.0f},
    glm::vec3(-1.74, 7.2f, 5.51f),
    time,
    Direction::None,
    _legStateMap.at(5)});
    
    _legMarkers.push_back(LegMarker{
    std::vector<float>{45.0f, 78.0f, 90.0f},
    glm::vec3(1.74, 7.2f, 5.51f),
    time,
    Direction::None,
    _legStateMap.at(5)});
    
    _legMarkers.push_back(LegMarker{
    std::vector<float>{45.0f, 78.0f, 90.0f},
    glm::vec3(5.21, 7.2f, 1.74f),
    time,
    Direction::None,
    _legStateMap.at(0)});
    
    _legMarkers.push_back(LegMarker{
    std::vector<float>{45.0f, 78.0f, 90.0f},
    glm::vec3(5.5, 7.2f, 0.0f),
    time,
    Direction::None,
    _legStateMap.at(5)});
    
    _legMarkers.push_back(LegMarker{
    std::vector<float>{45.0f, 78.0f, 90.0f},
    glm::vec3(4.57, 7.2f, -3.05f),
    time,
    Direction::None,
    _legStateMap.at(0)});
        
}

std::vector<glm::mat4> Spider::getModels(float time, Direction direction)
{
    std::vector<glm::mat4> models{};
    

    float endTime = _headMarker.getEndTime();
    if( _lastDirection == Direction::Forward)
    {
        //   HEAD    //
        glm::mat4 modelHead(1.0f);
        if(endTime < time)
        {
            int nextHeadMotion = _subsequentStatesPerState.at(_headMarker.getMotionType()).first;
            std::cout << "CHANGE HEAD: nextHead: " << nextHeadMotion << std::endl;
            
            
            _headMarker = HeadMarker{
                glm::vec3{_headMarker.getStartGlobalRotationX()+_headMarker.getDeltaRotationX(),
                          _headMarker.getStartGlobalRotationY()+_headMarker.getDeltaRotationY(),
                          _headMarker.getStartGlobalRotationZ()+_headMarker.getDeltaRotationZ()},
                glm::vec3{_headMarker.getStartGlobalPosX() + _headMarker.getDeltaXPos(),
                          _headMarker.getStartGlobalPosY() + _headMarker.getDeltaYPos(),
                          _headMarker.getStartGlobalPosZ() + _headMarker.getDeltaZPos()},
                endTime,
                direction,
                _headStateMap.at(nextHeadMotion)};
        }
        //
        float timeDiff = time - _headMarker.getStartGlobalTime();
        float diffX         = timeDiff * _headMarker.getDeltaXPos() / _headMarker.getMotionTotalTime();
        float diffY         = timeDiff * _headMarker.getDeltaYPos() / _headMarker.getMotionTotalTime();
        float diffZ         = timeDiff * _headMarker.getDeltaZPos() / _headMarker.getMotionTotalTime();
        float diffRotationX = timeDiff * _headMarker.getDeltaRotationX() / _headMarker.getMotionTotalTime();
        float diffRotationY = timeDiff * _headMarker.getDeltaRotationY() / _headMarker.getMotionTotalTime();
        float diffRotationZ = timeDiff * _headMarker.getDeltaRotationZ() / _headMarker.getMotionTotalTime();
        //std::cout << "rotations: " << diffRotationX << ", " << diffRotationY << ", " << diffRotationZ << std::endl;
        
        glm::vec3 headPos{_headMarker.getStartGlobalPosX() + diffX,
                          _headMarker.getStartGlobalPosY() + diffY,
                          _headMarker.getStartGlobalPosZ() + diffZ};
                          
        //std::cout << "startRotations: " << _headMarker.getStartGlobalRotationX() << ", " << _headMarker.getStartGlobalRotationY() << ", " << _headMarker.getStartGlobalRotationZ() << std::endl;
        
        float rotationX = _headMarker.getStartGlobalRotationX() + diffRotationX;
        float rotationY = _headMarker.getStartGlobalRotationY() + diffRotationY;
        float rotationZ = _headMarker.getStartGlobalRotationZ() + diffRotationZ;
        
        //std::cout << "rotationX: " << rotationX << std::endl;
        modelHead = glm::rotate(modelHead, glm::radians(rotationX), glm::vec3{1.0f, 0.0f, 0.0f});
        modelHead = glm::rotate(modelHead, glm::radians(rotationY), glm::vec3{0.0f, 1.0f, 0.0f});
        modelHead = glm::rotate(modelHead, glm::radians(rotationZ), glm::vec3{0.0f, 0.0f, 1.0f});
        
        modelHead = glm::translate(modelHead, headPos);
        models.push_back(modelHead);
    
        //    LEGS    //
        std::cout << "_legLengths.size() :" << _legLengths.size();
        for(int ii=0; ii<_legLengths.size(); ++ii)
        {
            glm::mat4 model0(1.0f);
            glm::mat4 model1(1.0f);
            glm::mat4 model2(1.0f);
            
            float endTime = _legMarkers[ii].getEndTime();
            bool change = false;
            if (_lastDirection == Direction::Forward)
            {
                // TODO is this less than or less than or equal to?
                if (endTime < time)
                {
                    change = true;
                    std::cout << "CHANGE ROTATIONS" << std::endl;
                    
                    int nextLegMotion = _subsequentStatesPerState.at(_legMarkers[ii].getMotionType()).first;
                    std::cout << "nextLegMotion: " << nextLegMotion << std::endl;
                    
                    float diffX         = _legMarkers[ii].getDeltaXPos();
                    float diffRotation0 = _legMarkers[ii].getDeltaRotation0();
                    float diffRotation1 = _legMarkers[ii].getDeltaRotation1();
                    float diffRotation2 = _legMarkers[ii].getDeltaRotation2();
                    
                    _legMarkers[ii] = LegMarker{
                        std::vector<float>{_legMarkers[ii].getStartRotation0() + diffRotation0,
                                           _legMarkers[ii].getStartRotation1() + diffRotation1,
                                           _legMarkers[ii].getStartRotation2() + diffRotation2},
                        glm::vec3{_legMarkers[ii].getStartGlobalPosX() + diffX,
                                  _legMarkers[ii].getStartGlobalPosY(),
                                  _legMarkers[ii].getStartGlobalPosZ()},
                        endTime,
                        direction,
                        _legStateMap.at(nextLegMotion)
                    };
                }
                
                float timeDiff = time - _legMarkers[ii].getStartGlobalTime();
                
                float diffX         = timeDiff * _legMarkers[ii].getDeltaXPos()      / _legMarkers[ii].getMotionTotalTime();
                float diffRotation0 = timeDiff * _legMarkers[ii].getDeltaRotation0() / _legMarkers[ii].getMotionTotalTime();
                float diffRotation1 = timeDiff * _legMarkers[ii].getDeltaRotation1() / _legMarkers[ii].getMotionTotalTime();
                float diffRotation2 = timeDiff * _legMarkers[ii].getDeltaRotation2() / _legMarkers[ii].getMotionTotalTime();
                
                glm::vec3 pos{
                    _legMarkers[ii].getStartGlobalPosX() + diffX,
                    _legMarkers[ii].getStartGlobalPosY(), 
                    _legMarkers[ii].getStartGlobalPosZ()};
                
                float rotation0_deg = _legMarkers[ii].getStartRotation0() + diffRotation0;
                float rotation1_deg = _legMarkers[ii].getStartRotation1() + diffRotation1;
                float rotation2_deg = _legMarkers[ii].getStartRotation2() + diffRotation2;
                
                model2 = glm::translate(model2, pos); // TODO this needs to change per leg 
                model2 = glm::rotate   (model2, glm::radians(rotation0_deg), _legRotations[ii]); 
                model2 = glm::translate(model2, glm::vec3(0.0f, _legLengths[0][0], 0.0f));
                model2 = glm::rotate   (model2, glm::radians(rotation1_deg), _legRotations[ii]); 
                model2 = glm::translate(model2, glm::vec3(0.0f, _legLengths[0][1], 0.0f));
                model2 = glm::rotate   (model2, glm::radians(rotation2_deg), _legRotations[ii]);  // c rotate
               
                model1 = glm::translate(model1, pos);
                model1 = glm::rotate   (model1, glm::radians(rotation0_deg), _legRotations[ii]);
                model1 = glm::translate(model1, glm::vec3(0.0f, _legLengths[0][0], 0.0f));
                model1 = glm::rotate   (model1, glm::radians(rotation1_deg), _legRotations[ii]);  // b rotate
                
                model0 = glm::translate(model0, pos);
                model0 = glm::rotate   (model0, glm::radians(rotation0_deg), _legRotations[ii]);
                
                if(change)
                {
                    std::cout << "r0, r1, r2: " << rotation0_deg << ", " << rotation1_deg << ", " << rotation2_deg << std::endl;
                    std::cout << "px, py, pz: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
                    std::cout << "len0, 1, 2: " << _legLengths[0][0] << ", " << _legLengths[0][1] << ", " << _legLengths[0][2] << std::endl;
                }

            }
            models.push_back(model0);
            models.push_back(model1);
            models.push_back(model2);
        }
    }
    //
    _lastTime = time;
    _lastDirection = direction;
    
    return models;

}

std::vector<ModelGeometry> Spider::getModelGeometries()
{
    return _modelGeometries;
}

