#ifndef RMultipointLineSourceModel_H	
#define RMultipointLineSourceModel_H	
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
class RContinuousRandomVariable;
	
class RMultipointLineSourceModel : public RModel	
{	
	Q_OBJECT
    Q_PROPERTY(QString PointLocationList READ getPointLocationList WRITE setPointLocationList)
	Q_PROPERTY(QObject *Depth READ getDepth WRITE setDepth)
	
public:	
	RMultipointLineSourceModel(QObject *parent, QString name);
	~RMultipointLineSourceModel();

	int evaluateModel(RGradientType theGradientType);
	
    QString getPointLocationList();
    void setPointLocationList(QString value);
	QObject *getDepth() const;
	void setDepth(QObject *value);

	QList<RParameter *> getPhysicalParameterList();

	QStringList getActionList();
	void reset();

private slots:
	void displayonMap();
	void removefromMap();

private:	
    QList<QPointer<RParameter> > thePointLocationList;
    QPointer<RContinuousRandomVariable> theTheta;
	QPointer<RParameter> theDepth;

	double theFaultLength;
	QVector<double> thePieceLengthVector;

	// Declare responses
    RLocationResponse *theEpicentreLocation;
};	

#endif	
