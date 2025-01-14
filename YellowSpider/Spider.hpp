#ifndef Spider_hpp
#define Spider_hpp

#include <vector>
#include <unordered_map>
#include "Mesh.hpp"
#include "ModelGeometry.hpp"
#include "Direction.hpp"
#include "LegMotion.hpp"
#include "LegMarker.hpp"
#include "ElementMotion.hpp"
#include "HeadMarker.hpp"

class Spider
{
    public:
        
    Spider(float time);
    Spider(const Spider& o) = default;
    Spider(Spider&& o) noexcept = default;
    Spider& operator= (const Spider& o) = default;
    Spider& operator= (Spider&& o) noexcept = default;
    ~Spider() = default;
    
    // TODO time should be a double
    void      setTime(float time);
    
    std::vector<glm::mat4> getModels(float time, Direction direction);
    std::vector<std::vector<Vertex>> getVertices(); 
    std::vector<std::vector<uint32_t>> getIndices();
    std::vector<ModelGeometry> getModelGeometries();
    
    private:
    
    ModelGeometry* _bodyModel = nullptr;
    ModelGeometry* _headModel = nullptr;
    std::vector<ModelGeometry>                   _modelGeometries;
    
    std::unordered_map<int, ElementMotion>       _headMotionMap{};
    HeadMarker                                   _headMarker;
    
    std::unordered_map<int, ElementMotion>       _bodyMotionMap{};
    HeadMarker                                   _bodyMarker;
    
    std::vector<std::vector<float>>              _legLengths{};
    std::vector<glm::vec3>                       _legAxesOfRotations{};
    
    std::unordered_map<int, LegMotion>           _legRightMotionMap{};
    std::unordered_map<int, LegMotion>           _legLeftMotionMap{};
    std::unordered_map<int, std::pair<int, int>> _subsequentStatesPerState{};
    std::vector<LegMarker>                       _legMarkers{};
    float                                        _lastTime = -1;
    Direction                                    _lastDirection = Direction::None;
    
};

#endif /* Spider_hpp */
