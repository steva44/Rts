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

#include "RZeroLength.h"
#include "RNode.h"
#include "RParameter.h"
#include "RTimeHistoryRecorder.h"

#include "UniaxialMaterial.h"
#include "ID.h"

RZeroLength::RZeroLength(QObject *parent, QString name)
    : RElement(parent, name)
{
    theOpenSeesElement = nullptr;
    theNode1 = nullptr;
    theNode2 = nullptr;
    theUniaxialMaterials = nullptr;
}


RZeroLength::~RZeroLength()
{

}


int  RZeroLength::update()
{

    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    auto removedObj = theOSDomain->removeElement(objectTag);

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
        qCritical()<<"Error updating OpenSees 'ElasticTimoshenkoBeam3d' in "<<this->objectName();
        return -1;
    }

    theOSDomain->addElement(theOpenSeesElement);

    //The element recorder
    theTransientRecorder->clear();

    return 0;
}


Element* RZeroLength::getTheOpenSeesElement(void)
{
    if(!theOpenSeesElement)
        this->createZeroLengthElement();

    return theOpenSeesElement;
}


int RZeroLength::createZeroLengthElement(void)
{

    //Check to make sure the property objects are not null pointers
    if(theNode1 == nullptr || theNode2 == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs nodes";
        return -1;
    }

    if(theUniaxialMaterials == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" in "<<this->objectName()<<" needs one or more uniaxial materials";
        return -1;
    }

    if(VecXAxis.empty())
    {
        qDebug()<<"The "<<__FUNCTION__<<" in "<<this->objectName()<<" needs a vector defining the elements local x-axis";
        return -1;
    }

    if(VecYAxis.empty())
    {
        qDebug()<<"The "<<__FUNCTION__<<" in "<<this->objectName()<<" needs a vector defining the elements local y-axis";
        return -1;
    }

    if(VecXAxis.size() != 3 || VecYAxis.size() != 3)
    {
        qDebug()<<"In "<<__FUNCTION__<<" in "<<this->objectName()<<" the x and y direction vectors must be of dimension 3";
        return -1;
    }

    return this->initialize();
}


int RZeroLength::initialize(void)
{
    // The vectors for the element orientation -- this should be the same orientation as the other element it connects to.
    Vector x(3);
    x(0) = VecXAxis[0];
    x(1) = VecXAxis[1];
    x(2) = VecXAxis[2];
    Vector y(3);
    y(0) = VecYAxis[0];
    y(1) = VecYAxis[1];
    y(2) = VecYAxis[2];

    if(theMaterialDirections.empty())
    {
        qDebug()<<"The "<<__FUNCTION__<<" in "<<this->objectName()<<" needs a vector defining the directions for the materials";
        return -1;
    }

    int numMat = theMaterialDirections.size();

    auto Nd1 = theNode1->getObjectTag();
    auto Nd2 = theNode2->getObjectTag();

    //Rts is always in 3-dimensions
    int dimension = 3;

    if (numMat == 1)
    {
        int direction = theMaterialDirections[0];
        // ZeroLength(int tag,
        // int dimension,
        // int Nd1, int Nd2,
        // const Vector& x,
        // const Vector& yprime,
        // UniaxialMaterial& theMaterial,
        // int direction,
        // int doRayleighDamping = 0);
        theOpenSeesElement = new ZeroLength(objectTag,dimension,Nd1,Nd2,x,y,**theUniaxialMaterials,direction);
    }
    else
    {
        ID direction(numMat);

        for(int i =0; i<theMaterialDirections.size(); ++i)
        {
            direction[i] = theMaterialDirections[i];
        }

        // ZeroLength(int tag,
        // int dimension,
        // int Nd1, int Nd2,
        // const Vector& x,
        // const Vector& yprime,
        // int n1dMat,
        // UniaxialMaterial** theMaterial,
        // const ID& direction,
        // int doRaylieghDamping = 0);
        theOpenSeesElement = new ZeroLength(objectTag,dimension,Nd1,Nd2,x,y,numMat,theUniaxialMaterials,direction);
    }

    if(theOpenSeesElement)
        return 0;
    else
        return -1;
}


std::vector<double> RZeroLength::getVecYAxis() const
{
    return VecYAxis;
}


void RZeroLength::setVecYAxis(const std::vector<double> &value)
{
    VecYAxis = value;
}


std::vector<int> RZeroLength::getTheMaterialDirections() const
{
    return theMaterialDirections;
}


void RZeroLength::setTheMaterialDirections(const std::vector<int> &value)
{
    theMaterialDirections = value;
}


UniaxialMaterial **RZeroLength::getTheUniaxialMaterials() const
{
    return theUniaxialMaterials;
}


void RZeroLength::setTheUniaxialMaterials(UniaxialMaterial **value)
{
    theUniaxialMaterials = value;
}


void RZeroLength::setVecXAxis(const std::vector<double> &value)
{
    VecXAxis = value;
}


std::vector<double> RZeroLength::getVecXAxis() const
{
    return VecXAxis;
}


int RZeroLength::setNode(int i, RNode *theNode)
{
    if (i==0) {
        theNode1 = theNode;
        return 0;
    }
    else if (i==1) {
        theNode2 = theNode;
        return 0;
    }
    else {
        qCritical() << "The linear frame element has nodes associated with i=0 and i=1";
    }
    return -1;
}


int RZeroLength::getNumNodes()
{
    return 2;
}


RNode * RZeroLength::getNode(int i) const
{
    if (i==0) {
        return theNode1;
    }
    else if (i==1) {
        return theNode2;
    }
    else {
        return 0;
    }
}


void RZeroLength::print(void)
{
    qDebug()<<"***********************************************\n";
    qDebug()<<"For zero-length element called: "<<this->objectName()<<" with tag "<<this->getObjectTag()<<"\n";
    //    theOpenSeesElement->Print((*opserrPtr),0);

    //    theOpenSeesElement->Print((*opserrPtr),2);
    qDebug()<<"***********************************************\n";


    return;
}


int RZeroLength::getResponseInGlobalXYZ(QVector<double> *response, QString type, double xi,RParameter *theParameter, RParameter *theParameter2,RGradientType theGradientType)
{
    if (type == QString("Displacement") && response) {
        getGlobalDispResponse(xi, response);
    }
    else if (type == QString("My") && response) {
        getForceResponse(xi,response);
    }


    return 0;
}


void RZeroLength::getGlobalDispResponse(double xi, QVector<double> *response)
{
    return;
}


double RZeroLength::getForceResponse(double xi, QVector<double> *response)
{
    return 0.0;
}


double RZeroLength::getStressResponse(QString type, double xi)
{
    return 0.0;
}


double RZeroLength::getStrainResponse(QString type)
{
    return 0.0;
}



