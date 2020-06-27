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

#include "RRigidBeam.h"
#include "RNode.h"
#include "RParameter.h"
#include "RCoordinateTransformation.h"

#include "Information.h"
#include "CrdTransf.h"
#include "MP_Constraint.h"

RRigidBeam::RRigidBeam(QObject *parent, QString name)
    : RObject(parent, name)
{
    theOpenSeesDomain = theDomain->getTheOpenSeesDomain();

    nodeR = -1;
    nodeC = -1;
}


RRigidBeam::~RRigidBeam()
{

}


int  RRigidBeam::update(void)
{
    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    auto removedObj = theOSDomain->removeMP_Constraint(objectTag);

    if(removedObj == nullptr)
    {
        qDebug()<<"Error in removing the element from the OpenSees domain"<<__FUNCTION__;
    }
    else
    {
        delete removedObj;

        removedObj = nullptr;
    }

    auto res = this->initialize();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees 'RigidBeam' in "<<this->objectName();
        return -1;
    }

    return 0;
}


int RRigidBeam::createRigidBeamElement(void)
{
    //Check to make sure the property objects are not null pointers
    if(nodeR == -1 || nodeC == -1)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs nodes";
        return -1;
    }

    if(theOpenSeesDomain == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs an the OpenSees domain";
        return -1;
    }

    auto res = this->initialize();

    return res;
}


int RRigidBeam::getNodeC() const
{
    return nodeC;
}


void RRigidBeam::setNodeC(int value)
{
    nodeC = value;
}


int RRigidBeam::getNodeR() const
{
    return nodeR;
}


void RRigidBeam::setNodeR(int value)
{
    nodeR = value;
}


int RRigidBeam::initialize(void)
{
    // This creates the rigid beam even though is self-destructs afterward
    // RigidBeam(Domain &theDomain, int nodeR, int nodeC);
    RigidBeam theLink(*theOpenSeesDomain, nodeR, nodeC);

    return 0;
}


void RRigidBeam::print(void)
{
    qDebug()<<"***********************************************\n";
    qDebug()<<"For rigid beam called: "<<this->objectName()<<" with tag "<<this->getObjectTag()<<"\n";
    qDebug()<<"Node 1: "<<nodeR<<" and Node2: "<<nodeC<<"\n";

    qDebug()<<"***********************************************\n";

    return;
}


int RRigidBeam::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(4);

    auto tag = std::to_string(this->objectTag);

    auto masterNode = std::to_string(nodeR);
    auto slaveNode = std::to_string(nodeC);

    if(flag == 1) //tcl file
    {
        // rigidLink beam 2 3; # connect node 3 to node 2 via a rigid link-beam.
        out<<"rigidLink beam "<<masterNode<<" "<<slaveNode<<std::endl;
    }
    else if(flag == 2) //Python
    {
        // rigidLink(type, rNodeTag, cNodeTag)
        out<<"rigidLink("<<"beam, "<<masterNode<<", "<<slaveNode<<")"<<std::endl;
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}
