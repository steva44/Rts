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
*   - Terje Haukaas                                                  *
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RResponse.h"
#include "RModel.h"
#include "RDomain.h"
#include "RAlgebraicExpressionModel.h"
#include "RContinuousRandomVariable.h"

RResponse::RResponse(QObject *parent, QString name)
    : RParameter(parent, name)
{
    isAnalyzedFlag = false;
    theModel = nullptr;
    theTotalStandardDeviation = 0;
    theBetaSensitivityWrtTotalStandardDeviation = 0;
    theTotalEpistemicStandardDeviation = 0;
    theBetaSensitivityWrtTotalEpistemicStandardDeviation = 0;
    theModelStandardDeviation = 0;
    theBetaSensitivityWrtModelStandardDeviation = 0;
    theModelEpistemicStandardDeviation = 0;
    theBetaSensitivityWrtModelEpistemicStandardDeviation = 0;
}


RResponse::~RResponse()
{

}


void RResponse::setCurrentValue(double value)
{
    RParameter::setCurrentValue(value);
    this->setAnalyzed(true);
}


void  RResponse::setCurrentValue(QString name, double value)
{
    // Does nothing, the function is implemented in RInfoRichResponse
}


bool RResponse::isAnalyzed()
{
    return isAnalyzedFlag;
}


void RResponse::setAnalyzed(bool analyzed)
{
    //isAnalyzedFlag = analyzed;

    isAnalyzedFlag = false;

    //qDebug() << "Terje is overriding the analyzed flag here.";
    //isAnalyzedFlag = false;
}


QObject * RResponse::getModel() const
{
    return theModel;
}


double RResponse::getTotalStandardDeviation() const	
{	
    return theTotalStandardDeviation;
}	


void RResponse::setTotalStandardDeviation(double value)	
{	
    theTotalStandardDeviation = value;
}	


double RResponse::getBetaSensitivityWrtTotalStandardDeviation() const	
{	
    return theBetaSensitivityWrtTotalStandardDeviation;
}	


void RResponse::setBetaSensitivityWrtTotalStandardDeviation(double value)	
{	
    theBetaSensitivityWrtTotalStandardDeviation = value;
}	


double RResponse::getTotalEpistemicStandardDeviation() const	
{	
    return theTotalEpistemicStandardDeviation;
}	


void RResponse::setTotalEpistemicStandardDeviation(double value)	
{	
    theTotalEpistemicStandardDeviation = value;
}	


double RResponse::getBetaSensitivityWrtTotalEpistemicStandardDeviation() const	
{	
    return theBetaSensitivityWrtTotalEpistemicStandardDeviation;
}	


void RResponse::setBetaSensitivityWrtTotalEpistemicStandardDeviation(double value)	
{	
    theBetaSensitivityWrtTotalEpistemicStandardDeviation = value;
}	


double RResponse::getModelStandardDeviation() const	
{	
    return theModelStandardDeviation;
}	


void RResponse::setModelStandardDeviation(double value)	
{	
    theModelStandardDeviation = value;
}	


double RResponse::getBetaSensitivityWrtModelStandardDeviation() const	
{	
    return theBetaSensitivityWrtModelStandardDeviation;
}	


void RResponse::setBetaSensitivityWrtModelStandardDeviation(double value)	
{	
    theBetaSensitivityWrtModelStandardDeviation = value;
}	


double RResponse::getModelEpistemicStandardDeviation() const	
{	
    return theModelEpistemicStandardDeviation;
}	


void RResponse::setModelEpistemicStandardDeviation(double value)	
{	
    theModelEpistemicStandardDeviation = value;
}	


double RResponse::getBetaSensitivityWrtModelEpistemicStandardDeviation() const	
{	
    return theBetaSensitivityWrtModelEpistemicStandardDeviation;
}	


void RResponse::setBetaSensitivityWrtModelEpistemicStandardDeviation(double value)	
{	
    theBetaSensitivityWrtModelEpistemicStandardDeviation = value;
}	


