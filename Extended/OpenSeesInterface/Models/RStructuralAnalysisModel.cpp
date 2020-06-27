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

#include "RStructuralAnalysisModel.h"
#include "RStructuralAnalysis.h"
#include "RLinearStaticStructuralAnalysis.h"
#include "RInelasticStaticStructuralAnalysis.h"
#include "RInelasticDynamicStructuralAnalysis.h"
#include "RUniformExcitationPattern.h"
#include "ROpenSeesTimeSeries.h"
#include "RMainWindow.h"
#include "RComponent.h"
#include "RMesh.h"
#include "RPoint.h"
#include "RFixedPlaneBoundaryCondition.h"
#include "RLoadPatternModel.h"
#include "RNode.h"
#include "RMathUtils.h"
#include "RLinearSeries.h"
#include "RLoadPattern.h"
#include "RSlaveNode.h"
#include "RResponse.h"
#include "RUniaxialMaterial.h"
#include "RFiber.h"
#include "RSectionForceDeformation.h"
#include "RIntensityModel.h"
#include "RNDMaterial.h"
#include "RNonLoadBearingWallComponent.h"
#include "RPlateComponent.h"
#include "RNodalLoad.h"
#include "RRigidBeam.h"
#include "ROpenSeesInputFileGenerator.h"

#ifdef DETAILEDBUILDINGS
#include "RBIM.h"
#endif

#ifdef OSG
#include "ROSGPlotter.h"
#endif

// OpenSees includes
#include <iostream>
#include <StandardStream.h>
#include <OPS_Globals.h>
#include "ID.h"
#include "SP_Constraint.h"
#include "TimeSeries.h"
#include "LoadPattern.h"
#include "NodalLoad.h"
#include "Matrix.h"
#include "RigidBeam.h"
#include "SP_ConstraintIter.h"

//Qt includes
#include <QMetaProperty>
#include <QDateTime>
#include <QCoreApplication>

// For OpenSees output
StandardStream sserr;
OPS_Stream *opserrPtr = &sserr;
#include <Domain.h>

RStructuralAnalysisModel::RStructuralAnalysisModel(QObject *parent, QString name)
    : RModel(parent, name), AnalysisModel()
{
    theOpenSeesDomain = std::make_unique<Domain>();

    //Set the OpenSees domain to Rts Domain
    theDomain->setTheOpenSeesDomain(theOpenSeesDomain.get());

    runOnce = false;
    initialized = false;
    hazardsApplied = false;
    areComponentsMeshed = false;
    theLinearLoadPattern = nullptr;
    theStructuralAnalysis = nullptr;

#ifdef OSG
    theMainWindow = qobject_cast<RMainWindow*>(theDomain->parent());
    thePlotter = theMainWindow->getOSGPlotter();
    thePlotter->setObjectName("detailedBuildingsPlotter");
#endif

#ifdef DETAILEDBUILDINGS
    theBIM = nullptr;
#endif

    // Create the response object
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theMaximumResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theMaximumResponse->setModel(this);
}

RStructuralAnalysisModel::~RStructuralAnalysisModel()
{
    theDomain->setTheOpenSeesDomain(nullptr);

}

QStringList RStructuralAnalysisModel::getActionList()
{
    QStringList actionList;

    actionList << "&Run"
               << "Separator"
               << "&Plot Undeformed"
               << "&Plot Deformed"
              #if defined(DETAILEDBUILDINGS) && defined(OSG)
               << "&Plot Components"
               << "&Plot Deformed Components"
               << "&Plot Dynamic Elements"
               << "&Plot Dynamic Components"
              #endif
               << "Separator"
               << "Create Mesh"
               << "Separator"
               << "Plot Visual Damage"
               << "Separator"
               << "Generate Input File"
               << "Separator"
               << "Plot BMD"
               << "Plot SFD"
               << "Plot AFD"
               << "Separator"
               << "Plot SigmaXX"
               << "Plot SigmaYY"
               << "Plot TauXY"
               << "Plot TauXZ"
               << "Plot TauYZ"
               << "Separator"
               << "&Import IFC"
               << "Separator"
               << "&Help";

    return actionList;
}


QObject * RStructuralAnalysisModel::getStructuralAnalysis() const
{
    return theStructuralAnalysis;
}

void RStructuralAnalysisModel::setStructuralAnalysis(QObject *value)
{
    theStructuralAnalysis = qobject_cast<RStructuralAnalysis *>(value);

    if(theStructuralAnalysis)
    {
        theStructuralAnalysis->setTheAnalysisModel(this);
    }
    else
    {
        qDebug()<<"A structural analysis is required in "<<__FUNCTION__;
    }
}

#ifdef OSG
void RStructuralAnalysisModel::plotUndeformed()
{
    thePlotter->plotStructure(false, "NOTIFC",0.0);
    //thePlotter->zoomBoundingSphere();
}

void RStructuralAnalysisModel::plotDeformed()
{
    thePlotter->plotStructure(true, "NOTIFC",0.0,true);
    //thePlotter->zoomBoundingSphere();
}

void RStructuralAnalysisModel::plotBMD()
{
    thePlotter->plotStructure(false, "BMD",0.0);
}

void RStructuralAnalysisModel::plotSFD()
{
    thePlotter->plotStructure(false, "SFD",0.0);
}

#endif

void RStructuralAnalysisModel::createMesh()
{

    //Check to see if the BIM has been meshed
#ifdef IFC
    if(theBIM != nullptr)
    {
        if(theBIM->isMeshed == true)
            return;
        else
        {
            theBIM->createMesh();
            theBIM->createTopoLogicalMesh();
            return;
        }
    }
#endif


    //Check to see if the components or elements are meshed
    if(areComponentsMeshed == false)
    {

#if  defined(DETAILEDBUILDINGS)
        QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();

        if(theComponentList.empty())
        {
            qDebug()<<"Error: The component list is empty in "<<__FUNCTION__;
            return;
        }

        for (auto&& it: theComponentList)
        {
            if(!it->meshWasCreated)
                it->createMesh();

            auto theMesh = it->getMesh();
            if(theMesh)
            {
                if(!theMesh->getTopologicalMesh())
                    it->createTopologicalMesh();
            }
            else
                qDebug()<<"Error: The component "<<it->objectName()<<" failed to mesh";
        }
#endif

        areComponentsMeshed = true;
    }

}

