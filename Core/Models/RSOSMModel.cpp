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
*   - Terje Haukaas                                                  *
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RSOSMModel.h"
#include "RResponse.h"
#include "RDomain.h"
#include "RContinuousRandomVariable.h"
#include "RDecisionVariable.h"

RSOSMModel::RSOSMModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theInputParameter = nullptr;
    theResponse = nullptr;

    isOrchestratingModel = true;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    theGradientMethod = "Finite Difference";
}


RSOSMModel::~RSOSMModel()
{

}


QObject * RSOSMModel::getInputParameter() const
{
    return theInputParameter;
}


void RSOSMModel::setInputParameter(QObject *value)
{
    theInputParameter = qobject_cast<RParameter *>(value);
}


QString RSOSMModel::getGradientMethod()
{
    return theGradientMethod;
}


void RSOSMModel::setGradientMethod(QString value)
{
    if (value.toLower().startsWith("finite")) {
        theGradientMethod = "Finite Difference";
    }
    else if (value.toLower().startsWith("direct")) {
        theGradientMethod = "Direct Differentiation";
    }
    else if (value.contains("DDM")) {
        theGradientMethod = "DDM";
    }
    else if (value.contains("D2DM")) {
        theGradientMethod = "D2DM";
    }
    else if (value.contains("D3DM")) {
        theGradientMethod = "D3DM";
    }
    else {
        qCritical() << "The gradient method in the SOSM model must be either Finite Difference or Direct Differentiation.";
    }
}


int RSOSMModel::evaluateModel(RGradientType theGradientType)
{
    // RUN UPSTREAM MODEL
    RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameter);

    if (!theInputCastAsResponse) {
        qCritical() << this->objectName() << "needs the input parameter to be a generic response";
    }

    RModel *theModel = qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    if (theGradientMethod == QString("Finite Difference"))
    {
        theModel->runAnalysis(RGradientType::F2DM);
    }
    else if (theGradientMethod == QString("D2DM"))
    {
        theModel->runAnalysis(RGradientType::D2DM);  //Get the second derivatives
    }
    else if (theGradientMethod == QString("D3DM"))
    {
        theModel->runAnalysis(RGradientType::D3DM);  //Get the third-order derivatives
    }
    else
    {
        qCritical()<<"Only Second Order Direct Differentiation of the Finite Difference Method can be used in SOSM";
    }

    double meanOfParameterFunction = theInputParameter->getCurrentValue();

    // qDebug()<<"The mean in SOSM before second-order correction (FIRST ORDER MEAN)is:"<<meanOfParameterFunction;

    // Get the second-order gradient map from the input response; we only need this to calculate the mean.
    QMap<RParameter *, double> theInputResponseGradientMap = theInputCastAsResponse->getSecondOrderGradientMap();

    // GET A LIST OF THE RANDOM VARIABLES
    QList<RRandomVariable *> theRVs = theDomain->findChildren<RRandomVariable *>();

    int numRVs = theRVs.size();
    for (int i=0; i<numRVs; i++)
    {
        RParameter* theParameter = qobject_cast<RParameter *>(theRVs.at(i));

        if(theParameter)
        {
            if(theInputResponseGradientMap.contains(theParameter))
            {
                double stdDev = (double)theRVs.at(i)->getStandardDeviationValue();

                // qDebug()<<"For the RV"<<theRVs.at(i)->objectName()<<"the standard deviation value is:"<<stdDev;
                // qDebug()<<"and the second order gradient is"<<theInputResponseGradientMap.value(theParameter);

                meanOfParameterFunction += (double)0.5*theInputResponseGradientMap.value(theParameter) * stdDev * stdDev;
            }
        }
    }

    if (theOutputLevel == RObject::Maximum)
        qDebug()<<"The mean in SOSM after correction"<<meanOfParameterFunction;

    theResponse->setCurrentValue(meanOfParameterFunction);

    //  COMPUTE DDM SENSITIVITIES
    if (theGradientType >= RGradientType::D3DM)
    {
        QMap<RParameter *, double> theLocalGradientMap;

        QMap<RParameter *, double> mapOf_dInput_dParFirst = theInputCastAsResponse->getGradientMap();
        QMap<RParameter *, double> mapOf_dInput_dParThird = theInputCastAsResponse->getThirdOrderGradientMap();

        QMapIterator<RParameter *, double> i(mapOf_dInput_dParFirst);
        while (i.hasNext())
        {
            i.next();

            RParameter *theParam = i.key();

            double dInputdPar = mapOf_dInput_dParFirst.value(theParam);

            // dMean/dParameters = dMean/dInput*dInput/dParameters + 0.5*dHdInputPar*std^2 = 1.0 * dInput/dParameters + 0.5*dHdInputPar*std^2

            // If its a random variable, have third order derivatives for correction
            RContinuousRandomVariable* theRVParam = qobject_cast<RContinuousRandomVariable *>(theParam);
            if(theRVParam)
            {
                double stdDev = theRVParam->getStandardDeviationValue();
                dInputdPar += 0.5*mapOf_dInput_dParThird.value(theRVParam)*stdDev*stdDev;
            }

            theLocalGradientMap.insert(i.key(), dInputdPar);
        }

        // Address the possibility of RVs having DVs as mean (must assume they haven't entered the gradient map before; risky?)
        QList<RContinuousRandomVariable *> theRVs = theDomain->findChildren<RContinuousRandomVariable *>();
        for (int i=0; i<theRVs.size(); i++)
        {
            RDecisionVariable *theDV = theDomain->findChild<RDecisionVariable *>(theRVs.at(i)->getMean());
            if (theDV)
            {
                theLocalGradientMap.insert(qobject_cast<RParameter *>(theDV), mapOf_dInput_dParFirst.value(theRVs.at(i)));
            }
        }

        theResponse->setGradientMap(theLocalGradientMap);
    }

    if (theOutputLevel == RObject::Maximum) {

        qDebug() << this->objectName() << "has response" << meanOfParameterFunction;

    }


    return 1;
}



