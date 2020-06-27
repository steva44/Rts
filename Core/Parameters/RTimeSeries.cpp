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

#include "RTimeSeries.h"
#include "RMainWindow.h"
#include "RTimeSeriesDialog.h"

#include <math.h>
#include <algorithm>
#include <iterator>
#include <vector>

#include "qcustomplot.h"

RTimeSeries::RTimeSeries(QObject *parent, QString name)
    : RParameter(parent, name)
{
    theTimeValues = nullptr;
    theXvalues    = nullptr;
    theYvalues    = nullptr;
    theZvalues    = nullptr;
    theRotXvalues = nullptr;
    theRotYvalues = nullptr;
    theRotZvalues = nullptr;
    theXParameterValues    = nullptr;
    theYParameterValues    = nullptr;
    theZParameterValues    = nullptr;

    lastIt = 0;
    tol = 1e-6;

    theTimeValues = std::make_unique<std::vector<double>>();
    theXvalues = std::make_unique<std::vector<double>>();
    theYvalues = std::make_unique<std::vector<double>>();
    theZvalues = std::make_unique<std::vector<double>>();

}


RTimeSeries::~RTimeSeries()
{

}


int RTimeSeries::setXYZvalues(double time, double x, double y, double z)
{

    theTimeValues->emplace_back(time);

    theXvalues->emplace_back(x);

    theYvalues->emplace_back(y);

    theZvalues->emplace_back(z);

    return 0;
}


int RTimeSeries::setXYZvalues(double time, RParameter* x, RParameter* y, RParameter* z)
{
    if(theXParameterValues==nullptr)
        theXParameterValues = std::make_unique<std::vector<RParameter*>>();
    if(theYParameterValues==nullptr)
        theYParameterValues = std::make_unique<std::vector<RParameter*>>();
    if(theZParameterValues==nullptr)
        theZParameterValues = std::make_unique<std::vector<RParameter*>>();

    theTimeValues->emplace_back(time);

    theXParameterValues->emplace_back(x);
    theYParameterValues->emplace_back(y);
    theZParameterValues->emplace_back(z);

    return 0;
}


int RTimeSeries::setXvalues(double time, double x)
{
    // Check if this time instant already exists
    int timeAlreadyExists = -1;


    for (int i=0; i<theTimeValues->size(); i++) {

        if (theTimeValues->at(i) == time) {
            timeAlreadyExists = i;
        }
    }


    // If the time instant did not exist, extend both vectors (this is strange; why was the x-vector not already created?)
    if (timeAlreadyExists == -1) {

        theTimeValues->emplace_back(time);
        theXvalues->emplace_back(x);

    }
    // If it did indeed exist, insert the y-value at the correct location
    else {
        (*theXvalues)[timeAlreadyExists] = x;
    }


    return 0;
}


int RTimeSeries::setYvalues(double time, double y)
{
    // Initial declarations
    int timeAlreadyExists = -1;

    // Check if this time instant already exists
    for (int i=0; i<theTimeValues->size(); i++) {

        if (theTimeValues->at(i) == time) {
            timeAlreadyExists = i;
        }
    }


    // If the time instant did not exist, extend both vectors (this is strange; why was the x-vector not already created?)
    if (timeAlreadyExists == -1) {

        theTimeValues->emplace_back(time);
        theYvalues->emplace_back(y);

    }
    // If it did indeed exist, insert the y-value at the correct location
    else {
        (*theYvalues)[timeAlreadyExists] = y;
    }


    return 0;
}


int RTimeSeries::setZvalues(double time, double z)
{
    // Check if this time instant already exists
    int timeAlreadyExists = -1;

    for (int i=0; i<theTimeValues->size(); i++) {

        if (theTimeValues->at(i) == time) {
            timeAlreadyExists = i;
        }
    }


    // If the time instant did not exist, extend both vectors (this is strange; why was the x-vector not already created?)
    if (timeAlreadyExists == -1) {

        theTimeValues->emplace_back(time);
        theZvalues->emplace_back(z);

    }
    // If it did indeed exist, insert the y-value at the correct location
    else {
        (*theZvalues)[timeAlreadyExists] = z;
    }

    return 0;
}


double RTimeSeries::getTimeItem(int item)
{
    if (theTimeValues!=0 && theTimeValues->size()>item) {

        return theTimeValues->at(item);
    }
    else {
        qCritical() << "Error in reading a time series item.";
        return 0.0;
    }
}


