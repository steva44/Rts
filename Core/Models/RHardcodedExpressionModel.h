#ifndef RHardcodedExpressionModel_H
#define RHardcodedExpressionModel_H
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

#include "RModel.h"

class RParameter;
class RResponse;

class RHardcodedExpressionModel : public RModel
{
	Q_OBJECT
    Q_PROPERTY(QObject *A READ getA WRITE setA)
    Q_PROPERTY(QObject *B READ getB WRITE setB)
    Q_PROPERTY(QObject *C READ getC WRITE setC)
    Q_PROPERTY(int EvaluationCount READ getEvaluationCount WRITE setEvaluationCount)

public:
    RHardcodedExpressionModel(QObject *parent, QString name);
	~RHardcodedExpressionModel();

    QObject *getA() const;
    void setA(QObject *value);

    QObject *getB() const;
    void setB(QObject *value);

    QObject *getC() const;
    void setC(QObject *value);

    int getEvaluationCount() const;
    void setEvaluationCount(int value);

	QList<RParameter *> getPhysicalParameterList();

	int evaluateModel(RGradientType theGradientType);

private:

    QPointer<RParameter> theA;
    QPointer<RParameter> theB;
    QPointer<RParameter> theC;
    int theEvaluationCount;
    RResponse *theResponse;

};

#endif