#ifdef DETAILEDBUILDINGS
QObject * RStructuralAnalysisModel::getBIM() const
{
    return theBIM;
}

void RStructuralAnalysisModel::setBIM(QObject *value)
{
    theBIM = qobject_cast<RBIM *>(value);
}

QList<QPair<double, QString> > RStructuralAnalysisModel::getTriggerTimes(double startTime, double endTime)
{
    QList<QPair<double, QString> > triggerTimes;

    if(theBIM != nullptr)
        triggerTimes = theBIM->getTriggerTimes(startTime, endTime);


    return triggerTimes;
}
#endif


#ifdef HAZARDS
void RStructuralAnalysisModel::setHazardList(QString value)
{
    theInputHazards = stringToModelList(value);

    for(auto&& it:theInputHazards)
    {
        auto theResponse = qobject_cast<RLoadPatternModel*>(it);

        if(theResponse == nullptr)
        {
            qCritical()<<"A null pointer came into the setHazardList for:"<<it->objectName()<<" in this "<<this->objectName();
            continue;
        }

        theHazardList.push_back(theResponse);
    }
}

QString RStructuralAnalysisModel::getHazardList()
{
    return modelListToString(theInputHazards);
}
#endif




QString RStructuralAnalysisModel::getTheLoadPattern()
{
    QString string;
    for (int i = 0; i < theLoadPatterns.count(); i++)
    {
        if (theLoadPatterns[i]) {
            string += theLoadPatterns[i]->objectName();
            string += "; ";
        } else {
            theLoadPatterns.removeAt(i);
            i--;
        }
    }

    return string;
}

