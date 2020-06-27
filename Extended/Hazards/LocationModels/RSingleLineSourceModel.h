#ifndef RSingleLineSourceModel_H	
#define RSingleLineSourceModel_H
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
	
class RParameter;
class RLocation;
class RLocationResponse;
	
class RSingleLineSourceModel : public RModel	
{	
	Q_OBJECT
    Q_PROPERTY(QObject *Point1Location READ getPoint1Location WRITE setPoint1Location)
    Q_PROPERTY(QObject *Point2Location READ getPoint2Location WRITE setPoint2Location)
	Q_PROPERTY(QObject *Theta READ getTheta WRITE setTheta)
	Q_PROPERTY(QObject *Depth READ getDepth WRITE setDepth)
	
public:	
	RSingleLineSourceModel(QObject *parent, QString name);
	~RSingleLineSourceModel();

	int evaluateModel(RGradientType theGradientType);

    QObject *getPoint1Location() const;
    void setPoint1Location(QObject *value);
    QObject *getPoint2Location() const;
    void setPoint2Location(QObject *value);
	QObject *getTheta() const;
	void setTheta(QObject *value);
	QObject *getDepth() const;
	void setDepth(QObject *value);

	QList<RParameter *> getPhysicalParameterList();

	QStringList getActionList();

private slots:
	void displayonMap();
	void removefromMap();

private:	
    QPointer<RParameter> thePoint1Location;
    QPointer<RParameter> thePoint2Location;
	QPointer<RParameter> theTheta;
	QPointer<RParameter> theDepth;

	// Declare responses
    RLocationResponse *theHypocentreLocation;
};	

#endif	
