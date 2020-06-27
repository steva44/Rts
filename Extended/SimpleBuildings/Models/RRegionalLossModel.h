#ifndef RRegionalLossModel_H
#define RRegionalLossModel_H
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
#include "RProbabilityDistributions.h"

#include <QColor>

class RParameter;
class RResponse;
class RLocation;

class RRegionalLossModel : public RModel
{
	Q_OBJECT

    Q_PROPERTY(QString BoundaryLocationList READ getBoundaryLocationList WRITE setBoundaryLocationList)
    Q_PROPERTY(QObject *CentroidLocation READ getCentroidLocation WRITE setCentroidLocation)
	Q_PROPERTY(QObject *Sa1 READ getSa1 WRITE setSa1)
	Q_PROPERTY(QObject *Sa2 READ getSa2 WRITE setSa2)
	Q_PROPERTY(QString UnitAreaCostList READ getUnitAreaCostList WRITE setUnitAreaCostList)
	Q_PROPERTY(QString AreaRatioList READ getAreaRatioList WRITE setAreaRatioList)
	Q_PROPERTY(QObject *Time READ getTime WRITE setTime)
	Q_PROPERTY(QObject *AnnualInterestRate READ getAnnualInterestRate WRITE setAnnualInterestRate)
	Q_PROPERTY(QString Theta1List READ getTheta1List WRITE setTheta1List)
	Q_PROPERTY(QString Theta2List READ getTheta2List WRITE setTheta2List)
	Q_PROPERTY(QString Theta3List READ getTheta3List WRITE setTheta3List)
	Q_PROPERTY(QString Theta4List READ getTheta4List WRITE setTheta4List)
	Q_PROPERTY(QString Theta5List READ getTheta5List WRITE setTheta5List)
	Q_PROPERTY(QString EpsilonList READ getEpsilonList WRITE setEpsilonList)
	Q_PROPERTY(QColor Color READ getColor WRITE setColor)
	Q_PROPERTY(double TotalArea READ getTotalArea)
    Q_PROPERTY(QObject *ProbabilityDistributions READ getProbabilityDistributions WRITE setProbabilityDistributions)


public:
	RRegionalLossModel(QObject *parent, QString name);
	~RRegionalLossModel();

    QString getBoundaryLocationList();
    void setBoundaryLocationList(QString value);
    QObject *getCentroidLocation() const;
    void setCentroidLocation(QObject *value);
	QObject *getSa1() const;
	void setSa1(QObject *value);
	QObject *getSa2() const;
	void setSa2(QObject *value);
	QString getUnitAreaCostList();
	void setUnitAreaCostList(QString value);
	QString getAreaRatioList();
	void setAreaRatioList(QString value);
	QObject *getTime() const;
	void setTime(QObject *value);
	QObject *getAnnualInterestRate() const;
	void setAnnualInterestRate(QObject *value);
	QString getTheta1List();
	void setTheta1List(QString value);
	QString getTheta2List();
	void setTheta2List(QString value);
	QString getTheta3List();
	void setTheta3List(QString value);
	QString getTheta4List();
	void setTheta4List(QString value);
	QString getTheta5List();
	void setTheta5List(QString value);
	QString getEpsilonList();
	void setEpsilonList(QString value);
	QColor getColor() const;
	void setColor(QColor value);
	double getTotalArea();
    QObject *getProbabilityDistributions() const;
    void setProbabilityDistributions(QObject *value);


	int evaluateModel(RGradientType theGradientType);

	// A method to determine if a locagtion falls within this sector
    bool hasInside(RLocation *passedLocation);

	QStringList getActionList();
	void reset();

public slots:
	void displayonMap();

private slots:
	void removefromMap();
    void displayLocationsInside();

private:
	void calculateArea();
	void calculateCentroid();

    QList<QPointer<RParameter> > theBoundaryLocationList;
    QPointer<RParameter> theCentroidLocation;
	QPointer<RParameter> theSa1;
	QPointer<RParameter> theSa2;
	QList<QPointer<RParameter> > theUnitAreaCostList;
	QList<QPointer<RParameter> > theAreaRatioList;
	QPointer<RParameter> theTime;
	QPointer<RParameter> theAnnualInterestRate;
	QList<QPointer<RParameter> > theTheta1List;
	QList<QPointer<RParameter> > theTheta2List;
	QList<QPointer<RParameter> > theTheta3List;
	QList<QPointer<RParameter> > theTheta4List;
	QList<QPointer<RParameter> > theTheta5List;
	QList<QPointer<RParameter> > theEpsilonList;
	QColor theColor;
	double theTotalArea;
    QPointer<RProbabilityDistributions> theProbabilityDistributions;


	double theInitialTime;

	RResponse *theLoss;

};

#endif // RRegionalLossModel_H
