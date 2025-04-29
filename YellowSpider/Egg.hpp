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
    
    std::vector<ModelGeometry> _modelGeometries;
    
    // TODO Make in initializer list (here I'm making it twice.
    
    const uint32_t _eggShapeNumOfSectionsAboutY      = 20;
    const float    _eggShapeAngleIncrementAboutZ_deg = 5.0f;
    const float    _eggShapeMediumRadius             = 5.0f;
    
    const float _rLarge  = 10.0f;
    const float _rMedium = _rLarge/2.0f;
    const float _rSmall  = _rLarge - (1.414f * _rMedium);
    
    EggShape _eggShape;
    
    float _firstTime_s    = -1.0f;              
    float _lastTime_s     = -1.0f;
    float _lastVelocity_s = -1.0f;
    float _lastRadians_r  = -1.0f;
    float _acc_r = -1.0f;
    
    Direction                  _lastDirection = Direction::None;
    float                      _tempCounter = 0.0f;
    
    const float _zero_rad          = 0.0f   * PI_F / 180.0f;
    const float _fortyFive_rad     = 45.0f  * PI_F / 180.0f;
    const float _ninety_rad        = 90.0f  * PI_F / 180.0f;
    const float _twoSeventy_rad    = 270.0f * PI_F / 180.0f;
    const float _oneThirtyFive_rad = 135.0f * PI_F / 180.0f;
    const float _twoTwentyFive_rad = 223.0f * PI_F / 180.0f;
    
    const std::vector<float> _aRads_r {
        0.0f     * (PI_F / 180.0f),
        76.917f  * (PI_F / 180.0f),
        103.083f * (PI_F / 180.0f),
        160.685f * (PI_F / 180.0f),
        PI_F,
        199.315f * (PI_F / 180.0f),
        256.917f * (PI_F / 180.0f),
        283.083f * (PI_F / 180.0f),
        2.0f* PI_F
    };
    
    const std::vector<float> _acc_rps2 {
           0.0f,
         58.1f,
           0.0f,
        -135.7f,
           0.0f,
         135.7f,
           0.0f,
        -58.1f, 
           0.0f
    };
    
    /*
    const std::vector<float> _acc_rps2 {
           0.0f,
         116.2f,
           0.0f,
        -271.4f,
           0.0f,
         271.4f,
           0.0f,
        -116.2f, 
           0.0f
    };
    */
    
    std::tuple<float, float, float> getEggState(float radians0_r, float velocity0_rps, float time0, float timeDiffRT_s);
    std::tuple<float, float, float> getNextEggStateSuper(float radians0_r, float velocity_rps, float time0, float timeDiffRT_s);
    std::tuple<int, float> abbrRadians(float radians);
    float getAcc(float radians);
};

#endif
