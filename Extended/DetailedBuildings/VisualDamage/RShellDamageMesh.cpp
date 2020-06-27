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

#include "RShellDamageMesh.h"
#include "RLinearShellElement.h"
#include "RComponent.h"

RShellDamageMesh::RShellDamageMesh(RComponent* component)
    : RDamageMesh(component)
{
    numRegionsAlong12 = 1;
    numRegionsAlong23 = 1;
}


RShellDamageMesh::~RShellDamageMesh()
{

}


void RShellDamageMesh::resetDamageMesh(void)
{
    for(auto&& it : theDamageRegionVector)
        it->reset();
}


std::vector<std::shared_ptr<RDamageRegion> > RShellDamageMesh::getDamageRegionVector() const
{
    return theDamageRegionVector;
}


void RShellDamageMesh::setDamageRegionVector(const std::vector<std::shared_ptr<RDamageRegion> > &value)
{
    theDamageRegionVector = value;
}


int RShellDamageMesh::createDamageMesh(void)
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
        // Make sure it is a shell element
        auto shellElement = dynamic_cast<RLinearShellElement*>(element);

        if(!shellElement)
        {
            qCritical()<<"Error, should be a shell element in "<<__FUNCTION__;
            return -1;
        }

        // Create a damage region
        auto damageRegion = std::make_shared<RShellSectionDamageRegion>();

        // Set the element to the region
        damageRegion->setTheElement(shellElement);

        theDamageRegionVector.push_back(damageRegion);
    }

    return 0;
}

size_t RShellDamageMesh::getNumRegionsAlong12() const
{
    return numRegionsAlong12;
}

void RShellDamageMesh::setNumRegionsAlong12(const size_t &value)
{
    numRegionsAlong12 = value;
}

size_t RShellDamageMesh::getNumRegionsAlong23() const
{
    return numRegionsAlong23;
}

void RShellDamageMesh::setNumRegionsAlong23(const size_t &value)
{
    numRegionsAlong23 = value;
}
