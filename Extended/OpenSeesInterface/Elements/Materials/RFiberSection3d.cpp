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

#include "RFiberSection3d.h"
#include "RFiber.h"
#include "RUniaxialMaterial.h"

#include "FiberSection3d.h"
#include "SectionForceDeformation.h"
#include "Fiber.h"
#include "ElasticMaterial.h"
#include "UniaxialMaterial.h"
#include "Information.h"

#include <sstream>

RFiberSection3d::RFiberSection3d(QObject *parent, QString &name)
    : RSectionForceDeformation(parent, name)
{
    theFiberSection3d = nullptr;
}


RFiberSection3d::~RFiberSection3d()
{
}


int RFiberSection3d::update(void)
{
    theFiberSection3d.reset();

    auto res = this->createFiberSection3d();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees 'FiberSection3d' in "<<this->objectName();
        return -1;
    }


    if(theFiberContainer)
        theFiberContainer->reset();

    return 0;
}


SectionForceDeformation* RFiberSection3d::getTheSectionForceDeformation(void)
{
    if(theFiberSection3d == nullptr)
    {
        this->createFiberSection3d();
    }

    return theFiberSection3d.get();
}


std::vector<RFiber*> RFiberSection3d::getFibers() const
{
    return fibers;
}


void RFiberSection3d::setFibers(std::vector<RFiber*> value)
{
    fibers = value;
}


int RFiberSection3d::getNumFibres() const
{
    return fibers.size();
}


int RFiberSection3d::createFiberSection3d()
{
    if(fibers.empty())
    {
        qDebug()<<this->objectName()<<" needs fibers in "<<__FUNCTION__;
        return -1;
    }

    auto numFibres = fibers.size();

    //Create a C-array of fibers to give to each section
    Fiber **fiber = new Fiber *[numFibres];

    for(auto i =0; i<numFibres; ++i)
    {
        auto theFibers = fibers[i]->getTheFiber();

        if(theFibers == nullptr)
        {
            qDebug()<<"Error, could not create an OpenSees fiber from "<< fibers[i]->objectName()<<" in "<<__FUNCTION__;

            delete [] fiber;
            return -1;
        }

        fiber[i] =  theFibers;
    }

    UniaxialMaterial *torsion = nullptr;

    if(torsionMaterial)
    {
        torsion = torsionMaterial->getUniaxialMaterial();

        if(torsion == nullptr)
        {
            qDebug()<<"Error, could not create an OpenSees 'Uniaxial Material' from "<<torsionMaterial->objectName()<<" in "<<__FUNCTION__;
        }
    }

    // FiberSection3d(int tag, int numFibers, UniaxialMaterial &torsion);
    theFiberSection3d = std::make_unique<FiberSection3d>(objectTag,numFibres,fiber,*torsion);

    delete [] fiber;

    return 0;
}


RUniaxialMaterial *RFiberSection3d::getTorsion() const
{
    return torsionMaterial;
}


void RFiberSection3d::setTorsion(RUniaxialMaterial *value)
{
    torsionMaterial = value;
}


int RFiberSection3d::generateOpenSeesInput(std::string& object, int flag)
{
    std::ostringstream out;

    out.precision(4);

    auto tag = std::to_string(this->objectTag);
    auto GJtag = std::to_string(torsionMaterial->getObjectTag());

    if(flag == 1) //tcl file
    {
        //        out<<"# section Fiber $secTag <-GJ $GJ> {...fibers... }"<<std::endl;
        out<<"section Fiber "<<tag<<" "<<GJtag<<" {;"<<std::endl;

        std::string fiberStr;
        for(auto&& fiber : fibers)
        {
            fiber->generateOpenSeesInput(fiberStr,flag);
        }

        out<<fiberStr<<"};"<<std::endl;
    }
    else if(flag == 2) //Python
    {
        out<<"Implement me in "<<__FUNCTION__<<std::endl;

        //out<<"# fiber(yloc, zloc, A, matTag)"<<std::endl;
        //out<<"fiber("<<yloc<<", "<<zloc<<", "<<A<<", "<<matTag<<")"<<std::endl;
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}



