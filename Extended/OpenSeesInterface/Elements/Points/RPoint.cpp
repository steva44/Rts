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

#include "RPoint.h"
#include "RTimeSeries.h"
#include "RTimeSeriesResponse.h"
#include "RGenericResponse.h"
#include "RParametricPoint.h"
#include "RSlaveNode.h"
#include "RPointLoad.h"
#include "RDomain.h"

RPoint::RPoint(QObject *parent, QString &name)
    : RObject(parent, name)
{
    isOn = true;

    theXCoordinateDouble = 0.0;
    theYCoordinateDouble = 0.0;
    theZCoordinateDouble = 0.0;

    theXDisplacementRestraint = false;
    theYDisplacementRestraint = false;
    theZDisplacementRestraint = false;
    theXRotationRestraint = false;
    theYRotationRestraint = false;
    theZRotationRestraint = false;

    theXForceDouble = 0.0;
    theYForceDouble = 0.0;
    theZForceDouble = 0.0;
    theXMomentDouble = 0.0;
    theYMomentDouble = 0.0;
    theZMomentDouble = 0.0;

    xSet = false;
    ySet = false;
    zSet = false;

    //set standard functions
    theXForceLambda=[this](void){return theXForceDouble;};
    theYForceLambda=[this](void){return theYForceDouble;};
    theZForceLambda=[this](void){return theZForceDouble;};

    xCoordinateLambda = nullptr;
    yCoordinateLambda = nullptr;
    zCoordinateLambda = nullptr;

    theXCoordinateParameter = nullptr;
    theYCoordinateParameter = nullptr;
    theZCoordinateParameter = nullptr;

}


RPoint::~RPoint()
{
    //qDebug()<<"The Point "<<this->objectName()<<" is being deleted";
    //dont delete the associated node here program will crash on close if system deletes it first
    //    theDomain->deleteObject(theAssociatedNode->objectName());
}


void RPoint::setMasterPoint(QObject* value)
{
    auto thePoint = qobject_cast<RPoint *>(value);

    if(masterPoint == thePoint)
        return;

    masterPoint = thePoint;

    RNode* aNode = nullptr;

    //auto theOrigNodeList = theDomain->findChildren<RNode *>();

    theDomain->createObject(this->objectName()+ "AssociatedSlaveNode" , "RSlaveNode");
    aNode = qobject_cast<RNode *>(theDomain->getLastAddedObject());

    //create the lamda functors
    std::function<double(void)> xCoordinatelambda = [this]()
    {
        return this->getXCoordinateValue();
    };
    std::function<double(void)> yCoordinatelambda = [this]()
    {
        return this->getYCoordinateValue();
    };
    std::function<double(void)> zCoordinatelambda = [this]()
    {
        return this->getZCoordinateValue();
    };

    //Pass the functor callback for the get coordinate functions
    aNode->setXCoordinate(xCoordinatelambda);
    aNode->setYCoordinate(yCoordinatelambda);
    aNode->setZCoordinate(zCoordinatelambda);

    this->deleteAssociatedNode();

    //Set the association
    theAssociatedNode = aNode;
    aNode->setAssociatedPoint(this);

    auto aSlaveNode = qobject_cast<RSlaveNode *>(theDomain->getLastAddedObject());

    aSlaveNode->setMasterPoint(thePoint);
    //auto theNewNodeList = theDomain->findChildren<RNode *>();
}


QList<RParameter*> RPoint::getXCoordinateParameters() const
{
    QList<RParameter*> theXCoordinateParameterList;

    if(theXCoordinateParameter)
    {
        //Check to see if the XCoordinate Parameter is actually a parametric point
        RParametricPoint *parametricPoint = qobject_cast<RParametricPoint *>(theXCoordinateParameter.data());
        if (parametricPoint)
        {
            //Evaluate the model using DDM
            parametricPoint->evaluateExpression(RGradientType::DDM);

            //Pulls the DDM Parameter Map from algebraic model and checks for parameter in map
            auto ddmList = parametricPoint->getExplicitParameterList();

            return ddmList;
        }
        else
        {
            theXCoordinateParameterList.append(theXCoordinateParameter);
        }
    }

    return theXCoordinateParameterList;
}