double RTimeSeries::getXitem(int item)
{
    if (theXvalues!=0 && theXvalues->size()>item) {

        return theXvalues->at(item);
    }
    else {
        qCritical() << "Error in reading a time series item.";
        return 0.0;
    }
}


double RTimeSeries::getYitem(int item)
{
    if (theYvalues != nullptr && theYvalues->size()>item) {

        return theYvalues->at(item);
    }
    else {
        qCritical() << "Error in reading a time series item.";
        return 0.0;
    }
}


double RTimeSeries::getZitem(int item)
{
    if (theZvalues != nullptr && theZvalues->size()>item) {

        return theZvalues->at(item);
    }
    else {
        qCritical() << "Error in reading a time series item.";
        return 0.0;
    }
}


double RTimeSeries::getXvalue(double time, RParameter* theDDMParameter1, RParameter* theDDMParameter2)
{
    // Check for consistency problems and zero-pointers
    if(!consistencyChecked)
        checkConsistency();

    for(int i=0;i<theTimeValues->size();++i)
    {
        auto timeVali = theTimeValues->at(i);

        if(fabs(timeVali - time)<tol)
               return theXvalues->at(i);

        if(timeVali < time)
            continue;

        // Then do interpolation
        auto timeBelow = theTimeValues->at(i-1);
        auto timeAbove = theTimeValues->at(i);

        double tRatio = (theXvalues->at(i) - theXvalues->at(i-1)) / (timeAbove - timeBelow);

        return theXvalues->at(i-1) + tRatio * (time-timeBelow);
    }

    // Then check if the input time is before the first item
    if (theTimeValues->front() > time)
    {
        return 0.0;
    }
    // Then check if the input time is after the last item
    else if (theTimeValues->back() < time) {

        return theXvalues->back();
    }
    else
    {
        // If we made it to here, then something went wrong
        qCritical() << "The time series could find a response for the time given.";
        return 0.0;
    }
}


double RTimeSeries::getYvalue(double time, RParameter* theDDMParameter1, RParameter* theDDMParameter2)
{
    // Check for consistency problems and zero-pointers
    if(!consistencyChecked)
        checkConsistency();

    for(int i=0;i<theTimeValues->size();++i)
    {
        auto timeVali = theTimeValues->at(i);

        if(fabs(timeVali - time)<tol)
               return theYvalues->at(i);

        if(timeVali < time)
            continue;

        // Then do interpolation
        auto timeBelow = theTimeValues->at(i-1);
        auto timeAbove = theTimeValues->at(i);

        double tRatio = (theYvalues->at(i) - theYvalues->at(i-1)) / (timeAbove - timeBelow);

        return theYvalues->at(i-1) + tRatio * (time-timeBelow);
    }

    // Then check if the input time is before the first item
    if (theTimeValues->front() > time)
    {
        return 0.0;
    }
    // Then check if the input time is after the last item
    else if (theTimeValues->back() < time) {

        return theYvalues->back();
    }
    else
    {
        // If we made it to here, then something went wrong
        qCritical() << "The time series could find a response for the time given.";
        return 0.0;
    }
}


double RTimeSeries::getZvalue(double time, RParameter* theDDMParameter1, RParameter* theDDMParameter2)
{

    // Check for consistency problems and zero-pointers
    if(!consistencyChecked)
        checkConsistency();

    for(int i=0;i<theTimeValues->size();++i)
    {
        auto timeVali = theTimeValues->at(i);

        if(fabs(timeVali - time)<tol)
               return theZvalues->at(i);

        if(timeVali < time)
            continue;

        // Then do interpolation
        auto timeBelow = theTimeValues->at(i-1);
        auto timeAbove = theTimeValues->at(i);

        double tRatio = (theZvalues->at(i) - theZvalues->at(i-1)) / (timeAbove - timeBelow);

        return theZvalues->at(i-1) + tRatio * (time-timeBelow);
    }

    // Then check if the input time is before the first item
    if (theTimeValues->front() > time)
    {
        return 0.0;
    }
    // Then check if the input time is after the last item
    else if (theTimeValues->back() < time) {

        return theZvalues->back();
    }
    else
    {
        // If we made it to here, then something went wrong
        qCritical() << "The time series could find a response for the time given.";
        return 0.0;
    }
}


