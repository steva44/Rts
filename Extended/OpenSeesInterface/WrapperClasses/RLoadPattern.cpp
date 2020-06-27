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

#include "RLoadPattern.h"
#include "ROpenSeesTimeSeries.h"
#include "TimeSeries.h"
#include "LoadPattern.h"
#include "Domain.h"

RLoadPattern::RLoadPattern(QObject *parent, QString name)
    : RObject(parent, name)
{
    theLoadPattern = nullptr;
    theTimeSeries = nullptr;
}


RLoadPattern::~RLoadPattern()
{
    auto OpenSeesDomain = this->getTheOpenSeesDomain();

    if(OpenSeesDomain != nullptr)
    {
         OpenSeesDomain->removeLoadPattern(this->getObjectTag());
    }
}


QObject *RLoadPattern::getTheTimeSeries() const
{
    return theTimeSeries;
}


void RLoadPattern::setTheTimeSeries(QObject *value)
{
    theTimeSeries = qobject_cast<ROpenSeesTimeSeries*>(value);
}


int RLoadPattern::update(void)
{
    return 0;
}


double RLoadPattern::getCFactor() const
{
    return  1.0;
}


void RLoadPattern::setCFactor(double value)
{

}


LoadPattern* RLoadPattern::getTheLoadPattern()
{
    if(!theLoadPattern)
    {
        theLoadPattern = new LoadPattern(objectTag);

        if(theTimeSeries)
        {
            auto timeSeries = theTimeSeries->getTheTimeSeries();
            theLoadPattern->setTimeSeries(timeSeries);
        }
        else
        {
            qDebug()<<"The load pattern requires a time series in "<<__FUNCTION__;
            return nullptr;
        }
    }

    return theLoadPattern;
}