void RResponse::setModel(QObject *value)
{
    RModel *tempModel = qobject_cast<RModel *>(value);
    if (tempModel) {
        theModel = tempModel;
        setParent(theModel);
    } else {
        if (theModel) {
            setParent(theModel->parent());
        }
        theModel = nullptr;
    }
}


QMap<RParameter *, double> RResponse::getGradientMap()
{
    return theGradientMap;
}


QMap<RParameter *, double> RResponse::getSecondOrderGradientMap()
{
    return theSecondOrderGradientMap;
}


QMap<RParameter *, double> RResponse::getThirdOrderGradientMap()
{
    return theThirdOrderGradientMap;
}


void RResponse::setGradientMap(QMap<RParameter *, double> passedGradeintMap)
{


    //    QMapIterator<RParameter *, double> i(passedGradeintMap);
    //    while (i.hasNext()) {
    //        i.next();

    //        QString parameter = i.key()->objectName();
    //        double value = i.value();

    //        qDebug()<<"RResponse For the parameter "<<parameter<<" the gradient is: "<<value;

    //    }
    if(!passedGradeintMap.empty())
    {
        theGradientMap.clear();
        theGradientMap = passedGradeintMap;
    }
}


void RResponse::setSecondOrderGradientMap(QMap<RParameter *, double> passedGradeintMap)
{


    //    QMapIterator<RParameter *, double> i(passedGradeintMap);
    //    while (i.hasNext()) {
    //        i.next();

    //        QString parameter = i.key()->objectName();
    //        double value = i.value();

    //        qDebug()<<"RResponse For the parameter "<<parameter<<" the gradient is: "<<value;

    //    }

    theSecondOrderGradientMap.clear();
    theSecondOrderGradientMap = passedGradeintMap;
}


void RResponse::setThirdOrderGradientMap(QMap<RParameter *, double> passedGradeintMap)
{


    //    QMapIterator<RParameter *, double> i(passedGradeintMap);
    //    while (i.hasNext()) {
    //        i.next();

    //        QString parameter = i.key()->objectName();
    //        double value = i.value();

    //        qDebug()<<"RResponse For the parameter "<<parameter<<" the gradient is: "<<value;

    //    }

    theThirdOrderGradientMap.clear();
    theThirdOrderGradientMap = passedGradeintMap;
}


void RResponse::getGradientWrtTheseParameters(QList<RParameter *> theParameters, QVector<double> *theGradientVector, RDomain *domain)
{
    // If there is a DDM map then use that, otherwise do finite difference
    if (theGradientMap.size() != 0) {

        for (int i=0; i<theParameters.size(); i++) {
            (*theGradientVector)[i] = theGradientMap.value(qobject_cast<RParameter *>(theParameters.at(i)));
        }
    }
    else {

        double originalResponseValue = this->getCurrentValue();

        //qDebug()<<"The original response is"<<originalResponseValue;

        //RModel *theModel = qobject_cast<RModel *>(this->getModel());
        double originalVariableValue, perturbedVariableValue, perturbedResponseValue, perturbation;

        double perturbationFactor = 0.00001;

        // Steve, don't forget to comment out these lines :)
        // qDebug()<<"********************FDM********************";

        for (int i=0; i<theParameters.size(); i++) {

            RParameter* theParameter = theParameters.at(i);

            originalVariableValue = theParameter->getCurrentValue();

            RContinuousRandomVariable *theRV = qobject_cast<RContinuousRandomVariable *>(theParameter);
            if(theRV) {
                perturbation = perturbationFactor * theRV->getStandardDeviationValue();

            }
            else {
                if (originalVariableValue == 0.0) {
                    perturbation = perturbationFactor;
                }
                else {
                    perturbation = perturbationFactor * originalVariableValue;
                }
            }

            perturbedVariableValue = originalVariableValue + perturbation;
            theParameter->setCurrentValue(perturbedVariableValue);


            // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
            domain->clearAnalyzedFlags();

            if (theModel)
            {
                theModel->runAnalysis(RGradientType::NONE);
            }
            else
            {
                qCritical() << this->objectName() << "needs a parent model to obtain finite difference results.";
            }
            perturbedResponseValue = this->getCurrentValue();

            (*theGradientVector)[i] = (perturbedResponseValue-originalResponseValue) / perturbation;

            // Steve: Don't forget to comment out these :)
            //qDebug()<<"First order FDM for the parameter "<<theParameter->objectName()<<" the value is: "<<(*theGradientVector)[i];

            theParameter->setCurrentValue(originalVariableValue);          
        }
    }
}