std::vector<double> RTimeSeries::getXYZvalues(double time, RParameter* theDDMParameter1, RParameter* theDDMParameter2)
{
    // Check for consistency problems and zero-pointers
    if(!consistencyChecked)
        checkConsistency();

    std::vector<double> valueVec = {0.0,0.0,0.0};

    // Check for consistency problems and zero-pointers
    if(!consistencyChecked)
        checkConsistency();

    for(int i=0;i<theTimeValues->size();++i)
    {
        auto timeVali = theTimeValues->at(i);

        if(fabs(timeVali - time)<tol)
        {
            valueVec[0]=theXvalues->at(i);
            valueVec[1]=theYvalues->at(i);
            valueVec[2]=theZvalues->at(i);

            return valueVec;
        }

        if(timeVali < time)
            continue;

        // Then do interpolation
        auto timeBelow = theTimeValues->at(i-1);
        auto timeAbove = theTimeValues->at(i);

        double txRatio = (theXvalues->at(i) - theXvalues->at(i-1)) / (timeAbove - timeBelow);
        double tyRatio = (theYvalues->at(i) - theYvalues->at(i-1)) / (timeAbove - timeBelow);
        double tzRatio = (theZvalues->at(i) - theZvalues->at(i-1)) / (timeAbove - timeBelow);

        valueVec[0] = theXvalues->at(i-1) + txRatio * (time-timeBelow);
        valueVec[1] = theYvalues->at(i-1) + tyRatio * (time-timeBelow);
        valueVec[2] = theZvalues->at(i-1) + tzRatio * (time-timeBelow);

        return valueVec;
    }

    // Then check if the input time is before the first item
    if (theTimeValues->front() > time)
    {
        return valueVec;
    }
    // Then check if the input time is after the last item
    else if (theTimeValues->back() < time) {

        valueVec[0] = theXvalues->back();
        valueVec[1] = theYvalues->back();
        valueVec[2] = theZvalues->back();

        return valueVec;
    }
    else
    {
        // If we made it to here, then something went wrong
        qCritical() << "The time series could find a response for the time given.";
        return valueVec;
    }





}


QStringList RTimeSeries::getActionList()
{
    QStringList actionList;
    actionList << "&Load File" << "Separator" <<
                  "Plot &3D" << "Plot &X" << "Plot &Y" << "Plot &Z" << "Separator" <<
                  "Plot Int 3D" << "Plot Int X" << "Plot Int Y" << "Plot Int Z" << "Separator" <<
                  "Plot Dbl Int 3D" << "Plot Dbl Int X" << "Plot Dbl Int Y" << "Plot Dbl Int Z" << "Separator" << "Print" << "Separator" << "&Help";
    return actionList;
}


void RTimeSeries::print()
{
    checkConsistency();

    if (theTimeValues!=nullptr && theXvalues!=nullptr && theYvalues!=nullptr && theZvalues!=nullptr) {
        qDebug() << "The time series, printed as t, x, y, z is:";
        for (size_t i=0; i<theTimeValues->size(); i++) {
            qDebug() << theTimeValues->at(i) << "   " << theXvalues->at(i) << "   " << theYvalues->at(i) << "   " << theZvalues->at(i);
        }
    }
    else if (theTimeValues!=nullptr && theYvalues!=nullptr && theZvalues!=nullptr) {
        qDebug() << "The time series, printed as t, y, z is:";
        for (size_t i=0; i<theTimeValues->size(); i++) {
            qDebug() << theTimeValues->at(i) << "   " << theYvalues->at(i) << "   " << theZvalues->at(i);
        }
    }
    else if (theTimeValues!=nullptr && theXvalues!=nullptr && theZvalues!=nullptr) {
        qDebug() << "The time series, printed as t, x, z is:";
        for (size_t i=0; i<theTimeValues->size(); i++) {
            qDebug() << theTimeValues->at(i) << "   " << theXvalues->at(i) << "   " << theZvalues->at(i);
        }
    }
    else if (theTimeValues!=nullptr && theXvalues!=nullptr && theYvalues!=nullptr) {
        qDebug() << "The time series, printed as t, x, y is:";
        for (size_t i=0; i<theTimeValues->size(); i++) {
            qDebug() << theTimeValues->at(i) << "   " << theXvalues->at(i) << "   " << theYvalues->at(i);
        }
    }
    else if (theTimeValues!=nullptr && theXvalues!=nullptr) {
        qDebug() << "The time series, printed as t, x is:";
        for (size_t i=0; i<theTimeValues->size(); i++) {
            qDebug() << theTimeValues->at(i) << "   " << theXvalues->at(i);
        }
    }
    else if (theTimeValues!=nullptr && theYvalues!=nullptr) {
        qDebug() << "The time series, printed as t, y is:";
        for (size_t i=0; i<theTimeValues->size(); i++) {
            qDebug() << theTimeValues->at(i) << "   " << theYvalues->at(i);
        }
    }
    else if (theTimeValues!=nullptr && theZvalues!=nullptr) {
        qDebug() << "The time series, printed as t, z is:";
        for (size_t i=0; i<theTimeValues->size(); i++) {
            qDebug() << theTimeValues->at(i) << "   " << theZvalues->at(i);
        }
    }
    else {
        qCritical() << "No time series data available for print.";
    }
}


