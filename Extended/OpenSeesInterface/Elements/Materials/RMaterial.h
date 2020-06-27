#ifndef RMaterial_H
#define RMaterial_H
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

#include "RObject.h"
#include "RMaterialKnowledge.h"

class RMaterial : public RObject
{
    Q_OBJECT

public:
    RMaterial(QObject *parent, QString &name);
    ~RMaterial();

    virtual double getDensityValue() = 0;
    virtual int    commitState()     = 0;
    virtual int    revertToStart() = 0;
    virtual void   setMaterialKnowledge(RMaterialKnowledge *materialKnowledge);

    void setFibreCentroidYCoordinate(double value);
    void setFibreCentroidZCoordinate(double value);

    void setFibreCentroidYCoordinate(std::function<double(void)> value);
    void setFibreCentroidZCoordinate(std::function<double(void)> value);

    double getFibreCentroidYCoordinate(void);
    double getFibreCentroidZCoordinate(void);

    void setLength(double value);
    void setArea(double value);

    void setLength(std::function<double(void)> value);
    void setArea(std::function<double(void)> value);

    double getVolume(void);
    double getLength(void);
    double getArea(void);

private:

    std::function<double(void)> theLengthLambda;
    std::function<double(void)> theAreaLambda;
    std::function<double(void)> theCentroidYCoordinateLambda;
    std::function<double(void)> theCentroidZCoordinateLambda;

    double theLength = 0.0;
    double theArea = 0.0;
    double theCentroidYCoordinate = 0.0;
    double theCentroidZCoordinate = 0.0;

};

#endif


