#ifndef RElasticIsotropicMaterial_H
#define RElasticIsotropicMaterial_H
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

#include "RNDMaterial.h"

class RParameter;
class RMaterialKnowledge;
class NDMaterial;
class ElasticIsotropicThreeDimensional;

class RElasticIsotropicMaterial : public RNDMaterial
{
    Q_OBJECT

public:
    RElasticIsotropicMaterial(QObject *parent, QString &name);
    ~RElasticIsotropicMaterial();

    NDMaterial* getTheNDMaterial(void);
    int update(void);

    RMaterialKnowledge *getTheMaterialKnowledge() const;
    void setTheMaterialKnowledge(RMaterialKnowledge *value);

    int generateOpenSeesInput(std::string& object, int flag);

private:

    RMaterialKnowledge* theMatKnowledge;
    int createElasticIsotropicMaterial(void);

    std::unique_ptr<ElasticIsotropicThreeDimensional> theElasticIsotropic3DMaterial;

    RParameter* EParameter;
    RParameter* nuParameter;
    RParameter* rhoParameter;
};

#endif