void RTimeSeries::loadFile()
{
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(this->parent());

    RTimeSeriesDialog *dialog = new RTimeSeriesDialog(mainWindow, theDomain, this);
    if (dialog->exec()) {

    }
    delete dialog;

}


void RTimeSeries::plot3D()
{
    // Check that the vectors that exist are consistent in size
    checkConsistency();



    // Check that all the needed data are present
    if (theXvalues==nullptr && theYvalues==nullptr && theZvalues==nullptr) {
        qCritical() << "Insufficient data to plot in the Time Series called " << this->objectName();
        return;
    }


    // Create the plot
    qCritical() << "Only the plot x method in the time series is correct now...";
}


void RTimeSeries::plotX()
{
    // Check that the vectors that exist are consistent in size
    checkConsistency();


    // Check that all the needed data are present
    if (theTimeValues==nullptr && theXvalues==nullptr) {
        qCritical() << "Insufficient data to plot in the Time Series called " << this->objectName();
        return;
    }



    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    QCustomPlot *customPlot = new QCustomPlot();
    mainWindow->addSubWindow(customPlot, "Time Series Plot", true);

    customPlot->addGraph();

    QVector<double> timeValuesGraph = QVector<double>::fromStdVector((*theTimeValues));
    QVector<double> xValuesGraph = QVector<double>::fromStdVector((*theXvalues));

    customPlot->graph(0)->setData(timeValuesGraph, xValuesGraph);

    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel("X");

    customPlot->xAxis->rescale();
    customPlot->yAxis->rescale();

    customPlot->rescaleAxes();

    customPlot->replot();
}


void RTimeSeries::plotY()
{
    qCritical() << "Only the plot x method in the time series is implemented now...";
}


void RTimeSeries::plotZ()
{
    qCritical() << "Only the plot x method in the time series is implemented now...";
}


void RTimeSeries::plotInt3D()
{
    // Check that the vectors that exist are consistent in size
    checkConsistency();



    // Check that all the needed data are present
    if (theXvalues==nullptr && theYvalues==nullptr && theZvalues==nullptr) {
        qCritical() << "Insufficient data to plot in the Time Series called " << this->objectName();
        return;
    }


    // Create the plot

    int k= theTimeValues->size();

    QVector<double> theXValuesInt(k);
    QVector<double> theYValuesInt(k);
    QVector<double> theZValuesInt(k);

    theXValuesInt[0] =0;
    theYValuesInt[0] =0;
    theZValuesInt[0] =0;


    //integration using trapezoidal rule

    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theXValuesInt[i+1]=(theXValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theXvalues->at(i+1))+(theXvalues->at(i)))*0.5);
        theYValuesInt[i+1]=(theYValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theYvalues->at(i+1))+(theYvalues->at(i)))*0.5);
        theZValuesInt[i+1]=(theZValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theZvalues->at(i+1))+(theZvalues->at(i)))*0.5);
    }

    qCritical() << "Only the plot x method in the time series is correct now...";

}


void RTimeSeries::plotIntX()
{
    // Check that the vectors that exist are consistent in size
    checkConsistency();


    // Check that all the needed data are present
    if (theTimeValues==0 && theXvalues==0) {
        qCritical() << "Insufficient data to plot in the Time Series called " << this->objectName();
        return;
    }


    // Create the plot

    int k= theTimeValues->size();

    QVector<double> theXValuesInt(k);

    theXValuesInt[0] =0;


    //integration using trapezoidal rule

    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theXValuesInt[i+1]=(theXValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theXvalues->at(i+1))+(theXvalues->at(i)))*0.5);
        qDebug() << "The integral is" << theXValuesInt[i+1];
    }


    qCritical() << "Only the plot x method in the time series is correct now...";
}


