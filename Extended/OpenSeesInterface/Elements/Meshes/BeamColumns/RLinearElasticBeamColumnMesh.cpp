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

#include "RLinearElasticBeamColumnMesh.h"
#include "RMeshGenerator.h"
#include "RElasticBeam3d.h"
#include "RComponent.h"
#include "RCrdTransf.h"
#include "RMaterialKnowledge.h"
#include "RParameter.h"
#include "RLinearElasticSectionKnowledge.h"
#include "RPoint.h"
#include "RElasticBeamColumnElement.h"


RLinearElasticBeamColumnMesh::RLinearElasticBeamColumnMesh(RDomain *theDomain,
                                                           QString parentName,
                                                           RMaterialKnowledge *material,
                                                           RSectionKnowledge *crossSection,
                                                           RPoint *point1,
                                                           RPoint *point2,
                                                           RParameter* orientation,
                                                           QList<RParameter*> &parameterList,
                                                           QList<RElement*> &elementList)
    : RBeamColumnMesh(theDomain,parentName,point1,point2),theCrossSection(crossSection),theMaterial(material),
      theParentName(parentName),theOrientationParameter(orientation)
{
    theElementList = &elementList;

}


RLinearElasticBeamColumnMesh::~RLinearElasticBeamColumnMesh()
{
}


int RLinearElasticBeamColumnMesh::createMesh(void)
{
    numberOfElementsAlongU = 1;

    //    QString elementType = "RElasticBeam3d";
    QString elementType = "RElasticTimoshenkoBeam3d";

    // Create the mesh
    RMeshGenerator theMeshGenerator(theDomain);

    theMeshGenerator.meshLinearElement(thePoint1, thePoint2, elementType, numberOfElementsAlongU, theParentName, *theElementList);
    if(theElementList->empty())
    {
        qCritical()<<"Error: An empty element list was returned from the mesh generator in "<<this->objectName();
        return -1;
    }

    // Create and extract the needed material parameters
    RParameter *theYoungsModulus = theMaterial->getYoungsModulus();
    RParameter* thePoissonsRatio = theMaterial->getPoissonsRatio();
    RParameter* theDensity = theMaterial->getDensity();

    auto crossSection = qobject_cast<RLinearElasticSectionKnowledge*>(theCrossSection);

    // Create and extract the needed cross-section parameters
    RParameter *theArea = crossSection->getArea();
    RParameter *theIy = crossSection->getIy();
    RParameter *theIz = crossSection->getIz();
    RParameter *theJ = crossSection->getJ();
    RParameter *theAvy = crossSection->getAvy();
    RParameter *theAvz = crossSection->getAvz();

    auto theElasticBeamColElement = qobject_cast<RElasticBeamColumnElement*>(theElementList->first());
    if(theElasticBeamColElement)
    {
        // Set element properties
        theElasticBeamColElement->setE(theYoungsModulus);
        theElasticBeamColElement->setA(theArea);
        theElasticBeamColElement->setIy(theIy);
        theElasticBeamColElement->setIz(theIz);
        theElasticBeamColElement->setNu(thePoissonsRatio);
        theElasticBeamColElement->setJ(theJ);
        theElasticBeamColElement->setRho(theDensity);
        theElasticBeamColElement->setOrientation(theOrientationParameter);
        theElasticBeamColElement->setAvY(theAvy);
        theElasticBeamColElement->setAvZ(theAvz);

        auto theOSElement = theElasticBeamColElement->getTheOpenSeesElement();

        if(theOSElement)
        {
            theDomain->getTheOpenSeesDomain()->addElement(theOSElement);
        }
        else
        {
            qCritical()<<"Error: failed to initialize the OpenSees element from"<<theElasticBeamColElement->objectName()<<" in "<<this->objectName()<<" in "<<__FUNCTION__;
            return -1;
        }
    }

    // qDebug()<<"theYoungsModulus "<<theYoungsModulus->getCurrentValue();
    // qDebug()<<"theArea "<<theArea->getCurrentValue();
    // qDebug()<<"theIy "<<theIy->getCurrentValue();
    // qDebug()<<"theIz "<<theIz->getCurrentValue();
    // qDebug()<<"thePoissonsRatio "<<thePoissonsRatio->getCurrentValue();
    // qDebug()<<"theJ "<<theJ->getCurrentValue();
    // qDebug()<<"theOrientation "<<theOrientation->getCurrentValue();
    // qDebug()<<"theDensity "<<theDensity->getCurrentValue();

    return 0;
}


double RLinearElasticBeamColumnMesh::getMaxStress()
{    
    return 0.0;
}


double RLinearElasticBeamColumnMesh::getAxialLoad(int elementNumber)
{
    auto theElement = qobject_cast<RElasticBeamColumnElement*>(theElementList->at(elementNumber));

    if(theElement == nullptr)
    {
        qDebug()<<"Error in "<<this->objectName()<<" in "<<__FUNCTION__;
        return 0.0;
    }

    QVector<double> response(12);
    theElement->getLocalResponse(&response,"EndForces",0);

    auto N1 = response[0];
    auto N2 = response[6];

    //Return the larger of the two
    return (fabs(N1) > fabs(N2)) ? N1 : N2;
}



