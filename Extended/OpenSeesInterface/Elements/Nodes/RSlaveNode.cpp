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

#include "RSlaveNode.h"
#include <utility>

RSlaveNode::RSlaveNode(QObject *parent, QString &name)
    : RNode(parent, name)
{
    setObjectName(name);
}

RSlaveNode::~RSlaveNode()
{}

//function to swap from the node being replaced to the slave node
void RSlaveNode::intializeNode(RNode* theNode)
{
    std::swap(this->theXCoordinateParameter, theNode->theXCoordinateParameter);
    std::swap(this->xCoordinateLambda,theNode->xCoordinateLambda);

    std::swap(this->theYCoordinateParameter,theNode->theYCoordinateParameter);
    std::swap(this->yCoordinateLambda,theNode->yCoordinateLambda);

    std::swap(this->theZCoordinateParameter,theNode->theZCoordinateParameter);
    std::swap(this->zCoordinateLambda,theNode->zCoordinateLambda);

    std::swap(this->Crd,theNode->Crd);
    std::swap(this->theDisplacementVector,theNode->theDisplacementVector);
    std::swap(this->theBoundaryConditionVector,theNode->theBoundaryConditionVector);
    std::swap(this->theNodeNumber,theNode->theNodeNumber);
    std::swap(this->theAssociatedPoint,theNode->theAssociatedPoint);
}