QList<RParameter*> RPoint::getYCoordinateParameters() const
{
    QList<RParameter*> theYCoordinateParameterList;

    if(theYCoordinateParameter)
    {
        //Check to see if the YCoordinate Parameter is actually a parametric point
        RParametricPoint *parametricPoint = qobject_cast<RParametricPoint *>(theYCoordinateParameter.data());
        if (parametricPoint)
        {
            //Evaluate the model using DDM
            parametricPoint->evaluateExpression(RGradientType::DDM);

            //Pulls the DDM Parameter Map from algebraic model and checks for parameter in map
            auto ddmList = parametricPoint->getExplicitParameterList();

            return ddmList;
        }
        else
        {
            theYCoordinateParameterList.append(theYCoordinateParameter);
        }
    }

    return theYCoordinateParameterList;

}


QList<RParameter*> RPoint::getZCoordinateParameters() const
{
    QList<RParameter*> theZCoordinateParameterList;

    if(theZCoordinateParameter)
    {
        //Check to see if the ZCoordinate Parameter is actually a parametric point
        RParametricPoint *parametricPoint = qobject_cast<RParametricPoint *>(theZCoordinateParameter.data());
        if (parametricPoint)
        {
            //Evaluate the model using DDM
            parametricPoint->evaluateExpression(RGradientType::DDM);

            //Pulls the DDM Parameter Map from algebraic model and checks for parameter in map
            auto ddmList = parametricPoint->getExplicitParameterList();

            return ddmList;
        }
        else
        {
            theZCoordinateParameterList.append(theZCoordinateParameter);
        }
    }
    return theZCoordinateParameterList;

}


int RPoint::createOrSetAssociatedNode(RNode* theNode)
{

    if(theAssociatedNode && theNode == nullptr)
        return 0;

    if(!xSet || !ySet || !zSet)
        return 0;

    if(theNode != nullptr)
    {
        theAssociatedNode = theNode;
        theNode->setAssociatedPoint(this);
        return 0;
    }

    RNode* aNode = nullptr;

    auto tol = 0.00001;

    //First check to see if a node exists at this location
    QList<RNode *> theNodeList = theDomain->findChildren<RNode *>();
    for (auto&& it: theNodeList)
    {
        auto x = this->getXCoordinateValue();
        auto y = this->getYCoordinateValue();
        auto z = this->getZCoordinateValue();

        if (fabs(it->getXCoordinateValue()-x)<tol &&
                fabs(it->getYCoordinateValue()-y)<tol &&
                fabs(it->getZCoordinateValue()-z)<tol)
        {
            qCritical()<<"Warning: a node "<<it->objectName()<<" exists at this location in "<<this->objectName()<<", using it";
            qDebug()<<"X coord: "<<it->getXCoordinateValue()<<",Y coord: "<<it->getYCoordinateValue()<<",Z coord: "<<it->getZCoordinateValue();

            //            qCritical()<<"this "<<this->objectName()<<"x"<<this->getXCoordinateValue()<<"the node "<<it->objectName()<<" x "<<it->getXCoordinateValue();
            //            qCritical()<<"this "<<this->objectName()<<"y"<<this->getYCoordinateValue()<<"the node "<<it->objectName()<<" y "<<it->getYCoordinateValue();
            //            qCritical()<<"this "<<this->objectName()<<"z"<<this->getZCoordinateValue()<<"the node "<<it->objectName()<<" z "<<it->getZCoordinateValue();

            aNode = it;
            break;
        }
    }

    //If not create one
    if(aNode == nullptr)
    {
        theDomain->createObject(this->objectName()+ "AssociatedNode" , "RNode");
        aNode = (qobject_cast<RNode *>(theDomain->getLastAddedObject()));

        //create the lamda functors
        std::function<double(void)> xCoordinatelambda = [this]()
        {
            return this->getXCoordinateValue();
        };
        std::function<double(void)> yCoordinatelambda = [this]()
        {
            return this->getYCoordinateValue();
        };
        std::function<double(void)> zCoordinatelambda = [this]()
        {
            return this->getZCoordinateValue();
        };

        //Pass the functor callback for the get coordinate functions
        aNode->setXCoordinate(xCoordinatelambda);
        aNode->setYCoordinate(yCoordinatelambda);
        aNode->setZCoordinate(zCoordinatelambda);

    }

    //Set the association
    theAssociatedNode = aNode;
    aNode->setAssociatedPoint(this);


    return 0;
}


