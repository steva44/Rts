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
*   - Mojtaba Mahsuli                                                *
*                                                                    *
*********************************************************************/

#include "RBIM.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RPoint.h"
#include "RTime.h"
#include "RLocation.h"
#include "RComponent.h"
#include "RMainWindow.h"
#include "RMesh.h"
#include "RBIMImportMethod.h"
#include "RCSICostMethod.h"
#include "RIFCImportData.h"
#include "RPlateComponent.h"
#ifdef IFC
#include "ROSGPlotter.h"
#include "IfcPlusPlusSystem.h"
#include "IfcCompoundPlaneAngleMeasure.h"
#endif

#include <QDir>
#include <QApplication>

RBIM::RBIM(QObject *parent, QString name)
    : RObject(parent, name)
{
    theLoadBearingSystem = RBIM::ShearWall;
    theConstructionMaterial = RBIM::ReinforcedConcrete;
    theNumberOfStoreys = 1;
    numberOfHousingUnits = 1;
    theOccupancyType = RBIM::SingleResidence;
    theOccupancyClass = "RES3";
    thePlanShape = RBIM::Rectangular;
    thePlanIrregularity = false;
    theVerticalIrregularity = false;
    theSoftStorey = false;
    theOpening = false;
    theShortColumn = false;
    thePounding = false;
    theRetrofitted = false;
    theBasement = false;
    theBalcony = false;

    theCodeLevel = RBIM::HighCode;
    thePrototype = RBIM::ConcreteShearWall;
    theBuildingHeight = 0.0;
    residentialFloorArea = 0.0;
    commercialFloorArea = 0.0;
    theFootPrintArea = 0.0;
    theTotalFloorlArea = 0.0;
    footprintArea = 0.0;
    theBuildingVolume = 0.0;

    theBuildingType = "C2M";

#ifdef IFC
    isMeshed = false;
    isTopoMeshed = false;
    ifcFileLoaded = false;

    theIFCSystem = nullptr;
    thePlotter = nullptr;
    theMainWindow = nullptr;
    theCSICostMethod = nullptr;

    theMainWindow = qobject_cast<RMainWindow*>(theDomain->parent());
    thePlotter = theMainWindow->getOSGPlotter();
    theCSICostMethod =  make_unique<RCSICostMethod>(theDomain,"CSICostMethod");
    theBIMImportMethod = make_unique<RBIMImportMethod>(this, theDomain);
#endif

}


RBIM::~RBIM()
{

}


QString RBIM::getIfcFile() const
{
    return theIfcFile;
}


void RBIM::setIfcFile(QString value)
{
#ifdef IFC
    auto filePath = value;

    QFileInfo check_file(filePath);
    // Check if file exists and if yes: Is it really a file and no directory?
    if(check_file.exists() && check_file.isFile())
    {
        if(value.endsWith(".ifc"))
        {
            IFCFilePath = filePath;
        }
        else
        {
            qCritical()<<"The file "<<value<<" is not a valid .ifc file";
        }
    }
    else{
        qCritical()<<"The file "<<filePath<<" does not exist, or it may not be a useable file";
    }
#else
    qCritical() << "An IFC file is set but the IFC option is OFF in this version of Rts";
#endif
}


QObject * RBIM::getLocation() const
{
    if(theLocation != nullptr)
        return theLocation;
#ifdef IFC
    else if(theBIMLocation != nullptr)
        return theBIMLocation.get();
#endif
    else {
        qCritical() << "The BIM is missing a location!";
        return nullptr;
    }
}


void RBIM::setLocation(QObject *value)
{
    theLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theLocation);
}


QObject * RBIM::getTimeOfConstruction() const
{
    return theTimeOfConstruction;
}


void RBIM::setTimeOfConstruction(QObject *value)
{
    theTimeOfConstruction = qobject_cast<RTime *>(value);

    if (!theTimeOfConstruction) {
        qCritical() << "These days the occurrence model in" << this->objectName() << "must be an RTime object.";
    }
}


double RBIM::getTimeOfConstructionValue()
{
    return theTimeOfConstruction->getCurrentValue();
}


