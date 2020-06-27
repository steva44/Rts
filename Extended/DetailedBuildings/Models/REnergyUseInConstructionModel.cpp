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
*   - Gurvinder Gill                                                 *
*                                                                    *
*********************************************************************/

#include "REnergyUseInConstructionModel.h"
#include "RComponent.h"
#include "RLocation.h"
#include "RContinuousRandomVariable.h"

REnergyUseInConstructionModel::REnergyUseInConstructionModel(QObject *parent, QString name)
    : RModel(parent, name)
{

    theBIM = nullptr;
    theTotalWorkerHours = nullptr;
    theHeavyMachineryUseIntensity = nullptr;
    theJouleDieselResponse = nullptr;

    theDomain->createObject(objectName() + "JouleDieselResponse", "RGenericResponse");
    theJouleDieselResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theJouleDieselResponse->setModel(this);

    theDomain->createObject(objectName() + "HeavyMachineryUseIntensity", "RContinuousRandomVariable");
    theHeavyMachineryUseIntensity = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theHeavyMachineryUseIntensity->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theHeavyMachineryUseIntensity->setMean(QString("268000000"));
    theHeavyMachineryUseIntensity->setCoefficientOfVariation("0.2");
}


REnergyUseInConstructionModel::~REnergyUseInConstructionModel()
{

}


QObject * REnergyUseInConstructionModel::getBIM() const
{
    return theBIM;
}


void REnergyUseInConstructionModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}


QObject *REnergyUseInConstructionModel::getTotalWorkerHours() const
{
    return theTotalWorkerHours;
}


void REnergyUseInConstructionModel::setTotalWorkerHours(QObject *value)
{
    theTotalWorkerHours = qobject_cast<RParameter *>(value);
}


int REnergyUseInConstructionModel::evaluateModel(RGradientType theGradientType)
{
    // Check that we have a BIM
    if(theBIM == nullptr){
        qCritical() << this->objectName() << "needs a BIM";
        return 0;
    }


    // Environmental impact
    if (theBIM->getTimeOfConstructionValue() == theCurrentTime->getCurrentValue()) {

        double worker_hours_for_construction = theTotalWorkerHours->getCurrentValue();
        double heavy_machinery_use_intensity = theHeavyMachineryUseIntensity->getCurrentValue();

        double heavy_machinery_ratio_high_rise_concrete = 0.3;


        theJouleDieselResponse->setCurrentValue(heavy_machinery_ratio_high_rise_concrete * worker_hours_for_construction *
                                                heavy_machinery_use_intensity);
    }
    else {
        theJouleDieselResponse->setCurrentValue(0.0);
    }


    return 0;
}
