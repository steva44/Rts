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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RConfinedConcreteMaterialKnowledge.h"
#include "RContinuousRandomVariable.h"
#include "RParameter.h"

#include <math.h>

RConfinedConcreteMaterialKnowledge::RConfinedConcreteMaterialKnowledge(RDomain *theDomain,
                                                                       QString componentName,
                                                                       QString strengthString)
    : RConcreteMaterialKnowledge(theDomain, componentName,strengthString)
{    
    fcc = nullptr;
    ftcc = nullptr;
    fyh = nullptr;
    rhoTrans = nullptr;
    fccUlt = nullptr;
    epscc = nullptr;
    epsccu = nullptr;
    stRatio = nullptr;
}


RConfinedConcreteMaterialKnowledge::~RConfinedConcreteMaterialKnowledge()
{

}


// Confined compressive strength
RParameter* RConfinedConcreteMaterialKnowledge::getCompressiveStrength(void)
{
    return this->getFpcc();
}


// Confined compressive strength
RParameter* RConfinedConcreteMaterialKnowledge::getFpcc(void)
{        
    if(fcc == nullptr)
    {
        if(rhoTrans  == nullptr || fyh == nullptr)
        {
            qDebug()<<"Warning: transverse reinforcement ratio or the transverse yield strength was not provide "<<this->objectName()<<" in "<<__FUNCTION__<<" setting f'cc to f'c";
            fcc = fpc;
            return fcc;
        }


        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto fc = this->getFpc()->getCurrentValue();

            auto fyh_ = fyh->getCurrentValue();

            auto rhoTrans_ = rhoTrans->getCurrentValue();

            auto fl = rhoTrans_*fyh_/2.0;

            // Ke = confinement effectiveness coeficient = Ae/Acc
            // Assume Ke = 1.0 for now.. that the area of confined concrete is very close to the total area of concrete
            // Look in http://docs.csiamerica.com/manuals/sap2000/Technical%20Notes/S-TN-MAT-001.pdf for more info
            auto Ke = 1.0;

            auto flp = Ke*fl;

            //set fcc [expr $fc*(-1.254+2.254*sqrt(1.0+7.94*$fl/$fc)-2.0*$fl/$fc)]; from an OpenSees example
            auto fpcc = fc*(-1.254+2.254*sqrt(1.0+7.94*flp/fc)-2.0*flp/fc);

            return fpcc;

        };

        theDomain->createObject(theComponentName + "Fpcc", "RConstant");
        fcc = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        fcc->setCurrentValue(fxn);
    }

    return fcc;
}


// strain at compression strength
RParameter* RConfinedConcreteMaterialKnowledge::getStrainAtfc(void)
{
    return this->getStrainAtfcc();
}


// strain at confined compression strength
RParameter*  RConfinedConcreteMaterialKnowledge::getStrainAtfcc(void)
{
    if (epscc == nullptr)
    {

        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto fc = this->getFpc()->getCurrentValue();

            auto fpcc = this->getFpcc()->getCurrentValue();

            auto epsc = this->getEpsc0()->getCurrentValue();

            // http://docs.csiamerica.com/manuals/sap2000/Technical%20Notes/S-TN-MAT-001.pdf (pg 13)
            auto epspcc = (5.0*((fpcc/fc)-1.0)+1.0)*epsc;

            return epspcc;

        };

        theDomain->createObject(theComponentName + "StrainAtConfinedFpc", "RConstant");
        epscc = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        epscc->setCurrentValue(fxn);
    }

    return epscc;
}


// Stress at ultimate (crushing) strain
RParameter* RConfinedConcreteMaterialKnowledge::getUltimateCrushingStress(void)
{    
    return this->getUltimateConfinedCrushingStress();
}


// Stress at ultimate (crushing) strain
RParameter* RConfinedConcreteMaterialKnowledge::getUltimateConfinedCrushingStress(void)
{   
    if(fccUlt == nullptr)
    {
        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto fpcc = this->getFpcc()->getCurrentValue();

            auto Ec = this->getYoungsModulus()->getCurrentValue();

            auto epsiloncc = this->getStrainAtfcc()->getCurrentValue();

            auto epsiloncu = this->getEpscu()->getCurrentValue();

            // Set ecr [expr $Ec/($Ec-$fcc/$epscc)]; #r factor (Eq. 6 in Mander, 1988)
            auto ecr = Ec/(Ec-fpcc/epsiloncc);

            // Strength that corresponds to ultimate strain (Mander, 1988)
            return fpcc*epsiloncu/epsiloncc*ecr/(ecr-1.0+pow(epsiloncu/epsiloncc,ecr));
        };

        theDomain->createObject(theComponentName + "fccUlt", "RConstant");
        fccUlt = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        fccUlt->setCurrentValue(fxn);
    }

    return fccUlt;
}


// Ultimate (crushing) strain
RParameter*  RConfinedConcreteMaterialKnowledge::getUltimateCrushingStrain(void)
{    
    return this->getUltimateConfinedCrushingStrain();
}


// Ultimate confined (crushing) strain
RParameter* RConfinedConcreteMaterialKnowledge::getUltimateConfinedCrushingStrain(void)
{
    if (epsccu == nullptr)
    {
        if(rhoTrans  == nullptr || fyh == nullptr)
        {
            qDebug()<<"Warning: transverse reinforcement ratio or the transverse yield strength was not provide "<<this->objectName()<<" in "<<__FUNCTION__<<" setting epsccu to epscu";
            epsccu = epscu;
            return epsccu;
        }

        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto fc = this->getFpc()->getCurrentValue();

            auto fyh_ = fyh->getCurrentValue();

            auto rhoTrans_ = rhoTrans->getCurrentValue();

            //ultimate strain (by Dawn Lehman,1998, PEER 1998/01)
            return  0.004+0.14*fyh_/fc*rhoTrans_;
        };

        theDomain->createObject(theComponentName + "UltConfinedCrushStrain", "RConstant");
        epsccu = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        epsccu->setCurrentValue(fxn);
    }

    return epsccu;
}


// Ratio between fc and f'c for confined concrete model
RParameter* RConfinedConcreteMaterialKnowledge::getStRatio(void)
{
    if (stRatio == nullptr)
    {
        theDomain->createObject(theComponentName + "ratFccFpcc", "RConstant");
        stRatio = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        // Assume stRatio = 1.0;
        stRatio->setCurrentValue(1.0);
    }

    return stRatio;
}


// Yield strength of the hoop reinforcement
void RConfinedConcreteMaterialKnowledge::setFyh(RParameter *value)
{
    fyh = value;
}


// Reinforcement ratio of transverse reinforcement
void RConfinedConcreteMaterialKnowledge::setRhoTrans(RParameter *value)
{
    rhoTrans = value;
}


// Confined tensile strength
RParameter* RConfinedConcreteMaterialKnowledge::getTensileStrength(void)
{
    return this->getFtcc();
}


// Confined tensile strength
RParameter *RConfinedConcreteMaterialKnowledge::getFtcc()
{
    if (ftcc == nullptr)
    {

        std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> fxn = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
        {
            auto fc = this->getFpc()->getCurrentValue();

            // Assume 20% of f'c
            auto tensileReductionFactor = 0.20;

            return fc*tensileReductionFactor;
        };

        theDomain->createObject(theComponentName + "tensileStressAtConfinedFpc", "RConstant");
        ftcc = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        ftcc->setCurrentValue(fxn);
    }

    return ftcc;
}