QObject * RBIM::getTimeOfDemolition() const
{
    return theTimeOfDemolition;
}


void RBIM::setTimeOfDemolition(QObject *value)
{
    theTimeOfDemolition = qobject_cast<RTime *>(value);


    if (!theTimeOfDemolition) {
        qCritical() << "These days the occurrence model in" << this->objectName() << "must be an RTime object.";
    }
}


double RBIM::getTimeOfDemolitionValue()
{
    return theTimeOfDemolition->getCurrentValue();
}


RBIM::RLoadBearingSystem RBIM::getLoadBearingSystem() const
{
    return theLoadBearingSystem;
}


void RBIM::setLoadBearingSystem(RBIM::RLoadBearingSystem value)
{
    theLoadBearingSystem = value;

    computePrototype();
}


RBIM::RConstructionMaterial RBIM::getConstructionMaterial() const
{
    return theConstructionMaterial;
}


void RBIM::setConstructionMaterial(RBIM::RConstructionMaterial value)
{
    theConstructionMaterial = value;

    computePrototype();
}


int RBIM::getNumberOfStoreys() const
{
    return theNumberOfStoreys;
}


void RBIM::setNumberOfStoreys(int value)
{
    theNumberOfStoreys = value;
}


double RBIM::getBuildingPeriod(void)
{
    // Estimate the building period according to its height
    return this->getNumberOfStoreys()*0.10;
}


double RBIM::getCommercialFloorArea() const
{
    return commercialFloorArea;
}


double RBIM::getResidentialFloorArea() const
{
    return residentialFloorArea;
}


double RBIM::getTotalFloorlArea() const
{
    return theTotalFloorlArea;
}


QString RBIM::getFootprintArea()
{
    return QString::number(footprintArea);
}


void RBIM::setFootprintArea(QString value)
{
    footprintArea = value.toDouble();
}


RBIM::ROccupancyType RBIM::getOccupancyType() const
{
    return theOccupancyType;
}


void RBIM::setOccupancyType(RBIM::ROccupancyType value)
{
    theOccupancyType = value;
}


RBIM::RPlanShape RBIM::getPlanShape() const
{
    return thePlanShape;
}


void RBIM::setPlanShape(RBIM::RPlanShape value)
{
    thePlanShape = value;
}


bool RBIM::getPlanIrregularity() const
{
    return thePlanIrregularity;
}


void RBIM::setPlanIrregularity(bool value)
{
    thePlanIrregularity = value;
}


bool RBIM::getVerticalIrregularity() const
{
    return theVerticalIrregularity;
}


void RBIM::setVerticalIrregularity(bool value)
{
    theVerticalIrregularity = value;
}


bool RBIM::getSoftStorey() const
{
    return theSoftStorey;
}


void RBIM::setSoftStorey(bool value)
{
    theSoftStorey = value;
}


bool RBIM::getOpening() const
{
    return theOpening;
}


void RBIM::setOpening(bool value)
{
    theOpening = value;
}


bool RBIM::getShortColumn() const
{
    return theShortColumn;
}


void RBIM::setShortColumn(bool value)
{
    theShortColumn = value;
}


bool RBIM::getPounding() const
{
    return thePounding;
}


void RBIM::setPounding(bool value)
{
    thePounding = value;
}


bool RBIM::getRetrofitted() const
{
    return theRetrofitted;
}


void RBIM::setRetrofitted(bool value)
{
    theRetrofitted = value;

    computeCodeLevel(this->getRetrofitted());
}


bool RBIM::getBasement() const
{
    return theBasement;
}


void RBIM::setBasement(bool value)
{
    theBasement = value;
}


bool RBIM::getBalcony() const
{
    return theBalcony;
}


void RBIM::setBalcony(bool value)
{
    theBalcony = value;
}


QList<RComponent *> RBIM::getComponentList()
{
    // For now keep it simple but soon have more than one building
    return theDomain->findChildren<RComponent *>();
}