RNode *RPoint::getAssociatedNode()
{
    return theAssociatedNode.data();
}


void RPoint::setCoordinateVector(std::vector<double>& vecIn)
{
    if(vecIn.size() != 3)
    {
        qCritical()<<"The vector coming into RPoint does not have three values";
        return;
    }

    theXCoordinateDouble = vecIn[0];
    theYCoordinateDouble = vecIn[1];
    theZCoordinateDouble = vecIn[2];

    xSet = true;
    ySet = true;
    zSet = true;
    createOrSetAssociatedNode();
}


QString RPoint::getXCoordinate()
{
    return getStringParameter(&theXCoordinateDouble, theXCoordinateParameter);
}


void RPoint::setXCoordinate(QString value)
{
    setStringParameter(value, &theXCoordinateDouble, &theXCoordinateParameter);

    if(theXCoordinateParameter)
        theXCoordinateParameter->setNodalParameterFlag(true);

    xSet = true;
    createOrSetAssociatedNode();
}


void RPoint::setXCoordinate(std::function<double(void)>& fxn)
{
    xCoordinateLambda = std::make_unique<std::function<double(void)>>(fxn);

    //Null the parameters... in case they were previously set
    theXCoordinateParameter = nullptr;

    xSet = true;
    createOrSetAssociatedNode();
}


void RPoint::setXCoordinate(double value)
{
    theXCoordinateDouble = value;

    //Null the parameters and release the lambda... in case they were previously set
    theXCoordinateParameter = nullptr;
    xCoordinateLambda.reset();

    //Update the coordinates of the associated nodes (the opensees coordinates)
    theAssociatedNode->updateNode();

    xSet = true;
    createOrSetAssociatedNode();
}


double RPoint::getXCoordinateValue()
{
    if(theXCoordinateParameter)
    {
        RParametricPoint* parametricPoint = qobject_cast<RParametricPoint*>(theXCoordinateParameter.data());

        if(parametricPoint)
        {
            return parametricPoint->evaluateExpression(RGradientType::NONE);
        }
        else
        {
            return theXCoordinateParameter->getCurrentValue();
        }
    }
    else if(xCoordinateLambda)
        return (*xCoordinateLambda)();
    else
        return theXCoordinateDouble;
}


QString RPoint::getYCoordinate()
{
    return getStringParameter(&theYCoordinateDouble, theYCoordinateParameter);
}


void RPoint::setYCoordinate(QString value)
{
    setStringParameter(value, &theYCoordinateDouble, &theYCoordinateParameter);

    if(theYCoordinateParameter)
        theYCoordinateParameter->setNodalParameterFlag(true);

    ySet = true;
    createOrSetAssociatedNode();
}


void RPoint::setYCoordinate(std::function<double(void)>& fxn)
{
    yCoordinateLambda = std::make_unique<std::function<double(void)>>(fxn);

    //Null the parameters... in case they were previously set
    theYCoordinateParameter = nullptr;

    ySet = true;
    createOrSetAssociatedNode();
}


void RPoint::setYCoordinate(double value)
{
    theYCoordinateDouble = value;

    //Null the parameters and release the lambda... in case they were previously set
    theYCoordinateParameter = nullptr;
    yCoordinateLambda.reset();

    //Update the coordinates of the associated nodes (the opensees coordinates)
    theAssociatedNode->updateNode();

    ySet = true;
    createOrSetAssociatedNode();
}


