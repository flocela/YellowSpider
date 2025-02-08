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
        uint32_t numOfSectionsAboutZ, // medium radius, other radii will be sectioned accordingly
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
    
    private:
    
    // Easy to read constants
    const float _zero_rad          =  0.0f  * PI_F / 180.0f;
    const float _fortyFive_rad     = 45.0f  * PI_F / 180.0f;
    const float _ninety_rad        = 90.0f  * PI_F / 180.0f;
    const float _oneThirtyFive_rad = 135.0f * PI_F / 180.0f;
    const float _twoSeventy_rad    = 270.0f * PI_F / 180.0f;
    const float _threeSixty_rad    = 360.0f * PI_F / 180.0f;
    
    // radii
    float _rMedium          = 0.0f;
    float _rLarge           = 0.0f;
    float _rSmall           = 0.0f;
    
    // Polar angle ranges used for each radii. Ranges are (,]. Exclusive and inclusive.
    std::pair<float, float> angleRangeMediumRadius = {_twoSeventy_rad, _threeSixty_rad};
    std::pair<float, float> angleRangeLargeRadius  = {_zero_rad, _fortyFive_rad};
    std::pair<float, float> angleRangeSmallRadius  = {_fortyFive_rad, _ninety_rad};
    
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
    uint32_t _numOfSectionsAboutZ           = 0.0f;
    uint32_t _numOfSectionsAboutY           = 0.0f;
    uint32_t _numOfSectionsUsingMRadius     = 0.0f;
    uint32_t _numOfSectionsUsingLRadius     = 0.0f;
    uint32_t _numOfSectionsUsingSRadius     = 0.0f;
    float    _polarAngleDeltaForMRadius_rad = 0.0f;
    float    _polarAngleDeltaForLRadius_rad = 0.0f;
    float    _polarAngleDeltaForSRadius_rad = 0.0f;
    
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
    
    std::vector<glm::vec3> _colors{{1.0f, 0.0f, 0.0f},
                                  {0.0f, 1.0f, 0.0f},
                                  {0.0f, 0.0f, 1.0f},
                                  {1.0f, 0.0f, 0.0f},
                                  {0.0f, 1.0f, 0.0f},
                                  {0.0f, 0.0f, 1.0f}
    };
    
    void populateVerticesAboutZAxis();
    
    void populateVerticesAboutZAxis(
        float centerX,
        float centerY,
        float startingPolarAngle_rad,
        float endingPolarAngle_rad,
        float polarAngleDelta_rad,
        float numOfSectionsAboutY,
        float radius);
        
    void populateVerticesAboutYAxis(glm::vec3 point, glm::vec3 color);
    
};

#endif /* EggShape_hpp */

