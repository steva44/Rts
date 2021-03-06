#ifndef RNDMaterial_H
#define RNDMaterial_H
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

class RUniaxialMaterial;
class RMaterialKnowledge;
class UniaxialMaterial;
class NDMaterial;

class RNDMaterial : public RObject
{
    Q_OBJECT

public:
    RNDMaterial(QObject *parent, QString &name);
    ~RNDMaterial();

    virtual NDMaterial* getTheNDMaterial(void) = 0;
    virtual int update(void) = 0;

    QStringList getActionList();

protected slots:

    virtual void plotStressStrainRelation(void);
    virtual void plotStrainHistoryRunOne(void);
    virtual void plotStrainHistoryRunTwo(void);
    virtual void plotStrainHistoryRunThree(void);
    virtual void plotStrainHistoryRunFour(void);
    virtual void plotStrainHistoryRunFive(void);
    virtual void plotStrainHistoryRunSix(void);
    virtual void plotStrainHistoryRunSeven(void);
    virtual void plotStrainHistoryRunEight(void);
    virtual void plotStrainHistoryRunNine(void);
    virtual void plotStrainHistoryRunTen(void);

private:

};

#endif


