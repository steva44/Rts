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

#include "RForceBeamColumn3d.h"
#include "RNode.h"
#include "RParameter.h"
#include "RSectionForceDeformation.h"
#include "RCoordinateTransformation.h"
#include "RTimeHistoryRecorder.h"

#include "ForceBeamColumn3d.h"
#include "Information.h"

RForceBeamColumn3d::RForceBeamColumn3d(QObject *parent, QString name)
    : RNonlinearBeamColumnElement(parent, name)
{
    theOpenSeesElement = nullptr;
}


RForceBeamColumn3d::~RForceBeamColumn3d()
{

}


int  RForceBeamColumn3d::update()
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

    theCrdTransformation.reset();

    this->createTheCrdTransformation();

    if(theCrdTransformation == nullptr)
    {
        qDebug()<<"Failed to create a coordinate transformation in "<<__FUNCTION__;
        theOpenSeesElement = nullptr;
        return -1;
    }

    auto res = this->initialize();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees 'DispBeamColumn3d' in "<<this->objectName();
        theOpenSeesElement = nullptr;
        return -1;
    }

    theOSDomain->addElement(theOpenSeesElement);

    //The element recorder
    theTransientRecorder->clear();

    return 0;
}


Element* RForceBeamColumn3d::getTheOpenSeesElement(void)
{
    if(!theOpenSeesElement)
        this->createForceBeamColumn3dElement();

    return theOpenSeesElement;
}


int RForceBeamColumn3d::createForceBeamColumn3dElement(void)
{

    //Check to make sure the property objects are not null pointers
    if(theNode1 == nullptr || theNode1 == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs nodes";
        return -1;
    }
    if(theRhoParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a Rho parameter";
        return -1;
    }
    if(theOrientationParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs an orientation parameter";
        return -1;
    }

    this->createTheCrdTransformation();

    if(theCrdTransformation == nullptr)
    {
        qDebug()<<"Failed to create a coordinate transformation in "<<__FUNCTION__;
        return -1;
    }

    this->createTheBeamIntegration();

    if(theBeamIntegration == nullptr)
    {
        qDebug()<<"Failed to create the beam integration in "<<__FUNCTION__;
        return -1;
    }

    if(theSectionForceDeformation == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs sections";
        return -1;
    }


    this->initialize();

}


int RForceBeamColumn3d::initialize(void)
{
    auto rho = theRhoParameter->getCurrentValue();

    auto Nd1 = theNode1->getObjectTag();
    auto Nd2 = theNode2->getObjectTag();

    numSections = theSectionForceDeformation->size();

    //Create the sections from the SectionForceDeformation
    SectionForceDeformation **sections = new SectionForceDeformation* [numSections];

    //Create the sections
    for (int i=0; i<numSections; ++i)
    {

        auto theRSection = theSectionForceDeformation->at(i);

        //Get the section force deformation
        SectionForceDeformation *theSection = theRSection->getTheSectionForceDeformation();

        //Check if creation was successful
        if (theSection == nullptr)
        {
            qDebug()<<"Error, could not create an OpenSees section from "<<theRSection->objectName()<<" in "<<__FUNCTION__;

            delete [] sections;
            return -1;
        }

        //Add it to the section array
        sections[i] = theSection;
    }

    // ForceBeamColumn3d(int tag, int nodeI, int nodeJ,
    // int numSections, SectionForceDeformation **sec,
    // BeamIntegration &beamIntegr,
    // CrdTransf &coordTransf, double rho = 0.0,
    // int maxNumIters = 10, double tolerance = 1.0e-12);
    theOpenSeesElement = new ForceBeamColumn3d(objectTag, Nd1, Nd2, numSections, sections,
                                               *theBeamIntegration,*theCrdTransformation, rho);

    if(theOpenSeesElement == nullptr)
        return -1;

    return 0;
}


void RForceBeamColumn3d::print(void)
{
    qDebug()<<"***********************************************\n";
    qDebug()<<"For beam called: "<<this->objectName()<<" with tag "<<this->getObjectTag()<<"\n";
    theOpenSeesElement->Print((*opserrPtr),OPS_PRINT_CURRENTSTATE);

    qDebug()<<"***********************************************\n";

    return;
}


int RForceBeamColumn3d::getGlobalResponse(QVector<double> *response, QString type, int sectionNum)
{
    auto res = this->getLocalResponse(response,type,sectionNum);

    if(res != 0)
        return res;

    RCoordinateTransformation theCoordinateTransformation(theDomain);

    // Rotate from local into the global coordinate system
    theCoordinateTransformation.transformVectorToGlobalII(theNode1, theNode2,VecxzPlane, response);

    return 0;
}


