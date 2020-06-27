#ifndef RBrick_H
#define RBrick_H
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

#include "RElement.h"

// Wrapper class for OpenSees Brick element
// Brick(int tag,
// int node1,
// int node2,
// int node3,
// int node4,
// int node5,
// int node6,
// int node7,
// int node8,
// NDMaterial &theMaterial,
// double b1 = 0.0, double b2 = 0.0, double b3 = 0.0);

class RNode;
class RParameter;
class CrdTransf;
class Brick;
class Element;
class RNDMaterial;

class RBrick : public RElement
{
    Q_OBJECT

public:
    RBrick(QObject *parent, QString name);
    ~RBrick();

    int  update(void);

    int    getNumNodes();
    RNode* getNode(int i) const;
    int    setNode(int i, RNode *theNode);

    Element* getTheOpenSeesElement(void);

    void print(void);

    int  getResponseInGlobalXYZ(QVector<double> *response, QString type, double xi,RParameter *theParameter, RParameter *theParameter2, RGradientType theGradientType);

    void  getGlobalDispResponse(double xi, QVector<double> *response);

    double getForceResponse(double xi, QVector<double> *response);

    double getStressResponse(QString type, double xi);

    double getStrainResponse(QString type);

    RNDMaterial *getTheMaterial() const;
    void setTheMaterial(RNDMaterial *value);

private:

    RNode *theNode1;
    RNode *theNode2;
    RNode *theNode3;
    RNode *theNode4;
    RNode *theNode5;
    RNode *theNode6;
    RNode *theNode7;
    RNode *theNode8;

    RNDMaterial *theMaterial;

    // Raw pointer; OpenSees controls the destruction, otherwise program crashes on exit
    Brick* theOpenSeesElement;

    int createBrickElement(void);
    int initialize(void);
};

#endif
