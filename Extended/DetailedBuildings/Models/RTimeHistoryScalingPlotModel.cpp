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

#include "RTimeHistoryScalingPlotModel.h"
#include "RParameter.h"
#include "RLoadBaseClass.h"
#include "RResponse.h"
#include "RModel.h"
#include "RComponent.h"
#include "RInfoRichResponse.h"
#include "RPlateComponent.h"
#include "RNodalLoad.h"

#include "UniformExcitation.h"
#include "GroundMotion.h"

RTimeHistoryScalingPlotModel::RTimeHistoryScalingPlotModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    isOrchestratingModel = true;
    
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    customPlot = nullptr;
}


RTimeHistoryScalingPlotModel::~RTimeHistoryScalingPlotModel()
{

}


QString RTimeHistoryScalingPlotModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}


void RTimeHistoryScalingPlotModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


QString RTimeHistoryScalingPlotModel::getTheLoadPattern()
{
    QString string;
    for (int i = 0; i < theLoadPatterns.count(); i++)
    {
        if (theLoadPatterns[i]) {
            string += theLoadPatterns[i]->objectName();
            string += "; ";
        } else {
            theLoadPatterns.removeAt(i);
            i--;
        }
    }

    return string;
}


void RTimeHistoryScalingPlotModel::setTheLoadPattern(QString value)
{
    QRegExp regExp = QRegExp(QString("[ ,;:\t\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator)));
    QStringList stringList = value.split(regExp, QString::SkipEmptyParts);

    for (int i = 0; i < stringList.count(); i++)
    {
        RLoadPattern *object = theDomain->findChild<RLoadPattern *>(stringList[i]);
        if (object)
        {
            theLoadPatterns.append(object);
        } else {
            qCritical() << "Error in" << objectName() << ": Could not find a load pattern named" << stringList[i] << "in the domain";
        }
    }
}


int RTimeHistoryScalingPlotModel::evaluateModel(RGradientType theGradientType)
{
    theResponse->setCurrentValue(0.0);

    // Create plot
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

    // Create the plot
    if (customPlot == nullptr) {

        customPlot = new QCustomPlot();
        mainWindow->addSubWindow(customPlot, "Plot", true);

        for(int i=0; i<theInputParameterList.size(); ++i)
        {
            customPlot->addGraph();
            customPlot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
            int r = rand() % 255;
            int g = rand() % 255;
            int b = rand() % 255;
            customPlot->graph(i)->setPen(QColor(r, g, b, 255));
        }

        customPlot->xAxis->setLabel("Ground Motion Scaling Factors");
        customPlot->yAxis->setLabel("Response Values");
    }


    // Get the models that serve input to this model
    QList<RModel *> inputModelList;
    for (int i=0; i<theInputParameterList.size(); i++) {
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameterList.at(i));
        inputModelList << qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    }

    // Re-generate occurrence times (we are called repeatedly by sampling, and we want a new scenario every time)
    QList<RModel *> theModels = theDomain->findChildren<RModel *>();
    for (int i=0; i<theModels.size(); i++) {
        theModels.at(i)->resetTime();
    }

    // Set the number of steps until we reach the final scaling factor
    auto numScalingFactors = 10;

    // Vectors to store the results
    QVector<double> scaleFactorVec(numScalingFactors);
    QVector< QVector<double> > responseVec(numScalingFactors, QVector<double>(theInputParameterList.size()));
    std::vector<double> maxGMAcc(theLoadPatterns.size());

    for(int i = 0; i<theLoadPatterns.size(); ++i)
    {
        auto theLoadPat = theLoadPatterns[i]->getTheLoadPattern();

        if(theLoadPat == nullptr)
        {
            qDebug()<<"Error in getting the OpenSees load pattern in "<<this->objectName()<<__FUNCTION__;
            continue;
        }

        //check if it is an earthquake load pattern
        auto uniformExcLoadPat = dynamic_cast<UniformExcitation*>(theLoadPat);
        if(uniformExcLoadPat)
        {
            auto gm = const_cast<GroundMotion*>(uniformExcLoadPat->getGroundMotion());

            auto peakAccel = gm->getPeakAccel();

            maxGMAcc[i] = fabs(peakAccel);
        }
    }


    for(int i = 0; i<numScalingFactors; ++i)
    {

        // A factor of "2.0" is here because we want to scale the GM up to 2 times its original acceleration
        double GMScalingFactor = 2.0*static_cast<double>(i+1)/static_cast<double>(numScalingFactors);

        for(int i = 0; i<theLoadPatterns.size(); ++i)
        {
            auto lpat = theLoadPatterns[i];

            auto theLoadPat = lpat->getTheLoadPattern();

            if(theLoadPat == nullptr)
            {
                qDebug()<<"Error in getting the OpenSees load pattern in "<<this->objectName()<<__FUNCTION__;
                continue;
            }

            // Check if it is an earthquake load pattern
            auto uniformExcLoadPat = dynamic_cast<UniformExcitation*>(theLoadPat);
            if(uniformExcLoadPat)
            {
                auto direction = uniformExcLoadPat->getDirection();

                // Only scale the horizontal directions
                // 1 - corresponds to translation along the global X axis
                // 2 - corresponds to translation along the global Y axis
                // 3 - corresponds to translation along the global Z axis
                // 4 - corresponds to rotation about the global X axis
                // 5 - corresponds to rotation about the global Y axis
                // 6 - corresponds to rotation about the global Z axis
                if(direction >= 3)
                    continue;

                // Get the peak acceleration for this direction
                if (theOutputLevel >= RObject::Medium)
                {
                    auto  peakAccel = maxGMAcc[i];
                    qDebug()<<"Scaling the ground motion "<<lpat->objectName()<<" with a peak ground acceleration of: "<<peakAccel<<", by a scale factor of: "<<GMScalingFactor;

                    auto newPGA = peakAccel*GMScalingFactor;
                    qDebug()<<"The new peak ground acceleration in the direction"<<direction<<" will be: "<<newPGA;
                }

                lpat->setCFactor(GMScalingFactor*9.81);
                lpat->update();
            }
        }


        // Run analysis in all models
        for (int i=0; i<inputModelList.size(); i++)
        {
            inputModelList.at(i)->runAnalysis(RGradientType::NONE);
        }


        // Save the values of the parameters
        for (int j=0; j<theInputParameterList.size(); j++)
        {

            auto parameterValue = theInputParameterList.at(j)->getCurrentValue();

            // qDebug()<<"For "<<theInputParameterList.at(j)->objectName()<<" the value is "<<parameterValue;

            responseVec[i][j] = parameterValue;

            // Output and save the analysis results
            qDebug()<<"For Step"<<i+1<<" the Sa at this step is "<<GMScalingFactor<<" and the value of "<<theInputParameterList[j]->objectName()<<" is "<<parameterValue;

            // Update the plots
            customPlot->graph(j)->addData(GMScalingFactor, parameterValue);

            // Rescale the axes and refresh the plot
            customPlot->rescaleAxes();
            customPlot->replot();
        }

        // Save the scaling factor
        scaleFactorVec[i] = GMScalingFactor;

        // Reset analysis in all models
        for (int i=0; i<inputModelList.size(); i++)
        {
            inputModelList.at(i)->reset();
        }

        // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
        theDomain->clearAnalyzedFlags();
    }


    // Create the header text for output
    QString headerStr = "Scaling Factor";

    for(int j = 0; j<theInputParameterList.size(); ++j)
    {
        headerStr.append(", "+ theInputParameterList[j]->objectName());
    }

    headerStr.append(", Step Number");

    qDebug()<< headerStr;

    for(int i = 0; i<numScalingFactors; ++i)
    {
        QString resultStr = QString::number(scaleFactorVec[i]);

        for(int j = 0; j<theInputParameterList.size(); ++j)
        {
            resultStr.append(","+QString::number(responseVec[i][j]));
        }

        resultStr.append(","+QString::number(i+1));

        qDebug()<<resultStr;
    }

    return 1;
}