void RTimeSeries::plotIntY()
{
    // Check that the vectors that exist are consistent in size
    checkConsistency();



    // Check that all the needed data are present
    if (theTimeValues==nullptr && theYvalues==nullptr) {
        qCritical() << "Insufficient data to plot in the Time Series called " << this->objectName();
        return;
    }


    // Create the plot

    int k= theTimeValues->size();
    QVector<double> theYValuesInt(k);
    theYValuesInt[0] =0;

    //integration using trapezoidal rule
    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theYValuesInt[i+1]=(theYValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theYvalues->at(i+1))+(theYvalues->at(i)))*0.5);
        qDebug() << "The integral is" << theYValuesInt[i+1];
    }


    qCritical() << "Only the plot x method in the time series is correct now...";
}


void RTimeSeries::plotIntZ()
{
    // Check that the vectors that exist are consistent in size
    checkConsistency();


    // Check that all the needed data are present
    if (theTimeValues==0 && theZvalues==0) {
        qCritical() << "Insufficient data to plot in the Time Series called " << this->objectName();
        return;
    }


    // Create the plot

    int k= theTimeValues->size();
    QVector<double> theZValuesInt(k);
    theZValuesInt[0] =0;

    //integration using trapezoidal rule
    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theZValuesInt[i+1]=(theZValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theZvalues->at(i+1))+(theZvalues->at(i)))*0.5);
        qDebug() << "The integral is" << theZValuesInt[i+1];
    }


    qCritical() << "Only the plot x method in the time series is correct now...";
}


void RTimeSeries::plotDblInt3D()
{
    // Check that the vectors that exist are consistent in size
    checkConsistency();


    // Check that all the needed data are present
    if (theXvalues==nullptr && theYvalues==nullptr && theZvalues==nullptr) {
        qCritical() << "Insufficient data to plot in the Time Series called " << this->objectName();
        return;
    }


    // Create the plot

    int k= theTimeValues->size();

    QVector<double> theXValuesInt(k); QVector<double> theXValuesDbInt(k);
    QVector<double> theYValuesInt(k); QVector<double> theYValuesDbInt(k);
    QVector<double> theZValuesInt(k); QVector<double> theZValuesDbInt(k);

    theXValuesInt[0] =0; theXValuesDbInt[0]=0;
    theYValuesInt[0] =0; theYValuesDbInt[0]=0;
    theZValuesInt[0] =0; theZValuesDbInt[0]=0;


    //integration using trapezoidal rule
    //velocity
    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theXValuesInt[i+1]=(theXValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theXvalues->at(i+1))+(theXvalues->at(i)))*0.5);
        theYValuesInt[i+1]=(theYValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theYvalues->at(i+1))+(theYvalues->at(i)))*0.5);
        theZValuesInt[i+1]=(theZValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theZvalues->at(i+1))+(theZvalues->at(i)))*0.5);
    }
    //displacement
    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theXValuesDbInt[i+1]=(theXValuesDbInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theXValuesInt[i+1])+(theXValuesInt[i]))*0.5);
        theYValuesDbInt[i+1]=(theYValuesDbInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theYValuesInt[i+1])+(theYValuesInt[i]))*0.5);
        theZValuesDbInt[i+1]=(theZValuesDbInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theZValuesInt[i+1])+(theZValuesInt[i]))*0.5);
    }

    qCritical() << "Only the plot x method in the time series is correct now...";
}


void RTimeSeries::plotDblIntX()
{
    // Check that the vectors that exist are consistent in size
    checkConsistency();


    // Check that all the needed data are present
    if (theTimeValues==nullptr && theXvalues==nullptr) {
        qCritical() << "Insufficient data to plot in the Time Series called " << this->objectName();
        return;
    }


    // Create the plot

    int k= theTimeValues->size();

    QVector<double> theXValuesInt(k); QVector<double> theXValuesDbInt(k);

    theXValuesInt[0] =0; theXValuesDbInt[0]=0;

    //integration using trapezoidal rule
    //velocity
    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theXValuesInt[i+1]=(theXValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theXvalues->at(i+1))+(theXvalues->at(i)))*0.5);
    }
    //displacement
    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theXValuesDbInt[i+1]=(theXValuesDbInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theXValuesInt[i+1])+(theXValuesInt[i]))*0.5);
    }


    qCritical() << "Only the plot x method in the time series is correct now...";
}


