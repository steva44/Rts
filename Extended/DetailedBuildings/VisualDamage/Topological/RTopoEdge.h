#ifndef RTOPOEDGE_H
#define RTOPOEDGE_H
/*********************************************************************
*                                                                    *
* This file is posted by Dr. Stevan Gavrilovic (steva44@hotmail.com) *
* as a result of work performed in the research group of Professor   *
* Terje Haukaas (terje@civil.ubc.ca) at the University of British    *
* Columbia in Vancouver. The code is part of the computer program    *
* Rts, which is an extension of the computer program Rt developed by *
* Dr. Mojtaba Mahsuli (mahsuli@sharif.edu) in the same group.        *
*                                                                    *
* The content of this file is the product of thesis-related work     *
* performed at the University of British Columbia (UBC). Therefore,  *
* the Rights and Ownership are held by UBC.                          *
*                                                                    *
* Please be cautious when using this code. It is provided “as is”    *
* without warranty of any kind, express or implied.                  *
*                                                                    *
* Contributors to this file:                                         *
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

// This class represents an edge object in a topological mesh - an edge is defined by two vertices, one at each end

#include "RTopoVertex.h"

#include <memory>
#include <vector>

class RTopoEdge
{
public:
    RTopoEdge(std::shared_ptr<RTopoVertex> vert1, std::shared_ptr<RTopoVertex> vert2);
    ~RTopoEdge();

    // Comparison operator
    inline bool operator==(const RTopoEdge& edge)
    {
        if(edge.vertex1 == vertex1 && edge.vertex2 == vertex2)
            return true;
        else
            return false;
    }    

    // Pointers to the vertices of the edges
    std::shared_ptr<RTopoVertex>& getVertex1(void){return vertex1;}
    std::shared_ptr<RTopoVertex>& getVertex2(void){return vertex2;}

protected:

    // Vertices of the edges
    std::shared_ptr<RTopoVertex> vertex1;
    std::shared_ptr<RTopoVertex> vertex2;

};

#endif // RTOPOEDGE_H