double RBIM::getManufacturingCost()
{
    // Declare the total manufacturing cost to be returned from this method
    double manufacturingCost = 0.0;


    // Get list of the components belonging to this BIM
    QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();

    for (int i=0; i<theComponentList.size(); i++) {

        manufacturingCost += theComponentList.at(i)->getManufacturingCost();
    }

    return manufacturingCost;
}


double RBIM::getConstructionCost()
{
    // Declare the total construction cost to be returned from this method
    double constructionCost = 0.0;


    // Get list of the components belonging to this BIM
    QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();

    for (int i=0; i<theComponentList.size(); i++) {

        constructionCost += theComponentList.at(i)->getConstructionCost();
    }

    return constructionCost;
}


double RBIM::getRepairCost()
{
    double cost = 0.0;

    // Get list of the components belonging to this BIM
    QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();

    for (int i=0; i<theComponentList.size(); i++) {

        cost += theComponentList.at(i)->getRepairCost();
    }

    // if(cost > 0.0)
    //     qDebug()<<"Repair cost: "<<cost;

    return cost;
}


double RBIM::getPreventativeRepairCost()
{
    // Declare the total cost to be returned from this method
    double cost = 0.0;


    // Get list of the components belonging to this BIM
    QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();

    for (int i=0; i<theComponentList.size(); i++) {

        cost += theComponentList.at(i)->getPreventativeRepairCost();
    }

    // if(cost > 0.0)
    //     qDebug()<<"Repair cost: "<<cost;

    return cost;
}


double RBIM::getVolumeOfConcrete()
{
    // Declare the volume to be returned from this method
    double volume = 0.0;


    // Get list of the components belonging to this BIM
    QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();

    for (int i=0; i<theComponentList.size(); i++) {

        volume += theComponentList.at(i)->getVolumeOfConcrete();
    }

    return volume;
}


double RBIM::getVolumeOfSteel()
{
    // Declare the volume to be returned from this method
    double volume = 0.0;


    // Get list of the components belonging to this BIM
    QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();

    for (int i=0; i<theComponentList.size(); i++) {

        volume += theComponentList.at(i)->getVolumeOfSteel();
    }

    return volume;
}


double RBIM::getVolumeOfWood()
{
    // Declare the volume to be returned from this method
    double volume = 0.0;


    // Get list of the components belonging to this BIM
    QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();

    for (int i=0; i<theComponentList.size(); i++) {

        volume += theComponentList.at(i)->getVolumeOfWood();
    }

    return volume;
}


double RBIM::getOccupancyNumber()
{
    // Let this number vary with hour-of-day (check current time)
    return 50.0;
}


QString RBIM::getConstructionType()
{
    return QString("high-rise-concrete");
}


QString RBIM::getBuildingDamageState()
{
    return QString("slight");
}


RBIM::RCodeLevel RBIM::getCodeLevel()
{
    return theCodeLevel;
}


RBIM::RPrototype RBIM::getPrototype()
{
    return thePrototype;
}


QString RBIM::getBuildingType()
{
    return theBuildingType;
}


void RBIM::setBuildingType(QString value)
{
    theBuildingType = value;

    qDebug()<<"Setting HAZUS Building Type: "<<value;
}


double RBIM::getMaxInterstoreyDrift(void)
{
    auto maxDrift  =0.0;
    auto componentList = this->getComponentList();

    for(auto&& it : componentList)
    {
        if(it->getIsStructural() == false)
            continue;

        auto theMesh = it->getMesh();

        if(theMesh == nullptr)
        {
            qDebug()<<"Could not get the mesh for component "<<it->objectName()<<" in "<<__FUNCTION__;
            continue;
        }

        auto drift = theMesh->getMaxDrift();
        if(drift > maxDrift)
            maxDrift = drift;
    }

    return maxDrift;
}