double RPoint::getYCoordinateValue()
{
    if(theYCoordinateParameter)
    {
        RParametricPoint* parametricPoint = qobject_cast<RParametricPoint*>(theYCoordinateParameter.data());

        if(parametricPoint)
        {
            return parametricPoint->evaluateExpression(RGradientType::NONE);
        }
        else
        {
            return theYCoordinateParameter->getCurrentValue();
        }
    }
    else if(yCoordinateLambda)
        return (*yCoordinateLambda)();
    else
        return theYCoordinateDouble;
}


QString RPoint::getZCoordinate()
{
    return getStringParameter(&theZCoordinateDouble, theZCoordinateParameter);
}


void RPoint::setZCoordinate(QString value)
{
    setStringParameter(value, &theZCoordinateDouble, &theZCoordinateParameter);

    if(theZCoordinateParameter)
        theZCoordinateParameter->setNodalParameterFlag(true);

    zSet = true;
    createOrSetAssociatedNode();
}


void RPoint::setZCoordinate(std::function<double(void)>& fxn)
{
    zCoordinateLambda = std::make_unique<std::function<double(void)>>(fxn);

    //Null the parameters... in case they were previously set
    theZCoordinateParameter = nullptr;

    zSet = true;
    createOrSetAssociatedNode();
}


void RPoint::setZCoordinate(double value)
{
    theZCoordinateDouble = value;

    //Null the parameters and release the lambda... in case they were previously set
    theZCoordinateParameter = nullptr;
    zCoordinateLambda.reset();

    //Update the coordinates of the associated nodes (the opensees coordinates)
    theAssociatedNode->updateNode();

    zSet = true;
    createOrSetAssociatedNode();
}


double RPoint::getZCoordinateValue()
{
    if(theZCoordinateParameter)
    {
        RParametricPoint* parametricPoint = qobject_cast<RParametricPoint*>(theZCoordinateParameter.data());

        if(parametricPoint)
        {
            return parametricPoint->evaluateExpression(RGradientType::NONE);
        }
        else
        {
            return theZCoordinateParameter->getCurrentValue();
        }
    }
    else if(zCoordinateLambda)
        return (*zCoordinateLambda)();
    else
        return theZCoordinateDouble;
}


QString  RPoint::getXDisplacementRestraint() const
{
    if (theXDisplacementRestraint) {
        return "Fixed";
    }
    else {
        return "Free";
    }
}


void RPoint::setXDisplacementRestraint(QString value)
{
    if (value.toLower().startsWith("fix")) {

        theXDisplacementRestraint = true;
    }
    else {

        theXDisplacementRestraint = false;
    }
}


QString  RPoint::getYDisplacementRestraint() const
{
    if (theYDisplacementRestraint) {
        return "Fixed";
    }
    else {
        return "Free";
    }
}


void RPoint::setYDisplacementRestraint(QString value)
{
    if (value.toLower().startsWith("fix")) {

        theYDisplacementRestraint = true;
    }
    else {

        theYDisplacementRestraint = false;
    }
}


QString  RPoint::getZDisplacementRestraint() const
{
    if (theZDisplacementRestraint) {
        return "Fixed";
    }
    else {
        return "Free";
    }
}


void RPoint::setZDisplacementRestraint(QString value)
{
    if (value.toLower().startsWith("fix")) {

        theZDisplacementRestraint = true;
    }
    else {

        theZDisplacementRestraint = false;
    }
}


QString  RPoint::getXRotationRestraint() const
{
    if (theXRotationRestraint) {
        return "Fixed";
    }
    else {
        return "Free";
    }
}


void RPoint::setXRotationRestraint(QString value)
{
    if (value.toLower().startsWith("fix")) {

        theXRotationRestraint = true;
    }
    else {

        theXRotationRestraint = false;
    }
}


QString  RPoint::getYRotationRestraint() const
{
    if (theYRotationRestraint) {
        return "Fixed";
    }
    else {
        return "Free";
    }
}


void RPoint::setYRotationRestraint(QString value)
{
    if (value.toLower().startsWith("fix")) {

        theYRotationRestraint = true;
    }
    else {

        theYRotationRestraint = false;
    }
}


QString  RPoint::getZRotationRestraint() const
{
    if (theZRotationRestraint) {
        return "Fixed";
    }
    else {
        return "Free";
    }
}


