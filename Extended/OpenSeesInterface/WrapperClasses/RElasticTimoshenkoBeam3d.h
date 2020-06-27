#ifndef RElasticTimoshenkoBeam3d_H
#define RElasticTimoshenkoBeam3d_H
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

// Wrapper class for OpenSees ElasticTimoshenkoBeam3d
// Stevan Gavrilovic 12/2018

// ElasticTimoshenkoBeam3d(int tag, int Nd1, int Nd2, double E, double G,
// double A, double Jx, double Iy, double Iz, double Avy, double Avz,
// CrdTransf &theTransf, double rho = 0.0, int cMass = 0);

#include "RElasticBeamColumnElement.h"
#include "ElasticTimoshenkoBeam3d.h"

class RNode;
class RParameter;
class ElasticTimoshenkoBeam3d;
class Element;

class RElasticTimoshenkoBeam3d : public RElasticBeamColumnElement
{
    Q_OBJECT

public:
    RElasticTimoshenkoBeam3d(QObject *parent, QString name);
    ~RElasticTimoshenkoBeam3d();

    int  update(void);
    Element* getTheOpenSeesElement(void);
    void print(void);

    int getGlobalResponse(QVector<double> *response, QString type, double xi);
    int getLocalResponse(QVector<double> *response, QString type, double xi);

private:
    //Raw ptr; OpenSees controls the destruction, otherwise program crashes on exit
    ElasticTimoshenkoBeam3d* theOpenSeesElement;

    int createElasticTimoshenkoBeam3dElement(void);

    void   getLocalDispResponse(double xi, QVector<double> *response);
    double getLocalForceResponse(double xi, QVector<double> *response);
    double getStressResponse(QString type, double xi);
    double getStrainResponse(QString type);
    int    getLocalForces(QVector<double> *response);

    int initialize(void);
};

#endif
