#ifndef Egg_hpp
#define Egg_hpp

#include <vector>
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
    
    std::vector<glm::mat4> getModels(float time, Direction direction);
    std::vector<std::vector<Vertex>> getVertices(); 
    std::vector<std::vector<uint32_t>> getIndices();
    std::vector<ModelGeometry> getModelGeometries();
    
    private:
    
    EggShape _eggShape{};
    
    std::vector<ModelGeometry> _modelGeometries;
    
    float _lastTime = -1;
    Direction _lastDirection = Direction::None;
    
};

#endif