double RBIM::getBuildingDrift(void)
{
    double maxDrift = 0.0;

    // Get the slab and roof components,from which to caclulate building drift
    auto plateCompList = theDomain->findChildren<RPlateComponent*>();

    RPlateComponent* theRoofComponent = nullptr;

    // Find the roof component
    // First check if it is explicitly given as a roof component by BIM
    for(auto&& plateComponent : plateCompList)
    {
       if(plateComponent->getPlateType() == RPlateComponent::plateType::ROOF)
       {
           theRoofComponent = plateComponent;
           break;
       }
    }

    // If not given by BIM, then find the component that is the highest in elevation
    auto maxHeight = 0.0;
    if(theRoofComponent == nullptr)
    {
        for(auto&& plateComponent : plateCompList)
        {
          auto p1 = qobject_cast<RPoint*>(plateComponent->getPoint1());
          auto p2 = qobject_cast<RPoint*>(plateComponent->getPoint2());
          auto p3 = qobject_cast<RPoint*>(plateComponent->getPoint3());
          auto p4 = qobject_cast<RPoint*>(plateComponent->getPoint4());

          auto p1z = p1->getZCoordinateValue();
          auto p2z = p2->getZCoordinateValue();
          auto p3z = p3->getZCoordinateValue();
          auto p4z = p4->getZCoordinateValue();

          auto height = 0.25*(p1z+p2z+p3z+p4z);

          if(height > maxHeight)
          {
              maxHeight = height;
              theRoofComponent = plateComponent;
          }
        }
    }

    // Get the nodes from these components
    std::vector<RNode*> theNodesVec;
    auto elementList = theRoofComponent->getElementList();
    for(auto&& element : elementList)
    {
        auto nodeVec = element->getNodes();

        theNodesVec.insert(theNodesVec.end(), nodeVec.begin(), nodeVec.end());
    }

    // Calculate the building drift
    for(auto&& it : theNodesVec)
    {
        auto xDisp = it->getXDisplacement();
        auto yDisp = it->getYDisplacement();

        auto dx = xDisp;
        auto dy = yDisp;

        auto heightOfNode = it->getZCoordinateValue();

        auto horizDisp = sqrt(dx*dx + dy*dy);

        auto drift = horizDisp/heightOfNode;

        if(drift>maxDrift)
            maxDrift = drift;
    }

    return maxDrift;
}


double RBIM::getBuildingHeight()
{
    return theBuildingHeight;
}


void RBIM::setBuildingHeight(double value)
{
    theBuildingHeight = value;
}


QString RBIM::getBuildingOccupancyClass() const
{
    return theOccupancyClass;
}


void RBIM::setBuildingOccupancyClass(QString value)
{
    theOccupancyClass = value;
}


void RBIM::setNumberOfHousingUnits(int value)
{
    numberOfHousingUnits = value;
}


int RBIM::getNumberOfHousingUnits() const
{
    return numberOfHousingUnits;
}


double RBIM::getTheBuildingVolume() const
{
    return theBuildingVolume;
}


void RBIM::setTheBuildingVolume(double value)
{
    theBuildingVolume = value;
}


QList<QPair<double, QString> > RBIM::getTriggerTimes(double startTime, double endTime)
{
    QList<QPair<double, QString> > repairTimes;

    // Get list of the components belonging to this BIM
    QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();

    // Build list of periods, not very efficient since some components will have the same periods
    for (int i=0; i<theComponentList.size(); i++) {

        repairTimes.append(theComponentList.at(i)->getTriggerTimes(startTime, endTime));
    }

    return repairTimes;

}


double RBIM::getBuildingStoreyHeight(int storey)
{
    auto numStorey = buildingStoreyHeight.size();

    if(storey<0 || storey >= numStorey)
    {
        qDebug()<<"Error in getting the building storey height in RBIM for storey number "<<storey<<" the building has "<<numStorey<< " storeys";

        return 0.0;
    }

    return buildingStoreyHeight.at(storey);
}


double RBIM::getBuildingStoreyElevation(int storey)
{
    auto numStorey = buildingStoreyElevation.size();

    if(storey<0 || storey >= numStorey)
    {
        qDebug()<<"Error in getting the building storey elevation in RBIM for storey number "<<storey<<" the building has "<<numStorey<< " storeys";

        return 0.0;
    }

    return buildingStoreyElevation.at(storey);
}


