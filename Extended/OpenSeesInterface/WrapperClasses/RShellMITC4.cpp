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

#include "RShellMITC4.h"
#include "RNode.h"
#include "RParameter.h"
#include "RSectionForceDeformation.h"
#include "RTimeHistoryRecorder.h"

#include "Information.h"

RShellMITC4::RShellMITC4(QObject *parent, QString name)
    : RLinearShellElement(parent, name)
{
    theOpenSeesElement = nullptr;
}


RShellMITC4::~RShellMITC4()
{

}


int  RShellMITC4::update(void)
{
    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    auto removedObj = theOSDomain->removeElement(objectTag);

    if(removedObj == nullptr)
    {
        qDebug()<<"Error in removing the element from the OpenSees domain"<<__FUNCTION__;
    }
    else
    {
        delete removedObj;

        removedObj = nullptr;
    }

    auto res = this->initialize();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees element in "<<this->objectName();
        return -1;
    }

    theOSDomain->addElement(theOpenSeesElement);

    // The element recorder
    theTransientRecorder->clear();

    return 0;
}




ShellMITC4* RShellMITC4::getTheOpenSeesElement(void)
{
    if(!theOpenSeesElement)
        this->createShellMITC4Element();

    return theOpenSeesElement;
}


int RShellMITC4::createShellMITC4Element(void)
{

    // Check to make sure the property objects are not null pointers
    if(theNode1 == nullptr || theNode1 == nullptr || theNode3 == nullptr || theNode4 == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs nodes";
        return -1;
    }
    if(theSectionForceDeformation == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a material";
        return -1;
    }

    return this->initialize();
}


void RShellMITC4::print(void)
{
    qDebug()<<"****************IMPLEMENT ME******************\n in:"<<this->objectName();

    return;
}


int RShellMITC4::initialize(void)
{
    auto Nd1 = theNode1->getObjectTag();
    auto Nd2 = theNode2->getObjectTag();
    auto Nd3 = theNode3->getObjectTag();
    auto Nd4 = theNode4->getObjectTag();

    SectionForceDeformation* section = theSectionForceDeformation->getTheSectionForceDeformation();

    if(section == nullptr)
    {
        qCritical()<<"Error, was not able to get the OpenSees 'Section Force Deformation' from "<<theSectionForceDeformation->objectName()<<" in"<<__FUNCTION__;
        return -1;
    }

    // ShellMITC4( int tag,
    // int node1,
    // int node2,
    // int node3,
    // int node4,
    // SectionForceDeformation &theMaterial,
    // bool updateBasis=false) ;
    theOpenSeesElement = new ShellMITC4(objectTag, Nd1, Nd2, Nd3, Nd4,(*section),true);

    if(theOpenSeesElement)
        return 0;
    else
        return -1;
}


int RShellMITC4::generateOpenSeesInput(std::string& object, int flag)
{

    SectionForceDeformation* section = theSectionForceDeformation->getTheSectionForceDeformation();

    if(section == nullptr)
    {
        qCritical()<<"Error, was not able to get the OpenSees 'Section Force Deformation' from "<<theSectionForceDeformation->objectName()<<" in"<<__FUNCTION__;
        return -1;
    }

    std::ostringstream out;

    out.precision(2);

    auto tag = std::to_string(this->objectTag);

    std::string iNode   = std::to_string(theNode1->getObjectTag());
    std::string jNode   = std::to_string(theNode2->getObjectTag());
    std::string kNode   = std::to_string(theNode3->getObjectTag());
    std::string lNode   = std::to_string(theNode4->getObjectTag());
    std::string secTag  = std::to_string(section->getTag());


    if(flag == 1) //tcl file
    {
        // out<<"# element ShellMITC4 $eleTag $iNode $jNode $kNode $lNode $secTag"<<std::endl;
        out<<"element ShellMITC4 "<<tag<<" "<<iNode<<" "<<jNode<<" "<<kNode<<" "<<lNode<<" "<<secTag<<std::endl;
    }
    else if(flag == 2) //Python
    {
        std::ostringstream elemList;

        elemList<<"elemList"<<tag<<" = ["<<iNode<<", "<<jNode<<", "<<kNode<<", "<<lNode<<" ];"<<std::endl;

        // out<<"element('ShellMITC4', eleTag, *eleNodes, secTag)"<<std::endl;
        out<<"element('ShellMITC4', "<<tag<<", "<<elemList.str()<<", "<<secTag<<")"<<std::endl;
    }
    else
    {
        return -1;
    }

    object.append(out.str());

    return 0;
}


