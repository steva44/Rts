#ifndef RScaledGroundMotionModel_H
#define RScaledGroundMotionModel_H
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

#include "REarthquakeBaseClass.h"

class RTimeSeries;

class RScaledGroundMotionModel : public REarthquakeBaseClass
{
    Q_OBJECT
    Q_PROPERTY(QString InputFile READ getInputFile WRITE setInputFile)
    Q_PROPERTY(QString scalingFactor READ getScalingFactor WRITE setScalingFactor)

public:
    RScaledGroundMotionModel(QObject *parent, QString name);
    ~RScaledGroundMotionModel();

    // Ground motion scaling factor
    void setScalingFactor(QString value);
    QString getScalingFactor(void){return QString::number(scalingFactor);}

    // File that contains the ground motion time-history
    QString getInputFile() const;
    void setInputFile(QString value);

    QStringList getActionList();

    int evaluateModel(RGradientType theGradientType);

    bool isOn();

private slots:

    // Plotting the time-series
    void plotX();
    void plotY();
    void plotZ();
    void printTimeSeries();

private:

    QString theInputFile;
    RResponse *theGroundMotionResponse;
    RTimeSeries *theInputTimeSeries;
    double scalingFactor;

};

#endif
