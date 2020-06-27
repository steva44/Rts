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

#include "RNodalLoad.h"
#include "RDomain.h"
#include "RLoadPattern.h"

#include "Load.h"
#include "NodalLoad.h"
#include "Node.h"
#include "Element.h"

RNodalLoad::RNodalLoad(QObject *parent, QString name)
    : RLoadBaseClass(parent, name)
{
    theNode = nullptr;
    theLoadPattern = nullptr;
    loadDirectionVector.resize(6);
}


RNodalLoad::~RNodalLoad()
{

}


int RNodalLoad::update()
{
    auto theOpenSeesDomain = theDomain->getTheOpenSeesDomain();

    auto nodeId = theNode->getTag();

    auto loadPatternTag = theLoadPattern->getObjectTag();

    // Remove the old NodalLoad
    auto removedObj = theOpenSeesDomain->removeNodalLoad(nodeId,loadPatternTag);

    if(removedObj == nullptr)
    {
        qDebug()<<"Error in removing the node from the OpenSees domain"<<__FUNCTION__;
    }
    else
    {
        delete removedObj;
        removedObj = nullptr;
    }

    auto res = this->createNodalLoad();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees 'NodalLoad' in "<<this->objectName();
        theOpenSeesLoad = nullptr;
        return -1;
    }


    if (theOpenSeesDomain->addNodalLoad(theOpenSeesLoad,loadPatternTag) == false)
    {
        qDebug()<< "WARNING could not add Nodal Load to OpenSees domain";
        delete theOpenSeesLoad;
        theOpenSeesLoad = nullptr;
        return -1;
    }

    return 0;
}


Load* RNodalLoad::getOpenSeesLoad()
{
    if(!theOpenSeesLoad)
        this->createNodalLoad();

    return theOpenSeesLoad;
}


double RNodalLoad::getFOGradient()
{
    return 1.0;
}


double RNodalLoad::getSOGradient()
{
    return 0.0;
}


void RNodalLoad::setParameter(QObject *value)
{	
    RParameter *theParam = qobject_cast<RParameter *>(value);

    if(!theParam)
    {
        qCritical()<<"The Object that came into the nodal load is not a parameter in "<<__FUNCTION__;
    }
    else
    {
        theLoadValueParameter = theParam;
    }
}


RLoadPattern* RNodalLoad::getTheLoadPattern()
{
    return theLoadPattern;
}


void RNodalLoad::setLoadDirectionVector(std::vector<double> value)
{
    loadDirectionVector = value;
}


std::vector<double> RNodalLoad::getLoadDirectionVector(void)
{
    return loadDirectionVector;
}


void RNodalLoad::setNode(Node* value)
{
    theNode = value;
}


Node* RNodalLoad::getNode(void)
{
    return theNode;
}


void RNodalLoad::setTheLoadPattern(RLoadPattern* value)
{
    theLoadPattern = value;
}


int RNodalLoad::createNodalLoad(void)
{

    if(theNode==nullptr)
    {
        qCritical()<<"The nodal load requires a node in "<<__FUNCTION__;
        return -1;
    }

    if(theLoadPattern==nullptr)
    {
        qCritical()<<"The nodal load requires a load pattern in "<<__FUNCTION__;
        return -1;
    }

    if(theLoadValueParameter==nullptr)
    {
        qCritical()<<"The nodal load requires a load value parameter in "<<__FUNCTION__;
        return -1;
    }

    auto nodeId = theNode->getTag();

    // NodalLoad(int tag, int node, const Vector &load, bool isLoadConstant = false);
    auto numDof = theNode->getNumberDOF();

    double loadVal = theLoadValueParameter->getCurrentValue();

    Vector theLoadValues(numDof);

    theLoadValues(0) = loadVal*loadDirectionVector[0];
    theLoadValues(1) = loadVal*loadDirectionVector[1];
    theLoadValues(2) = loadVal*loadDirectionVector[2];
    theLoadValues(3) = loadVal*loadDirectionVector[3];
    theLoadValues(4) = loadVal*loadDirectionVector[4];
    theLoadValues(5) = loadVal*loadDirectionVector[5];

    if(theLoadValues.Norm() != 0.0)
    {
        theOpenSeesLoad = new NodalLoad(nodeId, nodeId, theLoadValues);
    }
    else
    {
        qCritical()<<"Something happened in "<<__FUNCTION__<<" and the load values are zero";
        return -1;
    }


    return 0;
}


