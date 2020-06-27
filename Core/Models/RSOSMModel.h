#ifndef RSOSMModel_H
#define RSOSMModel_H
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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RModel.h"

class RResponse;
class RParameter;
class RContinuousRandomVariable;
class RDecisionVariable;

class RSOSMModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *InputParameter READ getInputParameter WRITE setInputParameter)
    Q_PROPERTY(QString GradientMethod READ getGradientMethod WRITE setGradientMethod)

public:
    RSOSMModel(QObject *parent, QString name);
    ~RSOSMModel();

    QObject *getInputParameter() const;
    void setInputParameter(QObject *value);

    QString getGradientMethod();
    void setGradientMethod(QString value);

    int evaluateModel(RGradientType theGradientType);

private:

    QPointer<RParameter> theInputParameter;
    QString theGradientMethod;
    RResponse *theResponse;

};

#endif
