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

#include "RProbabilityTransformation.h"
#include "RRandomVariable.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"

#include <math.h>

RProbabilityTransformation::RProbabilityTransformation(QObject *parent, QString name)
    : RMethod(parent, name)
{
	randomVariablesCount = 0;
    randomVariableList = nullptr;
}


RProbabilityTransformation::~RProbabilityTransformation()
{
	
}


int RProbabilityTransformation::setRandomVariableList(QList<RRandomVariable *>* passedRandomVariableList)
{
	// Assign the random variable list
    randomVariableList = passedRandomVariableList;
    randomVariablesCount = randomVariableList->count();

	int result = initializeAtStartOfAnalysis();
	if (result < 0) {
		qCritical() << "Error: Could not establish the correlation matrix.";
		return -1;
	}

	return 0;
}


int RProbabilityTransformation::computeDzDMeanStdv(RProbabilityDistributions *theProbDistTool, RContinuousRandomVariable *randomVariable, double x, double z, double *DzDMean, double *DzDStdv)
{
	int status = 0;

	QString distributionType = randomVariable->getDistributionType();

	double dzdMean = 0.0;
	double dzdStdv = 0.0;
	if (distributionType == QString("Normal (mean, stdv)")) {
		double mean = randomVariable->getParameter1Value();
		double stdv = randomVariable->getParameter2Value();
		dzdMean = -1.0 / stdv;
		dzdStdv = -(x - mean) / (stdv * stdv);
	}
	else if (distributionType == QString("Uniform (a, b)")) {
		double a = randomVariable->getParameter1Value();
		double b = randomVariable->getParameter2Value();
		double pz = 0.39894228048 * exp(-z * z / 2.0);
		dzdMean = -1.0 / (pz * (b - a));
		dzdStdv = 1.732050807 * (a + b - 2.0 * x) * (-dzdMean / (b - a));
	}
	else if (distributionType == QString("Lognormal (zeta, sigma)")) {
		double mean = randomVariable->getMeanValue();
		double stdv = randomVariable->getStandardDeviationValue();
		double a = mean * mean + stdv * stdv;
		dzdMean = 0.5 * (-2.0 * mean * mean * log(a) + 4.0 * mean * mean * log(mean) - 3.0 * stdv * stdv * log(a) + 4.0 * stdv * stdv * log(mean) + 2.0 * stdv * stdv * log(x)) / (pow(log(a) - 2.0 * log(mean), 1.5) * mean * a);
		dzdStdv = 0.5 * stdv * (log(a) - 2.0 * log(x)) / (pow(log(a) - 2.0 * log(mean), 1.5) * a);
	}
	else {
		double rvCDF = randomVariable->getCDFValue(x);

        double factor = 1.0 / theProbDistTool->getNormalPDF(theProbDistTool->getNormalCDF(rvCDF, 1.0), 1.0);
		double dCDFdMean;
		double dCDFdStdv;
		status = randomVariable->computeCDFSensitivityToParameters(x, &dCDFdMean, &dCDFdStdv);
		dzdMean = dCDFdMean * factor;
		dzdStdv = dCDFdStdv * factor;
	}

	*DzDMean = dzdMean;
	*DzDStdv = dzdStdv;

	return 0;
}
