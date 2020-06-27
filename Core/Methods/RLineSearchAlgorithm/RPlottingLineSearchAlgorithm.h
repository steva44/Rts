#ifndef RPlottingLineSearchAlgorithm_H
#define RPlottingLineSearchAlgorithm_H
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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RLineSearchAlgorithm.h"
#include "qcustomplot.h"

class RPlottingLineSearchAlgorithm : public RLineSearchAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double From READ getFrom WRITE setFrom)
    Q_PROPERTY(double To READ getTo WRITE setTo)

public:
    RPlottingLineSearchAlgorithm(QObject *parent, QString name);
    ~RPlottingLineSearchAlgorithm();

    double getFrom() const;
    void setFrom(double value);

    double getTo() const;
    void setTo(double value);

    double findPoint(RLineSearchCallBackAssistant *theCallBackAssistant);

private:

    double theFrom;
    double theTo;

    QPointer<QCustomPlot> customPlot;

};

#endif
