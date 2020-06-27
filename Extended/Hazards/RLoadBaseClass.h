#ifndef RLoadBaseClass_H
#define RLoadBaseClass_H
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
*                                                                    *
*********************************************************************/

#include "RObject.h"

class RParameter;

class RLoadBaseClass : public RObject
{
	Q_OBJECT
    Q_PROPERTY(ROutputLevelType OutputLevel READ getOutputLevel WRITE setOutputLevel)

public:
    RLoadBaseClass(QObject *parent, QString name);
    virtual ~RLoadBaseClass();

	// Runs the model analysis by first calling the runAnalysis for upstream responses
    int runAnalysis(RGradientType theGradientType);

    virtual double getFOGradient() = 0;
    virtual double getSOGradient() = 0;

    RParameter *getParameter() const {	return theLoadValueParameter;}

protected:

    RParameter* theLoadValueParameter = nullptr;


private:

    RGradientType theGradientType;
	
};

#endif // RLoadBaseClass_H
