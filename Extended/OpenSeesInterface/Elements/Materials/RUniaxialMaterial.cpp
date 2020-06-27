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

#include "RUniaxialMaterial.h"
#include "RMaterialKnowledge.h"

#include "UniaxialMaterial.h"

RUniaxialMaterial::RUniaxialMaterial(QObject *parent, QString &name)
    : RObject(parent, name)
{
    theMaterial = nullptr;
}


RUniaxialMaterial::~RUniaxialMaterial()
{
}


QStringList RUniaxialMaterial::getActionList()
{
    QStringList actionList;

    actionList << "&Plot Stress Strain Backbone"
               << "&Plot Strain History Run One"
               << "&Plot Strain History Run Two"
               << "&Plot Strain History Run Three"
               << "&Plot Strain History Run Four"
               << "&Plot Strain History Run Five"
               << "&Plot Strain History Run Six"
               << "&Plot Strain History Run Seven"
               << "&Plot Strain History Run Eight"
               << "&Plot Strain History Run Nine"
               << "&Plot Strain History Run Ten";

    return actionList;
}


void RUniaxialMaterial::plotStressStrainBackbone(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RUniaxialMaterial::plotStrainHistoryRunOne(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RUniaxialMaterial::plotStrainHistoryRunTwo(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RUniaxialMaterial::plotStrainHistoryRunThree(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RUniaxialMaterial::plotStrainHistoryRunFour(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}

void RUniaxialMaterial::plotStrainHistoryRunFive(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RUniaxialMaterial::plotStrainHistoryRunSix(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RUniaxialMaterial::plotStrainHistoryRunSeven(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}

void RUniaxialMaterial::plotStrainHistoryRunEight(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RUniaxialMaterial::plotStrainHistoryRunNine(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RUniaxialMaterial::plotStrainHistoryRunTen(void)
{
    qDebug()<<"Implement the plotting of the stress strain relation in this material"<<this->objectName();
}


void RUniaxialMaterial::setMaterialKnowledge(RMaterialKnowledge *materialKnowledge)
{
    qCritical()<<"ERROR: setMaterialKnowledge function has not been implemented for this material";
}

