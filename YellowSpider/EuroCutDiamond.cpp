#include "EuroCutDiamond.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

EuroCutDiamond::EuroCutDiamond(
    float y0, float r0,
    float y1, float r1,
    float y2, float r2,
    float y3, float r3,
    float y4, float r4,
    float y5, float r5)
{
    _radii.reserve(_numOfLevels);
    _heights.reserve(_numOfLevels);
    
    _radii.push_back(r0);
    _radii.push_back(r1);
    _radii.push_back(r2);
    _radii.push_back(r3);
    _radii.push_back(r4);
    _radii.push_back(r5);
    
    _heights.push_back(y0);
    _heights.push_back(y1);
    _heights.push_back(y2);
    _heights.push_back(y3);
    _heights.push_back(y4);
    _heights.push_back(y5);
    
    glm::vec3 rotationAxis{0.0f, 1.0f, 0.0f};
    glm::mat4 transformMatrix{1.0f};
    for(int level=0; level<_numOfLevels; ++level)
    {
        glm::vec3 point = {0.0f, _heights[level], -_radii[level]};
        
        if( (level == 1) || (level==4))
        {
            for(int ii=0; ii < _numOfSides*2; ++ii)
            {
                _vertices.push_back( Vertex{ glm::rotate( transformMatrix, glm::radians(ii * 360.0f/(_numOfSides*2)), rotationAxis ) * glm::vec4(point, 1.0), _colors[level]});
            }
        }
        else
        {
            for(int ii=0; ii < _numOfSides; ++ii)
            {
                _vertices.push_back( Vertex{ glm::rotate( transformMatrix, glm::radians(360.0f/(_numOfSides)) * ii, rotationAxis ) * glm::vec4(point, 1.0), _colors[level]});
            }
        }
    }
    
    populateIndices();
    //
}