void RPoint::setZRotationRestraint(QString value)
{
    if (value.toLower().startsWith("fix")) {

        theZRotationRestraint = true;
    }
    else {

        theZRotationRestraint = false;
    }
}


QString RPoint::getXForce()
{
    return getStringParameter(&theXForceDouble, theXForceParameter);
}


QObject * RPoint::getXForceParameter() const
{
    return theXForceParameter;
}


void RPoint::setXForce(QString value)
{
    auto theLoad = theDomain->findChild<RPointLoad*>(value);

    if(theLoad)
    {
        setStringParameter(theLoad->getParameter()->objectName(),&theXForceDouble, &theXForceParameter);
        theLoad->setPoint(this);
    }
    else
    {
        setStringParameter(value, &theXForceDouble, &theXForceParameter);
    }

}


void RPoint::setXForce(RParameter *value)
{
    theXForceParameter = value;
}


void RPoint::setXForce(std::function<double(void)> lambda)
{
    theXForceLambda = std::move(lambda);
}


double RPoint::getXForceValue()
{

    if (theXForceParameter)
    {
        return theXForceParameter->getCurrentValue();
    }

    return theXForceLambda();
}


RParameter* RPoint::getXMass() const
{
    return theXMassParameter;
}

double RPoint::getXMassValue()
{
    if (theXMassParameter)
    {
        return theXMassParameter->getCurrentValue();
    }
    else
        return 0.0;

}


void RPoint::setXMass(QObject *value)
{
    theXMassParameter = qobject_cast<RParameter*>(value);
}


RParameter* RPoint::getYMass() const
{
    return theYMassParameter;
}


double RPoint::getYMassValue()
{

    if (theYMassParameter)
    {
        return theYMassParameter->getCurrentValue();
    }
    else
        return 0.0;

}


void RPoint::setYMass(QObject *value)
{
    theYMassParameter = qobject_cast<RParameter*>(value);
}


RParameter* RPoint::getZMass() const
{
    return theZMassParameter;
}


double RPoint::getZMassValue()
{
    if (theZMassParameter)
    {
        return theZMassParameter->getCurrentValue();
    }
    else
        return 0.0;

}


void RPoint::setZMass(QObject *value)
{
    theZMassParameter = qobject_cast<RParameter*>(value);
}


RParameter* RPoint::getMass() const
{
    return theMassParameter;
}


double RPoint::getMassValue()
{
    if (theMassParameter)
    {
        return theMassParameter->getCurrentValue();
    }
    else
        return 0.0;

}


void RPoint::setMass(QObject *value)
{
    theMassParameter = qobject_cast<RParameter*>(value);
}


QString RPoint::getYForce()
{
    return getStringParameter(&theYForceDouble, theYForceParameter);
}


QObject * RPoint::getYForceParameter() const
{
    return theYForceParameter;
}


void RPoint::setYForce(QString value)
{
    auto theLoad = theDomain->findChild<RPointLoad*>(value);

    if(theLoad)
    {
        setStringParameter(theLoad->getParameter()->objectName(),&theYForceDouble, &theYForceParameter);
        theLoad->setPoint(this);
    }
    else
    {
        setStringParameter(value, &theYForceDouble, &theYForceParameter);
    }
}


void RPoint::setYForce(RParameter *value)
{
    theYForceParameter = value;
}


void RPoint::setYForce(std::function<double(void)> lambda)
{
    theYForceLambda = std::move(lambda);
}


double RPoint::getYForceValue()
{

    // First do things if its a parameter
    if (theYForceParameter  != nullptr) {

        return theYForceParameter->getCurrentValue();
    }

    // Having checked those special cases, proceed with the normal case:
    return theYForceLambda();
}


QString RPoint::getZForce()
{
    return getStringParameter(&theZForceDouble, theZForceParameter);
}


QObject * RPoint::getZForceParameter() const
{
    return theZForceParameter;
}


void RPoint::setZForce(QString value)
{    
    auto theLoad = theDomain->findChild<RPointLoad*>(value);

    if(theLoad)
    {
        setStringParameter(theLoad->getParameter()->objectName(),&theZForceDouble, &theZForceParameter);
        theLoad->setPoint(this);
    }
    else
    {
        setStringParameter(value, &theZForceDouble, &theZForceParameter);
    }
}


