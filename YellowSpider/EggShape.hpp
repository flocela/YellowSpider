#include <glm/glm.hpp>
#include <vector>
#include "Utilities.hpp"

#ifndef EggShape_hpp
#define EggShape_hpp

const float  PI_F=3.14159265358979f;

class EggShape
{

    public:

    // Creates an egg whose small end is in the positive y direction.
    // It's bottom larger end is centered at (0, 0, 0).
    EggShape(
        uint32_t numOfSectionsAboutY,
        float    angleAboutZ, // medium radius, other radii will be sectioned accordingly
        float mediumRadius
    );
    EggShape()                                  = delete;
    EggShape(const EggShape& o)                 = default;
    EggShape(EggShape&& o) noexcept             = default;
    EggShape& operator= (const EggShape& o)     = default;
    EggShape& operator= (EggShape&& o) noexcept = default;
    ~EggShape()                                 = default;
    
    std::vector<Vertex>    getVertices();
    std::vector<uint32_t>  getIndices();
    std::vector<float>     getRotations();
    glm::vec3              getPos(int index);
    float                  getRotation(int index);
    int                    getNumOfRotations();
    std::vector<glm::vec3> getEggOutline();
    float                  getEggCircumferenceAboutZ();
    float                  getRotationGivenCircumferenceDistance(float dist);
    
    private:
    
    // Easy to read constants
    const float _zero_rad          =  0.0f  * PI_F / 180.0f;
    const float _fortyFive_rad     = 45.0f  * PI_F / 180.0f;
    const float _ninety_rad        = 90.0f  * PI_F / 180.0f;
    const float _oneThirtyFive_rad = 135.0f * PI_F / 180.0f;
    const float _oneEighty_rad     = 180.0f * PI_F / 180.0f;
    const float _twoSeventy_rad    = 270.0f * PI_F / 180.0f;
    const float _threeSixty_rad    = 360.0f * PI_F / 180.0f;
    
    // radii
    float _rMedium  = 0.0f;
    float _rLarge   = 0.0f;
    float _rSmall   = 0.0f;
    
    float _halfWeight = 0.0f;
    
    // Total number of sections about z is _numOfSectionsAboutZ.
    // medCount is number of sections using medium radius.
    // lgCount is number of sections using only large radius.
    // smCount is number of sections using only small radius.
    // numOfSectionsAboutZ = medCount + lgCount + smCount.
    // numOfSectionsAboutZ = medCount +
    //                       1/2 * lgRadius/mdRadius * medCount +
    //                       1/2 * smRadius/mdRadius * medCount.
    // There is 1/2 the number of degrees that the lgRadius traverses vs the mdRadius.
    // There is 1/2 the number of degrees that the smRadius traverses vs the mdRadius.
    // That is, the mdRadius traverses [270, 360)deg, lgRadius traverses [0, 45) deg,
    // smRadius traverses [45, 90).
    // The distance the lgRadius traverses is 1/2 * (lgRadius/mdRadius) * (distance mdRadius traverses).
    uint32_t _numOfSectionsAboutY    = 0.0f;
    float    _angleAboutZ            = 1.0f * PI_F / 180.0f;
    
    std::vector<glm::vec3> _eggOutlineAboutZ{};
    std::vector<Vertex>    _vertices{};
    std::vector<uint32_t>  _indices{};
    
    // Think of the egg as being described by a form of polar coordinates.
    // The bottom circular part of the egg is a radius rotating about the center of the circle. It
    // creates an arc range [270, 360). The azimuths are the digital representation of those angles.
    // The azimuths continue, but this time rotating about the center of the larger circle
    // in the range [360, 45).
    // Then finally from [45, 90) using the smallest radius.
    // TODO create this in the initializer list to the correct size using _azimuths{correct size}.
    std::vector<float>     _referenceAnglesAboutZ{};
    
    // Distances around the egg starting at 270 deg (which would be distance zero), and going counter clockwise.
    std::vector<float>     _circumferenceTraveledAboutZ{};
    float                  _circumference = 0.0f;
    
    std::vector<float>     _forwardWeights{};
    std::vector<float>     _backwardWeights{};
    float                  _startWeight = 34.97780;
    
    std::vector<glm::vec3> _colors{{1.0f, 0.0f, 0.0f},
                                  {0.0f, 1.0f, 0.0f},
                                  {0.0f, 0.0f, 1.0f},
                                  {1.0f, 0.0f, 0.0f},
                                  {0.0f, 1.0f, 0.0f},
                                  {0.0f, 0.0f, 1.0f}
    };
    
    void populateVerticesAboutZAxis();
        
    void populateVerticesAboutZAxis( float startingPolarAngle_rad, float endingPolarAngle_rad);
        
    void populateVerticesAboutYAxis(glm::vec3 point, glm::vec3 color);
    
    void populateForwardAndBackwardsWeights();
    
    float getCorrespondingRadius(float polarAngle);
    glm::vec2 getCorrespondingCenter(float polarAngle);
    
};

#endif /* EggShape_hpp */

