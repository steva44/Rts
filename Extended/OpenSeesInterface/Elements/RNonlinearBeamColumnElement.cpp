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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RNonlinearBeamColumnElement.h"
#include "RCoordinateTransformation.h"

#include "CrdTransf.h"
#include "LinearCrdTransf3d.h"
#include "BeamIntegration.h"
#include "SectionForceDeformation.h"
#include "LobattoBeamIntegration.h"
#include "LegendreBeamIntegration.h"
#include "RadauBeamIntegration.h"
#include "NewtonCotesBeamIntegration.h"
#include "TrapezoidalBeamIntegration.h"
#include "CompositeSimpsonBeamIntegration.h"
#include "PDeltaCrdTransf3d.h"


RNonlinearBeamColumnElement::RNonlinearBeamColumnElement(QObject *parent, QString &name)
    : RElement(parent, name)
{  
    numSections = -1;

    theNode1 = nullptr;
    theNode2 = nullptr;
    theRhoParameter = nullptr;
    theOrientationParameter = nullptr;
    theCrdTransformation = nullptr;
    theBeamIntegration = nullptr;
    theSectionForceDeformation = nullptr;

    theCrdTransformationType = "LinearCrdTransf3d";
    theIntegrationType = "Lobatto";

    VecxzPlane.resize(3);
}


RNonlinearBeamColumnElement::~RNonlinearBeamColumnElement()
{

}


void RNonlinearBeamColumnElement::createTheCrdTransformation(void)
{

    RCoordinateTransformation theCoordinateTransformation(theDomain);

    auto res = theCoordinateTransformation.getVectorInXZPlane(theNode1,theNode2,&VecxzPlane, theOrientationParameter->getCurrentValue());

    if(res != 0)
        return;

    Vector vecxzPlane(3);

    //Initialize the OpenSees vector value
    vecxzPlane[0]=VecxzPlane[0];
    vecxzPlane[1]=VecxzPlane[1];
    vecxzPlane[2]=VecxzPlane[2];

    if(theCrdTransformationType == "LinearCrdTransf3d")
        theCrdTransformation = std::make_unique<LinearCrdTransf3d>(objectTag,vecxzPlane);
    if(theCrdTransformationType == "PDeltaCrdTransf3d")
        theCrdTransformation = std::make_unique<PDeltaCrdTransf3d>(objectTag,vecxzPlane);
    else
        qDebug()<<"Could not understand the coordinate transformation type in "<<__FUNCTION__;

    return;
}


void RNonlinearBeamColumnElement::createTheBeamIntegration(void)
{
    if (theIntegrationType =="Lobatto")
      theBeamIntegration =  std::make_unique<LobattoBeamIntegration>();
    else if (theIntegrationType =="Legendre")
      theBeamIntegration =  std::make_unique<LegendreBeamIntegration>();
    else if (theIntegrationType =="Radau")
      theBeamIntegration =  std::make_unique<RadauBeamIntegration>();
    else if (theIntegrationType =="NewtonCotes")
      theBeamIntegration =  std::make_unique<NewtonCotesBeamIntegration>();
    else if (theIntegrationType =="Trapezoidal")
      theBeamIntegration =  std::make_unique<TrapezoidalBeamIntegration>();
    else if (theIntegrationType =="CompositeSimpson")
      theBeamIntegration =  std::make_unique<CompositeSimpsonBeamIntegration>();
    else
        qDebug()<<"The type of beam integration is not supported";

}


std::vector<double> RNonlinearBeamColumnElement::getVecxzPlane() const
{
    return VecxzPlane;
}


QPointer<RParameter> RNonlinearBeamColumnElement::getTheOrientation() const
{
    return theOrientationParameter;
}


int RNonlinearBeamColumnElement::setNode(int i, RNode *theNode)
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


int RNonlinearBeamColumnElement::getNumNodes()
{
    return 2;
}


RNode * RNonlinearBeamColumnElement::getNode(int i) const
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


QString RNonlinearBeamColumnElement::getTheCrdTransformationType() const
{
    return theCrdTransformationType;
}


void RNonlinearBeamColumnElement::setCrdTransformationType(const QString &value)
{
    theCrdTransformationType = value;
}


std::shared_ptr<std::vector<RSectionForceDeformation*>> RNonlinearBeamColumnElement::getTheSectionForceDeformation() const
{
    return theSectionForceDeformation;
}


void RNonlinearBeamColumnElement::setTheSectionForceDeformation(std::shared_ptr<std::vector<RSectionForceDeformation*>> value)
{
    theSectionForceDeformation = value;
}


QString RNonlinearBeamColumnElement::getIntegrationType() const
{
    return theIntegrationType;
}


void RNonlinearBeamColumnElement::setIntegrationType(const QString &value)
{
    theIntegrationType = value;
}


double RNonlinearBeamColumnElement::getLength(void) const
{
    auto xcoord1 = theNode1->getXCoordinateValue();
    auto xcoord2 = theNode2->getXCoordinateValue();
    auto ycoord1 = theNode1->getYCoordinateValue();
    auto ycoord2 = theNode2->getYCoordinateValue();
    auto zcoord1 = theNode1->getZCoordinateValue();
    auto zcoord2 = theNode2->getZCoordinateValue();

    auto dx = xcoord2-xcoord1;
    auto dy = ycoord2-ycoord1;
    auto dz = zcoord2-zcoord1;

    return sqrt(dx*dx + dy*dy + dz*dz);
}


std::vector<double> RNonlinearBeamColumnElement::getSectionLocations(void) const
{
    std::vector<double>locationVec(numSections);

    auto L = this->getLength();

    double pts[20];
    theBeamIntegration->getSectionLocations(numSections, L, pts);

    for (size_t i = 0; i < numSections; i++)
      locationVec[i] = pts[i]*L;

    return locationVec;
}


int RNonlinearBeamColumnElement::getNumSections() const
{
    return numSections;
}



