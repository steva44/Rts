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

#include "RFrameDamageMesh.h"
#include "RFibreDamageRegion.h"
#include "RFiberSection3d.h"
#include "RNonlinearBeamColumnElement.h"
#include "RSectionForceDeformation.h"
#include "RComponent.h"

RFrameDamageMesh::RFrameDamageMesh(RComponent* component)
    : RDamageMesh(component)
{
    numRegionsAlongMember = 1;
}


RFrameDamageMesh::~RFrameDamageMesh()
{

}


void RFrameDamageMesh::resetDamageMesh(void)
{
    for(auto&& it : theDamageRegionVector)
        it->reset();
}


std::vector<std::shared_ptr<RDamageRegion> > RFrameDamageMesh::getDamageRegionVector() const
{
    return theDamageRegionVector;
}


void RFrameDamageMesh::setDamageRegionVector(const std::vector<std::shared_ptr<RDamageRegion> > &value)
{
    theDamageRegionVector = value;
}


int RFrameDamageMesh::createDamageMesh(void)
{

    // Get the damage mesh
    auto damageMesh = theComponent->getDamageMesh();

    if(damageMesh == nullptr)
    {
        qCritical()<<"Need a damage mesh "<<__FUNCTION__;
    }

    auto theComponentElementList = theComponent->getElementList();

    // Create a damage region for every element
    for(auto&& element : theComponentElementList)
    {
        // Make sure it is a beam column element
        auto beamColumnElem = dynamic_cast<RNonlinearBeamColumnElement*>(element);

        if(!beamColumnElem)
        {
            qCritical()<<"Error, should be a fibre discretized beam column element in "<<__FUNCTION__;
            return -1;
        }

        // Get the sections - each segment is a section of the beam/column
        auto vectorOfSegments = beamColumnElem->getTheSectionForceDeformation();

        if(vectorOfSegments == nullptr)
        {
            qCritical()<<"Error, could not get the sections from the element in "<<__FUNCTION__;
            return -1;
        }

        auto fibreLength = beamColumnElem->getLength()/static_cast<double>(vectorOfSegments->size());

        for(auto&& segment : *vectorOfSegments)
        {

            if(segment == nullptr)
            {
                qCritical()<<"Error, could not get the segment from the element in "<<__FUNCTION__;
                return -1;
            }

            auto damageRegion = segment->getSegmentDamageRegion();

            damageRegion->setFibreLength(fibreLength);

            auto fiberSection = dynamic_cast<RFiberSection3d*>(segment);

            damageRegion->setTheSection(fiberSection);

            damageRegion->setTheElement(beamColumnElem);

            theDamageRegionVector.push_back(damageRegion);
        }
    }

    return 0;
}

size_t RFrameDamageMesh::getNumRegionsAlongMember() const
{
    return numRegionsAlongMember;
}

void RFrameDamageMesh::setNumRegionsAlongMember(const size_t &value)
{
    numRegionsAlongMember = value;
}