//**********RESPONSE PROCEDURES******************


int RShellMITC4::getGlobalResponse(QVector<double> *response, QString type)
{

    if (type == QString("Displacement"))
    {
        this->getGlobalDisplacementResponse(response);
    }
    else if (type == QString("Forces"))
    {
        this->getGlobalForceResponse(response);
    }
    else
    {
        qDebug()<<"Could not understand the type of response in "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    return 0;
}


int RShellMITC4::getLocalResponse(QVector<double> *response, QString type)
{

    if(response == nullptr)
    {
        qDebug()<<"A null response object was given in "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }
    if (type == QString("Stress"))
    {
        this->getLocalStressResponse(response);
    }
    else if (type == QString("Strain"))
    {
        this->getLocalStrainResponse(response);
    }
    else
    {
        qDebug()<<"Could not understand the type of response in "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    return 0;
}


int RShellMITC4::getGlobalDisplacementResponse(QVector<double> *response)
{

    if(response->size() != 24)
    {
        qDebug()<<"Error, the input response vector has a size of "<<response->size()<<" but it needs to have a size of '24' in "<<this->objectName();
        return -1;
    }

    auto nodes = theOpenSeesElement->getNodePtrs();

    auto node1 = nodes[0];
    auto node2 = nodes[1];
    auto node3 = nodes[2];
    auto node4 = nodes[3];

    auto disp1 = node1->getDisp();
    auto disp2 = node2->getDisp();
    auto disp3 = node3->getDisp();
    auto disp4 = node4->getDisp();


    QVector<double> u(24);
    for (int i = 0; i < 6; ++i)
    {
        (*response)[i]   = disp1(i);
        (*response)[i+6] = disp2(i);
        (*response)[i+12] = disp3(i);
        (*response)[i+18] = disp4(i);
    }

    return 0;
}


// Forces at each dof:
// response(0) =   Nxx  i.e.   (11)-strain
// response(1) =   Nyy  i.e.   (22)-strain
// response(2) =   Qxy  i.e.   (12)-shear
// response(3) =   Mxx  i.e.   (11)-curvature
// response(4) =   Myy  i.e.   (22)-curvature
// response(5) =   Mxy  i.e. 2*(12)-curvature
int RShellMITC4::getGlobalForceResponse(QVector<double> *response)
{
    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    //This tells us we want global forces from the element
    std::string s1 = {"globalForce"};

    //some C-code to interface with the OpenSees get response function
    char const *s[20] = {s1.c_str()};
    char const** argv = s;
    int argc = 3;

    auto elementResponse = theOSDomain->getElementResponse(this->objectTag,argv,argc);

    if(elementResponse == nullptr)
    {
        qDebug()<<"Error getting the fiber response from "<<this->objectName();
        return -1;
    }

    if(response->size() != elementResponse->Size())
    {
        qDebug()<<"Error, the input response vector has a size of "<<response->size()<<" but it needs to be of size "<<elementResponse->Size()<<" in "<<this->objectName();
        return -1;
    }

    (*response)[0]   = (*elementResponse)[0];
    (*response)[1]   = (*elementResponse)[1];
    (*response)[2]   = (*elementResponse)[2];
    (*response)[3]   = (*elementResponse)[3];
    (*response)[4]   = (*elementResponse)[4];
    (*response)[5]   = (*elementResponse)[5];
    (*response)[6]   = (*elementResponse)[6];
    (*response)[7]   = (*elementResponse)[7];
    (*response)[8]   = (*elementResponse)[8];
    (*response)[9]   = (*elementResponse)[9];
    (*response)[10]  = (*elementResponse)[10];
    (*response)[11]  = (*elementResponse)[11];
    (*response)[12]  = (*elementResponse)[12];
    (*response)[13]  = (*elementResponse)[13];
    (*response)[14]  = (*elementResponse)[14];
    (*response)[15]  = (*elementResponse)[15];
    (*response)[16]  = (*elementResponse)[16];
    (*response)[17]  = (*elementResponse)[17];
    (*response)[18]  = (*elementResponse)[18];
    (*response)[19]  = (*elementResponse)[19];
    (*response)[20]  = (*elementResponse)[20];
    (*response)[21]  = (*elementResponse)[21];
    (*response)[22]  = (*elementResponse)[22];
    (*response)[23]  = (*elementResponse)[23];

    return 0;
}


int RShellMITC4::getLocalForceResponse(QVector<double> *response)
{
    return 0;
}

// membrane stresses at each integration point (4) total points:
// response(0) =   sigma_xx                                                       i.e.   (11)-axial strain
// response(1) =   sigma_yy                                                       i.e.   (22)-axial strain
// response(2) =   tau_xy                                                         i.e.   (12)-in-plane shear strain
// response(3) =   sigma_xx = -z * kappa00  + eps00_membrane                      i.e.   (11)-bending curvature PLUS axial strain
// response(4) =   sigma_yy = -z * kappa11  + eps11_membrane                      i.e.   (22)-bending curvature PLUS axial strain
// response(5) =   tau_xy   = 2*epsilon01 = -z * (2*kappa01) + gamma01_membrane   i.e. 2*(12)-bending curvature PLUS in-plane shear strain
// response(6) =   tau_xz                                                         i.e.   (13)-out-of-plane shear
// response(7) =   tau_yz                                                         i.e.   (23)-out-of-plane shear
//
// Out-of-plane shear constant through cross section
//
// WARNING: THERE IS A BUG THAT NEEDS TO FIXED SOMEWHERE, POSSIBLY IN OPENSEES
// -->IF YOU GET ZERO STRESS/STRAIN TRY GETTING THE FORCE RESPONSE FIRST FROM THE ELEMENT
// THEN THE STRESSES AND STRAINS WILL BE UPDATED OTHERWISE THEY ARE ZERO
int RShellMITC4::getLocalStressResponse(QVector<double> *response)
{
    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    //This tells OpenSees that we want stresses from the element
    std::string s1 = {"stresses"};

    //some C-code to interface with the OpenSees get response function
    char const *s[20] = {s1.c_str()};
    char const** argv = s;
    int argc = 3;

    auto elementResponse = theOSDomain->getElementResponse(this->objectTag,argv,argc);

    if(elementResponse == nullptr)
    {
        qDebug()<<"Error getting the fiber response from "<<this->objectName();
        return -1;
    }

    if(response->size() != elementResponse->Size()/4)
    {
        qDebug()<<"Error, the input response vector has a size of "<<response->size()<<" but it needs to be of size "<<elementResponse->Size()<<" in "<<this->objectName();
        return -1;
    }

    //calculate the average stresses from the 4 integration points
    (*response)[0]   = (*elementResponse)[0];
    (*response)[1]   = (*elementResponse)[1];
    (*response)[2]   = (*elementResponse)[2];
    (*response)[3]   = (*elementResponse)[3];
    (*response)[4]   = (*elementResponse)[4];
    (*response)[5]   = (*elementResponse)[5];
    (*response)[6]   = (*elementResponse)[6];
    (*response)[7]   = (*elementResponse)[7];

    (*response)[0]  += (*elementResponse)[8];
    (*response)[1]  += (*elementResponse)[9];
    (*response)[2]  += (*elementResponse)[10];
    (*response)[3]  += (*elementResponse)[11];
    (*response)[4]  += (*elementResponse)[12];
    (*response)[5]  += (*elementResponse)[13];
    (*response)[6]  += (*elementResponse)[14];
    (*response)[7]  += (*elementResponse)[15];

    (*response)[0]  += (*elementResponse)[16];
    (*response)[1]  += (*elementResponse)[17];
    (*response)[2]  += (*elementResponse)[18];
    (*response)[3]  += (*elementResponse)[19];
    (*response)[4]  += (*elementResponse)[20];
    (*response)[5]  += (*elementResponse)[21];
    (*response)[6]  += (*elementResponse)[22];
    (*response)[7]  += (*elementResponse)[23];

    (*response)[0]  += (*elementResponse)[24];
    (*response)[1]  += (*elementResponse)[25];
    (*response)[2]  += (*elementResponse)[26];
    (*response)[3]  += (*elementResponse)[27];
    (*response)[4]  += (*elementResponse)[28];
    (*response)[5]  += (*elementResponse)[29];
    (*response)[6]  += (*elementResponse)[30];
    (*response)[7]  += (*elementResponse)[31];

    (*response)[0]  /= 4.0;
    (*response)[1]  /= 4.0;
    (*response)[2]  /= 4.0;
    (*response)[3]  /= 4.0;
    (*response)[4]  /= 4.0;
    (*response)[5]  /= 4.0;
    (*response)[6]  /= 4.0;
    (*response)[7]  /= 4.0;

    return 0;
}


// response(0) = eps11     i.e.   (11)-strain
// response(1) = eps22     i.e.   (22)-strain
// response(2) = gamma12   i.e.   (12)-shear
// response(3) = theta11   i.e.   (11)-rotation
// response(4) = theta22   i.e.   (22)-rotation
// response(5) = theta33   i.e. 2*(12)-rotation
// response(6) = gamma13   i.e.   (13)-shear
// response(7) = gamma23   i.e.   (23)-shear
// WARNING: THERE IS A BUG THAT NEEDS TO FIXED SOMEWHERE, POSSIBLY IN OPENSEES
// -->IF YOU GET ZERO STRESS/STRAIN TRY GETTING THE FORCE RESPONSE FIRST FROM THE ELEMENT
// THEN THE STRESSES AND STRAINS WILL BE UPDATED OTHERWISE THEY ARE ZERO
int RShellMITC4::getLocalStrainResponse(QVector<double> *response)
{
    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    //This tells us we want global forces from the element
    std::string s1 = {"strains"};

    //some C-code to interface with the OpenSees get response function
    char const *s[20] = {s1.c_str()};
    char const** argv = s;
    int argc = 3;

    auto elementResponse = theOSDomain->getElementResponse(this->objectTag,argv,argc);

    if(elementResponse == nullptr)
    {
        qDebug()<<"Error getting the fiber response from "<<this->objectName();
        return -1;
    }

    if(response->size() != elementResponse->Size()/4)
    {
        qDebug()<<"Error, the input response vector has a size of "<<response->size()<<" but it needs to be of size "<<elementResponse->Size()<<" in "<<this->objectName();
        return -1;
    }

    //calculate the average strains from the 4 integration points
    (*response)[0]   = (*elementResponse)[0];
    (*response)[1]   = (*elementResponse)[1];
    (*response)[2]   = (*elementResponse)[2];
    (*response)[3]   = (*elementResponse)[3];
    (*response)[4]   = (*elementResponse)[4];
    (*response)[5]   = (*elementResponse)[5];
    (*response)[6]   = (*elementResponse)[6];
    (*response)[7]   = (*elementResponse)[7];

    (*response)[0]  += (*elementResponse)[8];
    (*response)[1]  += (*elementResponse)[9];
    (*response)[2]  += (*elementResponse)[10];
    (*response)[3]  += (*elementResponse)[11];
    (*response)[4]  += (*elementResponse)[12];
    (*response)[5]  += (*elementResponse)[13];
    (*response)[6]  += (*elementResponse)[14];
    (*response)[7]  += (*elementResponse)[15];

    (*response)[0]  += (*elementResponse)[16];
    (*response)[1]  += (*elementResponse)[17];
    (*response)[2]  += (*elementResponse)[18];
    (*response)[3]  += (*elementResponse)[19];
    (*response)[4]  += (*elementResponse)[20];
    (*response)[5]  += (*elementResponse)[21];
    (*response)[6]  += (*elementResponse)[22];
    (*response)[7]  += (*elementResponse)[23];

    (*response)[0]  += (*elementResponse)[24];
    (*response)[1]  += (*elementResponse)[25];
    (*response)[2]  += (*elementResponse)[26];
    (*response)[3]  += (*elementResponse)[27];
    (*response)[4]  += (*elementResponse)[28];
    (*response)[5]  += (*elementResponse)[29];
    (*response)[6]  += (*elementResponse)[30];
    (*response)[7]  += (*elementResponse)[31];

    (*response)[0]  /= 4.0;
    (*response)[1]  /= 4.0;
    (*response)[2]  /= 4.0;
    (*response)[3]  /= 4.0;
    (*response)[4]  /= 4.0;
    (*response)[5]  /= 4.0;
    (*response)[6]  /= 4.0;
    (*response)[7]  /= 4.0;

    return 0;
}

