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

#include "RNode.h"
#include "RDomain.h"
#include "RPoint.h"

#include "Node.h"

RNode::RNode(QObject *parent, QString &name)
    : RObject(parent, name), Node(objectTag, numDOFsPerNode, 0.0, 0.0, 0.0)
{
    theXCoordinateParameter = nullptr;
    theYCoordinateParameter = nullptr;
    theZCoordinateParameter = nullptr;

    xCoordinateLambda = nullptr;
    yCoordinateLambda = nullptr;
    zCoordinateLambda = nullptr;

    //If this is a master node to some other node, this is will not be empty
    theSlaveNode = nullptr;
    theAssociatedPoint = nullptr;

    theDisplacementVector = std::make_unique<QVector<double>>(numDOFsPerNode);
    theBoundaryConditionVector = std::make_unique<QVector<int>>(numDOFsPerNode);

    theNodeNumber = -1;
}


RNode::~RNode()
{
    //std::cout<<"deleting node: "<<this->getObjectTag()<<std::endl;

    auto OpenSeesDomain = getTheOpenSeesDomain();

    if(OpenSeesDomain != nullptr)
        OpenSeesDomain->removeNode(this->getObjectTag());

}


int RNode::generateOpenSeesInput(std::string& object, int flag)
{

    std::ostringstream out;

    out.precision(4);

    auto tag = std::to_string(this->objectTag);

    auto xAsString = std::to_string(this->getXCoordinateValue());
    auto yAsString = std::to_string(this->getYCoordinateValue());
    auto zAsString = std::to_string(this->getZCoordinateValue());

    auto xBc =  theBoundaryConditionVector->at(0);
    auto yBc =  theBoundaryConditionVector->at(1);
    auto zBc =  theBoundaryConditionVector->at(2);
    auto rotxBc = theBoundaryConditionVector->at(3);
    auto rotyBc = theBoundaryConditionVector->at(4);
    auto rotzBc = theBoundaryConditionVector->at(5);

    auto xBcStr = std::to_string(theBoundaryConditionVector->at(0));
    auto yBcStr = std::to_string(theBoundaryConditionVector->at(1));
    auto zBcStr = std::to_string(theBoundaryConditionVector->at(2));
    auto rotxBcStr = std::to_string(theBoundaryConditionVector->at(3));
    auto rotyBcStr = std::to_string(theBoundaryConditionVector->at(4));
    auto rotzBcStr = std::to_string(theBoundaryConditionVector->at(5));

    // Dont pollute the file with many free dofs (they are initialized as free)
    // Only specify boundary conditions when some dof is fixed
    // Fixed = 1 and Free = 0
    auto sum = xBc + yBc + zBc + rotxBc + rotyBc + rotzBc;

    //First do the coordinates
    //Then do the boundary conditions
    if(flag == 1) //tcl file
    {
        out<<"node "<<tag<<" "<<xAsString<<" "<<yAsString<<" "<<zAsString<<std::endl;

        if(sum > 0)
        {
            out<<"fix "<<tag<<" "<<xBcStr<<" "<<yBcStr<<" "<<zBcStr<<" "<<rotxBcStr<<" "<<rotyBcStr<<" "<<rotzBcStr<<std::endl;
        }
    }
    else if(flag == 2) //Python
    {
        out<<"node("<<tag<<", "<<xAsString<<", "<<yAsString<<", "<<zAsString<<")"<<std::endl;

        if(sum > 0)
        {
            out<<"fix("<<tag<<", "<<xBcStr<<", "<<yBcStr<<", "<<zBcStr<<", "<<rotxBcStr<<", "<<rotyBcStr<<", "<<rotzBcStr<<std::endl;
        }
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}


void RNode::setNumDof(int num)
{
    this->numberDOF = num;
}


void RNode::setXCoordinate(double value)
{
    (*Crd)[0] = value;
}


void RNode::setXCoordinate(std::function<double(void)>& fxn)
{
    xCoordinateLambda = std::make_unique<std::function<double(void)>>(fxn);
    (*Crd)[0] = (*xCoordinateLambda)();
}


void RNode::setXCoordinate(RParameter* value)
{
    theXCoordinateParameter = value;
    (*Crd)[0] = value->getCurrentValue();
}


RParameter * RNode::getXCoordinateParameter() const
{    
    return theXCoordinateParameter;
}


double RNode::getXCoordinateValue()
{
    return this->getCrds()[0];
}


void RNode::setYCoordinate(double value)
{
    (*Crd)[1] = value;
}


void RNode::setYCoordinate(std::function<double(void)>& fxn)
{
    yCoordinateLambda = std::make_unique<std::function<double(void)>>(fxn);
    (*Crd)[1] = (*yCoordinateLambda)();
}


void RNode::setYCoordinate(RParameter* value)
{
    theYCoordinateParameter = value;
    (*Crd)[1] = value->getCurrentValue();
}


RParameter * RNode::getYCoordinateParameter() const
{
    return theYCoordinateParameter;
}


double RNode::getYCoordinateValue()
{   
    return this->getCrds()[1];
}


void RNode::setZCoordinate(double value)
{
    (*Crd)[2] = value;
}


void RNode::setZCoordinate(std::function<double(void)>& fxn)
{
    zCoordinateLambda = std::make_unique<std::function<double(void)>>(fxn);
    (*Crd)[2] = (*zCoordinateLambda)();
}


void RNode::setZCoordinate(RParameter* value)
{
    theZCoordinateParameter = value;
    (*Crd)[2] = value->getCurrentValue();
}


RParameter * RNode::getZCoordinateParameter() const
{
    return theZCoordinateParameter;
}


double RNode::getZCoordinateValue()
{
    return this->getCrds()[2];
}


double RNode::getXDisplacement()
{
    return this->getDisp()[0];
}


double RNode::getYDisplacement()
{
    return this->getDisp()[1];
}


double RNode::getZDisplacement()
{
    return this->getDisp()[2];
}


double RNode::getXRotation()
{
    return this->getDisp()[3];
}


double RNode::getYRotation()
{
    return this->getDisp()[4];
}


double RNode::getZRotation()
{
    return this->getDisp()[5];
}


double RNode::getWarping()
{
    return this->getDisp()[6];
}


QVector<double>* RNode::getDisplacementVector()
{
    return theDisplacementVector.get();
}


int RNode::setBoundaryConditions(int Xdisp, int Ydisp, int Zdisp, int Xrot, int Yrot, int Zrot)
{
    // 1=fixed
    // 0=free

    if (!theBoundaryConditionVector) {
        theBoundaryConditionVector = std::make_unique<QVector<int>>(numberDOF);
    }

    (*theBoundaryConditionVector)[0]=Xdisp;
    (*theBoundaryConditionVector)[1]=Ydisp;
    (*theBoundaryConditionVector)[2]=Zdisp;
    (*theBoundaryConditionVector)[3]=Xrot;
    (*theBoundaryConditionVector)[4]=Yrot;
    (*theBoundaryConditionVector)[5]=Zrot;

    return 0;
}


QVector<int> * RNode::getBoundaryConditionVector()
{
    return theBoundaryConditionVector.get();
}


int RNode::setAssociatedPoint(RPoint *passedPoint)
{
    theAssociatedPoint = passedPoint;

    return 0;
}


RPoint *RNode::getAssociatedPoint()
{
    return theAssociatedPoint;
}


void RNode::updateNode(void)
{
    auto xCrd = this->getXCoordinateValue();
    auto yCrd = this->getYCoordinateValue();
    auto zCrd = this->getZCoordinateValue();

    if(theXCoordinateParameter)
        xCrd = theXCoordinateParameter->getCurrentValue();
    else if(xCoordinateLambda)
        xCrd = (*xCoordinateLambda)();

    if(theYCoordinateParameter)
        yCrd = theYCoordinateParameter->getCurrentValue();
    else if(yCoordinateLambda)
        yCrd = (*yCoordinateLambda)();

    if(theZCoordinateParameter)
        zCrd = theZCoordinateParameter->getCurrentValue();
    else if(zCoordinateLambda)
        zCrd = (*zCoordinateLambda)();

    this->setXCoordinate(xCrd);
    this->setYCoordinate(yCrd);
    this->setZCoordinate(zCrd);

}


//Need to override the default RObject function so that the OpenSees tag can be updated after the Rts object is instantiated
void RNode::setObjectTag(int value)
{
    objectTag = value;
    this->setTag(objectTag);

#ifdef OPENSEES
    //Set the newly created node to the OpenSees domain
    auto OpenSeesDomain = getTheOpenSeesDomain();

    OpenSeesDomain->addNode(this);

#endif

}




