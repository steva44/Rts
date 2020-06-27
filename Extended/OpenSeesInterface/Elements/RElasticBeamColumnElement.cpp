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

#include "RElasticBeamColumnElement.h"
#include "RCoordinateTransformation.h"

#include "CrdTransf.h"
#include "LinearCrdTransf3d.h"
#include "PDeltaCrdTransf3d.h"

RElasticBeamColumnElement::RElasticBeamColumnElement(QObject *parent, QString &name)
    : RElement(parent, name)
{  
    theNode1 = nullptr;
    theNode2 = nullptr;
    theRhoParameter = nullptr;
    theOrientationParameter = nullptr;
    theEParameter = nullptr;
    theNuParameter = nullptr;
    theAParameter = nullptr;
    theIzParameter = nullptr;
    theIyParameter = nullptr;
    theIyzParameter = nullptr;
    theJParameter = nullptr;
    theAvZParameter = nullptr;
    theAvYParameter = nullptr;
    theCrdTransformation = nullptr;

    theCrdTransformationType = "LinearCrdTransf3d";
    VecxzPlane.resize(3);
}


RElasticBeamColumnElement::~RElasticBeamColumnElement()
{

}


void RElasticBeamColumnElement::createTheCrdTransformation(void)
{
    RCoordinateTransformation theCoordinateTransformation(theDomain);

    auto res = theCoordinateTransformation.getVectorInXZPlane(theNode1,theNode2,&VecxzPlane, theOrientationParameter->getCurrentValue());

    if(res != 0)
    {
        qCritical()<<"Error in creating the coordinate transformation in "<<__FUNCTION__;
        return;
    }

    Vector vecxzPlane(3);

    // Initialize the OpenSees vector value
    vecxzPlane[0]=VecxzPlane[0];
    vecxzPlane[1]=VecxzPlane[1];
    vecxzPlane[2]=VecxzPlane[2];

    // Create the coordinate transformation
    if(theCrdTransformationType == "LinearCrdTransf3d")
        theCrdTransformation = std::make_unique<LinearCrdTransf3d>(objectTag,vecxzPlane);
    else if(theCrdTransformationType == "PDeltaCrdTransf3d")
        theCrdTransformation = std::make_unique<PDeltaCrdTransf3d>(objectTag,vecxzPlane);
    else
        qDebug()<<"The coordinate transformation type not recognized in "<<__FUNCTION__;

    return;
}


std::vector<double> RElasticBeamColumnElement::getVecxzPlane() const
{
    return VecxzPlane;
}


QPointer<RParameter> RElasticBeamColumnElement::getTheOrientation() const
{
    return theOrientationParameter;
}


int RElasticBeamColumnElement::setNode(int i, RNode *theNode)
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


int RElasticBeamColumnElement::getNumNodes()
{
    return 2;
}


RNode * RElasticBeamColumnElement::getNode(int i) const
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


QString RElasticBeamColumnElement::getTheCrdTransformationType() const
{
    return theCrdTransformationType;
}


void RElasticBeamColumnElement::setCrdTransformationType(const QString &value)
{
    theCrdTransformationType = value;
}


double RElasticBeamColumnElement::getLength(void)
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



