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

#include "RFORMModel.h"
#include "RFOSMModel.h"
#include "RSOSMModel.h"
#include "RRiskModel.h"
#include "RDomain.h"
#include "RMainWindow.h"
#include "RContinuousRandomVariable.h"
#include "RGenericResponse.h"

#include "qcustomplot.h"
#include <QCoreApplication>
#include <QPointer>
#include <QDateTime>

RRiskModel::RRiskModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theFORMModel = nullptr;
    theFOSMModel = nullptr;
    theSOSMModel = nullptr;
    theInputToTheUpstreamModel = nullptr;
    theOutputFromTheUpstreamModel = nullptr;
    theResponse = nullptr;
    customPlot = nullptr;

    isOrchestratingModel = true;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    theCostIncrement = 1000.0;
    theProbabilityTolerance = 0.005;
}


RRiskModel::~RRiskModel()
{

}


QObject * RRiskModel::getInput() const
{
    return theOutputFromTheUpstreamModel;
}


void RRiskModel::setInput(QObject *value)
{
    // Here we take the input as a starting point, and figur out which model it has, and which input that model has
    theOutputFromTheUpstreamModel = qobject_cast<RGenericResponse *>(value);

    if (theOutputFromTheUpstreamModel) {

        theFORMModel = qobject_cast<RFORMModel *>(theOutputFromTheUpstreamModel->getModel());
        theFOSMModel = qobject_cast<RFOSMModel *>(theOutputFromTheUpstreamModel->getModel());
        theSOSMModel = qobject_cast<RSOSMModel *>(theOutputFromTheUpstreamModel->getModel());

        if (theFORMModel) {

            theInputToTheUpstreamModel = qobject_cast<RGenericResponse *>(theFORMModel->getInputParameter());
        }
        else if (theFOSMModel) {

            theInputToTheUpstreamModel = qobject_cast<RGenericResponse *>(theFOSMModel->getInputParameter());
        }
        else if (theSOSMModel) {

            theInputToTheUpstreamModel = qobject_cast<RGenericResponse *>(theSOSMModel->getInputParameter());
        }
        else {
            qCritical() << this->objectName() << "requires the input to be either from a FORM model or a FOSM model.";
        }

        if (!theInputToTheUpstreamModel) {

            qCritical() << this->objectName() << "requires the input to the upstream model to be a generic response.";
        }

    }
    else {

        qCritical() << this->objectName() << "requires the input to be a generic response object.";
    }
}


double RRiskModel::getCostIncrement() const
{
    return theCostIncrement;
}


void RRiskModel::setCostIncrement(double value)
{
    theCostIncrement = value;
}


double RRiskModel::getProbabilityTolerance() const
{
    return theProbabilityTolerance;
}


void RRiskModel::setProbabilityTolerance(double value)
{
    theProbabilityTolerance = value;
}


