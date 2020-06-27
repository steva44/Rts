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
*   - Gurvinder Gill                                                 *
*   - Seadon Chui                                                    *
*                                                                    *
*********************************************************************/

#include "RNonLoadBearingWallComponent.h"
#include "RContinuousRandomVariable.h"
#include "RPoint.h"
#include "RMeshGenerator.h"

#include <tgmath.h>

RNonLoadBearingWallComponent::RNonLoadBearingWallComponent(QObject *parent, QString &name)
    : RWallComponent(parent, name)
{
    // For now, just create a random variable for the repair cost
    theDomain->createObject(objectName() + "RepairCost", "RContinuousRandomVariable");
    RContinuousRandomVariable *theRepairCost = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theRepairCost->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theRepairCost->setMean(QString("500.0"));
    theRepairCost->setCoefficientOfVariation("0.3");

    meshWasCreated = false;

    // Change this to true if you want it to have a stiffness
    isStructural = false;

    // For energy loss calculations
    // https://www.richmond.ca/__shared/assets/part9energyefficiencyrequirementsdrawings41752.pdf
    // EXTERIOR WALLS: 2.78 RSI
    // RSI CALCULATION FOR COMPONENT
    // 140mm STUDS @ 406mm O/C WITH R-19 BATT (R-20 COMPRESSED)
    // 100 / ((23%)+(77%)) 1.19 3.34
    //   2.36
    // ADDITIONAL COMPONENTS
    // EXTERIOR AIR FILM
    // 0.03 (FROM TABLE)
    // CLADDING MATERIAL
    // AS INDICATED BY DESIGNER
    // -
    // 9.5mm STRAPPING
    // 0.15 (FROM TABLE)
    // 12.5mm PLYWOOD SHEATHING
    // 0.109 (FROM TABLE)
    // 12.7mm GYPSUM WALL BOARD
    // 12.7mm x 0.0061 RSI/mm
    // 0.07747
    // INTERIOR AIR FILM
    // 0.12 (FROM TABLE)

    RSIValue = 2.85;
}


RNonLoadBearingWallComponent::~RNonLoadBearingWallComponent()
{

}


double RNonLoadBearingWallComponent::getConstructionCost()
{
    return getElevationArea() * 100.0;
}


double RNonLoadBearingWallComponent::getNumberOfWorkerHoursForConstruction()
{
    return 7.0;
}


double RNonLoadBearingWallComponent::getDrift()
{
    double theXCoordinate = thePoint1->getXCoordinateValue();
    double theYCoordinate = thePoint1->getYCoordinateValue();

    RPoint *thePair  = nullptr;
    RPoint *theCross = nullptr;

    if ((thePoint2->getXCoordinateValue()==theXCoordinate)&&(thePoint2->getYCoordinateValue()==theYCoordinate))
    {
        thePair = thePoint2;
        theCross = thePoint4;

    }
    else if ((thePoint4->getXCoordinateValue()==theXCoordinate)&&(thePoint4->getYCoordinateValue()==theYCoordinate))
    {
        thePair = thePoint4;
        theCross = thePoint2;
    }
    else
    {
        thePair = thePoint4;
        theCross = thePoint2;
    }

    double theXLength = theCross->getXCoordinateValue() - theXCoordinate;
    double theYLength = theCross->getYCoordinateValue() - theYCoordinate;
    double theLength = sqrt(theXLength*theXLength + theYLength*theYLength);

    double theXDisplacement = thePair->getXDisplacement() - thePoint1->getXDisplacement();
    double theYDisplacement = thePair->getYDisplacement() - thePoint1->getYDisplacement();

    // Use dot product to find in-plane component of drift vector
    double theDrift = (theXLength * theXDisplacement + theYLength * theYDisplacement)/theLength;

    double theStoreyHeight = (thePoint1->getZCoordinateValue())-(thePair->getZCoordinateValue());

    return abs(theDrift/theStoreyHeight);

}


double RNonLoadBearingWallComponent::getRepairCost()
{
    auto drift = getDrift();

    if (drift < 0.05)
    {
        return drift/0.05*getConstructionCost();
    }
    else
    {
        return getConstructionCost();
    }

}


int RNonLoadBearingWallComponent::createMesh()
{

    // To do: implement a mesh to provide a little stiffness in the non-structural walls

    meshWasCreated = true;

    return 0;
}
