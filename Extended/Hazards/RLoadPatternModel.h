#ifndef RLoadPatternModel_H
#define RLoadPatternModel_H
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

#include "RModel.h"

class RResponse;
class RParameter;
class RLoadPattern;

class RLoadPatternModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QString LoadPatterns    READ getTheLoadPatternsInput     WRITE setTheLoadPatternsInput)
    Q_PROPERTY(QObject *Occurrence     READ getOccurrence               WRITE setOccurrence)
    Q_PROPERTY(QObject* ScalingModel   READ getScalingModelInput        WRITE setScalingModelnput)
    Q_PROPERTY(double ScaleFactor       READ getScaleFactor         WRITE setScaleFactor)

public:
    RLoadPatternModel(QObject *parent, QString name);
    ~RLoadPatternModel();

    virtual bool isOn();
    virtual QList<QPointer<RLoadPattern> > getTheLoadPatterns();

    int evaluateModel(RGradientType theGradientType);

    // Reset the load patterns to original without any scaling
    void reset(void);

    QObject* getScalingModelInput() const;
    void setScalingModelnput(QObject* value);

    bool& isApplied();
    void setAppplied(bool value);

    QString getTheLoadPatternsInput();
    void setTheLoadPatternsInput(QString value);

    QObject *getOccurrence() const;
    void setOccurrence(QObject *value);

    double getScaleFactor() const;
    void setScaleFactor(double value);

    QList<RParameter *> getAllParametersList(bool *ok);

protected:

    QList<QPointer<RLoadPattern> > theLoadPatterns;
    RResponse *theResponse;

private:

    // A flag to track if this load pattern has been applied
    bool patternApplied;

    double scaleFactor;

    QPointer<RParameter> theOccurrence;

    QPointer<RParameter> scalingModelResponse;


};

#endif
