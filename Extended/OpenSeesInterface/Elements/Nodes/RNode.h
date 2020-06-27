#ifndef RNode_H
#define RNode_H
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

//****************************************************************
// This version of RNode is a wrapper class for OpenSees Node class
// When updating OpenSees, remember to make the private data members protected in OpenSees class "Node.h"
//****************************************************************

#include "RParameter.h"
#include <QVector>
#include <functional>
#include <QPointer>

const int numDOFsPerNode = 6;
const double accelerationOfGravity = 9.81;

#include "Node.h"

class RDomain;
class RPoint;

class RNode : public RObject,public Node
{
    Q_OBJECT

public:
    RNode(QObject *parent, QString &name);
    ~RNode();

    inline void setNodeNumber(int i){theNodeNumber = i;}
    inline int  getNodeNumber(){return theNodeNumber;}

    void setXCoordinate(RParameter* value);
    void setXCoordinate(double value);
    void setXCoordinate(std::function<double(void)>& fxn);
    RParameter *getXCoordinateParameter() const;
    double getXCoordinateValue();

    void setYCoordinate(double value);
    void setYCoordinate(RParameter* value);
    void setYCoordinate(std::function<double(void)>& fxn);
    RParameter *getYCoordinateParameter() const;
    double getYCoordinateValue();

    void setZCoordinate(RParameter* value);
    void setZCoordinate(double value);
    void setZCoordinate(std::function<double(void)>& fxn);
    RParameter *getZCoordinateParameter() const;
    double getZCoordinateValue();

    double getXDisplacement();
    double getYDisplacement();
    double getZDisplacement();
    double getXRotation();
    double getYRotation();
    double getZRotation();
    double getWarping();

    QVector<double> * getDisplacementVector();

    int setBoundaryConditions(int Xdisp, int Ydisp, int Zdisp, int Xrot, int Yrot, int Zrot);
    QVector<int> * getBoundaryConditionVector();

    int setAssociatedPoint(RPoint *passedPoint);
    RPoint *getAssociatedPoint();

    //If this is a master point it will return a slave node or a nullptr otherwise
    inline RNode* getSlaveNode(){return theSlaveNode;}

    //The functions for the gradients below do not currently work, they are just there for
    //Lambda function for first and second order nodal gradients from RMeshGenerator
    inline void setFOGradientXLambda(std::function<double(RParameter*)>& fxn){nodalXGradientLambdaFO = fxn;}
    inline void setFOGradientYLambda(std::function<double(RParameter*)>& fxn){nodalYGradientLambdaFO = fxn;}
    inline void setFOGradientZLambda(std::function<double(RParameter*)>& fxn){nodalZGradientLambdaFO = fxn;}

    inline void setSOGradientXLambda(std::function<double(RParameter*)>& fxn){nodalXGradientLambdaSO = fxn;}
    inline void setSOGradientYLambda(std::function<double(RParameter*)>& fxn){nodalYGradientLambdaSO = fxn;}
    inline void setSOGradientZLambda(std::function<double(RParameter*)>& fxn){nodalZGradientLambdaSO = fxn;}

    //First order gradient functions
    std::function<double(RParameter*)> nodalXGradientLambdaFO;
    std::function<double(RParameter*)> nodalYGradientLambdaFO;
    std::function<double(RParameter*)> nodalZGradientLambdaFO;

    //Second order gradient functions
    std::function<double(RParameter*)> nodalXGradientLambdaSO;
    std::function<double(RParameter*)> nodalYGradientLambdaSO;
    std::function<double(RParameter*)> nodalZGradientLambdaSO;

    //Need to override the default RObject function so that the OpenSees tag can be updated after the Rts object is instantiated
    void setObjectTag(int value);

    //This function updates the coordinates of the OpenSees node
    void updateNode(void);

    //This function is required to set the number of dof for an OpenSees node as some elements use a custom number of dof
    void setNumDof(int num);

    int generateOpenSeesInput(std::string& object, int flag);

private:

    QPointer<RParameter> theXCoordinateParameter;
    std::unique_ptr<std::function<double(void)>> xCoordinateLambda;

    QPointer<RParameter> theYCoordinateParameter;
    std::unique_ptr<std::function<double(void)>> yCoordinateLambda;

    QPointer<RParameter> theZCoordinateParameter;
    std::unique_ptr<std::function<double(void)>> zCoordinateLambda;

    std::unique_ptr<QVector<double>> theDisplacementVector;
    std::unique_ptr<QVector<int>> theBoundaryConditionVector;

    int theNodeNumber;

    //If this is a master node to some other node, this is will not be empty
    QPointer<RNode>  theSlaveNode;
    QPointer<RPoint> theAssociatedPoint;

    friend class RSlaveNode; //Allow RSlaveNode accesss to private and protected members

};

#endif