void RResponse::getSecondOrderGradientsWrtTheseParameters(QList<RParameter *> theParameters, QVector<double> *theGradientVector, RDomain *domain)
{
    // If there is a DDM map then use that, otherwise do finite difference
    if (theSecondOrderGradientMap.size() != 0) {

        for (int i=0; i<theParameters.size(); i++) {
            (*theGradientVector)[i] = theSecondOrderGradientMap.value(qobject_cast<RParameter *>(theParameters.at(i)));
        }
    }
    else {
        double perturbationFactor = 0.001;

        qDebug()<<"********************FD2M********************";

        double originalVariableValue, perturbedVariableValuefwd, perturbedVariableValueback, perturbedResponseValueFwd,perturbedResponseValueBack, perturbation;

        //Get F2DM using the central difference scheme

        double originalResponseValue = this->getCurrentValue();

        for (int i=0; i<theParameters.size(); i++)
        {
            RParameter* theParameter = theParameters.at(i);

            originalVariableValue = theParameter->getCurrentValue();

            RContinuousRandomVariable *theRV = qobject_cast<RContinuousRandomVariable *>(theParameter);
            if(theRV) {
                perturbation = perturbationFactor * theRV->getStandardDeviationValue();

            }
            else {
                perturbation = perturbationFactor * originalVariableValue;
            }

            perturbedVariableValuefwd = originalVariableValue + perturbation;
            perturbedVariableValueback = originalVariableValue - perturbation;

            //First do the forward value
            theParameter->setCurrentValue(perturbedVariableValuefwd);

            // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
            domain->clearAnalyzedFlags();

            if (theModel)
            {
                theModel->runAnalysis(RGradientType::NONE);
            }
            else
            {
                qCritical() << this->objectName() << "needs a parent model to obtain finite difference results.";
            }
            perturbedResponseValueFwd = this->getCurrentValue();

            //now rerun with the back value
            theParameter->setCurrentValue(perturbedVariableValueback);          

            // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
            domain->clearAnalyzedFlags();

            if (theModel)
            {
                theModel->runAnalysis(RGradientType::NONE);
            }
            else
            {
                qCritical() << this->objectName() << "needs a parent model to obtain finite difference results.";
            }
            perturbedResponseValueBack = this->getCurrentValue();


            (*theGradientVector)[i] = (perturbedResponseValueFwd - 2*originalResponseValue + perturbedResponseValueBack) /(perturbation*perturbation);

            qDebug()<<"For second order finite difference gradient for the parameter "<<theParameter->objectName()<<" is: "<<(*theGradientVector)[i];

            //Reset the parameter
            theParameter->setCurrentValue(originalVariableValue);

        }
    }
}


