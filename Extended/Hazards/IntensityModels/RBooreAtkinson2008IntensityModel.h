#ifndef RBooreAtkinson2008IntensityModel_H	
#define RBooreAtkinson2008IntensityModel_H	
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

#include "RContinuousRandomVariable.h"
#include "RLinearSolver.h"
#include "REarthquakeBaseClass.h"

class RParameter;	
class RResponse;
class RLocation;
	
class RBooreAtkinson2008IntensityModel : public REarthquakeBaseClass
{	
	Q_OBJECT
    Q_PROPERTY(QString MagnitudeList READ getMagnitudeList WRITE setMagnitudeList)
    Q_PROPERTY(QString HypocentreList READ getHypocentreList WRITE setHypocentreList)
	Q_PROPERTY(RFaultType FaultType READ getFaultType WRITE setFaultType)
	Q_PROPERTY(RSmoothness Smoothness READ getSmoothness WRITE setSmoothness)
    Q_PROPERTY(QObject *LinearSolver READ getLinearSolver WRITE setLinearSolver)
	Q_ENUMS(RFaultType)
	Q_ENUMS(RSmoothness)

public:	
	RBooreAtkinson2008IntensityModel(QObject *parent, QString name);
	~RBooreAtkinson2008IntensityModel();

    int resetTime();

	enum RFaultType{Unspecified, StrikeSlip, NormalSlip, ReverseSlip};
	enum RSmoothness{Smoothed, Original};

	QString getMagnitudeList();
	void setMagnitudeList(QString value);

    QString getHypocentreList();
    void setHypocentreList(QString value);

	RFaultType getFaultType() const;
	void setFaultType(RFaultType value);

	RSmoothness getSmoothness() const;
	void setSmoothness(RSmoothness value);

    QObject *getLinearSolver() const;
    void setLinearSolver(QObject *value);

    bool isOn();
    int evaluateModel(RGradientType theGradientType);

private:

    QList<QPointer<RParameter> > theMagnitudeList;
    QList<QPointer<RParameter> > theHypocentreList;
    QPointer<RContinuousRandomVariable> theEpsilon = nullptr;
	RFaultType theFaultType;
	RSmoothness theSmoothness;
    QPointer<RLinearSolver> theLinearSolver = nullptr;
    RResponse *theResponse = nullptr;

    // Returns the coefficients a, b, c, and d in the cubic polynomial ax^3+bx^2+cx+d that is used to smooth out the kinks in the model form
    int cubicSmooth(double x0, double epsilon, double y1, double y2, double dy1, double dy2, double *a, double *b, double *c, double *d);

    // Overloaded method that returns the smoothed function value (y) at point x
    int cubicSmooth(double x0, double epsilon, double y1, double y2, double dy1, double dy2, double x, double *y);
};	
	
#endif
