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

#include "RBrick.h"
#include "RNode.h"
#include "RParameter.h"
#include "RCoordinateTransformation.h"
#include "RNDMaterial.h"
#include "RTimeHistoryRecorder.h"

#include "Information.h"
#include "Brick.h"
#include "NDMaterial.h"

RBrick::RBrick(QObject *parent, QString name)
    : RElement(parent, name)
{
    theOpenSeesElement = nullptr;
    theNode1 = nullptr;
    theNode2 = nullptr;
    theNode3 = nullptr;
    theNode4 = nullptr;
    theNode5 = nullptr;
    theNode6 = nullptr;
    theNode7 = nullptr;
    theNode8 = nullptr;
    theMaterial= nullptr;
}


RBrick::~RBrick()
{

}


int  RBrick::update(void)
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
        qCritical()<<"Error updating OpenSees element in "<<this->objectName();
        return -1;
    }

    theOSDomain->addElement(theOpenSeesElement);

    //The element recorder
    theTransientRecorder->clear();

    return 0;
}


Element* RBrick::getTheOpenSeesElement(void)
{
    if(!theOpenSeesElement)
        this->createBrickElement();

    return theOpenSeesElement;
}


int RBrick::createBrickElement(void)
{
    //Check to make sure the property objects are not null pointers
    if(theNode1 == nullptr || theNode2 == nullptr || theNode3 == nullptr || theNode4 == nullptr
            || theNode5 == nullptr || theNode6 == nullptr || theNode7 == nullptr || theNode8 == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs nodes";
        return -1;
    }
    if(theMaterial == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a material";
        return -1;
    }

    return this->initialize();
}


int RBrick::initialize(void)
{

    auto NDMat = theMaterial->getTheNDMaterial();
    if(theMaterial == nullptr)
    {
        qDebug()<<"Could not get the OpenSees material in "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    //    auto NDMatCpy = NDMat->getCopy();

    auto Nd1 = theNode1->getObjectTag();
    auto Nd2 = theNode2->getObjectTag();
    auto Nd3 = theNode3->getObjectTag();
    auto Nd4 = theNode4->getObjectTag();
    auto Nd5 = theNode5->getObjectTag();
    auto Nd6 = theNode6->getObjectTag();
    auto Nd7 = theNode7->getObjectTag();
    auto Nd8 = theNode8->getObjectTag();

    // Brick(int tag,
    // int node1,
    // int node2,
    // int node3,
    // int node4,
    // int node5,
    // int node6,
    // int node7,
    // int node8,
    // NDMaterial &theMaterial,
    // double b1 = 0.0, double b2 = 0.0, double b3 = 0.0);
    theOpenSeesElement = new Brick(objectTag,Nd1,Nd2,Nd3,Nd4,Nd5,Nd6,Nd7,Nd8,*NDMat);

    if(theOpenSeesElement)
        return 0;
    else
        return -1;
}


int RBrick::setNode(int i, RNode *theNode)
{
    if (i==0) {
        theNode1 = theNode;
        return 0;
    }
    else if (i==1) {
        theNode2 = theNode;
        return 0;
    }
    else if (i==2) {
        theNode3 = theNode;
        return 0;
    }
    else if (i==3) {
        theNode4 = theNode;
        return 0;
    }
    else if (i==4) {
        theNode5 = theNode;
        return 0;
    }
    else if (i==5) {
        theNode6 = theNode;
        return 0;
    }
    else if (i==6) {
        theNode7 = theNode;
        return 0;
    }
    else if (i==7) {
        theNode8 = theNode;
        return 0;
    }
    else {
        qCritical() << "The linear frame element has nodes associated with i=0 and i=1";
    }
    return -1;
}


int RBrick::getNumNodes()
{
    return 8;
}


RNode * RBrick::getNode(int i) const
{
    if (i==0) {
        return theNode1;
    }
    else if (i==1) {
        return theNode2;
    }
    else if (i==2) {
        return theNode3;
    }
    else if (i==3) {
        return theNode4;
    }
    else if (i==4) {
        return theNode5;
    }
    else if (i==5) {
        return theNode6;
    }
    else if (i==6) {
        return theNode7;
    }
    else if (i==7) {
        return theNode8;
    }
    else {
        return 0;
    }
}


void RBrick::print(void)
{
    qDebug()<<"***********************************************\n";
    qDebug()<<"For beam called: "<<this->objectName()<<" with tag "<<this->getObjectTag()<<"\n";
    theOpenSeesElement->Print((*opserrPtr),0);

    theOpenSeesElement->Print((*opserrPtr),2);
    qDebug()<<"***********************************************\n";


    return;
}


int RBrick::getResponseInGlobalXYZ(QVector<double> *response, QString type, double xi,RParameter *theParameter, RParameter *theParameter2,RGradientType theGradientType)
{
    if (type == QString("Displacement") && response) {
        getGlobalDispResponse(xi, response);
    }
    else if (type == QString("My") && response) {
        getForceResponse(xi,response);
    }

    return 0;
}


void RBrick::getGlobalDispResponse(double xi, QVector<double> *response)
{

}


double RBrick::getForceResponse(double xi, QVector<double> *response)
{
    return 0.0;
}


double RBrick::getStressResponse(QString type, double xi)
{
    return 0.0;
}


double RBrick::getStrainResponse(QString type)
{
    return 0.0;
}


RNDMaterial *RBrick::getTheMaterial() const
{
    return theMaterial;
}


void RBrick::setTheMaterial(RNDMaterial *value)
{
    theMaterial = value;
}





