#ifndef RAlgebraicExpressionModel_H
#define RAlgebraicExpressionModel_H
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
#include "muParser.h"

class RParameter;
class RResponse;
class RMainWindow;

class RAlgebraicExpressionModel : public RModel
{
	Q_OBJECT
	Q_PROPERTY(QString Expression READ getExpression WRITE setExpression)
    Q_PROPERTY(int EvaluationCount READ getEvaluationCount WRITE setEvaluationCount)

public:
	RAlgebraicExpressionModel(QObject *parent, QString name);
	~RAlgebraicExpressionModel();

    QStringList getActionList();

	QString getExpression() const;
	void setExpression(QString value);

    int getEvaluationCount() const;
    void setEvaluationCount(int value);

	// ATTENTION: "ALL" OF THE PARAMETERS IN THIS MODEL ARE RETURNED AS A PHYSICAL PARAMETER LIST. IN CASE THIS IS NOT WHAT'S NEEDED, THE CODE SHOULD BE MODIFIED.
	QList<RParameter *> getPhysicalParameterList();

    int evaluateModel(RGradientType theGradientType);

    inline QMap<RParameter*,double> getFODDMParameterMap(){return theModelFDMMap;}
    inline QList<RParameter *> getExplicitParameterList(){return theExplicitParameterList;}

private slots:

    void modelInference();

private:

    int parseExpression();
	void reset();

	QString theExpression;
    RResponse *theResponse = nullptr;
    std::unique_ptr<mu::Parser> theParser = nullptr;
    int theEvaluationCount;
    QList<RParameter *> theExplicitParameterList;

    QMap<RParameter*,double> theModelFDMMap;

};

#endif