int RForceBeamColumn3d::getLocalResponse(QVector<double> *response, QString type, int sectionNum)
{

    if(response == nullptr)
    {
        qDebug()<<"A null response object was given in "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }
    if (type == QString("Displacement"))
    {
        this->getLocalDisplacementResponse(response,sectionNum);
    }
    else if (type == QString("Forces"))
    {
        this->getLocalForceResponse(response,sectionNum);
    }
    else if (type == QString("Stress"))
    {
        this->getLocalStressResponse(response,sectionNum);
    }
    else if (type == QString("Strain"))
    {
        this->getLocalStrainResponse(response,sectionNum);
    }
    else if (type == QString("EndForces"))
    {
        this->getLocalForces(response);
    }
    else
    {
        qDebug()<<"Could not understand the type of response in "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    return 0;
}


int RForceBeamColumn3d::getLocalDisplacementResponse(QVector<double> *response, int sectionNum)
{
    auto nodes = theOpenSeesElement->getNodePtrs();

    auto node1 = nodes[0];
    auto node2 = nodes[1];

    auto disp1 = node1->getDisp();
    auto disp2 = node2->getDisp();

    QVector<double> u(12);
    for (int i = 0; i < 6; ++i)
    {
        u[i]   = disp1(i);
        u[i+6] = disp2(i);
    }

    // Transform to the local configuration
    RCoordinateTransformation theCoordinateTransformation(theDomain);
    theCoordinateTransformation.transformVectorToLocalII(theNode1, theNode2, VecxzPlane, &u);

    double xi = static_cast<double>(sectionNum)/static_cast<double>(numSections);

    (*response)[0] = u[0] + (u[6]-u[0])*xi;
    (*response)[1] = u[1] + (u[7]-u[1])*xi;
    (*response)[2] = u[2] + (u[8]-u[2])*xi;
    (*response)[3] = u[3] + (u[9]-u[3])*xi;
    (*response)[4] = u[4] + (u[10]-u[4])*xi;
    (*response)[5] = u[5] + (u[11]-u[5])*xi;

    return 0;
}


//(*response)[0]="N_1"
//(*response)[1]="Vy_1"
//(*response)[2]="Vz_1"
//(*response)[3]="T_1"
//(*response)[4]="My_1"
//(*response)[5]="Mz_1"
//(*response)[6]="N_2"
//(*response)[7]="Vy_2"
//(*response)[8]="Vz_2"
//(*response)[9]="T_2"
//(*response)[10]="My_2"
//(*response)[11]="Mz_2"
int RForceBeamColumn3d::getLocalForces(QVector<double> *response)
{
    if(response->size() != 12)
    {
        qDebug()<<"Error, the response vector must be of size 12 in "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    //This tells us we want a loval force response from the element
    std::string s1 = {"localForce"};

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

    (*response)[0]  = (*elementResponse)[0];   //"N_1"
    (*response)[1]  = (*elementResponse)[1];   //"Vy_1"
    (*response)[2]  = (*elementResponse)[2];   //"Vz_1"
    (*response)[3]  = (*elementResponse)[3];   //"T_1"
    (*response)[4]  = (*elementResponse)[4];   //"My_1"
    (*response)[5]  = (*elementResponse)[5];   //"Mz_1"
    (*response)[6]  = (*elementResponse)[6];   //"N_2"
    (*response)[7]  = (*elementResponse)[7];   //"Vy_2"
    (*response)[8]  = (*elementResponse)[8];   //"Vz_2"
    (*response)[9]  = (*elementResponse)[9];   //"T_2"
    (*response)[10] = (*elementResponse)[10];  //"My_2"
    (*response)[11] = (*elementResponse)[11];  //"Mz_2"

    return 0;
}


//(*response)[0] = "kappaZ"
//(*response)[1] = "eps"
//(*response)[2] = "gammaY"
//(*response)[3] = "kappaY"
//(*response)[4] = "gammaZ"
//(*response)[5] = "theta"
//(*response)[6] = "Mz"
//(*response)[7] = "P"
//(*response)[8] = "Vy"
//(*response)[9] = "My"
//(*response)[10] = "Vz"
//(*response)[11] = "T"
int RForceBeamColumn3d::getLocalForceResponse(QVector<double> *response, int sectionNum)
{
    if (sectionNum > numSections)
    {
        qDebug()<<"Error, the section number "<<sectionNum<<" in "<<__FUNCTION__<<" is greater than the number of sections: "<<numSections<<" in the element: "<<this->objectName();
        return -1;
    }

    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    //This tells us we want a section response from the element
    std::string s1 = {"section"};

    //This gets the response from a section number
    //NOTE: be careful with the section numbering, the sections start from 1..n not 0..n so add a +1 to compensate
    std::string s2 = std::to_string(sectionNum+1);

    //This is the response we want from the section
    std::string s3 = {"forceAndDeformation"};

    //some C-code to interface with the OpenSees get response function
    char const *s[20] = {s1.c_str(),s2.c_str(),s3.c_str()};
    char const** argv = s;
    int argc = 3;

    auto elementResponse = theOSDomain->getElementResponse(this->objectTag,argv,argc);

    if(elementResponse == nullptr)
    {
        qDebug()<<"Error getting the fiber response from "<<this->objectName();
        return -1;
    }

    (*response)[0]  = (*elementResponse)[0];      //"kappaZ"
    (*response)[1]  = (*elementResponse)[1];      //"eps"
    (*response)[2]  = (*elementResponse)[2];      //"gammaY"
    (*response)[3]  = (*elementResponse)[3];      //"kappaY"
    (*response)[4]  = (*elementResponse)[4];      //"gammaZ"
    (*response)[5]  = (*elementResponse)[5];      //"theta"
    (*response)[6]  = (*elementResponse)[6];      //"Mz"
    (*response)[7]  = (*elementResponse)[7];      //"P"
    (*response)[8]  = (*elementResponse)[8];      //"Vy"
    (*response)[9]  = (*elementResponse)[9];      //"My"
    (*response)[10] = (*elementResponse)[10];     //"Vz"
    (*response)[11] = (*elementResponse)[11];     //"T"

    return 0;
}


int RForceBeamColumn3d::getLocalStressResponse(QVector<double> *response, int sectionNum)
{
    auto res = this->getFibreResponses(response,sectionNum);

    if(res!=0)
        return res;

    return 0;
}


int RForceBeamColumn3d::getLocalStrainResponse(QVector<double> *response, int sectionNum)
{
    auto res = this->getFibreResponses(response,sectionNum);

    if(res!=0)
        return res;

    return 0;
}


//Incoming response vector size should be (number of fibers)*5
//response(count) = yLoc;
//response(count+1) = zLoc;
//response(count+2) = A;
//response(count+3) = stress;
//response(count+4) = strain;
int RForceBeamColumn3d::getFibreResponses(QVector<double> *response, int sectionNum)
{
    if (sectionNum > numSections)
    {
        qDebug()<<"Error, the section number "<<sectionNum<<" in "<<__FUNCTION__<<" is greater than the number of sections: "<<numSections<<" in the element: "<<this->objectName();
        return -1;
    }

    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    //This tells us we want a section response from the element
    std::string s1 = {"section"};

    //This gets the response from a section number
    //NOTE: be careful with the section numbering, the sections start from 1..n not 0..n so add a +1 to compensate
    std::string s2 = std::to_string(sectionNum+1);

    //This is the response we want from the section
    std::string s3 = {"fiberData"};

    //some C-code to interface with the OpenSees get response function
    char const *s[20] = {s1.c_str(),s2.c_str(),s3.c_str()};
    char const** argv = s;
    int argc = 3;

    auto elementResponse = theOSDomain->getElementResponse(this->objectTag,argv,argc);

    if(elementResponse == nullptr)
    {
        qDebug()<<"Error getting the fiber response from "<<this->objectName();
        return -1;
    }

    auto numFibers = elementResponse->Size()/5;

    if(response->empty())
    {
        response->resize(numFibers*5);
    }
    else if(numFibers*5 != response->size())
    {
        qDebug()<<"Error, the response vector is not the correct size in "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    //data(count) = yLoc; data(count+1) = zLoc; data(count+2) = A; data(count+3) = stress; data(count+4) = strain
    int count = 0;
    for(int i = 0; i<numFibers; ++i)
    {
        //response(count) = yLoc;
        //response(count+1) = zLoc;
        //response(count+2) = A;
        //response(count+3) = stress;
        //response(count+4) = strain;
        auto yLoc = (*elementResponse)[count];
        auto zLoc = (*elementResponse)[count+1];
        auto area = (*elementResponse)[count+2];
        auto stress = (*elementResponse)[count+3];
        auto strain = (*elementResponse)[count+4];

        //        qDebug()<<"Stress "<<stress;
        //        qDebug()<<"Strain "<<strain;

        (*response)[count]  = yLoc;
        (*response)[count+1] = zLoc;
        (*response)[count+2] = area;
        (*response)[count+3] = stress;
        (*response)[count+4] = strain;

        //update the counter
        count += 5;
    }

    return 0;
}