void RPoint::setZForce(RParameter *value)
{
    theZForceParameter = value;
}


void RPoint::setZForce(std::function<double(void)> lambda)
{
    theZForceLambda = std::move(lambda);
}


double RPoint::getZForceValue()
{
    // First do things if its a parameter
    if (theZForceParameter)
    {
        return theZForceParameter->getCurrentValue();
    }

    return theZForceLambda(); // Having checked those special cases, proceed with the normal case:
}


QString RPoint::getXMoment()
{
    return getStringParameter(&theXMomentDouble, theXMomentParameter);
}


QObject * RPoint::getXMomentParameter() const
{
    return theXMomentParameter;
}


void RPoint::setXMoment(QString value)
{
    setStringParameter(value, &theXMomentDouble, &theXMomentParameter);
}


double RPoint::getXMomentValue()
{
    return getDoubleParameter(&theXMomentDouble, theXMomentParameter);
}


QString RPoint::getYMoment()
{
    return getStringParameter(&theYMomentDouble, theYMomentParameter);
}


QObject * RPoint::getYMomentParameter() const
{
    return theYMomentParameter;
}


void RPoint::setYMoment(QString value)
{
    setStringParameter(value, &theYMomentDouble, &theYMomentParameter);
}


double RPoint::getYMomentValue()
{
    return getDoubleParameter(&theYMomentDouble, theYMomentParameter);
}


QString RPoint::getZMoment()
{
    return getStringParameter(&theZMomentDouble, theZMomentParameter);
}


QObject * RPoint::getZMomentParameter() const
{
    return theZMomentParameter;
}


void RPoint::setZMoment(QString value)
{
    setStringParameter(value, &theZMomentDouble, &theZMomentParameter);
}


double RPoint::getZMomentValue()
{
    return getDoubleParameter(&theZMomentDouble, theZMomentParameter);
}


double RPoint::getXDisplacement() const
{
    if (theAssociatedNode != 0) {

        return theAssociatedNode->getXDisplacement();
    }
    else {
        return 0.0;
    }
}


double RPoint::getYDisplacement() const
{
    if (theAssociatedNode != 0) {

        return theAssociatedNode->getYDisplacement();
    }
    else {
        return 0.0;
    }
}


double RPoint::getZDisplacement() const
{
    if (theAssociatedNode != 0) {

        return theAssociatedNode->getZDisplacement();
    }
    else {
        return 0.0;
    }
}


double RPoint::getXRotation() const
{
    if (theAssociatedNode != 0) {

        return theAssociatedNode->getXRotation();
    }
    else {
        return 0.0;
    }
}


double RPoint::getYRotation() const
{
    if (theAssociatedNode != 0) {

        return theAssociatedNode->getYRotation();
    }
    else {
        return 0.0;
    }
}


double RPoint::getZRotation() const
{
    if (theAssociatedNode != 0) {

        return theAssociatedNode->getZRotation();
    }
    else {
        return 0.0;
    }
}


double RPoint::getXCoordinateFOGradient(RParameter* theParameter)
{
    //Check to see if the ZCoordinate Parameter is actually a parametric point
    RParametricPoint *parametricPoint = qobject_cast<RParametricPoint *>(theXCoordinateParameter.data());
    if (parametricPoint)
    {
        //Evaluate the model using DDM
        parametricPoint->evaluateExpression(RGradientType::DDM);

        //Pulls the DDM Parameter Map from algebraic model and checks for parameter in map
        auto ddmMap = parametricPoint->getFOFDMParameterMap();

        return ddmMap.value(theParameter,0.0);
    }
    else if (theXCoordinateParameter == theParameter)
    {
        return 1.0;
    }

    else
    {
        return 0.0;
    }
}


