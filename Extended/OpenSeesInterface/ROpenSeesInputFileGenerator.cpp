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

#include "ROpenSeesInputFileGenerator.h"
#include "RDomain.h"
#include "RNode.h"
#include "RElement.h"
#include "RUniaxialMaterial.h"
#include "RNDMaterial.h"
#include "RFiber.h"
#include "RSectionForceDeformation.h"
#include "RRigidBeam.h"

#include "Element.h"
#include "OPS_Stream.h"
#include "StandardStream.h"
#include <iostream>

ROpenSeesInputFileGenerator::ROpenSeesInputFileGenerator(RDomain* domain): theDomain(domain)
{

}


int ROpenSeesInputFileGenerator::generateInputFile(void)
{

    // Create the string object that will be saved as the input file
    std::string sser2;

    int option = 1;

    if(option == 1)
    {
        sser2.append("### Generating OpenSees Tcl input file ###");
    }
    else if(option == 2)
    {
        sser2.append("### Generating OpenSees Python .py file for OpenSeesPy ###");
    }

    sser2 += '\n';
    sser2 += '\n';

    sser2.append("### 3-D model with units in N, m, kg ###");

    sser2 += '\n';
    sser2 += '\n';

    auto nodeList = theDomain->findChildren<RNode*>();
    auto elemList = theDomain->findChildren<RElement*>();
    auto rigidLinks = theDomain->findChildren<RRigidBeam*>();
    auto uniMatList = theDomain->findChildren<RUniaxialMaterial*>();
    auto nDmatList = theDomain->findChildren<RNDMaterial*>();
    auto fiberList = theDomain->findChildren<RFiber*>();
    auto sectionList = theDomain->findChildren<RSectionForceDeformation*>();

    //Note: ORDER MATTERS!
    sser2.append("### Nodes ###");
    sser2 += '\n';
    sser2 += '\n';

    auto nodeCount = 0;
    auto rigidLinkCount = 0;
    auto materialsCount = 0;
    auto sectionsCount = 0;
    auto elementsCount = 0;

    // First the nodes get created
    for(auto&& it: nodeList)
    {
        auto res = it->generateOpenSeesInput(sser2,option);

        if(res != 0)
        {
            qCritical()<<"Error in generating the OpenSeesinput file ";
            return -1;
        }

        ++nodeCount;
    }

    // Then the rigid links
    sser2 += '\n';
    sser2.append("### Multi-point Constraints ###");
    sser2 += '\n';
    sser2 += '\n';

    for(auto&& it: rigidLinks)
    {
        auto res = it->generateOpenSeesInput(sser2,option);

        if(res != 0)
        {
            qCritical()<<"Error in generating the OpenSeesinput file ";
            return -1;
        }

        ++rigidLinkCount;
    }

    // Then the materials
    sser2 += '\n';
    sser2.append("### Uniaxial Materials ###");
    sser2 += '\n';
    sser2 += '\n';

    for(auto&& it: uniMatList)
    {
        auto res = it->generateOpenSeesInput(sser2,option);

        if(res != 0)
        {
            qCritical()<<"Error in generating the OpenSeesinput file ";
            return -1;
        }

        ++materialsCount;
    }

    sser2 += '\n';
    sser2.append("### nD Materials ###");
    sser2 += '\n';
    sser2 += '\n';

    for(auto&& it: nDmatList)
    {
        auto res = it->generateOpenSeesInput(sser2,option);

        if(res != 0)
        {
            qCritical()<<"Error in generating the OpenSeesinput file ";
            return -1;
        }

        ++materialsCount;
    }

    sser2 += '\n';
    sser2.append("### Sections ###");
    sser2 += '\n';
    sser2 += '\n';

    // Then the fibre sections
    for(auto&& it : sectionList)
    {
        auto res = it->generateOpenSeesInput(sser2,option);

        if(res != 0)
        {
            qCritical()<<"Error in generating the OpenSeesinput file ";
            return -1;
        }

        ++sectionsCount;
    }

    sser2 += '\n';
    sser2.append("### Elements ###");
    sser2 += '\n';
    sser2 += '\n';

    // Then the elements
    for(auto&& it: elemList)
    {
        auto res = it->generateOpenSeesInput(sser2,option);

        if(res != 0)
        {
            qCritical()<<"Error in generating the OpenSeesinput file ";
            return -1;
        }

        ++elementsCount;
    }

    sser2.append("### Analysis tools typically appear here, but they are not part of the BIM import algorithm developed in this work  ###");

    // Add fibers to the count they are created in the sections
    auto fibresCount = fiberList.size();

    auto totalCount = nodeCount + rigidLinkCount + materialsCount + sectionsCount + elementsCount + fibresCount;

    ofstream inputFile;

    // Change this directory to the location where you want the input file to be created
    inputFile.open ("/Users/steve/Desktop/OpenSeesInputFile.tcl");
    inputFile << sser2;
    inputFile.close();

    qDebug()<<"Number of nodes generated: "<<nodeCount;
    qDebug()<<"Number of multi-point constraints generated: "<<rigidLinkCount;
    qDebug()<<"Number of materials generated: "<<materialsCount;
    qDebug()<<"Number of fibres generated: "<<fibresCount;
    qDebug()<<"Number of sections generated: "<<sectionsCount;
    qDebug()<<"Number of elements generated: "<<elementsCount;

    qDebug()<<"Total OpenSees items generated: "<<totalCount;

    return 0;
}
