#ifndef RLoadPattern_H
#define RLoadPattern_H
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

// Wrapper class for OpenSees theLoadPattern
// Stevan Gavrilovic 12/2018

#include "RObject.h"

class ROpenSeesTimeSeries;
class LoadPattern;

class RLoadPattern : public RObject
{
    Q_OBJECT

    Q_PROPERTY(QObject *TimeSeries READ getTheTimeSeries WRITE setTheTimeSeries)

public:
    RLoadPattern(QObject *parent, QString name);
    virtual ~RLoadPattern();

    virtual LoadPattern* getTheLoadPattern();

    virtual int update(void);

    virtual double getCFactor() const;
    virtual void setCFactor(double value);

    QObject *getTheTimeSeries() const;
    void setTheTimeSeries(QObject *value);

protected:

    ROpenSeesTimeSeries* theTimeSeries;

    //Raw ptr; OpenSees controls the destruction, otherwise program crashes on exit
    LoadPattern* theLoadPattern;

};

#endif
