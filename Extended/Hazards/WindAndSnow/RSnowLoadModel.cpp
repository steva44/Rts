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

#include "RSnowLoadModel.h"
#include "RLoadPattern.h"
#include "RLinearSeries.h"
#include "RDomain.h"

RSnowLoadModel::RSnowLoadModel(QObject *parent, QString name)
    : RLoadPatternModel(parent, name)
{

}


RSnowLoadModel::~RSnowLoadModel()
{

}


QList<QPointer<RLoadPattern> > RSnowLoadModel::getTheLoadPatterns()
{

    if(theLoadPatterns.empty())
        this->initialize();

    return theLoadPatterns;
}


int RSnowLoadModel::initialize(void)
{
    // Check to make sure the core property objects are given
    auto theOpenSeesDomain = theDomain->getTheOpenSeesDomain();
    if(theOpenSeesDomain == nullptr)
    {
        qCritical()<<"Error: the OpenSees 'domain' was not set in "<<__FUNCTION__;
        return -1;
    }

    // Construct a linear time series since this is a static analysis
    theDomain->createObject(this->objectName()+"LinearTimeSeries","RLinearSeries");
    auto theSeries = qobject_cast<RLinearSeries*>(theDomain->getLastAddedObject());

    if(!theSeries)
    {
        qCritical() << "Failed to create a times series in RDomain in "<<__FUNCTION__;
        return -1;
    }

    theDomain->createObject(this->objectName()+"LoadPattern","RLoadPattern");
    theLoadPattern = qobject_cast<RLoadPattern*>(theDomain->getLastAddedObject());

    if(!theLoadPattern)
    {
        qCritical() << "Failed to create a load pattern in RDomain in "<<__FUNCTION__;
        return -1;
    }

    theLoadPattern->setTheTimeSeries(theSeries);

    theLoadPatterns.push_back(theLoadPattern);

    return 0;
}