void EuroCutDiamond::populateIndices()
{
    for(int level=1; level<_numOfLevels; ++level)
    {
        if( (level==1) || (level== 4))
        {
            int startIndexLevel_0 = 0;
            int startIndexLevel_1 = _numOfSides;
            // startIndexLevel_3 =  (level_0 nodes) + (level_1 nodes) + (level_2);
            int startIndexLevel_3 = (_numOfSides) + (_numOfSides * 2) + _numOfSides;
            int startIndexLevel_4 = startIndexLevel_3 + _numOfSides;
            //
            int topLevelStartNode = (level==4) ? (startIndexLevel_4) : (startIndexLevel_1);
            int botLevelStartNode = (level==4) ? (startIndexLevel_3) : (startIndexLevel_0);
            int numOfTopSides = _numOfSides*2;
            for (int ii=0; ii<numOfTopSides; ii+=2)
            {
                if (ii != (numOfTopSides - 2))
                {
                    _indices.push_back( topLevelStartNode + (ii)         );
                    _indices.push_back( botLevelStartNode + (ii/2)       );
                    _indices.push_back( topLevelStartNode + (ii + 1)     );
            
                    _indices.push_back( topLevelStartNode + (ii + 1)     );
                    _indices.push_back( botLevelStartNode + (ii/2)       );
                    _indices.push_back( botLevelStartNode + ((ii/2) + 1) );
                    
                    _indices.push_back( topLevelStartNode + (ii + 1)     );
                    _indices.push_back( botLevelStartNode + ((ii/2) + 1) );
                    _indices.push_back( topLevelStartNode + (ii + 2)     );
                }
                else
                {
                    _indices.push_back( topLevelStartNode + (ii)         );
                    _indices.push_back( botLevelStartNode + (ii/2)       );
                    _indices.push_back( topLevelStartNode + (ii + 1)     );
            
                    _indices.push_back( topLevelStartNode + (ii + 1)     );
                    _indices.push_back( botLevelStartNode + (ii/2)       );
                    _indices.push_back( botLevelStartNode + (0)          );
                    
                    _indices.push_back( topLevelStartNode + (ii + 1)     );
                    _indices.push_back( botLevelStartNode + (0)          );
                    _indices.push_back( topLevelStartNode + (0)          );
                }
            }
        }
        else if ( (level==2) || (level ==5) )
        {
            //  startIndeLevel_1 = level_0 nodes;
            int startIndexLevel_1 = _numOfSides;
            int startIndexLevel_2 = startIndexLevel_1 + (2*_numOfSides);
            
            // startIndexLevel_4 = (level_0 nodes) + (level_1 nodes) + (level 2 + level3);
            int startIndexLevel_4 = _numOfSides + (2*_numOfSides) + (2 * _numOfSides);
            int startIndexLevel_5 = startIndexLevel_4 + (2*_numOfSides);
            
            int topLevelStartNode = (level==2) ? (startIndexLevel_2) : (startIndexLevel_5);
            int botLevelStartNode = (level==2) ? (startIndexLevel_1) : (startIndexLevel_4);
            
            for (int ii=0; ii<_numOfSides; ++ii)
            {
                if (ii != (_numOfSides - 1))
                {
                    _indices.push_back( topLevelStartNode + ((ii)      ) );
                    _indices.push_back( botLevelStartNode + ((ii*2)    ) );
                    _indices.push_back( botLevelStartNode + ((ii*2) + 1) );
            
                    _indices.push_back( topLevelStartNode + ((ii)      ) );
                    _indices.push_back( botLevelStartNode + ((ii*2) + 1) );
                    _indices.push_back( topLevelStartNode + ((ii)   + 1) );
                    
                    _indices.push_back( topLevelStartNode + ((ii)   + 1) );
                    _indices.push_back( botLevelStartNode + ((ii*2) + 1) );
                    _indices.push_back( botLevelStartNode + ((ii*2) + 2) );
                }
                else
                {
                    _indices.push_back( topLevelStartNode + ((ii)      ) );
                    _indices.push_back( botLevelStartNode + ((ii*2)    ) );
                    _indices.push_back( botLevelStartNode + ((ii*2) + 1) );
            
                    _indices.push_back( topLevelStartNode + ((ii)      ) );
                    _indices.push_back( botLevelStartNode + ((ii*2) + 1) );
                    _indices.push_back( topLevelStartNode + (0)          );
                    
                    _indices.push_back( topLevelStartNode + (0)          );
                    _indices.push_back( botLevelStartNode + ((ii*2) + 1) );
                    _indices.push_back( botLevelStartNode + (0)          );
                }
            }
        }
        else
        {
            //  startIndexLevel_2 = (level_0 nodes) + (level_1 nodes);
            int startIndexLevel_2 = _numOfSides + (2*_numOfSides);
            int startIndexLevel_3 = startIndexLevel_2 + _numOfSides;
            
            int topLevelStartNode = startIndexLevel_3;
            int botLevelStartNode = startIndexLevel_2;

            for (int ii=0; ii<_numOfSides; ++ii)
            {
                if (ii != (_numOfSides - 1))
                {
                    _indices.push_back( topLevelStartNode + (ii    ) );
                    _indices.push_back( botLevelStartNode + (ii    ) );
                    _indices.push_back( topLevelStartNode + (ii + 1) );
            
                    _indices.push_back( topLevelStartNode + (ii + 1) );
                    _indices.push_back( botLevelStartNode + (ii    ) );
                    _indices.push_back( botLevelStartNode + (ii + 1) );
                }
                else
                {
                    _indices.push_back( topLevelStartNode + (ii    ) );
                    _indices.push_back( botLevelStartNode + (ii    ) );
                    _indices.push_back( topLevelStartNode + (0)      );
            
                    _indices.push_back( topLevelStartNode + (0)      );
                    _indices.push_back( botLevelStartNode + (ii    ) );
                    _indices.push_back( botLevelStartNode + (0)      );
                }
            }
        }
    }
}

std::vector<Vertex> EuroCutDiamond::getVertices()
{
    return _vertices;
}

std::vector<uint32_t>  EuroCutDiamond::getIndices()
{
    return _indices;
}
