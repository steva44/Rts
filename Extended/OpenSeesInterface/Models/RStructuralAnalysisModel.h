#ifndef RComponentResponseModel_H
#define RComponentResponseModel_H
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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

// The load pattern property can be used to apply simple load patterns like user loads and other loads that will evaluate every single time this model is run, and will remain applied indefinitely
// The hazard property is used for hazards that come bundled with occurance time(s). The main difference is that a hazard will only be applied on the time that it occurs.
// Also, if only hazards are applied, then the analysis will only run when the hazard(s) occur. Otherwise, it will not.

#include "RModel.h"
#include "AnalysisModel.h"

class ROSGPlotter;
class RStructuralAnalysis;
class RBIM;
class RMainWindow;
class RLoadPattern;
class RResponse;
class RLoadPatternModel;
class RInelasticDynamicStructuralAnalysis;

class RStructuralAnalysisModel : public RModel, AnalysisModel
{
    Q_OBJECT
#ifdef DETAILEDBUILDINGS
    Q_PROPERTY(QObject *BIM READ getBIM WRITE setBIM)
#endif
    Q_PROPERTY(QString Responses           READ getResponses            WRITE setResponses)
    Q_PROPERTY(QObject *StructuralAnalysis READ getStructuralAnalysis   WRITE setStructuralAnalysis)
    Q_PROPERTY(QString LoadPattern         READ getTheLoadPattern       WRITE setTheLoadPattern)
#ifdef HAZARDS
    Q_PROPERTY(QString Hazards             READ getHazardList           WRITE setHazardList)
#endif

public:
    RStructuralAnalysisModel(QObject *parent, QString name);
    ~RStructuralAnalysisModel();

    QObject *getStructuralAnalysis() const;
    void setStructuralAnalysis(QObject *value);

    QString getTheLoadPattern();
    void    setTheLoadPattern(QString value);

#ifdef DETAILEDBUILDINGS
    QObject *getBIM() const;
    void setBIM(QObject *value);
    QList<QPair<double, QString> > getTriggerTimes(double startTime, double endTime);
#endif

#ifdef HAZARDS
    QString getHazardList();
    void setHazardList(QString value);
#endif

    QString getResponses() const;
    void setResponses(QString value);

    int evaluateModel(RGradientType theGradientType);

    QList<RParameter *> getAllParametersList(bool *ok);

    QStringList getActionList();

    double getPeriod(void);

    // Updates the load and mass values
    void update();

public slots:

    void plotDeformed();

protected slots:

    void createMesh();

    void generateInputFile();

#ifdef OSG
    void plotUndeformed();
    // void plotVisualDamage();

    void plotBMD();
    void plotSFD();
    // void plotAFD();
    // void plotSigmaXX();
    // void plotSigmaYY();
    // void plotTauXY();
    // void plotTauXZ();
    // void plotTauYZ();
#endif

#if defined(IFC) && defined(OSG)
    void plotComponents();

    void plotDeformedComponents();

    void plotDynamicElements();

    void plotDynamicComponents();
#endif

private:

    bool initializeTheAnalysis();
    int createStandardLoadPattern(void);
    int createRigidLinks(void);

    std::unique_ptr<Domain> theOpenSeesDomain;
    RStructuralAnalysis*    theStructuralAnalysis;
    RLoadPattern*           theLinearLoadPattern;

    QList<QPointer<RLoadPattern> > theLoadPatterns;

#ifdef OSG
    ROSGPlotter* thePlotter;
#endif

#ifdef DETAILEDBUILDINGS
    QPointer<RBIM> theBIM;
#endif

#ifdef HAZARDS
    QList<QPointer<RModel> > theInputHazards;

    //A vector to hold the responses
    QVector<RLoadPatternModel *> theHazardList;

    bool applyHazards(void);

    bool hazardsApplied;
#endif

    // Flags for initializing the analysis
    bool runOnce ;
    bool initialized;
    bool areComponentsMeshed;

    RResponse *theMaximumResponse;
    QStringList theResponses;
    QList<RResponse *> theResponseList;
    QList<QPointer<RObject> > theStructuralObjects;
    QList<RParameter*> theDDMParamterList;
    RMainWindow* theMainWindow;

    void autoSetTransientAnalysisParameters(QList<QPointer<RLoadPattern> >& loadPatternList);

};

#endif