RCSICostMethod* RBIM::getTheCSICostMethod()
{
    return theCSICostMethod.get();
}


#ifdef IFC
void RBIM::createMesh()
{
    if (isMeshed || IFCFilePath.isEmpty()) {
        return;
    }

    theMainWindow->openIFCFile(IFCFilePath);
    theMainWindow->importBIMFromIFC(this);

    isMeshed = true;
}


void RBIM::setIfcPPSystem(QPointer<IfcPlusPlusSystem> IfcPPSystem)
{
    if(IfcPPSystem.isNull())
    {
        qCritical()<<"Empty pointer of Ifc++ system given in "<<__FUNCTION__;
        return;
    }
    else
    {
        theIFCSystem = IfcPPSystem;
        initializeRBIM();
    }

    return;
}


void RBIM::createComponentsFromBIM()
{

    theBIMImportMethod->convertBIMtoRBIM();

    isMeshed = true;
}


void RBIM::createTopoLogicalMesh(void)
{
    if(!isTopoMeshed)
    {
        auto mapofRtsIfcObjects = theBIMImportMethod->getMapofRtsIfcObjects();

        for(auto&& it: mapofRtsIfcObjects)
        {
            RComponent* theComponent = it.second->theRComponent;

            if(theComponent)
            {
                int isMeshed = theComponent->createTopologicalMesh();

                if(isMeshed != 0)
                    qDebug()<<"Warning: this component failed to mesh in RBIM: "<<theComponent->objectName();
            }
        }

        isTopoMeshed = true;
    }
}


void RBIM::initializeRBIM()
{
    // Get the Ifc++ model from the system - this provides the Ifc entities
    theIFCppModel = theIFCSystem->getIFCModel();

    if(theIFCppModel == nullptr)
    {
        qCritical()<<"Empty pointer of IFCpp model given in RBIM.";
        return;
    }

    // Map of IFC entites from the Ifc++ model
    auto mapOfIfcEntity = theIFCppModel->getMapIfcEntities();


    if(mapOfIfcEntity.empty())
    {
        qDebug()<<"There are no IFC entities in the project in "<<__FUNCTION__;
        return;
    }

    // Set the IFC system and model to the BIM import method
    theBIMImportMethod->setTheIFCSystem(theIFCSystem);
    theBIMImportMethod->setTheIFCppModel(theIFCppModel);
    theBIMImportMethod->setMapOfIfcEntity(mapOfIfcEntity);

    shared_ptr<IfcSite> theSite = theIFCppModel->getSite();

    if(theSite != nullptr)
    {
        // Create an RLocation from IFC site data
        theBIMLocation = make_shared<RLocation>(this,"To do: name of project");

        // Get the lat, long, and elevation
        shared_ptr<IfcCompoundPlaneAngleMeasure> theLongitude;
        shared_ptr<IfcCompoundPlaneAngleMeasure> theLatitude;
        shared_ptr<IfcLengthMeasure>             theElevation;

        if(theSite->m_RefLongitude)
            theLongitude = theSite->m_RefLongitude;
        if(theSite->m_RefLatitude)
            theLatitude = theSite->m_RefLatitude;

        // Check to make sure we have both lat and long
        if(!theLatitude != !theLongitude)
            qCritical()<<"Error, the latitude and longitude are not defined in "<<__FUNCTION__;

        if(theLatitude && theLongitude)
        {
            auto latVec = theLatitude->m_vec;
            auto longVec = theLongitude->m_vec;

            //Convert from vector (degrees, minutes, seconds format into decimal degrees)
            auto latDbl = latVec[0]+latVec[1]/60.0+latVec[2]/3600.0+latVec[3]/(3600.0*1.0e6);
            auto longDbl = longVec[0]+longVec[1]/60.0+longVec[2]/3600.0+longVec[3]/(3600.0*1.0e6);

            theBIMLocation->setLatitude(latDbl);
            theBIMLocation->setLongitude(longDbl);
        }

        if(theSite->m_RefElevation)
        {
            theElevation = theSite->m_RefElevation;
            auto buildingElevation = theElevation->m_value;
            theBIMLocation->setAltitude(buildingElevation);
        }
    }
}
#endif


