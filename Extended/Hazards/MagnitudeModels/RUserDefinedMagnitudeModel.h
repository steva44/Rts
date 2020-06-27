#ifndef RUserDefinedMagnitudeModel_H
#define RUserDefinedMagnitudeModel_H
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
*                                                                    *
*********************************************************************/

#include "RModel.h"
#include "RContinuousRandomVariable.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RTime.h"

class RUserDefinedMagnitudeModel : public RModel
{
	Q_OBJECT
    Q_PROPERTY(QString XPoints READ getXPoints WRITE setXPoints)
    Q_PROPERTY(QString PDFPoints READ getPDFPoints WRITE setPDFPoints)
    Q_PROPERTY(QObject *Occurrence READ getOccurrence WRITE setOccurrence)

public:
	RUserDefinedMagnitudeModel(QObject *parent, QString name);
	~RUserDefinedMagnitudeModel();

    QString getXPoints();
    void setXPoints(QString value);

    QString getPDFPoints();
    void setPDFPoints(QString value);

    QObject *getOccurrence() const;
    void setOccurrence(QObject *value);

	int evaluateModel(RGradientType theGradientType);

private:

    QPointer<RContinuousRandomVariable> theStandardNormalRandomVariable;
    QString theXPoints;
    QString thePDFPoints;
    RResponse *theMagnitude;
    QPointer<RParameter> theOccurrence;

};

#endif
