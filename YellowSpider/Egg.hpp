#ifndef Egg_hpp
#define Egg_hpp

#include <vector>
#include <unordered_map>
#include "ModelGeometry.hpp"
#include "Utilities.hpp"
#include "Direction.hpp"
#include "EggShape.hpp"

class Egg
{

    public:
    
    Egg(float time);
    Egg(const Egg& o) = default;
    Egg(Egg&& o) noexcept = default;
    Egg& operator= (const Egg& o) = default;
    Egg& operator= (Egg&& o) noexcept = default;
    
    void setTime(float time);
    
    std::vector<glm::mat4>             getModels(float time_s, Direction direction);
    std::vector<std::vector<Vertex>>   getVertices(); 
    std::vector<std::vector<uint32_t>> getIndices();
    std::vector<ModelGeometry>         getModelGeometries();
    std::vector<glm::mat4>             getModelsPerRotation(float rotation_rad);
    
    private:
    
    std::vector<glm::mat4> getModelsPerDistance(float dist);
    float moduloRotationsAsPositive(float rotation_rad);
    
    // TODO Make in initializer list (here I'm making it twice.
    EggShape                   _eggShape;
    std::vector<ModelGeometry> _modelGeometries;
    
    float                      _lastTime_s   = -1.0f;
    Direction                  _lastDirection = Direction::None;
    float                      _tempCounter = 0.0f;
    
    float _fortyFive_rad     = 45.0f  * PI_F / 180.0f;
    float _ninety_rad        = 90.0f  * PI_F / 180.0f;
    float _twoSeventy_rad    = 270.0f * PI_F / 180.0f;
    float _oneThirtyFive_rad = 135.0f * PI_F / 180.0f;
    float _twoTwentyFive_rad = 223.0f * PI_F / 180.0f;
    
    // Taken from EggShape class. Must get these values in a more modular way.
    float _rLarge  = 10.0f;
    float _rMedium = _rLarge/2.0f;
    float _rSmall  = _rLarge - (1.414f * _rMedium);
    
    float _angle0_rad   = _twoSeventy_rad;
    float _v0_radPerSec = 5.0f;
    float _a0_radPerSecSec = 0.0f;
    float _va_radPerSec = 0.0f;
    

};

#endif
