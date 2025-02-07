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
    
    std::vector<glm::mat4>             getModels(float time, Direction direction);
    std::vector<std::vector<Vertex>>   getVertices(); 
    std::vector<std::vector<uint32_t>> getIndices();
    std::vector<ModelGeometry>         getModelGeometries();
    
    private:
    
    std::vector<glm::mat4> getModelsPerDistance(float dist);
    
    // TODO Make in initializer list (here I'm making it twice.
    EggShape                   _eggShape;
    std::vector<ModelGeometry> _modelGeometries;
    
    float                      _lastTime      = -1;
    Direction                  _lastDirection = Direction::None;
    int                        _tempCounter   = 0;
    
};

#endif