void RTimeSeries::plotDblIntY()
{
    // Check that the vectors that exist are consistent in size
    checkConsistency();



    // Check that all the needed data are present
    if (theTimeValues==nullptr && theYvalues==nullptr) {
        qCritical() << "Insufficient data to plot in the Time Series called " << this->objectName();
        return;
    }


    // Create the plot

    int k= theTimeValues->size();

    QVector<double> theYValuesInt(k); QVector<double> theYValuesDbInt(k);

    theYValuesInt[0] =0; theYValuesDbInt[0]=0;

    //integration using trapezoidal rule
    //velocity
    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theYValuesInt[i+1]=(theYValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theYvalues->at(i+1))+(theYvalues->at(i)))*0.5);
    }
    //displacement
    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theYValuesDbInt[i+1]=(theYValuesDbInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theYValuesInt[i+1])+(theYValuesInt[i]))*0.5);
    }

    qCritical() << "Only the plot x method in the time series is correct now...";
}


void RTimeSeries::plotDblIntZ()
{
    // Check that the vectors that exist are consistent in size
    checkConsistency();


    // Check that all the needed data are present
    if (theTimeValues==0 && theZvalues==0) {
        qCritical() << "Insufficient data to plot in the Time Series called " << this->objectName();
        return;
    }


    // Create the plot

    int k= theTimeValues->size();

    QVector<double> theZValuesInt(k); QVector<double> theZValuesDbInt(k);

    theZValuesInt[0] =0; theZValuesDbInt[0]=0;

    //integration using trapezoidal rule
    //velocity
    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theZValuesInt[i+1]=(theZValuesInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theZvalues->at(i+1))+(theZvalues->at(i)))*0.5);
    }
    //displacement
    for (int i=0; i<theTimeValues->size()-1; i++)  {
        theZValuesDbInt[i+1]=(theZValuesDbInt[i]+((theTimeValues->at(i+1))-(theTimeValues->at(i)))*((theZValuesInt[i+1])+(theZValuesInt[i]))*0.5);
    }

    qCritical() << "Only the plot x method in the time series is correct now...";
}


int RTimeSeries::getNumEntries()
{
    return theTimeValues->size();
}


void RTimeSeries::checkConsistency()
{
    //Only check for consistency once
    if(consistencyChecked)
        return;

    size_t timeSize = 0;
    if (theTimeValues!=nullptr) {
        timeSize = theTimeValues->size();

        // And check that the time instants are in consequtive order...
    }

    if (theXvalues!= nullptr) {
        if(!theXvalues->empty())
            if (theXvalues->size() != timeSize) {
                qDebug() << "The number of X-values the Time Series called " << this->objectName() << " is not matching the number of time instants.";
            }
    }

    if (theYvalues!= nullptr) {
        if(!theYvalues->empty())
            if (theYvalues->size() != timeSize) {
                qDebug() << "The number of Y-values the Time Series called " << this->objectName() << " is not matching the number of time instants.";
            }
    }

    if (theZvalues!= nullptr) {
        if(!theZvalues->empty())
            if (theZvalues->size() != timeSize) {
                qDebug() << "The number of Z-values the Time Series called " << this->objectName() << " is not matching the number of time instants.";
            }
    }

    if (theRotXvalues!= nullptr) {
        if (theRotXvalues->size() != timeSize) {
            qDebug() << "The number of X-rotation-values the Time Series called " << this->objectName() << " is not matching the number of time instants.";
        }
    }

    if (theRotYvalues!= nullptr) {
        if (theRotYvalues->size() != timeSize) {
            qDebug() << "The number of Y-rotation-values the Time Series called " << this->objectName() << " is not matching the number of time instants.";
        }
    }

    if (theRotZvalues!= nullptr) {
        if (theRotZvalues->size() != timeSize) {
            qDebug() << "The number of Z-rotation-values the Time Series called " << this->objectName() << " is not matching the number of time instants.";
        }
    }

    consistencyChecked = true;


}


void RTimeSeries::clean()
{
    theTimeValues.release();
    theXvalues.release();
    theYvalues.release();
    theZvalues.release();
    theRotXvalues.release();
    theRotYvalues.release();
    theRotZvalues.release();

    theTimeValues = std::make_unique<std::vector<double>>();
    theXvalues = std::make_unique<std::vector<double>>();
    theYvalues = std::make_unique<std::vector<double>>();
    theZvalues = std::make_unique<std::vector<double>>();
    theRotXvalues = nullptr;
    theRotYvalues = nullptr;
    theRotZvalues = nullptr;
}
