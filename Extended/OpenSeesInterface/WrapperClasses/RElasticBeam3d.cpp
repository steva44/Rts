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

#include "RElasticBeam3d.h"
#include "RNode.h"
#include "RParameter.h"
#include "RCoordinateTransformation.h"
#include "RTimeHistoryRecorder.h"

#include "Information.h"
#include "CrdTransf.h"
#include "LinearCrdTransf3d.h"
#include "PDeltaCrdTransf3d.h"

RElasticBeam3d::RElasticBeam3d(QObject *parent, QString name)
    : RElasticBeamColumnElement(parent, name)
{
    theOpenSeesElement = nullptr;
}


RElasticBeam3d::~RElasticBeam3d()
{

}


int  RElasticBeam3d::update(void)
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
        return -1;
    }

    auto res = this->initialize();

    if(res!=0)
    {
        qCritical()<<"Error updating OpenSees element in "<<this->objectName();
        return -1;
    }

    theOSDomain->addElement(theOpenSeesElement);

    //The element recorder
    theTransientRecorder->clear();

    return 0;
}


Element* RElasticBeam3d::getTheOpenSeesElement(void)
{
    if(!theOpenSeesElement)
        this->createElasticBeam3dElement();

    return theOpenSeesElement;
}


int RElasticBeam3d::createElasticBeam3dElement(void)
{

    //Check to make sure the property objects are not null pointers
    if(theNode1 == nullptr || theNode2 == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs nodes";
        return -1;
    }
    if(theAParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs an area parameter";
        return -1;
    }
    if(theEParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs an E parameter";
        return -1;
    }
    if(theIyParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs an Iy parameter";
        return -1;
    }
    if(theIzParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs an Iz parameter";
        return -1;
    }
    if(theNuParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a Nu parameter";
        return -1;
    }
    if(theJParameter == nullptr)
    {
        qDebug()<<"The "<<__FUNCTION__<<" needs a Jx parameter";
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

    return this->initialize();
}


int RElasticBeam3d::initialize(void)
{
    auto G = theEParameter->getCurrentValue()/(2.0*(1.0+theNuParameter->getCurrentValue()));
    auto A = theAParameter->getCurrentValue();
    auto E = theEParameter->getCurrentValue();
    auto Jx = theJParameter->getCurrentValue();
    auto Iy = theIyParameter->getCurrentValue();
    auto Iz = theIzParameter->getCurrentValue();
    auto rho = theRhoParameter->getCurrentValue();

    auto Nd1 = theNode1->getObjectTag();
    auto Nd2 = theNode2->getObjectTag();

    //    ElasticBeam3d(int tag, double A, double E, double G,
    //		  double Jx, double Iy, double Iz,
    //          int Nd1, int Nd2, CrdTransf &theTransf,
    //          double rho = 0.0, int cMass = 0,
    //          int sectionTag = 0);
    theOpenSeesElement = new ElasticBeam3d(objectTag, A, E, G,Jx, Iy, Iz, Nd1, Nd2, *theCrdTransformation, rho);

    if(theOpenSeesElement == nullptr)
        return -1;

    return 0;
}


void RElasticBeam3d::print(void)
{
    qDebug()<<"***********************************************\n";
    qDebug()<<"For beam called: "<<this->objectName()<<" with tag "<<this->getObjectTag()<<"\n";
    theOpenSeesElement->Print((*opserrPtr),0);

    theOpenSeesElement->Print((*opserrPtr),2);
    qDebug()<<"***********************************************\n";


    return;
}


int RElasticBeam3d::getGlobalResponse(QVector<double> *response, QString type, double xi)
{
    auto res = this->getLocalResponse(response,type,xi);

    if(res != 0)
        return res;

    RCoordinateTransformation theCoordinateTransformation(theDomain);

    // Rotate from local into the global coordinate system
    theCoordinateTransformation.transformVectorToGlobalII(theNode1, theNode2,VecxzPlane, response);

    return 0;
}


int RElasticBeam3d::getLocalResponse(QVector<double> *response, QString type, double xi)
{
    if(response == nullptr)
    {
        qDebug()<<"A null response object was given in "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    if (type == QString("Displacement"))
    {
        this->getLocalDisplacementResponse(xi, response);
    }
    else if (type == QString("Forces"))
    {
        this->getLocalForceResponse(xi,response);
    }
    else if (type == QString("Strain"))
    {
        this->getLocalStrainResponse(xi,response);
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


int RElasticBeam3d::getLocalDisplacementResponse(double xi, QVector<double> *response)
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

    // Initial declarations
    double u1, u2, u3, u4;

    double xcoord1 = theNode1->getXCoordinateValue();
    double xcoord2 = theNode2->getXCoordinateValue();
    double ycoord1 = theNode1->getYCoordinateValue();
    double ycoord2 = theNode2->getYCoordinateValue();
    double zcoord1 = theNode1->getZCoordinateValue();
    double zcoord2 = theNode2->getZCoordinateValue();

    auto dx = xcoord2-xcoord1;
    auto dy = ycoord2-ycoord1;
    auto dz = zcoord2-zcoord1;

    //get the Length
    auto L = sqrt(dx*dx + dy*dy + dz*dz);

    //Axial displacement
    (*response)[0] = u[0] + (u[6]-u[0])*xi;

    //v
    {
        // Extract displacements
        u1 = u[1];
        u2 = u[5];
        u3 = u[7];
        u4 = u[11];

        (*response)[1] = (double) ((u2 + u4) * L + 2.0* u1 - 2.0 * u3) * pow(xi, 0.3e1) + (double) ((-2.0 * u2 - u4) * L - 3.0 * u1 + 3.0 * u3) * xi * xi + (double) L * (double) u2 * xi + (double) u1;

    }
    //w
    {
        // Extract displacements
        u1 = u[2];
        u2 = -u[4];
        u3 = u[8];
        u4 = -u[10];

        (*response)[2] = (double) ((u2 + u4) * L + 2.0* u1 - 2.0 * u3) * pow(xi, 0.3e1) + (double) ((-2.0 * u2 - u4) * L - 3.0 * u1 + 3.0 * u3) * xi * xi + (double) L * (double) u2 * xi + (double) u1;
    }

    return 0;
}


int RElasticBeam3d::getLocalForceResponse(double xi, QVector<double> *response)
{

    //Local Forces
    Information eleInfo;
    theOpenSeesElement->getResponse(3,eleInfo);

    Vector* localElResponse = eleInfo.theVector;

    auto nodes = theOpenSeesElement->getNodePtrs();

    auto node1 = nodes[0];
    auto node2 = nodes[1];

    auto disp1 = node1->getDisp();
    auto disp2 = node2->getDisp();

    QVector<double> ug(12);
    for (int i = 0; i < 6; i++) {
        ug[i]   = disp1(i);
        ug[i+6] = disp2(i);
    }

    // Transform to the local configuration
    RCoordinateTransformation theCoordinateTransformation(theDomain);
    theCoordinateTransformation.transformVectorToLocalII(theNode1, theNode2, VecxzPlane, &ug);

    // Initial declarations
    double u1, u2, u3, u4;

    double xcoord1 = theNode1->getXCoordinateValue();
    double xcoord2 = theNode2->getXCoordinateValue();
    double ycoord1 = theNode1->getYCoordinateValue();
    double ycoord2 = theNode2->getYCoordinateValue();
    double zcoord1 = theNode1->getZCoordinateValue();
    double zcoord2 = theNode2->getZCoordinateValue();

    auto dx = xcoord2-xcoord1;
    auto dy = ycoord2-ycoord1;
    auto dz = zcoord2-zcoord1;

    auto L = sqrt(dx*dx + dy*dy + dz*dz);

    auto E = theEParameter->getCurrentValue();
    auto Iz = theIzParameter->getCurrentValue();
    auto Iy = theIyParameter->getCurrentValue();

    auto EIz = E * Iz;
    auto EIy = E * Iy;

    {
        u1 = ug[1];
        u2 = ug[5];
        u3 = ug[7];
        u4 = ug[11];

        //"Vy"
        (*response)[1] = 6.0 * EIz * ((u2 + u4) * L + 2.0 * u1 - 2.0 * u3) * (double) pow((double) L, (double) (-3.0));

        //"Mz"
        (*response)[2]  = -0.4e1 * EIz * ((u2 + u4 / 0.2e1) * L * L + ((-0.3e1 / 0.2e1 * u2 - 0.3e1 / 0.2e1 * u4) * xi * L + 0.3e1 / 0.2e1 * u1 - 0.3e1 / 0.2e1 * u3) * L - 0.3e1 * xi * L * (u1 - u3)) * pow(L, -0.3e1);
    }

    {
        // Extract displacements
        u1 = ug[2];
        u2 = -ug[4];
        u3 = ug[8];
        u4 = -ug[10];

        //"Vz"
        (*response)[3] = 6.0 * EIy * ((u2 + u4) * L + 2.0 * u1 - 2.0 * u3) * (double) pow((double) L, (double) (-3.0));

        //"My"
        (*response)[4]  = -0.4e1 * EIy * ((u2 + u4 / 0.2e1) * L * L + ((-0.3e1 / 0.2e1 * u2 - 0.3e1 / 0.2e1 * u4) * xi * L + 0.3e1 / 0.2e1 * u1 - 0.3e1 / 0.2e1 * u3) * L - 0.3e1 * xi * L * (u1 - u3)) * pow(L, -0.3e1);
    }

    //Torsion
    auto T1 = (*localElResponse)[3];

    auto T2 = (*localElResponse)[9];

    //Assume linear variation
    (*response)[5]=  T1+(T2-T1)*xi;

    //    //Stiffness
    //    Information eleInfo;
    //    theOpenSeesElement->getResponse(1,eleInfo);

    //    //Basic Forces
    //    Information eleInfo;
    //    theOpenSeesElement->getResponse(4,eleInfo);

    //    //Basic Displacements
    //    Information eleInfo;
    //    theOpenSeesElement->getResponse(5,eleInfo);

    return 0;
}


int RElasticBeam3d::getLocalStressResponse(double z, double y, double Qz, double Qy, double tz, double ty, QVector<double> *response)
{

    // Get section stiffnesses
    auto E = theEParameter->getCurrentValue();
    auto area =  theAParameter->getCurrentValue();
    auto Iz = theIzParameter->getCurrentValue();
    auto Iy = theIyParameter->getCurrentValue();

    auto EIz = E * Iz;
    auto EIy = E * Iy;
    auto EA  = E*area;

    QVector<double> elementResponse(12);

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
    this->getLocalForces(&elementResponse);

    auto N = elementResponse[0];
    auto Vy = elementResponse[1];
    auto Vz = elementResponse[2];
    auto T = elementResponse[3];
    auto My = elementResponse[4];
    auto Mz = elementResponse[5];

    //    auto epsilonXX = My*z/EIy - Mz*y/EIz + N/EA;

    auto sigmaXX =  My*z/Iy - Mz*y/Iz + N/area;

    auto TauY = Qy*Vy/(Iy*ty);

    auto TauZ = Qz*Vz/(Iz*tz);

    auto Tors = 0.0;

    (*response)[0]  = sigmaXX;   //"SigmaXX_1"
    (*response)[1]  = TauY;   //"TauY_1"
    (*response)[2]  = TauZ;   //"TauZ_1"
    (*response)[3]  = Tors;   //"Torsion_1"
    (*response)[4]  = 0.0;
    (*response)[5]  = 0.0;
    (*response)[6]  = sigmaXX;   //"SigmaXX_1"
    (*response)[7]  = TauY;   //"TauY_1"
    (*response)[8]  = TauZ;   //"TauZ_1"
    (*response)[9]  = Tors;   //"Torsion_1"
    (*response)[10] = 0.0;
    (*response)[11] = 0.0;

    return 0;
}


int RElasticBeam3d::getLocalStrainResponse(double xi, QVector<double> *response)
{
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
int RElasticBeam3d::getLocalForces(QVector<double> *response)
{
    if(response->size() != 12)
    {
        qDebug()<<"Error, the response vector must be of size 12 in "<<this->objectName()<<" in "<<__FUNCTION__;
        return -1;
    }

    auto theOSDomain = theDomain->getTheOpenSeesDomain();

    //This tells us we want a local force response from the element
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