void RBIM::computeCodeLevel(bool retrofitted)
{
    /*
    if (theYearOfConstruction <= 1940) {
        if (retrofitted) {
            theCodeLevel = RBIM::ModerateCode;
        } else {
            theCodeLevel = RBIM::PreCode;
        }
    }
    else if (theYearOfConstruction <= 1975) {
        if (retrofitted) {
            theCodeLevel = RBIM::ModerateCode;
        } else {
            theCodeLevel = RBIM::LowCode;
        }
    }
    else {
        if (retrofitted) {
            theCodeLevel = RBIM::HighCode;
        } else {
            theCodeLevel = RBIM::ModerateCode;
        }
    }
    */
}


void RBIM::computePrototype()
{
    /*
    buildingTypes << "Concrete frame"
                  << "Concrete shear wall"
                  << "Concrete frame with masonry infill"
                  << "Steel frame"
                  << "Braced steel frame"
                  << "Light steel frame"
                  << "Steel frame with concrete shear wall"
                  << "Steel frame  with masonry infill"
                  << "Precast concrete"
                  << "Reinforced masonry"
                  << "Unreinforced masonry"
                  << "Light wood frame"
                  << "Large wood frame";

    theBuildingType = "Concrete frame";

    bool ok = true;
    if (theMaterial == RBIM::ReinforcedConcrete) {
        if (theLoadBearingSystem == RBIM::ShearWall) {
            thePrototype = RBIM::ConcreteShearWall;
        } else if (theLoadBearingSystem == RBIM::MomentFrame) {
            thePrototype = RBIM::ConcreteMomentFrame;
        } else if (theLoadBearingSystem == RBIM::MasonryWall) {
            thePrototype = RBIM::ConcreteFrameWithMasonryInfillWall;
        } else {
            ok = false;
        }
    } else if (theMaterial == RBIM::PrecastConcrete) {
        if (theLoadBearingSystem == RBIM::ShearWall) {
            thePrototype = RBIM::PrecastFrameWithShearWall;
        } else {
            ok = false;
        }
    } else if (theMaterial == RBIM::Steel) {
        if (theLoadBearingSystem == RBIM::ShearWall) {
            thePrototype = RBIM::SteelFrameWithConcreteShearWall;
        } else if (theLoadBearingSystem == RBIM::MomentFrame) {
            thePrototype = RBIM::SteelMomentFrame;
        } else if (theLoadBearingSystem == RBIM::BracedFrame) {
            thePrototype = RBIM::SteelBracedFrame;
        } else if (theLoadBearingSystem == RBIM::LightFrame) {
            thePrototype = RBIM::SteelLightFrame;
        } else if (theLoadBearingSystem == RBIM::MasonryWall) {
            thePrototype = RBIM::SteelFrameWithMasonryInfillWall;
        } else {
            ok = false;
        }
    } else if (theMaterial == RBIM::Wood) {
        if (theLoadBearingSystem == RBIM::MomentFrame) {
            thePrototype = RBIM::WoodLargeFrame;
        } else if (theLoadBearingSystem == RBIM::LightFrame) {
            thePrototype = RBIM::WoodLightFrame;
        } else {
            ok = false;
        }
    } else if (theMaterial == RBIM::ReinforcedMasonry) {
        if (theLoadBearingSystem == RBIM::MasonryWall) {
            thePrototype = RBIM::ReinforcedMasonryBearingWall;
        } else {
            ok = false;
        }
    } else if (theMaterial == RBIM::UnreinforcedMasonry) {
        if (theLoadBearingSystem == RBIM::MasonryWall) {
            thePrototype = RBIM::UnreinforcedMasonryBearingWall;
        } else {
            ok = false;
        }
    } else if (theMaterial == RBIM::MobileHome) {
        thePrototype = RBIM::Mobile;
    }

    if (ok == false) {
        //qCritical() << "Error in model" << objectName() << ": This combination of the material and the load bearing system is no allowed. Rts will use the default prototype.";
    }
    */
}










