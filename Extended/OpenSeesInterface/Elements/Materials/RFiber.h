#ifndef RFiber_H
#define RFiber_H
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

//Wrapper class for the OpenSees Fiber

#include "RObject.h"

class UniaxialMaterial;
class RUniaxialMaterial;
class Fiber;

class RFiber : public RObject
{
    Q_OBJECT

public:
    RFiber(QObject *parent, QString &name);
    ~RFiber();

    void setFibreYCoordinate(double value);
    void setFibreZCoordinate(double value);

    void setFibreYCoordinate(std::function<double(void)> value);
    void setFibreZCoordinate(std::function<double(void)> value);

    double getFibreYCoordinate(void);
    double getFibreZCoordinate(void);

    void setLength(double value);
    void setArea(double value);

    void setLength(std::function<double(void)> value);
    void setArea(std::function<double(void)> value);

    double getVolume(void);
    double getLength(void);
    double getArea(void);

    UniaxialMaterial *getTheMaterial() const;

    void setTheUniaxialMaterial(RUniaxialMaterial *value);

    virtual Fiber* getTheFiber(void) = 0;

    //Class to update the fiber in the case of reoccuring analyses
    virtual int update(void) = 0;

    QString getDamageType() const;
    void setDamageType(const QString &value);

    int generateOpenSeesInput(std::string& object, int flag);

protected:

    RUniaxialMaterial *theMaterial;

    QString damageType;

private:

    std::function<double(void)> theLengthLambda;
    std::function<double(void)> theAreaLambda;
    std::function<double(void)> theCentroidYCoordinateLambda;
    std::function<double(void)> theCentroidZCoordinateLambda;

    double theLength = 0.0;
    double theArea = 0.0;
    double theYCoordinate = 0.0;
    double theZCoordinate = 0.0;

};

#endif


