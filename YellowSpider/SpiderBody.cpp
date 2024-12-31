#include "SpiderBody.hpp"
#include "UtilityCircles.hpp"
#include "EuroCutDiamond.hpp"

#include <iostream>
const float  PI_F=3.14159265358979f;
//
using std::vector;

SpiderBody::SpiderBody()
{
    
    EuroCutDiamond head{0.0f, 8.0f,
                        1.6f, 10.0f,
                        3.2f, 12.0f,
                        4.8f, 12.0f,
                        6.4f, 10.0f,
                        8.0f, 8.0f};
                        
    _vertices = head.getVertices();
    _indices = head.getIndices();
}

vector<Vertex> SpiderBody::getVertices()
{
    return _vertices;
}

    std::vector<uint32_t> SpiderBody::getIndices()
{
    return _indices;
}

void SpiderBody::populateIndices(int numOfSides, int numOfRows)
{
    for(int rr=1; rr<numOfRows; ++rr)
    {
        for(int jj=0; jj<numOfSides; ++jj)
        {
            if(jj != (numOfSides-1))
            {
                _indices.push_back( (numOfSides*rr)     + (jj)  );
                _indices.push_back( (numOfSides*(rr-1)) + (jj)  );
                _indices.push_back( (numOfSides*rr)     + (jj+1));
                
                _indices.push_back( (numOfSides*rr)     + (jj+1));
                _indices.push_back( (numOfSides*(rr-1)) + (jj)  );
                _indices.push_back( (numOfSides*(rr-1)) + (jj+1));
            }
            else
            {
                _indices.push_back( (numOfSides*rr)     + (jj)  );
                _indices.push_back( (numOfSides*(rr-1)) + (jj)  );
                _indices.push_back( (numOfSides*rr)     + (0)   );
                
                _indices.push_back( (numOfSides*rr)     + (0)   );
                _indices.push_back( (numOfSides*(rr-1)) + (jj)  );
                _indices.push_back( (numOfSides*(rr-1)) + (0)   );
            }
        }
    }
    
    for(int ii=2; ii<_indices.size(); ++ii)
    {
        std::cout << _indices[ii-2] << ", " << _indices[ii-1] << ", " << _indices[ii] << std::endl;
        ++ii;
    }
}
