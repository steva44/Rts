#ifndef RForceBeamColumn3d_H
#define RForceBeamColumn3d_H
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

// Wrapper class for OpenSees ForceBeamColumn3d
// Stevan Gavrilovic 12/2018

// orceBeamColumn3d(int tag, int nodeI, int nodeJ,
// int numSections, SectionForceDeformation **sec,
// BeamIntegration &beamIntegr,
// CrdTransf &coordTransf, double rho = 0.0,
// int maxNumIters = 10, double tolerance = 1.0e-12);

#include "RNonlinearBeamColumnElement.h"

class ForceBeamColumn3d;
class Element;

class RForceBeamColumn3d : public RNonlinearBeamColumnElement
{
    Q_OBJECT

public:
    RForceBeamColumn3d(QObject *parent, QString name);
    ~RForceBeamColumn3d();

    Element* getTheOpenSeesElement(void);

    void print(void);
    int  update();

    int    getGlobalResponse(QVector<double> *response, QString type, int sectionNum);
    int    getLocalResponse(QVector<double> *response, QString type, int sectionNum);

private:

    //Raw ptr; OpenSees controls the destruction, otherwise program crashes on exit
    ForceBeamColumn3d* theOpenSeesElement;

    int createForceBeamColumn3dElement(void);
    int initialize(void);

    int getLocalDisplacementResponse(QVector<double> *response, int sectionNum);
    int getLocalForceResponse(QVector<double> *response, int sectionNum);
    int getLocalStressResponse(QVector<double> *response, int sectionNum);
    int getLocalStrainResponse(QVector<double> *response, int sectionNum);
    int getFibreResponses(QVector<double> *response, int sectionNum);
    int getLocalForces(QVector<double> *response);

};

#endif
