#ifndef RIFCImportData_H
#define RIFCImportData_H
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

// This class stores and provides key IFC information in the BIM import process

#include "RComponent.h"

#include <IFC4/include/IfcProduct.h>
#include <IFC4/include/IfcMaterialSelect.h>
#include <IFC4/include/IfcRepresentationContext.h>
#include <IFC4/include/IfcMaterialLayer.h>
#include <IFC4/include/IfcMaterialList.h>

class RIFCImportData
{

public:
    RIFCImportData()
    {

    }


    ~RIFCImportData()
    {

    }

    // Material type, more to be added
    enum materialType {airSpace, battInsulation, concrete, reinforcedConcrete, rigidInsulation, wood, steel, LAYERED, NA };

    // IFC representation - contains the various representations of the object, e.g., geometry representation, material, etc.
    weak_ptr<IfcRepresentationContext> theIfcRepresentationContext;

    // Pointer to the IFC product
    weak_ptr<IfcProduct> theIfcProduct;

    // Object name
    std::wstring objectName;

    // Representation indentifier
    std::wstring representationIdentifier;

    // Entity type
    std::string theEntityType;

    // The component being created
    RComponent* theRComponent = nullptr;

    // Material information class, containing material definition, material lists, etc.
    std::vector<std::shared_ptr<IfcMaterialLayer> >	 materialLayers;
    std::vector<materialType> theMatList;
    materialType theMaterialType = materialType::NA;
    weak_ptr<IfcMaterialSelect> theIfcMaterial;

    // IFC entity ID number from the step file
    int entityId = -1;

};

#endif
