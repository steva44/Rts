#ifndef RElasticMaterial_H
#define RElasticMaterial_H
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

//Wrapper class is used to construct an elastic uniaxial material object: OpenSees ElasticMaterial
//uniaxialMaterial Elastic $matTag $E <$eta> <$Eneg>
//$matTag	integer tag identifying material
//$E	tangent
//$eta	damping tangent (optional, default=0.0)
//$Eneg	tangent in compression (optional, default=E)
//http://opensees.berkeley.edu/wiki/index.php/Elastic_Uniaxial_Material
//Stevan Gavrilovic 12/2018

#include "RUniaxialMaterial.h"

class UniaxialMaterial;
class ElasticMaterial;

class RElasticMaterial : public RUniaxialMaterial
{
    Q_OBJECT

public:
    RElasticMaterial(QObject *parent, QString &name);
    ~RElasticMaterial();

    int update(void);

    UniaxialMaterial* getUniaxialMaterial();

    RParameter *getTheEParameter() const;
    void setTheEParameter(RParameter *value);

    RParameter *getTheEtaParameter() const;
    void setTheEtaParameter(RParameter *value);

    RParameter *getTheEnegParameter() const;
    void setTheEnegParameter(RParameter *value);

    int generateOpenSeesInput(std::string& object, int flag);

private:

    RParameter* theEParameter;
    RParameter* theEtaParameter;
    RParameter* theEnegParameter;

    std::unique_ptr<ElasticMaterial> theElasticMaterial;
    int createElasticMaterial(void);
};

#endif


