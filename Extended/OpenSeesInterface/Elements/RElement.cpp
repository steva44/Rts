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

#include "RElement.h"
#include "RDomain.h"
#include "RTimeHistoryRecorder.h"
#include "Element.h"

RElement::RElement(QObject *parent, QString &name)
    : RObject(parent, name)
{
    theDamageRegion = nullptr;

    // Create the transient recorder for this element
    theTransientRecorder = std::make_unique<RTimeHistoryRecorder>();
}


RElement::~RElement()
{
    // Remove the element from the OpenSees domain when it is destroyed in this one
    auto OpenSeesDomain = this->getTheOpenSeesDomain();

    if(OpenSeesDomain != nullptr)
    {
        auto removedObj = OpenSeesDomain->removeElement(this->getObjectTag());

        if(removedObj == nullptr)
        {
            qDebug()<<"Error in removing the element from the OpenSees domain";
        }
        else
        {
            delete removedObj;

            removedObj = nullptr;
        }
    }
}


std::vector<RNode*>  RElement::getNodes(void)
{
    unsigned long numNodes = static_cast<unsigned long>(this->getNumNodes());

    std::vector<RNode*> nodeVec(numNodes);

    for(unsigned long i = 0; i<numNodes; ++i)
    {
        nodeVec[i] = this->getNode(i);
    }

    return nodeVec;
}


RDamageRegion *RElement::getTheDamageRegion() const
{
    return theDamageRegion;
}


void RElement::setTheDamageRegion(RDamageRegion *value)
{
    theDamageRegion = value;
}


void RElement::print(void)
{
    return;
}


RTimeHistoryRecorder* RElement::getTheTransientRecorder() const
{
    return theTransientRecorder.get();
}


Element* RElement::getTheOpenSeesElement(void)
{
    qCritical()<<"This function "<<__FUNCTION__<<" must be implemented in the sublclass of "<<this->objectName();
    return nullptr;
}


