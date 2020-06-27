#ifndef RIntensityModel_H
#define RIntensityModel_H	
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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RModel.h"	

class RParameter;	
class RResponse;
class RLocation;
class RBIM;

class RIntensityModel : public RModel
{	
    Q_OBJECT
    Q_ENUMS(RResponseType)
    Q_PROPERTY(QString ResponseType READ getResponseType WRITE setResponseType)
#ifdef DETAILEDBUILDINGS
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
#endif

public:	
    RIntensityModel(QObject *parent, QString name);
    ~RIntensityModel();

    QString getResponseType();
    void setResponseType(QString value);

    int resetTime();

    void setPeriod(double value);

    void setLocation(QObject *value);

    double getRjb();

    void setIntensityMeasure(QString theIntensityMeasure);

    virtual bool isOn() = 0;

    enum RResponseType{PGA, PGD, PGV, Sa};

#ifdef DETAILEDBUILDINGS
    QObject *getBIM() const;
    void setBIM(QObject *value);
#endif

protected:

    double thePeriod;
    double theRjb;
    QPointer<RLocation> theLocation;
    RResponseType theResponseType;
#ifdef DETAILEDBUILDINGS
    QPointer<RBIM> theBIM;
#endif

};

#endif