void RStructuralAnalysisModel::setTheLoadPattern(QString value)
{
    QRegExp regExp = QRegExp(QString("[ ,;:\t\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator)));
    QStringList stringList = value.split(regExp, QString::SkipEmptyParts);

    for (int i = 0; i < stringList.count(); i++)
    {
        RLoadPattern *object = theDomain->findChild<RLoadPattern *>(stringList[i]);
        if (object)
        {
            theLoadPatterns.append(object);
        } else {
            qCritical() << "Error in" << objectName() << ": Could not find a load pattern named" << stringList[i] << "in the domain";
        }
    }
}

QString RStructuralAnalysisModel::getResponses() const
{
    QString result;
    for (int i = 0; i < theStructuralObjects.count(); i++) {
        QString structuralObjectName = theStructuralObjects[i]->objectName();
        QString propertyName = theResponses[i];

        result += structuralObjectName + '.' + propertyName + "; ";
    }

    return result;
}

void RStructuralAnalysisModel::setResponses(QString value)
{
    QList<QPointer<RObject> > tempStructuralObjects;
    QStringList tempResponses;

    QRegExp regExp = QRegExp(QString("[ ,;:\t\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator)));
    QStringList stringList = value.split(regExp, QString::SkipEmptyParts);
    for (int i = 0; i < stringList.count(); i++) {
        QStringList nameProperty = stringList[i].split('.',  QString::SkipEmptyParts);
        if (nameProperty.count() != 2) {
            //Error
            qCritical() << "Error in" << objectName() << ": Invalid response.";
            return;
        }
        QString structuralObjectName = nameProperty[0].trimmed();
        RObject *structuralObject = theDomain->findChild<RObject *>(structuralObjectName);
        if (!structuralObject) {
            qCritical() << "Error in" << objectName() << ": Could not find an object named" << structuralObjectName;
            return;
        }
        tempStructuralObjects << structuralObject;

        // Error handling here: if this structural object has such a property
        const QMetaObject *metaObject = structuralObject->metaObject();
        QString propertyStr = nameProperty[1].trimmed();
        QByteArray byteArrayName = propertyStr.toLatin1();
        const char * propertyCharStar = byteArrayName.data();
        int propertyIndex = metaObject->indexOfProperty(propertyCharStar);
        if (propertyIndex < 0) {
            qCritical() << "Error in" << objectName() << ": Could not find property named" << propertyStr << "in the structural object" << structuralObjectName << ".";
            return;
        }

        tempResponses << propertyStr;
    }

    // Removing the old responses
    theStructuralObjects.clear();
    theResponses.clear();
    theStructuralObjects = tempStructuralObjects;
    theResponses = tempResponses;

    while (theResponseList.count() > 0) {
        //theResponseList[0]->setModel(0);
        delete theResponseList.takeAt(0);
    }

    // Instantiating the response objects one for each structure
    for (int i = 0; i < theStructuralObjects.count(); i++) {
        theDomain->createObject(objectName() + theStructuralObjects[i]->objectName() + theResponses[i], "RGenericResponse");
        RResponse *response = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
        response->setModel(this);
        theResponseList << response;
    }

    // Update the domain tree
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    mainWindow->updateTree("Parameters");

}

#ifdef IFC

void RStructuralAnalysisModel::generateInputFile()
{
    ROpenSeesInputFileGenerator theOSGen(theDomain);

    auto res = this->initializeTheAnalysis();

    if(res == false)
    {
        qCritical()<<"Could not generate an OpenSees input file, failing in initializing the structural analysis ";
        return;
    }

    theOSGen.generateInputFile();

    qDebug()<<"OpenSees input file generation complete";

    return;
}


void RStructuralAnalysisModel::plotComponents()
{
    thePlotter->plotTopology();
}

void RStructuralAnalysisModel::plotDeformedComponents()
{
    thePlotter->plotTopology(true);
}

void RStructuralAnalysisModel::plotDynamicElements()
{
    thePlotter->plotDynamicStructure();
}

void RStructuralAnalysisModel::plotDynamicComponents()
{
    thePlotter->plotDynamicTopo();
}

#endif


int RStructuralAnalysisModel::evaluateModel(RGradientType theGradientType)
{

    theMaximumResponse->setCurrentValue(0.0);

    if(theHazardList.empty() && theLoadPatterns.isEmpty())
    {
        return 0;
    }
#ifdef HAZARDS
    else if(!theHazardList.empty() && theLoadPatterns.isEmpty())
    {

        bool hazardApplied = this->applyHazards();

        //If no hazard applied, this means that no hazards occur at this time so no need to run the analysis
        if(!hazardApplied)
            return 0;
    }
#endif


    if(!initialized)
    {
        // Check for the structural analysis
        if(theStructuralAnalysis == nullptr)
        {
            qDebug()<<"Error: a structural analysis is required in "<<__FUNCTION__;
            return -1;
        }

        this->createMesh();

#ifdef DETAILEDBUILDINGS
        // Get the components
        QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();


        // Check that the components are meshed
        for (auto&& it : theComponentList)
        {
            if (it->meshWasCreated == false)
            {
                qCritical() << "The structural analysis encountered components in which a mesh was not yet created.";
                return -1;
            }

#ifdef IFC
            if(theBIM)
            {
                // Check for connectivity between components
                componentConnectivityStruct& theCompConnStruct = it->theConnectivityStruct;

                if(theCompConnStruct.theConnectivityList.empty())
                {
                    // qCritical()<<"Error: The component "<<it->objectName()<<" is not connected to any other components";
                    // break;
                }
            }
#endif
        }
#endif

        if(initialized == false)
            initialized = initializeTheAnalysis();

        if(initialized == false)
        {
            qCritical()<<"Error: The structural analysis failed to initialize in "<<this->objectName();
            return -1;
        }


        if (theOutputLevel >= RObject::Medium) {
            auto numNodes = theOpenSeesDomain->getNumNodes();
            auto numElems = theOpenSeesDomain->getNumElements();
            auto numSPConstraints = theOpenSeesDomain->getNumSPs();
            auto numLoad = theOpenSeesDomain->getNumLoadPatterns();
            auto numMPConstraints = theOpenSeesDomain->getNumMPs();
            auto RVlist = theDomain->findChildren<RRandomVariable*>();
            auto numRv = RVlist.size();
            // for(auto&& it:RVlist ){
            //     qDebug()<<"RV:"<<it->objectName()<< "\n";
            // }
            qDebug()<<"Starting the analysis: \n";
            qDebug()<<"Number of nodes: "<<numNodes<<"\n";
            qDebug()<<"Number of elements: "<<numElems<<"\n";
            qDebug()<<"Number of single point constraints: "<<numSPConstraints<<"\n";
            qDebug()<<"Number of multi-point constraints: "<<numMPConstraints<<"\n";
            qDebug()<<"Number of load patterns: "<<numLoad<<"\n";
            qDebug()<<"Number of random variables: "<<numRv<<"\n";
        }

        // Get the number of steps and dT from the load pattern
        this->autoSetTransientAnalysisParameters(theLoadPatterns);

    }

    if(runOnce)
    {
        // Revert the analysis to start
        theOpenSeesDomain->revertToStart();

        auto nodeList = theDomain->findChildren<RNode*>();
        auto elemList = theDomain->findChildren<RElement*>();
        auto componentList = theDomain->findChildren<RComponent*>();
        auto uniMatList = theDomain->findChildren<RUniaxialMaterial*>();
        auto nDmatList = theDomain->findChildren<RNDMaterial*>();
        auto fiberList = theDomain->findChildren<RFiber*>();
        auto sectionList = theDomain->findChildren<RSectionForceDeformation*>();

        // Note: ORDER MATTERS!
        // Update the nodal coordinates
        for(auto&& it: nodeList)
            it->updateNode();

        // Update the material parameters
        for(auto&& it: uniMatList)
            it->update();

        for(auto&& it: nDmatList)
            it->update();

        // Update the fiber locations and areas
        for(auto&& it: fiberList)
            it->update();

        // Update the sections with the new fibers
        for(auto&& it: sectionList)
            it->update();

        // Update the elements
        for(auto&& it: elemList)
            it->update();

        // Update the components
        for(auto&& it: componentList)
        {
            it->reset();
            it->resetVisualDamage();
        }

        // Update the nodal load, mass, and load pattern values
        this->update();

    }


    // START TIMER
    QDateTime startTime = QDateTime::currentDateTime();

    if (theOutputLevel >= RObject::Medium) {
        qDebug() << "The analysis in" << this->objectName() << "started...";
        qDebug() << "";
    }

    // Keep the GUI responsive
    QCoreApplication::processEvents();

    int status = theStructuralAnalysis->conductStructuralAnalysis();

    //This should be here. Even if the structural analysis fails, things need to be reset to the start.
    runOnce = true;

    if (status != 0) {
        qCritical() << "Error: Something went wrong when running the structural analysis in "<<__FUNCTION__<<" in "<<this->objectName();
        return -1;
    }

    // STOP TIMER
    QDateTime finishTime = QDateTime::currentDateTime();
    double numDays = startTime.daysTo(finishTime);
    double numMilliSecs = startTime.time().msecsTo(finishTime.time());
    double numSecs = numDays * 86400.0 + numMilliSecs / 1000.0;
    if (theOutputLevel >= RObject::Medium) {
        qDebug() << "The analysis in" << this->objectName() << "completed in" << numSecs << "seconds.";
        qDebug() << "";
    }

    // UPDATE USER-REQUESTED RESPONSES
    for (auto i = 0; i < theResponseList.count(); ++i)
    {
        //For each response....
        RResponse* theResponse = theResponseList[i];

        QByteArray byteArrayName = theResponses[i].toLatin1();
        const char *propertyName = byteArrayName.data();

        bool ok;
        double response = theStructuralObjects[i]->property(propertyName).toDouble(&ok);
        if (!ok) {
            qCritical() << "Error: In the model" << objectName() << ": Could not read the property" << theResponses[i] << "in the structural object" << theStructuralObjects[i]->objectName() << "DDM not performed.";
            return -1;
        }

        theResponse->setCurrentValue(response);
    }


    //Update this response
    auto maxDisp = theStructuralAnalysis->getMaxDisplacement();

    theMaximumResponse->setCurrentValue(maxDisp);

    //    qDebug()<<"Nodal Displacements:\n";
    //    for(auto&& it: nodeList)
    //    {
    //        qDebug()<<"Node with tag: "<<it->getObjectTag()<<"and name"<<it->objectName()<<"\n";
    //        auto vecDisp = it->getDisp();
    //        for(auto i = 0; i< vecDisp.Size(); ++i)
    //        {
    //            qDebug()<<vecDisp(i);
    //        }
    //    }

    //        auto elementList = theDomain->findChildren<RElement*>();
    //        for(auto it: elementList)
    //        {
    //            it->print();
    //        }

    return 1;
}


bool RStructuralAnalysisModel::initializeTheAnalysis()
{

    // DECLARATIONS
    double tol = 1.0e-3;

    QList<RNode *> theNodeList = theDomain->findChildren<RNode *>();
    QList<RPoint*> thePointList  = theDomain->findChildren<RPoint*>();

    if(theLoadPatterns.empty() && theHazardList.empty())
    {
        qCritical()<<"Error: the OpenSees 'Load Patterns and/or Hazards' was not set in "<<this->objectName()<<" in "<<__FUNCTION__;
        return false;
    }

    // A user can supply their own load patterns, but, a default load pattern will be created for gravity and to add the user loads such as point and element loads.
    // Note that an analysis can have more than one loading pattern; e.g., a static dead load, a linearly increasing snow load, and/or time history acceleration or excitation
    for(auto&& it: theLoadPatterns)
    {
        auto loadPatternFromParam = qobject_cast<RLoadPattern*>(it);
        if(loadPatternFromParam == nullptr)
        {
            qCritical()<<"Error: could not cast a parameter to a load pattern from "<<it->objectName()<<" in "<<__FUNCTION__;
            return false;
        }

        auto loadPattern = loadPatternFromParam->getTheLoadPattern();
        if(loadPattern == nullptr)
        {
            qCritical()<<"Error: could not get the load pattern from "<<it->objectName()<<" in "<<__FUNCTION__;
            return false;
        }

        theOpenSeesDomain->addLoadPattern(loadPattern);
    }


    auto res = this->createStandardLoadPattern();
    if(res != 0)
        return false;

    int loadPatternTag = theLinearLoadPattern->getObjectTag();

    RNode *theNode = nullptr;

    // APPLY LOADS FROM POINTS
    for (auto&& pointIt :thePointList)
    {
        if (pointIt->getIsOn() == false)
            continue;

        // Get the node at this point
        theNode = pointIt->getAssociatedNode();
        if (theNode == nullptr)
        {
            qCritical() << "The point "<<pointIt->objectName()<<"does not have a corresponding node in "<<__FUNCTION__;
            return false;
        }

        auto nodeId = theNode->getObjectTag();

        // NodalLoad(int tag, int node, const Vector &load, bool isLoadConstant = false);
        auto numDof = theNode->getNumberDOF();

        Vector theLoadValues(numDof);

        theLoadValues(0) = pointIt->getXForceValue();
        theLoadValues(1) = pointIt->getYForceValue();
        theLoadValues(2) = pointIt->getZForceValue();
        theLoadValues(3) = pointIt->getXMomentValue();
        theLoadValues(4) = pointIt->getYMomentValue();
        theLoadValues(5) = pointIt->getZMomentValue();

        if(theLoadValues.Norm() != 0.0)
        {
            NodalLoad *theLoad = new NodalLoad(nodeId, nodeId, theLoadValues);
            if (theOpenSeesDomain->addNodalLoad(theLoad,loadPatternTag) == false)
            {
                qDebug()<< "WARNING could not add Nodal Load to OpenSees domain";
                delete theLoad;
            }
        }
    }


#ifdef DETAILEDBUILDINGS
    if(theBIM != nullptr)
    {
        // Fixed all points at ground-level, i.e., at z=0
        //  addSP_Constraint(int axisDirn, double axisValue, const ID &fixityConditions, double tol)

        auto numDof = 6;

        ID fixityConditions(numDof);

        for(auto i =0; i<numDof;++i)
        {
            fixityConditions(i) = 1;
        }

        theOpenSeesDomain->addSP_Constraint(2,0.0,fixityConditions,tol);

        // Set the boundary conditions to the nodes as well, for generating the OpenSees input file
        for (auto& node : theNodeList)
        {
            auto zVal = node->getZCoordinateValue();

            if(fabs(zVal) < tol)
                node->setBoundaryConditions(1,1,1,1,1,1);
        }
    }
#endif

    // APPLY BOUNDARY CONDITIONS FROM POINTS
    int xDisp, yDisp, zDisp, xRot, yRot, zRot;
    for (auto&& pointIt :thePointList)
    {
        if (pointIt->getIsOn() == false)
            continue;

        // Get the node at this point
        theNode = pointIt->getAssociatedNode();
        if (theNode == nullptr)
        {
            qCritical() << "A point does not have a corresponding node in "<<__FUNCTION__;
            return false;
        }

        auto nodeId = theNode->getObjectTag();

        xDisp = 0;
        yDisp = 0;
        zDisp = 0;
        xRot = 0;
        yRot = 0;
        zRot = 0;

        if (pointIt->getXDisplacementRestraint().toLower().startsWith("fixed")) {
            SP_Constraint *theSP = new SP_Constraint(nodeId, 0, 0.0, true);
            // add it to the domain
            if (theOpenSeesDomain->addSP_Constraint(theSP) == false) {
                qDebug()<< "WARNING could not add SP_Constraint to OpenSees domain - node may already be constrained";
                delete theSP;
            }
            xDisp = 1;
        }
        if (pointIt->getYDisplacementRestraint().toLower().startsWith("fixed")) {
            SP_Constraint *theSP = new SP_Constraint(nodeId, 1, 0.0, true);
            if (theOpenSeesDomain->addSP_Constraint(theSP) == false) {
                qDebug()<< "WARNING could not add SP_Constraint to OpenSees domain - node may already be constrained";
                delete theSP;
            }
            yDisp = 1;
        }
        if (pointIt->getZDisplacementRestraint().toLower().startsWith("fixed")) {
            SP_Constraint *theSP = new SP_Constraint(nodeId, 2, 0.0, true);
            if (theOpenSeesDomain->addSP_Constraint(theSP) == false) {
                qDebug()<< "WARNING could not add SP_Constraint to OpenSees domain - node may already be constrained";
                delete theSP;
            }
            zDisp = 1;
        }
        if (pointIt->getXRotationRestraint().toLower().startsWith("fixed")) {
            SP_Constraint *theSP = new SP_Constraint(nodeId, 3, 0.0, true);
            if (theOpenSeesDomain->addSP_Constraint(theSP) == false) {
                qDebug()<< "WARNING could not add SP_Constraint to OpenSees domain - node may already be constrained";
                delete theSP;
            }
            xRot = 1;
        }
        if (pointIt->getYRotationRestraint().toLower().startsWith("fixed")) {
            SP_Constraint *theSP = new SP_Constraint(nodeId, 4, 0.0, true);
            if (theOpenSeesDomain->addSP_Constraint(theSP) == false) {
                qDebug()<< "WARNING could not add SP_Constraint to OpenSees domain - node may already be constrained";
                delete theSP;
            }
            yRot = 1;
        }
        if (pointIt->getZRotationRestraint().toLower().startsWith("fixed")) {
            SP_Constraint *theSP = new SP_Constraint(nodeId, 5, 0.0, true);
            if (theOpenSeesDomain->addSP_Constraint(theSP) == false) {
                qDebug()<< "WARNING could not add SP_Constraint to OpenSees domain - node may already be constrained";
                delete theSP;
            }
            zRot = 1;
        }

        if(xDisp+yDisp+zDisp+xRot+yRot+zRot != 0)
        {
            theNode->setBoundaryConditions(xDisp, yDisp, zDisp, xRot, yRot, zRot);
        }

    }

    RMathUtils theUtils;


    // APPLY BOUNDARY CONDITIONS FROM FIXED PLANES
    QList<RFixedPlaneBoundaryCondition *> theFixedPlaneBCList = theDomain->findChildren<RFixedPlaneBoundaryCondition *>();
    for (auto&& it : theFixedPlaneBCList)
    {
        // Determine the fixed plane - only need three points to define a plane
        RPoint *point1 = qobject_cast<RPoint *>(it->getPoint1());
        RPoint *point2 = qobject_cast<RPoint *>(it->getPoint2());
        RPoint *point3 = qobject_cast<RPoint *>(it->getPoint3());

        std::vector<RPoint*> plane {point1,point2,point3};

        for (auto& it2 : theNodeList)
        {

            auto x = it2->getXCoordinateValue();
            auto y = it2->getYCoordinateValue();
            auto z = it2->getZCoordinateValue();

            std::vector<double> nodeXYZ{x,y,z};

            auto dist = theUtils.distanceFromPointToPlane(plane,nodeXYZ);

            // Lock the node if it lies in the fixed plane
            if ( dist <tol)
            {
                auto nodeId = it2->getObjectTag();

                auto numDof = it2->getNumberDOF();

                for(auto i =0; i<numDof; ++i)
                {
                    SP_Constraint *theSP = new SP_Constraint(nodeId,i, 0.0, true);
                    if (theOpenSeesDomain->addSP_Constraint(theSP) == false) {
                        qDebug()<< "WARNING could not add SP_Constraint to domain - node may already be constrained";
                        delete theSP;
                    }
                }

                it2->setBoundaryConditions(1,1,1,1,1,1);
            }
        }
    }


    //ADD MASS TO THE NODES WHERE SPECIFIED IN THE POINTS
    for (auto&& pointIt :thePointList)
    {
        if (pointIt->getIsOn() == false)
            continue;

        // Get the node at this point
        theNode = pointIt->getAssociatedNode();
        if (theNode == nullptr)
        {
            qCritical() << "A point does not have a corresponding node in "<<__FUNCTION__;
            return false;
        }

        auto ndf = theNode->getNumberDOF();

        auto massParam = pointIt->getMass();

        if (massParam != nullptr)
        {
            Matrix mass(ndf,ndf);

            double theMass = massParam->getCurrentValue();
            for (int i=0; i<ndf; ++i)
                mass(i,i) = theMass;

            theNode->setMass(mass);
        }
    }

    this->createRigidLinks();

    return true;
}


int RStructuralAnalysisModel::createRigidLinks(void)
{
    //CREATE THE RIGID LINKS
    auto theSlaveNodeList = theDomain->findChildren<RSlaveNode *>();

    for(int i = 0; i<theSlaveNodeList.size(); ++i)
    {
        auto it = theSlaveNodeList[i];

        auto slaveNodeTag = it->getObjectTag();

        auto masterNode = it->getMasterNode();

        if(masterNode == nullptr)
        {
            qCritical() <<"Error: In "<<this->objectName()<< ", a slave node does not have a corresponding master node in "<<__FUNCTION__;
            return false;
        }

        auto masterNodeTag = masterNode->getObjectTag();

        QString name = "RigidBeam_" + QString::number(slaveNodeTag) + "_" + QString::number(masterNodeTag);

        theDomain->createObject(name,"RRigidBeam");

        auto theLink = qobject_cast<RRigidBeam*>(theDomain->getLastAddedObject());

        if(theLink == nullptr)
        {
            qCritical() <<"Error: In "<<this->objectName()<< ", could not create the rigid link in "<<__FUNCTION__;
            return false;
        }

        theLink->setNodeR(masterNodeTag);
        theLink->setNodeC(slaveNodeTag);

        auto res = theLink->createRigidBeamElement();

        if(res != 0)
        {
            qCritical() <<"Error: In "<<this->objectName()<< ", could not create the rigid link in "<<__FUNCTION__;
            return false;
        }
    }
}


void RStructuralAnalysisModel::update()
{

    QList<RPoint*> thePointList  = theDomain->findChildren<RPoint*>();

    QList<RNodalLoad*> theNodalLoadList  = theDomain->findChildren<RNodalLoad*>();

    int loadPatternTag = theLinearLoadPattern->getObjectTag();

    RNode *theNode = nullptr;

    // Update the loads from the points
    for (auto&& pointIt :thePointList)
    {
        if (pointIt->getIsOn() == false)
            continue;

        // Get the node at this point
        theNode = pointIt->getAssociatedNode();

        auto nodeId = theNode->getObjectTag();

        // NodalLoad(int tag, int node, const Vector &load, bool isLoadConstant = false);
        auto numDof = theNode->getNumberDOF();

        Vector theLoadValues(numDof);

        theLoadValues(0) = pointIt->getXForceValue();
        theLoadValues(1) = pointIt->getYForceValue();
        theLoadValues(2) = pointIt->getZForceValue();
        theLoadValues(3) = pointIt->getXMomentValue();
        theLoadValues(4) = pointIt->getYMomentValue();
        theLoadValues(5) = pointIt->getZMomentValue();

        if(theLoadValues.Norm() != 0.0)
        {
            // Remove the old NodalLoad
            auto removedObj = theOpenSeesDomain->removeNodalLoad(nodeId,loadPatternTag);

            if(removedObj == nullptr)
            {
                qDebug()<<"Error in removing the nodal load from the OpenSees domain in "<<__FUNCTION__;
            }
            else
            {
                delete removedObj;
                removedObj = nullptr;
            }

            //Add the new load with the updated value
            NodalLoad *theLoad = new NodalLoad(nodeId, nodeId, theLoadValues);
            if (theOpenSeesDomain->addNodalLoad(theLoad,loadPatternTag) == false)
            {
                qDebug()<< "WARNING could not add nodal load to OpenSees domain in "<<__FUNCTION__;
                delete theLoad;
            }
        }
    }


    // Update the mass from the points
    for (auto&& pointIt :thePointList)
    {
        if (pointIt->getIsOn() == false)
            continue;

        // Get the node at this point
        theNode = pointIt->getAssociatedNode();

        auto ndf = theNode->getNumberDOF();

        auto massParam = pointIt->getMass();

        if (massParam != nullptr)
        {
            Matrix mass(ndf,ndf);

            double theMass = massParam->getCurrentValue();
            for (int i=0; i<ndf; ++i)
                mass(i,i) = theMass;

            theNode->setMass(mass);
        }
    }


    // Update the nodal loads
    for(auto&& nodeIt : theNodalLoadList)
        nodeIt->update();

    // Update the load patterns
    for(auto&& loadPattern : theLoadPatterns)
    {
        //Remove the old one from the OpenSees domain
        auto removedObj = theOpenSeesDomain->removeLoadPattern(loadPattern->getObjectTag());

        if(removedObj == nullptr)
        {
            qCritical()<<"Error in removing the load pattern"<<loadPattern->objectName()<<"from the OpenSees domain in "<<__FUNCTION__;
            continue;
        }
        else
        {
            // This causes a crash... perhaps it gets deleted on its own somewhere else. Check if it causes a memory leak
            // delete removedObj;
            removedObj = nullptr;
        }

        loadPattern->update();

        auto lp = loadPattern->getTheLoadPattern();

        if(lp == nullptr)
        {
            qCritical()<<"Error: could not get the load pattern from "<<loadPattern->objectName()<<" in "<<__FUNCTION__;
        }

        auto res = theOpenSeesDomain->addLoadPattern(lp);

        if(res == false)
        {
            qCritical()<<"Error: in appplying the load pattern in the OpenSees domain "<<loadPattern->objectName()<<" in "<<__FUNCTION__;
        }
    }

    return;
}


int RStructuralAnalysisModel::createStandardLoadPattern(void)
{

    // Check to see if a load pattern with a linear series is not already provided, use the first one that comes in
    for(auto&& it:theLoadPatterns)
    {
        auto loadPatternFromParam = qobject_cast<RLoadPattern*>(it);
        if(loadPatternFromParam == nullptr)
        {
            qCritical()<<"Error: could not cast a parameter to a load pattern from "<<it->objectName()<<" in "<<__FUNCTION__;
            return -1;
        }

        auto timeSeries = loadPatternFromParam->getTheTimeSeries();
        if(timeSeries)
        {
            auto isLinearSeries = dynamic_cast<RLinearSeries*>(timeSeries);
            if(isLinearSeries)
            {
                theLinearLoadPattern = loadPatternFromParam;
                break;
            }
        }
    }


    // If nothing was given, create one
    if(theLinearLoadPattern == nullptr)
    {
        //Check to make sure the core property objects are given
        auto theOpenSeesDomain = theDomain->getTheOpenSeesDomain();
        if(theOpenSeesDomain == nullptr)
        {
            qCritical()<<"Error: the OpenSees 'domain' was not set in "<<__FUNCTION__;
            return -1;
        }

        //Construct a linear load series, assume that this is a static analysis
        theDomain->createObject(this->objectName()+"LinearTimeSeries","RLinearSeries");
        auto theSeries = qobject_cast<RLinearSeries*>(theDomain->getLastAddedObject());

        if(!theSeries)
        {
            qCritical() << "Failed to create a load series in RDomain in "<<__FUNCTION__;
            return -1;
        }

        theDomain->createObject(this->objectName()+"LoadPattern","RLoadPattern");
        theLinearLoadPattern = qobject_cast<RLoadPattern*>(theDomain->getLastAddedObject());

        if(!theLinearLoadPattern)
        {
            qCritical() << "Failed to create a load pattern in RDomain in "<<__FUNCTION__;
            return -1;
        }

        theLinearLoadPattern->setTheTimeSeries(theSeries);
    }

    auto loadPat = theLinearLoadPattern->getTheLoadPattern();

    if(!loadPat)
    {
        qCritical()<<"Error: the OpenSees 'Load Pattern' called "<<theLinearLoadPattern->objectName()<<" failed to initialize in "<<__FUNCTION__;
        return -1;
    }

    theOpenSeesDomain->addLoadPattern(loadPat);

    return 0;
}


bool RStructuralAnalysisModel::applyHazards(void)
{

    hazardsApplied = false;

    for(auto&& it : theHazardList)
    {

        if(theBIM != nullptr && it->isOn() == false)
            continue;

        auto isApplied = it->isApplied();

        // If the hazard is applied, remove it so that it can be replaced
        if(isApplied)
        {
            auto loadPatterns = it->getTheLoadPatterns();

            for(auto&& loadPattern : loadPatterns)
            {
                // Remove the old one from the OpenSees domain
                auto removedObj = theOpenSeesDomain->removeLoadPattern(loadPattern->getObjectTag());

                if(removedObj == nullptr)
                {
                    qDebug()<<"Error in removing the element from the OpenSees domain"<<__FUNCTION__;
                }
                else
                {
                    // This causes a crash... perhaps it gets deleted on its own somewhere else. Check if it causes a memory leak
                    // delete removedObj;
                    removedObj = nullptr;
                }
            }

            it->setAppplied(false);
        }

        // (Re)Apply the hazards
        auto loadPatterns = it->getTheLoadPatterns();

        for(auto&& loadPattern : loadPatterns)
        {
            auto lp = loadPattern->getTheLoadPattern();

            if(lp == nullptr)
            {
                qCritical()<<"Error: could not get the load pattern from "<<it->objectName()<<" in "<<__FUNCTION__;
                return false;
            }

            auto res = theOpenSeesDomain->addLoadPattern(lp);

            if(res == false)
            {
                qCritical()<<"Error: in appplying the load pattern in the OpenSees domain "<<it->objectName()<<" in "<<__FUNCTION__;
                return false;
            }

        }

        it->setAppplied(true);

        this->autoSetTransientAnalysisParameters(loadPatterns);

        hazardsApplied = true;
    }


    return hazardsApplied;
}



QList<RParameter *> RStructuralAnalysisModel::getAllParametersList(bool *ok)
{
    theAllParametersList.clear();

    // Creating a list of parameters to return at the end
    QList<RParameter *> theParameterList;

    // Creating a list of all parameters, for reference, for later
    QList<RParameter *> theFullParameterList = theDomain->findChildren<RParameter *>();

    //The lines below make the analysis very slow where the model is large
    //Only the models that are input to this one should be picked up here so that they can be evaluated before this one
    /*

#ifdef DETAILEDBUILDINGS
    // Loop over all Components (at this location... :) to see if they have decision variables, random variables, etc.
    QList<RComponent *> theComponentList = theDomain->findChildren<RComponent *>();
    int numComponents = theComponentList.count();
    for (int i=0; i<numComponents; i++)
    {
        // First convey the list provided by the developer of the component
        theParameterList << theComponentList[i]->getParameterList();

        // Looping over QProperties
        for (int j=0; j<theComponentList[i]->metaObject()->propertyCount(); j++) {

            // Getting the meta property
            QMetaProperty metaProperty = theComponentList[i]->metaObject()->property(j);

            // Getting the tpe of the meta property
            QVariant::Type type = metaProperty.type();

            // Getting the name of the property
            const char * charName = metaProperty.name();


            if (type == QMetaType::QObjectStar) {

                // Getting the vaue of the property, which is a "QObject *"
                QObject *propertyObject = theComponentList[i]->property(charName).value<QObject *>();


                // Down-casting a pointer with the type "RParameter" to the "QObject *"
                RParameter *parameter = qobject_cast<RParameter *>(propertyObject);

                // If the casting succeeds, it mean that the type of this pointer is RParameter. Proceeding to add it to the list.
                if (parameter) {

                    //qDebug() << "This is what I found:" << parameter;

                    // If the parameter is not already in the list ...
                    if (!theParameterList.contains(parameter)) {

                        // Down-casting a pointer with the type "RResponse"
                        RResponse *response = qobject_cast<RResponse *>(parameter);

                        // If the type of this parameter is RResponse, it should be checked hat this RResponse is not an OUTPUT to this model.
                        // Thus, if the casting to RResponse succeeds, the "outputResponseList" is searched for this RResponse.
                        // If this RResponse was not found in "outputResponseList", OR this parameter is not an RResponse at all ("response" is NULL), then this RParameter pointer is added to the parmeter list.
                        if (((response) && (!theResponseList.contains(response))) || (!response)) {
                            theParameterList << parameter;
                        }
                    }
                }

                continue;
            }

            if (type == QMetaType::QString) {

                RParameter *parameter = 0;


                // Loop over all paramters to see if this string is indeed a parameter
                for (int m=0; m<theFullParameterList.count(); m++) {

                    if (theComponentList[i]->property(charName).toString() == theFullParameterList[m]->objectName()) {

                        parameter = theFullParameterList[m];

                    }

                }


                // If the casting succeeds, it means that the type of this pointer is RParameter. Proceeding to add it to the list.
                if (parameter) {

                    // If the parameter is not already included in "theParameterList", then add it.
                    if (!theParameterList.contains(parameter)) {
                        theParameterList << parameter;
                    }
                }

                continue;
            }
        }
    }
#endif


    // Now loop over the Points too (at this location :) because they may also have random variables or decision variables as input
    QList<RPoint *> thePointList = theDomain->findChildren<RPoint *>();
    int numPoints = thePointList.count();
    for (int i=0; i<numPoints; i++)
    {



        // Looping over QProperties
        for (int j=0; j<thePointList[i]->metaObject()->propertyCount(); j++) {

            // Getting the meta property
            QMetaProperty metaProperty = thePointList[i]->metaObject()->property(j);

            // Getting the tpe of the meta property
            QVariant::Type type = metaProperty.type();

            // Getting the name of the property
            const char * charName = metaProperty.name();


            if (type == QMetaType::QObjectStar) {

                // Getting the vaue of the property, which is a "QObject *"
                QObject *propertyObject = thePointList[i]->property(charName).value<QObject *>();

                // Down-casting a pointer with the type "RParameter" to the "QObject *"
                RParameter *parameter = qobject_cast<RParameter *>(propertyObject);

                // If the casting succeeds, it mean that the type of this pointer is RParameter. Proceeding to add it to the list.
                if (parameter) {

                    // If the parameter is not already in the list ...
                    if (!theParameterList.contains(parameter)) {

                        // Down-casting a pointer with the type "RResponse"
                        RResponse *response = qobject_cast<RResponse *>(parameter);

                        // If the type of this parameter is RResponse, it should be checked hat this RResponse is not an OUTPUT to this model.
                        // Thus, if the casting to RResponse succeeds, the "outputResponseList" is searched for this RResponse.
                        // If this RResponse was not found in "outputResponseList", OR this parameter is not an RResponse at all ("response" is NULL), then this RParameter pointer is added to the parmeter list.
                        if (((response) && (!theResponseList.contains(response))) || (!response)) {
                            theParameterList << parameter;
                        }
                    }
                }

                continue;
            }

            if (type == QMetaType::QString) {

                RParameter *parameter = 0;


                // Loop over all paramters to see if this string is indeed a parameter
                for (int m=0; m<theFullParameterList.count(); m++) {

                    if (thePointList[i]->property(charName).toString() == theFullParameterList[m]->objectName()) {

                        parameter = theFullParameterList[m];

                    }

                }


                // If the casting succeeds, it means that the type of this pointer is RParameter. Proceeding to add it to the list.
                if (parameter) {

                    // If the parameter is not already included in "theParameterList", then add it.
                    if (!theParameterList.contains(parameter)) {
                        theParameterList << parameter;
                    }
                }

                continue;
            }
        }
    }
*/

    //Add the hazard response list from this model
    for(auto&& it: theHazardList)
    {
        // for(auto&& it2 : it->getResponseList())
        //     theParameterList << it2;

        auto theHazModelResponse = theDomain->findChild<RResponse*>(it->objectName()+"Response");
        if(theHazModelResponse)
            theParameterList<<theHazModelResponse;
        else
            qDebug()<<"Warning, could not find the response from the hazard model in "<<this->objectName()<<" in "<<__FUNCTION__;
    }


    // Sorting the generated list
    theDomain->sort(&theParameterList);

    // Assigning to the data member
    theAllParametersList = theParameterList;

    *ok = true;

    return theAllParametersList;

}

// Get the number of steps and dT from the load pattern
// Use the smallest number of steps and the smalleset dt from all patterns
void RStructuralAnalysisModel::autoSetTransientAnalysisParameters(QList<QPointer<RLoadPattern> >& loadPatternList)
{
    auto dynamicStructuralAnalysis = qobject_cast<RInelasticDynamicStructuralAnalysis*>(theStructuralAnalysis);

    //Check to see if the Number of time steps and the time set were not set by the user, and that there is a dynamic analysis
    if(!dynamicStructuralAnalysis || loadPatternList.empty())
        return;


    auto numSteps = 1e6;
    auto timeStep = 1.0e6;

    for(auto&& it:loadPatternList)
    {
        auto uniformExcitation = qobject_cast<RUniformExcitationPattern*>(it);
        if(uniformExcitation)
        {
            auto timeSeries = qobject_cast<ROpenSeesTimeSeries*>(uniformExcitation->getTheTimeSeries());
            if(timeSeries)
            {
                auto nSteps = timeSeries->getNumberSteps();
                auto tStep = timeSeries->getTimeStep();

                if(nSteps <= numSteps)
                    numSteps = nSteps;

                if(tStep <= timeStep)
                    timeStep = tStep;
            }
        }
    }

    auto setTimeStep = dynamicStructuralAnalysis->getTimeStep();

    if(setTimeStep < timeStep)
    {
        numSteps *= timeStep/setTimeStep;
    }

    if(setTimeStep == 0.0)
    {
        dynamicStructuralAnalysis->setTimeStep(timeStep);
    }
    if(dynamicStructuralAnalysis->getNumSteps()<1)
    {
        dynamicStructuralAnalysis->setNumSteps(numSteps);
    }
}

// Estimate the period according to the building height
double RStructuralAnalysisModel::getPeriod(void)
{
    if(theBIM)
        return theBIM->getNumberOfStoreys()*0.10;
    else
        return 0.1;
}

