#ifndef RPolygonalAreaSourceModel_H	
#define RPolygonalAreaSourceModel_H	
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
	
class RParameter;	
class RLocationResponse;
class RLocation;
class RContinuousRandomVariable;
	
class RPolygonalAreaSourceModel : public RModel	
{	
	Q_OBJECT
    Q_PROPERTY(QString BoundaryLocationList READ getBoundaryLocationList WRITE setBoundaryLocationList)
	Q_PROPERTY(QObject *Depth READ getDepth WRITE setDepth)

public:	
	RPolygonalAreaSourceModel(QObject *parent, QString name);
	~RPolygonalAreaSourceModel();

	int evaluateModel(RGradientType theGradientType);
	
    QString getBoundaryLocationList();
    void setBoundaryLocationList(QString value);

	QObject *getDepth() const;
	void setDepth(QObject *value);

	QList<RParameter *> getPhysicalParameterList();
	void reset();

	QStringList getActionList();

private slots:
	void displayonMap();
	void removefromMap();
	
private:	
    QList<QPointer<RParameter> > theBoundaryLocationList;
    QPointer<RContinuousRandomVariable> theTheta1;
    QPointer<RContinuousRandomVariable> theTheta2;
	QPointer<RParameter> theDepth;

	// Declare responses
    RLocationResponse *theHypocentreLocation;

	// Internal data members
	RContinuousRandomVariable *theLatitudeRandomVariable;

};	

#endif	
