#ifndef RDispBeamColumn3d_H
#define RDispBeamColumn3d_H
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

// Wrapper class for OpenSees DispBeamColumn3d
// Stevan Gavrilovic 12/2018

// DispBeamColumn3d(int tag, int nd1, int nd2,
//  int numSections, SectionForceDeformation **s,
//  BeamIntegration &bi, CrdTransf &coordTransf,
//  double rho = 0.0, int cMass = 0);

#include "RNonlinearBeamColumnElement.h"

class DispBeamColumn3d;
class Element;

class RDispBeamColumn3d : public RNonlinearBeamColumnElement
{
    Q_OBJECT

public:
    RDispBeamColumn3d(QObject *parent, QString name);
    ~RDispBeamColumn3d();

    Element* getTheOpenSeesElement(void);

    void print(void);
    int  update();

    int    getGlobalResponse(QVector<double> *response, QString type, int sectionNum);
    int    getLocalResponse(QVector<double> *response, QString type, int sectionNum);

    int generateOpenSeesInput(std::string& object, int flag);

private:

    //Raw ptr; OpenSees controls the destruction, otherwise program crashes on exit
    DispBeamColumn3d* theOpenSeesElement;

    int createDispBeamColumn3dElement(void);
    int initialize(void);

    int getLocalDisplacementResponse(QVector<double> *response, int sectionNum);
    int getLocalForceResponse(QVector<double> *response, int sectionNum);
    int getLocalStressResponse(QVector<double> *response, int sectionNum);
    int getLocalStrainResponse(QVector<double> *response, int sectionNum);
    int getFibreResponses(QVector<double> *response, int sectionNum);
    int getLocalForces(QVector<double> *response);

};

#endif
