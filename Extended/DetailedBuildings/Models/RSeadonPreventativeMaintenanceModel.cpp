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
*   - Seadon Chui                                                    *
*                                                                    *
*********************************************************************/

#include "RSeadonPreventativeMaintenanceModel.h"
#include "RContinuousRandomVariable.h"
#include "RLocation.h"
#include "math.h"
#include "RComponent.h"

RSeadonPreventativeMaintenanceModel::RSeadonPreventativeMaintenanceModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBIM = nullptr;
    theResponse = nullptr;
    priceConcreteM = 0.8338;
    priceConcreteB = 142.9;

    // Create the response object
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

}


RSeadonPreventativeMaintenanceModel::~RSeadonPreventativeMaintenanceModel()
{
}


QObject * RSeadonPreventativeMaintenanceModel::getBIM() const
{
    return theBIM;
}


void RSeadonPreventativeMaintenanceModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


int RSeadonPreventativeMaintenanceModel::evaluateModel(RGradientType theGradientType)
{
    // Make the numbers location dependent (will affect conversions in constructor)
    RLocation *theLocation = qobject_cast<RLocation *>(theBIM->getLocation());
    double cost = 0.0;

    // Check list of repair times
    for (int i=0; i<repairTimes.size(); i++)
    {
        // Check if it is time for maintenance
        if (fabs(repairTimes.at(i).first - theCurrentTime->getCurrentValue()) < 1.0/(24.0*60.0))
        {
            // Check what component class this is
            if (qobject_cast<RCircularRCColumnComponent *>(repairTimes.at(i).second))
            {
                RCircularRCColumnComponent *theComponent = qobject_cast<RCircularRCColumnComponent *>(repairTimes.at(i).second);

                cost += theComponent->getSurfaceArea() * theComponent->getConcreteCoverThickness()->getCurrentValue() * getPriceOfConcrete(theComponent->getConcreteStrength());
            }
            else if (qobject_cast<RRectangularRCColumnComponent *>(repairTimes.at(i).second))
            {
                RRectangularRCColumnComponent *theComponent = qobject_cast<RRectangularRCColumnComponent *>(repairTimes.at(i).second);

                cost += theComponent->getSurfaceArea() * theComponent->getConcreteCoverThickness()->getCurrentValue() * getPriceOfConcrete(theComponent->getConcreteStrength());
            }
        }
    }

    theResponse->setCurrentValue(1.1*cost);

    return 0;
}


QList<QPair<double, QString> > RSeadonPreventativeMaintenanceModel::getTriggerTimes(double startTime, double endTime)
{
    // Clear this class variable list
    repairTimes.clear();

    // Declarations
    // Need this to return a string name while keeping the original pointer list
    QList<QPair<double, QString> > repairTimesString;
    double timeIncrement;
    double newTriggerTime;

    // There should be a more efficient algorithm to do this, for when there are large building inputs
    QList<RComponent *> theComponentList = theBIM->getComponentList();
    for (auto&& it: theComponentList)
    {
        if (qobject_cast<RCircularRCColumnComponent *>(it))
        {
            timeIncrement = getConcreteMaintenancePeriod(qobject_cast<RCircularRCColumnComponent *>(it)->getConcreteStrength(),
                                                    qobject_cast<RCircularRCColumnComponent *>(it)->getConcreteCoverThickness()->getCurrentValue());
        }
        else if (qobject_cast<RRectangularRCColumnComponent *>(it))
        {
            timeIncrement = getConcreteMaintenancePeriod(qobject_cast<RRectangularRCColumnComponent *>(it)->getConcreteStrength(),
                                                    qobject_cast<RRectangularRCColumnComponent *>(it)->getConcreteCoverThickness()->getCurrentValue());
        }
        else {
            // Note that some components simply don't need significant maintenance
            qDebug() << this->objectName() << " is missing a model for " << it->objectName();
            timeIncrement = 999999999999;
        }
        // Add repair trigger times within the period from startTime to endTime
        newTriggerTime = startTime + timeIncrement;
        while (newTriggerTime < endTime)
        {
            repairTimes.append(qMakePair(newTriggerTime, it));
            repairTimesString.append(qMakePair(newTriggerTime, it->objectName() + QString("Maintenance")));
            newTriggerTime += timeIncrement;
        }
    }

    return repairTimesString;
}


double RSeadonPreventativeMaintenanceModel::getConcreteMaintenancePeriod(QString theConcreteStrength, double theConcreteCover)
{
    // Error if there is no cover
    if (theConcreteCover <= 0.0) {
        qCritical() << "Something doesn't have any cover concrete depth";
        return 999999999999;
    }

    // The expression below uses MPa
    double fc = 20.0;

    if (theConcreteStrength == "C20")
        fc = 20.0;
    else if (theConcreteStrength == "C25")
        fc = 25.0;
    else if (theConcreteStrength == "C30")
        fc = 30.0;

    // Based on Monteiro et al (2012)
    // Statistical analysis of the carbonation coefficient in open air concrete structures

    // K in mm/y^0.5
    double K = 847*pow(fc,-1.435);

    // Expression uses cover in millimetres
    double covermm = theConcreteCover*1000;

    return 365*covermm*covermm/K/K;
}


double RSeadonPreventativeMaintenanceModel::getPriceOfConcrete(QString theConcreteStrength)
{
    double fc = 20.0;
    if (theConcreteStrength == "C20")
        fc = 20.0;
    else if (theConcreteStrength == "C25")
        fc = 25.0;
    else if (theConcreteStrength == "C30")
        fc = 30.0;

    return priceConcreteM * fc + priceConcreteB;
}
