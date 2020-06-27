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
*   - Mojtaba Mahsuli                                                *
*                                                                    *
*********************************************************************/

#include "RSyntheticGroundMotionModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RFileResponse.h"
#include "RCommandResponse.h"
#include "RTimeSeries.h"
#include <QDir>
#include <QApplication>
#include "RContinuousRandomVariable.h"

#include <math.h>

RSyntheticGroundMotionModel::RSyntheticGroundMotionModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    // Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RTimeSeriesResponse");
    theGroundMotionResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theGroundMotionResponse->setModel(this);

    theNumberOfRandomVariables = 200;
    theStartTime = 0.0;
    theEndTime = 20.0;

    // Create the list that will contain the random variables, so we can access them later, in evaluateModel
    theRVlist = new QList<RContinuousRandomVariable*>;

    // Assign a standard normal distribution to random variables
    theAllParametersList.clear();
    RContinuousRandomVariable *theRV;
    for (int i = 0; i < theNumberOfRandomVariables; i++) {
        theDomain->createObject(this->objectName() + QString("RV_%1").arg(i), "RContinuousRandomVariable");
        theRV = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theRV->setDistributionType(QString("Normal (mean, stdv)"));
        theRV->setMean(QString("0.0"));
        theRV->setStandardDeviation("1.0");
        theRV->setCurrentValue(0.1);
        theRVlist->append(theRV);
        theAllParametersList << theRV;
    }

}


RSyntheticGroundMotionModel::~RSyntheticGroundMotionModel()
{

}


QString RSyntheticGroundMotionModel::getGammaModulatingFunctionParameterAList()
{
    return parameterListToString(theGammaModulatingFunctionParameterAList);
}


void RSyntheticGroundMotionModel::setGammaModulatingFunctionParameterAList(QString value)
{
    theGammaModulatingFunctionParameterAList = stringToParameterList(value);

}


QString RSyntheticGroundMotionModel::getGammaModulatingFunctionParameterBList()
{
    return parameterListToString(theGammaModulatingFunctionParameterBList);
}


void RSyntheticGroundMotionModel::setGammaModulatingFunctionParameterBList(QString value)
{
    theGammaModulatingFunctionParameterBList = stringToParameterList(value);

}


QString RSyntheticGroundMotionModel::getGammaModulatingFunctionParameterCList()
{
    return parameterListToString(theGammaModulatingFunctionParameterCList);
}


void RSyntheticGroundMotionModel::setGammaModulatingFunctionParameterCList(QString value)
{
    theGammaModulatingFunctionParameterCList = stringToParameterList(value);

}


QString RSyntheticGroundMotionModel::getFilterDampingList()
{
    return parameterListToString(theFilterDampingList);
}


void RSyntheticGroundMotionModel::setFilterDampingList(QString value)
{
    theFilterDampingList = stringToParameterList(value);

}


QString RSyntheticGroundMotionModel::getFilterFrequencyList()
{
    return parameterListToString(theFilterFrequencyList);
}


void RSyntheticGroundMotionModel::setFilterFrequencyList(QString value)
{
    theFilterFrequencyList = stringToParameterList(value);

}


double RSyntheticGroundMotionModel::getNumberOfRandomVariables() const
{
    return theNumberOfRandomVariables;
}


void RSyntheticGroundMotionModel::setNumberOfRandomVariables(double value)
{
    // Set class variable or issue error message if less than one random variable is prescribed
    if (value <= 0.0) {
        qCritical() << "The number of random variables must be greater than zero.";
        value = 200;
    }
    theNumberOfRandomVariables = value;



    // Check if random variables have already been created, and delete them
    for (int i=0; i<theRVlist->size(); i++) {
        theDomain->deleteObject(theRVlist->at(i)->objectName());
    }
    theRVlist->clear();



    // Create new standard normal random variables
    theAllParametersList.clear();
    RContinuousRandomVariable *theRV;
    for (int i = 0; i < theNumberOfRandomVariables; i++) {
        theDomain->createObject(this->objectName() + QString("RV_%1").arg(i), "RContinuousRandomVariable");
        theRV = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        theRV->setDistributionType(QString("Normal (mean, stdv)"));
        theRV->setMean(QString("0.0"));
        theRV->setStandardDeviation("1.0");
        theRV->setCurrentValue(0.1);
        theRVlist->append(theRV);
        theAllParametersList << theRV;
    }


}


double RSyntheticGroundMotionModel::getStartTime() const
{
    return theStartTime;
}