int RRiskModel::evaluateModel(RGradientType theGradientType)
{

    QList<RContinuousRandomVariable *> randomVariableList = theDomain->findChildren<RContinuousRandomVariable *>();
    double mean = 0.0;

    if (theFOSMModel)
    {
        //  Run the FOSM model
        for (int i=0; i<randomVariableList.size(); i++)
        {
            randomVariableList[i]->setCurrentValue(randomVariableList.at(i)->getMeanValue());
        }

        theFOSMModel->runAnalysis(theGradientType);

        mean = theOutputFromTheUpstreamModel->getCurrentValue();

        theResponse->setCurrentValue(mean);

        if (theGradientType == RGradientType::DDM) {

            QMap<RParameter *, double> theLocalGradientMap;
            QMap<RParameter *, double> mapOf_dInput_dPar;

            double dMean_dInput = 1.0;
            mapOf_dInput_dPar = theOutputFromTheUpstreamModel->getGradientMap();


            QMapIterator<RParameter *, double> i(mapOf_dInput_dPar);

            while (i.hasNext())
            {
                i.next();
                double dInput_dPar = mapOf_dInput_dPar.value(i.key());
                double existingValue = theLocalGradientMap.value(i.key());

                //  qDebug()<<"In RISK: For the parameter "<<i.key()->objectName()<<" the upstream model gradient is: "<<dInput_dPar<<
                //            "and the existing value is"<< existingValue;

                theLocalGradientMap.insert(i.key(), existingValue + dMean_dInput*dInput_dPar);
            }
            theResponse->setGradientMap(theLocalGradientMap);
        }

        if (theOutputLevel == RObject::Maximum)
        {
            qDebug() << "The FOSM analysis"<<theFOSMModel->objectName()<<"in" << this->objectName() << "has a mean value of" <<mean;
            qDebug() << "";
        }
    }
    else if (theSOSMModel)
    {

        //  Run the SOSM model
        for (int i=0; i<randomVariableList.size(); i++)
        {
            randomVariableList[i]->setCurrentValue(randomVariableList.at(i)->getMeanValue());
        }

        theSOSMModel->runAnalysis(RGradientType::D3DM);

        mean = theOutputFromTheUpstreamModel->getCurrentValue();

        theResponse->setCurrentValue(mean);

        if (theGradientType >= RGradientType::D3DM) {

            QMap<RParameter *, double> theLocalGradientMap;
            QMap<RParameter *, double> mapOf_dInput_dPar;

            double dMean_dInput = 1.0;
            mapOf_dInput_dPar = theOutputFromTheUpstreamModel->getGradientMap();


            QMapIterator<RParameter *, double> i(mapOf_dInput_dPar);

            while (i.hasNext())
            {
                i.next();
                double dInput_dPar = mapOf_dInput_dPar.value(i.key());
                double existingValue = theLocalGradientMap.value(i.key());

                qDebug()<<"In RISK: For the parameter "<<i.key()->objectName()<<" the upstream model gradient is: "<<dInput_dPar<<
                          "and the existing value is"<< existingValue;

                theLocalGradientMap.insert(i.key(), existingValue + dMean_dInput*dInput_dPar);
            }
            theResponse->setGradientMap(theLocalGradientMap);
        }

        if (theOutputLevel == RObject::Maximum)
        {
            qDebug() << "The SOSM analysis"<<theSOSMModel->objectName()<<"in" << this->objectName() << "has a mean value of" <<mean;
            qDebug() << "";
        }

    }
    else if (theFORMModel)
    {

        double threshold, probability, previousProbability, theProbabilityNearestTheMean;
        QMap<RParameter *, double> theLocalGradientMap;
        QMap<RParameter *, double> mapOf_dProb_dPar;
        QMap<RParameter *, double> mapOfPrevious_dProb_dPar;

        QMap<RParameter *, double> mapOf_dProb_dParAtMean;
        double dMean_dProbability;
        int ii;


        // Create the FORM plot
        RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
        if (theOutputLevel >= RObject::Medium) {
            if (customPlot == nullptr) {

                customPlot = new QCustomPlot();
                mainWindow->addSubWindow(customPlot, "Exceedance Probability Curve", true);

                customPlot->addGraph();
                customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

                customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
                customPlot->xAxis->setLabel("Threshold");
                customPlot->yAxis->setLabel("Probability");
                customPlot->yAxis->setRange(0.0, 1.0);

            }
            else {

                customPlot->graph(0)->clearData();
            }
        }

        // Carry out FORM at thereshold near the mean estimated by FOSM
        RModel *theModelThatGivesInputToFORM = qobject_cast<RModel *>(theInputToTheUpstreamModel->getModel());
        QVector<double> backupOfCurrentValues(randomVariableList.size());
        for (int i=0; i<randomVariableList.size(); i++) {
            backupOfCurrentValues[i] = randomVariableList[i]->getCurrentValue();
            randomVariableList[i]->setCurrentValue(randomVariableList[i]->getMeanValue());
        }
        theModelThatGivesInputToFORM->runAnalysis(RGradientType::NONE);
        double meanEstimateFromFOSM = theInputToTheUpstreamModel->getCurrentValue();
        if (theCostIncrement > 0.3*meanEstimateFromFOSM*0.5) {
            qCritical() << "In" << this->objectName() << "the threshold increment seems very high; the stdv is roughly" << 0.3*meanEstimateFromFOSM;
        }
        int numIncrementsToMean = (double)(round(meanEstimateFromFOSM/theCostIncrement));
        double theThresholdNearestTheMean = numIncrementsToMean * theCostIncrement;

        theFORMModel->setThreshold( QString("%1").arg(theThresholdNearestTheMean, 0, 'g', 14));
        theDomain->clearAnalyzedFlags();
        theFORMModel->evaluateModel(theGradientType);
        theProbabilityNearestTheMean = theOutputFromTheUpstreamModel->getCurrentValue();

        probability = theProbabilityNearestTheMean;
        previousProbability = theProbabilityNearestTheMean;


        // Add point to plot
        if (theOutputLevel >= RObject::Medium) {
            customPlot->graph(0)->addData(theThresholdNearestTheMean, theProbabilityNearestTheMean);
            customPlot->xAxis->setRange(theThresholdNearestTheMean-theCostIncrement, theThresholdNearestTheMean+theCostIncrement);
            customPlot->replot();

        }


        if (theGradientType == RGradientType::DDM) {

            dMean_dProbability = theCostIncrement * 0.5;
            mapOf_dProb_dParAtMean = theOutputFromTheUpstreamModel->getGradientMap();

            QMapIterator<RParameter *, double> i(mapOf_dProb_dParAtMean);
            while (i.hasNext()) {
                i.next();
                double dProb_dPar = mapOf_dProb_dParAtMean.value(i.key());
                double existingValue = theLocalGradientMap.value(i.key());
                //  qDebug()<<"At the mean: For the parameter "<<i.key()->objectName()<<" the upstream model gradient is: "<<dProb_dPar<<
                //            "and the existing value is"<< existingValue;


                theLocalGradientMap.insert(i.key(), existingValue + dMean_dProbability*dProb_dPar);
            }

        }

        // Walk up the cost axis from the mean
        ii = 1;
        while (probability > theProbabilityTolerance)
        {

            // Keep GUI responsive
            QCoreApplication::processEvents();

            if(ii ==1)
            {
                mapOfPrevious_dProb_dPar = mapOf_dProb_dParAtMean;
            }


            // Run FORM
            threshold = theThresholdNearestTheMean + ii * theCostIncrement;

            theFORMModel->setThreshold( QString("%1").arg(threshold, 0, 'g', 14));
            theDomain->clearAnalyzedFlags();
            theFORMModel->evaluateModel(theGradientType);
            probability = theOutputFromTheUpstreamModel->getCurrentValue();

            // Add point to plot
            if (theOutputLevel >= RObject::Medium) {
                customPlot->graph(0)->addData(threshold, probability);
                customPlot->xAxis->setRange(theThresholdNearestTheMean-theCostIncrement, threshold+theCostIncrement);
                customPlot->replot();
            }


            // Add to the mean
            mean += theCostIncrement * 0.5 * (probability+previousProbability);
            previousProbability = probability;


            // DDM sensitivities
            if (theGradientType == RGradientType::DDM)
            {
                dMean_dProbability = theCostIncrement * 0.5;

                //The current probability map
                mapOf_dProb_dPar = theOutputFromTheUpstreamModel->getGradientMap();

                QMapIterator<RParameter *, double> i(mapOf_dProb_dPar);
                while (i.hasNext()) {

                    i.next();

                    double dProb_dPar = i.value();

                    double dProb_dPrevPar = mapOfPrevious_dProb_dPar.value(i.key());

                    double existingValue = theLocalGradientMap.value(i.key());

                    // qDebug()<<"From mean to infinity: For the parameter "<<i.key()->objectName()<<" the upstream model gradient is: "<<dProb_dPar<<
                    //           "and the existing value is"<< existingValue;

                    theLocalGradientMap.insert(i.key(), existingValue + dMean_dProbability*(dProb_dPar + dProb_dPrevPar));

                }

                // Set the previous probability map
                mapOfPrevious_dProb_dPar = mapOf_dProb_dPar;
            }

            ii++;
        }


        // Let's say the exceedance curve goes completely to zero a few increments further out
        mean += 3.0 * theCostIncrement * 0.5 * (previousProbability);

        // Walk down the cost axis, from the mean towards the origin
        ii = 1;
        previousProbability = theProbabilityNearestTheMean;
        double maxThreshold = threshold;
        while (probability < 1.0-theProbabilityTolerance) {

            if(ii ==1)
            {
                mapOfPrevious_dProb_dPar = mapOf_dProb_dParAtMean;
            }

            // Keep GUI responsive
            QCoreApplication::processEvents();

            // Run FORM
            threshold = theThresholdNearestTheMean - ii * theCostIncrement;
            theFORMModel->setThreshold( QString("%1").arg(threshold, 0, 'g', 14));
            theDomain->clearAnalyzedFlags();
            theFORMModel->evaluateModel(theGradientType);
            probability = theOutputFromTheUpstreamModel->getCurrentValue();


            // Add point to plot
            if (theOutputLevel >= RObject::Medium) {
                customPlot->graph(0)->addData(threshold, probability);
                customPlot->xAxis->setRange(threshold-theCostIncrement, maxThreshold+theCostIncrement);
                customPlot->replot();
            }


            // Add to the mean
            mean += theCostIncrement * 0.5 * (probability+previousProbability);
            previousProbability = probability;


            // DDM sensitivities
            if (theGradientType == RGradientType::DDM) {

                dMean_dProbability = theCostIncrement * 0.5;

                mapOf_dProb_dPar = theOutputFromTheUpstreamModel->getGradientMap();

                QMapIterator<RParameter *, double> i(mapOf_dProb_dPar);

                while (i.hasNext())
                {
                    i.next();

                    double dProb_dPar = i.value();

                    double dProb_dPrevPar = mapOfPrevious_dProb_dPar.value(i.key());

                    double existingValue = theLocalGradientMap.value(i.key());

                    // qDebug()<<"From mean to origin: For the parameter "<<i.key()->objectName()<<" the upstream model gradient is: "<<dProb_dPar<<
                    //           "and the existing value is"<< existingValue;

                    theLocalGradientMap.insert(i.key(), existingValue + dMean_dProbability*(dProb_dPar + dProb_dPrevPar));

                }

                // Set the previous probability map
                mapOfPrevious_dProb_dPar = mapOf_dProb_dPar;
            }
            ii++;
        }

        // Adding the contribution from zero up to the last threshold, assuming the curve goes to unity a few steps from there
        mean += (threshold - 3.0 * theCostIncrement);
        mean += 3.0 * theCostIncrement * 0.5 * (previousProbability + 1.0);



        // And when all is done, set the response and its gradient
        theResponse->setCurrentValue(mean);

        if (theGradientType == RGradientType::DDM) {
            theResponse->setGradientMap(theLocalGradientMap);
        }



        if (theOutputLevel == RObject::Maximum)
        {
            qDebug() << "The FORM analysis"<<theFORMModel->objectName()<<"in" << this->objectName() << "has a mean value of" <<mean;
            qDebug() << "";
        }

    }
    else
    {

        qCritical() << this->objectName() << "found neither a FORM model nor a FOSM model";
    }


    // The return value 1 means DDM is implemented...
    return 1;
}


