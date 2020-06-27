#ifndef RElasticBeam3d_H
#define RElasticBeam3d_H
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

// Wrapper class for OpenSees ElasticBeam3d
// Stevan Gavrilovic 12/2018

//ElasticBeam3d(int tag, double A, double E, double G,
// double Jx, double Iy, double Iz,
// int Nd1, int Nd2, CrdTransf &theTransf,
// double rho = 0.0, int cMass = 0,
// int sectionTag = 0);

#include "RElasticBeamColumnElement.h"
#include "ElasticBeam3d.h"

class RNode;
class RParameter;
class ElasticBeam3d;
class Element;

class RElasticBeam3d : public RElasticBeamColumnElement
{
    Q_OBJECT

public:
    RElasticBeam3d(QObject *parent, QString name);
    ~RElasticBeam3d();

    int  update(void);
    Element* getTheOpenSeesElement(void);
    void print(void);

    int    getGlobalResponse(QVector<double> *response, QString type, double xi);
    int    getLocalResponse(QVector<double> *response, QString type, double xi);

private:

    //Raw ptr; OpenSees controls the destruction, otherwise program crashes on exit
    ElasticBeam3d* theOpenSeesElement;

    int createElasticBeam3dElement(void);
    int initialize(void);

    int getLocalDisplacementResponse(double xi, QVector<double> *response);
    int getLocalForceResponse(double xi, QVector<double> *response);
    int getLocalStressResponse(double z, double y, double Qz, double Qy, double tz, double ty, QVector<double> *response);
    int getLocalStrainResponse(double xi, QVector<double> *response);
    int getLocalForces(QVector<double> *response);

};

#endif
