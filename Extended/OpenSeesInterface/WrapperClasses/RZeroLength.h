#ifndef RZeroLength_H
#define RZeroLength_H
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

// Wrapper class for OpenSees ZeroLength
// Stevan Gavrilovic 12/2018

#include "RElement.h"
#include "ZeroLength.h"

// Note this class is still a work in progress
// This command is used to construct a zeroLength element object, which is defined by two nodes at the same location. The nodes are connected by multiple UniaxialMaterial objects to represent the force-deformation relationship for the element.
// element zeroLength $eleTag $iNode $jNode -mat $matTag1 $matTag2 ... -dir $dir1 $dir2 ...<-doRayleigh $rFlag> <-orient $x1 $x2 $x3 $yp1 $yp2 $yp3>

// $eleTag	unique element object tag
// $iNode $jNode	end nodes
// $matTag1 $matTag2 ...	tags associated with previously-defined UniaxialMaterials
// $dir1 $dir2 ...	material directions:
// 1,2,3 - translation along local x,y,z axes, respectively;
// 4,5,6 - rotation about local x,y,z axes, respectively
// $x1 $x2 $x3	vector components in global coordinates defining local x-axis (optional)
// $yp1 $yp2 $yp3	vector components in global coordinates defining vector yp which lies in the local x-y plane for the element. (optional)
// $rFlag	optional, default = 0
// rFlag = 0 NO RAYLEIGH DAMPING (default)
// rFlag = 1 include rayleigh damping
// NOTE:
// If the optional orientation vectors are not specified, the local element axes coincide with the global axes. Otherwise the local z-axis is defined by the cross product between the vectors x and yp vectors specified on the command line.
// The valid queries to a zero-length element when creating an ElementRecorder object are 'force,' 'deformation,' and 'material $i matArg1 matArg2 ...' Where $i is an integer indicating which of the materials whose data is to be output (a 1 corresponds to $matTag1, a 2 to $matTag2, and so on). EXAMPLE:

//**********NOTE*********: THIS WRAPPER ELEMENT EXCLUDES RAYLEIGH DAMPING FOR NOW BUT THAT OPTION EXISTS FOR FUTURE IMPLEMENTATION

class RNode;
class RParameter;
class UniaxialMaterial;
class ZeroLength;
class Element;

class RZeroLength : public RElement
{
    Q_OBJECT

public:
    RZeroLength(QObject *parent, QString name);
    ~RZeroLength();

    int getNumNodes();
    RNode* getNode(int i) const;
    int setNode(int i, RNode *theNode);

    Element* getTheOpenSeesElement(void);

    void print(void);
    int update();

    int getResponseInGlobalXYZ(QVector<double> *response, QString type, double xi,RParameter *theParameter, RParameter *theParameter2, RGradientType theGradientType);
    void getGlobalDispResponse(double xi, QVector<double> *response);
    double getForceResponse(double xi, QVector<double> *response);
    double getStressResponse(QString type, double xi);
    double getStrainResponse(QString type);

    std::vector<double> getVecXAxis() const;
    void setVecXAxis(const std::vector<double> &value);

    UniaxialMaterial **getTheUniaxialMaterials() const;
    void setTheUniaxialMaterials(UniaxialMaterial **value);

    std::vector<int> getTheMaterialDirections() const;
    void setTheMaterialDirections(const std::vector<int> &value);

    std::vector<double> getVecYAxis() const;
    void setVecYAxis(const std::vector<double> &value);

private:
    int createZeroLengthElement(void);
    int initialize(void);

    RNode *theNode1;
    RNode *theNode2;

    std::vector<double> VecXAxis;
    std::vector<double> VecYAxis;
    UniaxialMaterial** theUniaxialMaterials;
    std::vector<int> theMaterialDirections;

    //Raw ptr; OpenSees controls the destruction, otherwise program crashes on exit
    ZeroLength* theOpenSeesElement;

};

#endif
