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

#include "RFOSMModel.h"
#include "RResponse.h"
#include "RDomain.h"
#include "RContinuousRandomVariable.h"
#include "RDecisionVariable.h"

RFOSMModel::RFOSMModel(QObject *parent, QString name)
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


RFOSMModel::~RFOSMModel()
{

}


QObject * RFOSMModel::getInputParameter() const
{
    return theInputParameter;
}


void RFOSMModel::setInputParameter(QObject *value)
{
    theInputParameter = qobject_cast<RParameter *>(value);
}


QString RFOSMModel::getGradientMethod()
{
    return theGradientMethod;
}


void RFOSMModel::setGradientMethod(QString value)
{
    if (value.toLower().startsWith("finite"))
    {
        theGradientMethod = "Finite Difference";
    }
    else if (value.toLower().startsWith("direct"))
    {
        theGradientMethod = "DDM";
    }
    else if (value.toLower().startsWith("ddm"))
    {
        theGradientMethod = "DDM";
    }
    else
    {
        qCritical() << "The gradient method in FOSM must be either Finite Difference or Direct Differentiation.";
    }
}


int RFOSMModel::evaluateModel(RGradientType theGradientType)
{

    //  Run the upstream models
    RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameter);
    if (!theInputCastAsResponse) {
        qCritical() << this->objectName() << "needs the input parameter to be a generic response";
    }
    RModel *theModel = qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    if (theGradientMethod == QString("Finite Difference")) {
        theModel->runAnalysis(RGradientType::FDM);
    }
    else if (theGradientMethod == QString("Direct Differentiation"))
    {
        theGradientType = RGradientType::DDM;
        theModel->runAnalysis(RGradientType::DDM);
    }
    else if (theGradientMethod == QString("DDM"))
    {
        theGradientType = RGradientType::DDM;
        theModel->runAnalysis(RGradientType::DDM);
    }
    else
    {
        return -1;
    }

    double meanOfParameterFunction = theInputParameter->getCurrentValue();
    theResponse->setCurrentValue(meanOfParameterFunction);

    // Compute the DDM sensitivities
    if (theGradientType >= RGradientType::DDM)
    {

        QMap<RParameter *, double> theLocalGradientMap;
        QMap<RParameter *, double> mapOf_dInput_dPar = theInputCastAsResponse->getGradientMap();

        QMapIterator<RParameter *, double> i(mapOf_dInput_dPar);
        while (i.hasNext()) {
            i.next();
            double dInputdPar = mapOf_dInput_dPar.value(i.key());

            // dMean/dParameters = dMean/dInput * dInput/dParameters = 1.0 * dInput/dParameters
            theLocalGradientMap.insert(i.key(), dInputdPar);
        }

        // Address the possibility of RVs having DVs as mean (must assume they haven't entered the gradient map before; risky?)
        QList<RContinuousRandomVariable *> theRVs = theDomain->findChildren<RContinuousRandomVariable *>();
        for (int i=0; i<theRVs.size(); i++) {
            RDecisionVariable *theDV = theDomain->findChild<RDecisionVariable *>(theRVs.at(i)->getMean());
            if (theDV) {
                theLocalGradientMap.insert(qobject_cast<RParameter *>(theDV), mapOf_dInput_dPar.value(theRVs.at(i)));
            }
        }

        theResponse->setGradientMap(theLocalGradientMap);
    }


    if (theOutputLevel == RObject::Maximum) {

        qDebug() << this->objectName() << "has response" << meanOfParameterFunction << "and gradients:";

        QMap<RParameter *, double> theGradMap = theResponse->getGradientMap();

        QMapIterator<RParameter *, double> i(theGradMap);
        while (i.hasNext())
        {
            i.next();

            qDebug()<<"For the parameter"<<i.key()->objectName()<<"the gradient value is: "<<i.value();
        }

    }



    return 1;
}


















