#ifndef RContinuousRandomVariable_H
#define RContinuousRandomVariable_H
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

#include "RRandomVariable.h"

class RProbabilityDistributions;
class RParameter;

class RContinuousRandomVariable : public RRandomVariable
{
	Q_OBJECT
	Q_PROPERTY(QString DistributionType READ getDistributionType WRITE setDistributionType)
	Q_PROPERTY(QString Mean READ getMean WRITE setMean)
	Q_PROPERTY(QString StandardDeviation READ getStandardDeviation WRITE setStandardDeviation)
	Q_PROPERTY(QString CoefficientOfVariation READ getCoefficientOfVariation WRITE setCoefficientOfVariation)
    Q_PROPERTY(QString Parameter1 READ getParameter1 WRITE setParameter1)
	Q_PROPERTY(QString Parameter2 READ getParameter2 WRITE setParameter2)
	Q_PROPERTY(QString Parameter3 READ getParameter3 WRITE setParameter3)
	Q_PROPERTY(QString Parameter4 READ getParameter4 WRITE setParameter4)
    Q_PROPERTY(double AlphaImportanceMeasure READ getAlphaImportanceMeasure)
    Q_PROPERTY(double GammaImportanceMeasure READ getGammaImportanceMeasure)

public:
	RContinuousRandomVariable(QObject *parent, QString name);
	~RContinuousRandomVariable();

	QString getDistributionType() const;
    void setDistributionType(QString value);

	QString getMean();
    double getMeanValue();
    void setMean(QString value);
    void setMeanValue(double value);

	QString getStandardDeviation();
    double getStandardDeviationValue();
    void setStandardDeviation(QString value);
    void setStandardDeviationValue(double value);

	QString getCoefficientOfVariation();
    double getCoefficientOfVariationValue();
    void setCoefficientOfVariation(QString value);

	QString getParameter1();
    double getParameter1Value();
    void setParameter1(QString value);

	QString getParameter2();
    double getParameter2Value();
    void setParameter2(QString value);

	QString getParameter3();
    double getParameter3Value();
    void setParameter3(QString value);

	QString getParameter4();
    double getParameter4Value();
    void setParameter4(QString value);

	double getAlphaImportanceMeasure() const;
	void setAlphaImportanceMeasure(double value);

	double getGammaImportanceMeasure() const;
	void setGammaImportanceMeasure(double value);

	double getPDFValue(double rvValue);
	double getCDFValue(double rvValue);
	double getInverseCDFValue(double probValue);

	QList<RParameter *> getParameterList();
    QStringList getActionList();

	void setDynamicProperty(QString propertyName, QVariant propertyValue);
	void setUserDefinedPDF(QVector<double> passedXVector, QVector<double> passedPDFVector, bool viaGUI = false);

	int computeCDFSensitivityToParameters(double rvValue, double *dCDFdMean, double *dCDFdStdv);
	
    int updateParameters();

private slots:

    void plotPDF();
	void plotCDF();
	void plotInverseCDF();

	void getPDF();
	void getCDF();
	void getInverseCDF();

private:

    void createDefaultDistributions();
	int updateMeanStdv();
	int updateMeanStdvByCOV();

	void plot(QString type);
	void get(QString type);

    int userDefinedErrorCheck();

    QPointer<RProbabilityDistributions> theProbabilityDistributions;

	QString theDistributionType;

	QPointer<RParameter> theMeanParameter;
    double theMeanDouble;

	QPointer<RParameter> theStandardDeviationParameter;
    double theStandardDeviationDouble;

    QPointer<RParameter> theCoefficientOfVariationParameter;
    double theCoefficientOfVariationDouble;

	QPointer<RParameter> theParameter1Parameter;
    double theParameter1Double;

	QPointer<RParameter> theParameter2Parameter;
    double theParameter2Double;

	QPointer<RParameter> theParameter3Parameter;
    double theParameter3Double;

	QPointer<RParameter> theParameter4Parameter;
	double theParameter4Double;

	double theAlphaImportanceMeasure;
	double theGammaImportanceMeasure;

	QVector<double> XVector;
	QVector<double> PDFVector;
};

#endif