void RSyntheticGroundMotionModel::setStartTime(double value)
{
    if (value < 0.0) {
        qCritical() << "The Start Time must be greater than or equal to 0.";
        value = 0.0;
    }
    theStartTime = value;
}


double RSyntheticGroundMotionModel::getEndTime() const
{
    return theEndTime;
}


void RSyntheticGroundMotionModel::setEndTime(double value)
{
    if (value <= 0.0) {
        qCritical() << "The End Time must be greater than zero.";
        value = 20.0;
    }
    theEndTime = value;
}


QStringList RSyntheticGroundMotionModel::getActionList()
{
    // Remember that the "run" and "help" methods are implemented in base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator" << "&Help";
    return actionList;
}


int RSyntheticGroundMotionModel::evaluateModel(RGradientType theGradientType)
{

    // Declare variables that will be used in this method
    int numberOfModulatingFunction;
    double modulatingFunctionAmplitude;
    double filterAmplitude;
    double sumOfImpulseResponseFunction;
    double sumOfModFunctionAndIRF;
    double wd, w, damping;
    double x, y, z;
    double a, b, c;
    double timeStepofGroundMotion = 0.02;


    // Extract the number of modulating functions from the list given by the user
    numberOfModulatingFunction=theGammaModulatingFunctionParameterAList.size();


    if((numberOfModulatingFunction!=theGammaModulatingFunctionParameterBList.size())||(numberOfModulatingFunction!=theGammaModulatingFunctionParameterCList.size())){
        qDebug()<<"The number of parameters of each modulating function should be same";
        return 1 ;
    }


    // Getting pointer to the response time series, and clean it
    RTimeSeries *theOutputTimeSeries = parameterToTimeSeries(theGroundMotionResponse);
    if (!theOutputTimeSeries) {
        qCritical() << "Error: The response object" << theGroundMotionResponse->objectName() << "does not contain a times series object.";
        return -1;
    }
    theOutputTimeSeries->clean();



    // In preparation for the loop, compute the time step interval
    double timeStep;
    timeStep = (theEndTime-theStartTime)/((double)theNumberOfRandomVariables);


    // Determine the time step of record
    double frequencyOfRecord=0;

    for (int i=0; i< theFilterFrequencyList.size(); i++){
        if(frequencyOfRecord < theFilterFrequencyList[i]->getCurrentValue()){
            frequencyOfRecord = theFilterFrequencyList[i]->getCurrentValue();
        }
    }

    timeStepofGroundMotion = 2*PI/(10*frequencyOfRecord);


    // Start the loop over the number of time steps
    for(double time=theStartTime; time<=theEndTime; time=time+timeStepofGroundMotion)
    {
        sumOfModFunctionAndIRF=0.0;


        // Start the loop over modulating functions
        for(int j=0; j<numberOfModulatingFunction; j++)
        {

            // Amplitude of modulating function at this particular time
            a = theGammaModulatingFunctionParameterAList[j]->getCurrentValue();
            b = theGammaModulatingFunctionParameterBList[j]->getCurrentValue();
            c = theGammaModulatingFunctionParameterCList[j]->getCurrentValue();
            modulatingFunctionAmplitude=a*pow(time,b)*exp(-c*time);

            sumOfImpulseResponseFunction=0.0;


            // Start the loop over the random variables
            for(int k=0; k<theNumberOfRandomVariables; k++){


                // Break the loop if current time is less than the product of time step and number of random variables
                if((time-timeStep*k)<0.0){
                    break;
                }


                w = theFilterFrequencyList[j]->getCurrentValue();
                damping = theFilterDampingList[j]->getCurrentValue();


                // Compute the damped natural frequency of the filter
                wd = w*sqrt(1-pow(damping,2.0));


                // Computer the amplitude of standard linear oscillator displacement filter at this particular time
                filterAmplitude = sin(wd*(time-timeStep*k))*exp(-damping*w*(time-timeStep*k));


                // Compute the response function at this particular time
                sumOfImpulseResponseFunction += theRVlist->at(k)->getCurrentValue() * filterAmplitude;

            }

            sumOfModFunctionAndIRF += sumOfImpulseResponseFunction * modulatingFunctionAmplitude;

        }

        // Give the value computed as time series
        x = sumOfModFunctionAndIRF*9.81;
        theOutputTimeSeries->setXYZvalues(time, x, y, z);


    }


    qDebug()<<"Record created";
    return 0;

}


QList<RParameter *> RSyntheticGroundMotionModel::getAllParametersList(bool *ok)
{
    *ok = true;
    return theAllParametersList;

}


