#ifndef RConstant_H
#define RConstant_H
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
*                                                                    *
*********************************************************************/

#include "RParameter.h"

class RDecisionVariable;
class RDomain;

class RConstant : public RParameter
{
	Q_OBJECT
	Q_PROPERTY(double CurrentValue READ getCurrentValue WRITE setCurrentValue)

public:
	RConstant(QObject *parent, QString name);
	~RConstant();

	// RConstant may be used as a response that flows upstream (e.g., when an attenuation model needs the natural period from a building model)
	// In this situation, the list of parameters that are used to compute such a response must be given to the RConstant object via this method
	void setUpstreamParameterList(QList<RParameter *> parameterList);

	// Returns the list of parameters that are used to compute a response that flows upstream, as explained above
	QList<RParameter *> getUpstreamParameterList();

    QStringList getActionList();

private:
	// Data member for the list of parameters as explained above
	QList<RParameter *> theUpstreamParameterList;
    //This parameter is used when associating this parameter into a decision variable.
    RParameter* theDecisionSlaveParameter = nullptr;
	
};

#endif
