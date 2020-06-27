#ifndef RSamplingModel_H
#define RSamplingModel_H
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

class RMatrixOperations;
class RRandomNumberGenerator;
class RProbabilityTransformation;
class QCustomPlot;

class RSamplingModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *InputParameter READ getInputParameter WRITE setInputParameter)
    Q_PROPERTY(QString Threshold READ getThreshold WRITE setThreshold)
    Q_PROPERTY(int MaxSamples READ getMaxSamples WRITE setMaxSamples)
    Q_PROPERTY(double TargetCov READ getTargetCov WRITE setTargetCov)
    Q_PROPERTY(int PlotInterval READ getPlotInterval WRITE setPlotInterval)
    Q_PROPERTY(QString SamplingCentre READ getSamplingCentre WRITE setSamplingCentre)
    Q_PROPERTY(QObject *RandomNumberGenerator READ getRandomNumberGenerator WRITE setRandomNumberGenerator)
    Q_PROPERTY(QObject *ProbabilityTransformation READ getProbabilityTransformation WRITE setProbabilityTransformation)

public:
    RSamplingModel(QObject *parent, QString name);
    ~RSamplingModel();

    QObject *getInputParameter() const;
    void setInputParameter(QObject *value);

    QString getThreshold();
    void setThreshold(QString value);
    double getThresholdValue();

    int getMaxSamples() const;
    void setMaxSamples(int value);

    int getPlotInterval() const;
    void setPlotInterval(int value);

    double getTargetCov() const;
    void setTargetCov(double value);

    QString getTheResponses();
    void setTheResponses(QString value);

    QObject *getRandomNumberGenerator() const;
    void setRandomNumberGenerator(QObject *value);

    QObject *getProbabilityTransformation() const;
    void setProbabilityTransformation(QObject *value);

    QString getSamplingCentre();
    void setSamplingCentre(QString value);

    QStringList getActionList();

    int evaluateModel(RGradientType theGradientType);

    void resample(void);

    int getSampleNumber() const;

private:

    double evaluateLimitStateFunction();

    std::unique_ptr<RMatrixOperations> theMatrixOperations;

    QVector<double> u;
    QVector<double> uCentre;
    QVector<double> x;
    QList<RRandomVariable *> theRVs;

    // Properties
    QPointer<RParameter> theInputParameter;
    QPointer<RParameter> theThresholdParameter;
    double theThresholdDouble;
    int maxSamples;
    int thePlotInterval;
    double targetCov;
    QPointer<RRandomNumberGenerator> theRandomNumberGenerator;
    QPointer<RProbabilityTransformation> theProbabilityTransformation;
    QString theSamplingCentre;

    // The plots
    QPointer<QCustomPlot> customPlot;
    QPointer<QCustomPlot> customPlot2;

    QList<QString> theResponses;

    // The response
    RResponse *theProbabilityResponse;
    RResponse *theBetaResponse;
    RResponse *theMeanResponse;
    RResponse *theStdvResponse;

    int sampleNumber;
    int numBins;

};

#endif
