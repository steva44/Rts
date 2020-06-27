#ifndef RTimeSeries_H
#define RTimeSeries_H
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
*   - Terje Haukaas                                                  *
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RParameter.h"
#include <memory>

class RMainWindow;
class RDomain;

class RTimeSeries : public RParameter
{
	Q_OBJECT	

public:
    RTimeSeries(QObject *parent, QString name);
	virtual ~RTimeSeries();
	
	QStringList getActionList();

    int setXvalues(double time, double x);
    int setYvalues(double time, double y);
    int setZvalues(double time, double z);
    int setXYZvalues(double time, double x, double y, double z);

    //Overload to accept parameters
    int setXYZvalues(double time, RParameter* x, RParameter* y, RParameter* z);

    virtual double getXvalue(double time, RParameter* theDDMParameter1 = nullptr, RParameter* theDDMParameter2 = nullptr);
    virtual double getYvalue(double time, RParameter* theDDMParameter1 = nullptr, RParameter* theDDMParameter2 = nullptr);
    virtual double getZvalue(double time, RParameter* theDDMParameter1 = nullptr, RParameter* theDDMParameter2 = nullptr);

    std::vector<double> getXYZvalues(double time, RParameter* theDDMParameter1  = nullptr, RParameter* theDDMParameter2 = nullptr);

    double getTimeItem(int item);
    double getXitem(int item);
    double getYitem(int item);
    double getZitem(int item);

    void clean();
    int getNumEntries();

public slots:

    void print();


public slots:

    void loadFile();

    void plot3D();
    void plotX();
    void plotY();
    void plotZ();

    void plotInt3D();
    void plotIntX();
    void plotIntY();
    void plotIntZ();

    void plotDblInt3D();
    void plotDblIntX();
    void plotDblIntY();
    void plotDblIntZ();

private:
	
    std::unique_ptr<std::vector<double>> theTimeValues;
    std::unique_ptr<std::vector<double>> theXvalues;
    std::unique_ptr<std::vector<double>> theYvalues;
    std::unique_ptr<std::vector<double>> theZvalues;
    std::unique_ptr<std::vector<double>> theRotXvalues;
    std::unique_ptr<std::vector<double>> theRotYvalues;
    std::unique_ptr<std::vector<double>> theRotZvalues;

    std::unique_ptr<std::vector<RParameter*>> theXParameterValues;
    std::unique_ptr<std::vector<RParameter*>> theYParameterValues;
    std::unique_ptr<std::vector<RParameter*>> theZParameterValues;

    void checkConsistency();

    bool consistencyChecked = false;

    int lastIt;
    double tol;

};

#endif