void RResponse::getThirdOrderGradientsWrtTheseParameters(QList<RParameter *> theParameters, QVector<double> *theGradientVector, RDomain *domain)
{
    // If there is a DDM map then use that, otherwise do finite difference
    if (theThirdOrderGradientMap.size() != 0)
    {

        for (int i=0; i<theParameters.size(); i++) {
            (*theGradientVector)[i] = theThirdOrderGradientMap.value(qobject_cast<RParameter *>(theParameters.at(i)));
        }
    }
    else
    {
        double perturbationFactor = 0.01;

        double originalVariableValue, perturbedVariableValuefwd, perturbedVariableValueback, perturbedVariableValuefwdTwo, perturbedVariableValuebackTwo, perturbation;

        double perturbedResponseValueFwd,perturbedResponseValueFwdTwo,perturbedResponseValueBack,perturbedResponseValueBackTwo;

        //Get F3DM using the central difference scheme

        double originalResponseValue = this->getCurrentValue();

        for (int i=0; i<theParameters.size(); i++)
        {
            RParameter* theParameter = theParameters.at(i);

            originalVariableValue = theParameter->getCurrentValue();

            RContinuousRandomVariable *theRV = qobject_cast<RContinuousRandomVariable *>(theParameter);
            if(theRV) {
                perturbation = perturbationFactor * theRV->getStandardDeviationValue();

            }
            else {
                perturbation = perturbationFactor * originalVariableValue;
            }


            perturbedVariableValuefwdTwo = originalVariableValue + perturbation;
            perturbedVariableValuefwd = originalVariableValue + 2*perturbation;
            perturbedVariableValueback = originalVariableValue - 2*perturbation;
            perturbedVariableValuebackTwo = originalVariableValue - perturbation;

            //First do the forward value
            theParameter->setCurrentValue(perturbedVariableValuefwd);


            // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
            domain->clearAnalyzedFlags();

            if (theModel)
            {
                theModel->runAnalysis(RGradientType::NONE);
            }
            else
            {
                qCritical() << this->objectName() << "needs a parent model to obtain finite difference results.";
            }
            perturbedResponseValueFwd = this->getCurrentValue();

            //now rerun with the back value
            theParameter->setCurrentValue(perturbedVariableValueback);


            // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
            domain->clearAnalyzedFlags();

            if (theModel)
            {
                theModel->runAnalysis(RGradientType::NONE);
            }
            else
            {
                qCritical() << this->objectName() << "needs a parent model to obtain finite difference results.";
            }
            perturbedResponseValueBack = this->getCurrentValue();

            //Now do the second forward value
            theParameter->setCurrentValue(perturbedVariableValuefwdTwo);

            // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
            domain->clearAnalyzedFlags();

            if (theModel)
            {
                theModel->runAnalysis(RGradientType::NONE);
            }
            else
            {
                qCritical() << this->objectName() << "needs a parent model to obtain finite difference results.";
            }
            perturbedResponseValueFwdTwo = this->getCurrentValue();

            //now rerun with the back value
            theParameter->setCurrentValue(perturbedVariableValuebackTwo);


            // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
            domain->clearAnalyzedFlags();

            if (theModel)
            {
                theModel->runAnalysis(RGradientType::NONE);
            }
            else
            {
                qCritical() << this->objectName() << "needs a parent model to obtain finite difference results.";
            }
            perturbedResponseValueBackTwo = this->getCurrentValue();

            /********/

            (*theGradientVector)[i] = (-2.0*perturbedResponseValueFwd - 2.0*perturbedResponseValueBack + perturbedResponseValueFwdTwo - perturbedResponseValueBackTwo) /(2*perturbation*perturbation*perturbation);

            qDebug()<<"For third order finite difference gradient for the parameter "<<theParameter->objectName()<<" is: "<<(*theGradientVector)[i];

            //Reset the parameter
            theParameter->setCurrentValue(originalVariableValue);       
        }
    }
}


void RResponse::clearGradientMap()
{
    theGradientMap.clear();
}


void RResponse::clearSecondOrderGradientMap()
{
    theSecondOrderGradientMap.clear();
}


void RResponse::clearThirdOrderGradientMap()
{
    theThirdOrderGradientMap.clear();
}


void RResponse::backup()
{
    backupValue = theCurrentValue;
}


void RResponse::restore()
{
    theCurrentValue = backupValue;
}


QVector<double> RResponse::getValueVector()
{
    QVector<double> resultVector;
    resultVector << theCurrentValue;
    return resultVector;
}


QVector<double> RResponse::getBackupVector()
{
    QVector<double> resultVector;
    resultVector << backupValue;
    return resultVector;
}


void RResponse::setValueVector(QVector<double> valueVector)
{
    if (valueVector.isEmpty()) {
        qCritical() << "Error: The value vector passed to the response" << objectName() << " is empty.";
        return;
    }
    theCurrentValue = valueVector[0];
}


int RResponse::updateValue()
{
    return 0;
}