double RPoint::getYCoordinateFOGradient(RParameter* theParameter)
{
    //Check to see if the ZCoordinate Parameter is actually a parametric point
    RParametricPoint *parametricPoint = qobject_cast<RParametricPoint *>(theYCoordinateParameter.data());
    if (parametricPoint)
    {
        //Evaluate the model using DDM
        parametricPoint->evaluateExpression(RGradientType::DDM);

        //Pulls the DDM Parameter Map from algebraic model and checks for parameter in map
        auto ddmMap = parametricPoint->getFOFDMParameterMap();

        return ddmMap.value(theParameter,0.0);
    }
    else if (theYCoordinateParameter == theParameter)
    {
        return 1.0;
    }

    else
    {
        return 0.0;
    }
}


double RPoint::getZCoordinateFOGradient(RParameter* theParameter)
{
    //Check to see if the ZCoordinate Parameter is actually a parametric point
    RParametricPoint *parametricPoint = qobject_cast<RParametricPoint *>(theZCoordinateParameter.data());
    if (parametricPoint)
    {
        //Evaluate the model using DDM
        parametricPoint->evaluateExpression(RGradientType::DDM);

        //Pulls the DDM Parameter Map from algebraic model and checks for parameter in map
        auto ddmMap = parametricPoint->getFOFDMParameterMap();

        return ddmMap.value(theParameter,0.0);
    }
    else if (theZCoordinateParameter == theParameter)
    {
        return 1.0;
    }

    else
    {
        return 0.0;
    }
}


double RPoint::getXCoordinateSOGradient(RParameter* theParameter)
{
    //Check to see if the ZCoordinate Parameter is actually a parametric point
    RParametricPoint *parametricPoint = qobject_cast<RParametricPoint *>(theXCoordinateParameter.data());
    if (parametricPoint)
    {
        //Evaluate the model using DDM
        parametricPoint->evaluateExpression(RGradientType::D2DM);

        //Pulls the DDM Parameter Map from algebraic model and checks for parameter in map
        auto ddmMap = parametricPoint->getSOFDMParameterMap();

        return ddmMap.value(theParameter,0.0);
    }
    else if (theXCoordinateParameter == theParameter)
    {
        return 0.0;
    }

    else
    {
        return 0.0;
    }
}


double RPoint::getYCoordinateSOGradient(RParameter* theParameter)
{
    //Check to see if the ZCoordinate Parameter is actually a parametric point
    RParametricPoint *parametricPoint = qobject_cast<RParametricPoint *>(theYCoordinateParameter.data());
    if (parametricPoint)
    {
        //Evaluate the model using DDM
        parametricPoint->evaluateExpression(RGradientType::D2DM);

        //Pulls the DDM Parameter Map from algebraic model and checks for parameter in map
        auto ddmMap = parametricPoint->getSOFDMParameterMap();

        return ddmMap.value(theParameter,0.0);
    }
    else if (theYCoordinateParameter == theParameter)
    {
        return 0.0;
    }

    else
    {
        return 0.0;
    }
}


double RPoint::getZCoordinateSOGradient(RParameter* theParameter)
{
    //Check to see if the ZCoordinate Parameter is actually a parametric point
    RParametricPoint *parametricPoint = qobject_cast<RParametricPoint *>(theZCoordinateParameter.data());
    if (parametricPoint)
    {
        //Evaluate the model using DDM
        parametricPoint->evaluateExpression(RGradientType::D2DM);

        //Pulls the DDM Parameter Map from algebraic model and checks for parameter in map
        auto ddmMap = parametricPoint->getSOFDMParameterMap();

        return ddmMap.value(theParameter,0.0);
    }
    else if (theZCoordinateParameter == theParameter)
    {
        return 0.0;
    }
    else
    {
        return 0.0;
    }
}


void RPoint::deleteAssociatedNode()
{
    if(theAssociatedNode != nullptr)
    {
        theDomain->deleteObject(theAssociatedNode->objectName());

        theAssociatedNode = nullptr;
    }
}


bool RPoint::getIsOn() const
{
    return isOn;
}


void RPoint::setIsOn(bool value)
{
    if(value == false)
    {
        this->deleteAssociatedNode();
    }
    else
    {
        this->createOrSetAssociatedNode();
    }

    isOn = value;
}


