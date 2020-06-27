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
*   - Mojtaba Mahsuli                                                *
*   - Stevan Gavrilovic                                              *
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RDomain.h"
#include "RMainWindow.h"
#include "RObject.h"

// PARAMETERS
#include "RLocation.h"
#include "RConstant.h"
#include "RResponse.h"
#include "RFileResponse.h"
#include "RCommandResponse.h"
#include "RGenericResponse.h"
#include "RLocationResponse.h"
#include "RTimeSeriesResponse.h"
#include "RDecisionVariable.h"
#include "RContinuousDecisionVariable.h"
#include "RRandomVariable.h"
#include "RContinuousRandomVariable.h"
#include "RCorrelation.h"
#include "RTime.h"
#include "RTimeSeries.h"
#include "RInfoRichResponse.h"

// METHODS
#include "RCholeskyLinearSolver.h"
#include "RGaussJordanLinearSolver.h"
#include "RLULinearSolver.h"
#include "RQRLinearSolver.h"
#include "RMatrixInverseLinearSolver.h"
#include "RInHouseLinearSolver.h"
#include "RInHouseProbabilityDistributions.h"
#include "RBasicCholeskyDecomposition.h"
#include "RNatafProbabilityTransformation.h"
#include "RIndependenceProbabilityTransformation.h"
#include "RInHouseRandomNumberGenerator.h"
#include "RHLRFSearchDirection.h"
#include "RSteepestDescentSearchDirection.h"
#include "RQuasiNewtonSearchDirection.h"
#include "RComboSearchDirection.h"
#include "RConjugateGradientSearchDirection.h"
#include "RADKZhangMeritFunction.h"
#include "RBasicMeritFunction.h"
#include "RDummyMeritFunction.h"
#include "RObjectiveMeritFunction.h"
#include "RStandardFORMConvergenceCheck.h"
#include "RGradientNormConvergenceCheck.h"
#include "RFixedLineSearchAlgorithm.h"
#include "RBisectionLineSearchAlgorithm.h"
#include "RNewtonLineSearchAlgorithm.h"
#include "RSecantLineSearchAlgorithm.h"
#include "RGoldenSectionLineSearchAlgorithm.h"
#include "RArmijoLineSearchAlgorithm.h"
#include "RPlottingLineSearchAlgorithm.h"
#include "RDirectionalLineSearchOptimizationAlgorithm.h"
#include "RDownhillSimplexOptimizationAlgorithm.h"

// MODELS
#include "RModel.h"
#include "RAlgebraicExpressionModel.h"
#include "RHardcodedExpressionModel.h"
#include "RFORMModel.h"
#include "RSOSMModel.h"
#include "RFOSMModel.h"
#include "ROptimizationModel.h"
#include "RRiskModel.h"
#include "RSamplingModel.h"
#include "RScriptModel.h"
#include "R1DOptimizationModel.h"
#include "RRootFindingModel.h"
#include "RRandomVariableWithRandomParametersModel.h"
#include "RScenarioModel.h"


#ifdef HAZARDS
#include "HazardIncludes.h"
#endif

#ifdef DETAILEDBUILDINGS
#include "DetailedBuildingsIncludes.h"
#include "RStatistics.h"
#endif

#ifdef SIMPLEBUILDINGS
#include "SimpleBuildingIncludes.h"
#endif

#ifdef NETWORKS
#include "NetworkIncludes.h"
#endif

#ifdef GSL
#include "GSLIncludes.h"
#endif

#ifdef OPENSEES
#include "OpenSeesIncludes.h"
#endif

#include <QMessageBox>
#include <QMetaProperty>
#include <QDir>

RConstant* theGlobalMeshOption = nullptr;
RTime* theCurrentTime = nullptr;

RDomain::RDomain(QObject *parent, QString name)
    : RObject(parent, name)
{
    idTag = 0;
    theLastAddedObject = nullptr;
    isInputFileBeingOpened = false;

    registerHeadersAsClasses();

#ifdef DETAILEDBUILDINGS
    // Create the global mesh option
    theGlobalMeshOption = qobject_cast<RConstant*>(factory.instantiate("RConstant",this,"officialMeshOption"));
    theGlobalMeshOption->setCurrentValue(1.0);
    theStatsMethod = make_unique<RStatistics>();
#endif

    // Create the global time object
    theCurrentTime = qobject_cast<RTime*>(factory.instantiate("RTime",this,"officialCurrentTime"));

#ifdef OPENSEES
    theOpenSeesDomain = nullptr;
#endif

}


RDomain::~RDomain()
{

}


QStringList RDomain::createObjectTreeBranches(QString passedClassName)
{
    QStringList result;
    QString className = passedClassName.remove(' ');

    if (className == QString("Objects").remove(' ')) {
        result << "MODELS"
               << "PARAMETERS"
               << "METHODS";
    }
    else if (className == QString("PARAMETERS").remove(' ')) {
        result << "Constants"
               << "Random Variables"
               << "Design Variables"
               << "Correlation"
               << "Locations"
               << "Generic Responses"
               << "File Responses"
               << "Command Responses"
               << "Location Responses"
               << "Time Series Responses"
               << "Information Rich Responses"
               << "Time Instants";

    }
    else if (className == QString("METHODS").remove(' ')) {
        result << "Linear Solvers"
               << "Eigen Solvers"
               << "Cholesky Decompositions"
               << "Search Directions"
               << "Optimization Algorithms"
               << "Direct Search Algorithms"
               << "Line Search Algorithms"
               << "Time Integrators"
               << "Convergence Checks"
               << "Merit Functions"
               << "Probability Distributions"
               << "Probability Transformations"
               << "Random Number Generators";
    }
    else if (className == QString("MODELS").remove(' ')) {
        result << "FORM"
               << "FOSM"
               << "SOSM"
               << "Sampling"
               << "Risk"
               << "Optimization"
               << "Expression"
               << "Script"
               << "OpenSees"
               << "1D Optimization"
               << "Root Finding"
               << "Scenarios"
               << "Random Variable With Random Parameters";
    }
    else if (className == QString("METHODS Linear Solvers").remove(' ')) {
        result << "Cholesky" << "LU" << "Gauss Jordan" << "Matrix Inverse";
    }
    else if (className == QString("METHODS Cholesky Decompositions").remove(' ')) {
        result << "Basic";
    }
    else if (className == QString("METHODS Eigen Solvers").remove(' ')) {
        result << "InHouse" << "QR";
    }
    else if (className == QString("METHODS Time Integrators").remove(' ')) {
        result << "Newmark" << "HHT";
    }
    else if (className == QString("METHODS Convergence Checks").remove(' ')) {
        result << "Standard FORM" << "Gradient Norm";
    }
    else if (className == QString("METHODS Merit Functions").remove(' ')) {
        result << "Basic" << "ADK Zhang" << "Objective" << "Dummy";
    }
    else if (className == QString("METHODS Probability Distributions").remove(' ')) {
        result << "InHouse";
    }
    else if (className == QString("METHODS Probability Transformations").remove(' ')) {
        result << "Independence" << "Nataf";
    }
    else if (className == QString("METHODS Random Number Generators").remove(' ')) {
        result << "InHouse" << "Default GSL";
    }
    else if (className == QString("METHODS Optimization Algorithms").remove(' ')) {
        result << "Directional Line Search" << "Downhill Simplex";
    }
    else if (className == QString("METHODS Search Directions").remove(' ')) {
        result << "HLRF" << "Steepest Descent" << "Quasi Newton" << "Conjugate Gradient" << "Combo";
    }
    else if (className == QString("METHODS Line Search Algorithms").remove(' ')) {
        result << "Fixed" << "Armijo" << "Golden Section" << "Newton" << "Secant" << "Bisection" << "Steffenson"<< "False Position" << "Brent Dekker" << "Armijo" << "Plotting";
    }

#ifdef DETAILEDBUILDINGS
#include "DetailedBuildingsMenu.h"
#endif

#ifdef SIMPLEBUILDINGS
#include "SimpleBuildingMenu.h"
#endif

#ifdef NETWORKS
#include "NetworkMenu.h"
#endif

#ifdef HAZARDS
#include "HazardMenu.h"
#endif

#ifdef GSL
#include "GSLMenu.h"
#endif

#ifdef OPENSEES
#include "OpenSeesMenu.h"
#endif

    return result;
}


int RDomain::createObject(QString name, QString passedClassName, bool isAutoGenerated)
{  
    int namingResult = 0;
    if (!isInputFileBeingOpened) {
        namingResult = nameIsValid(name);
    }

    if(name == "officialMeshOption")
    {
        if(theGlobalMeshOption)
        {
            delete theGlobalMeshOption;
            theGlobalMeshOption = nullptr;
        }
        theGlobalMeshOption = qobject_cast<RConstant*>(factory.instantiate("RConstant",this,name));
        theLastAddedObject = theGlobalMeshOption;
        return 0;
    }
    else if(name == "officialCurrentTime")
    {
        if(theCurrentTime)
        {
            delete theCurrentTime;
            theCurrentTime = nullptr;
        }
        theCurrentTime = qobject_cast<RTime*>(factory.instantiate("RTime",this,name));
        theLastAddedObject = theCurrentTime;
        return 0;
    }

    QString className = passedClassName;
    if (namingResult == 0) {

        theLastAddedObject = factory.instantiate(className.toStdString(),this,name);

        if (theLastAddedObject) {

            RObject *rObject = qobject_cast<RObject *>(theLastAddedObject);
            rObject->setAutoGenerated(isAutoGenerated);
            rObject->setObjectTag(idTag);
            ++idTag;

        } else {

            qCritical()<<"Could not find class "<< passedClassName <<" in RDomain; check to see if it was added to the registry";
            return -1;
        }

        return 0;
    }
    else if (namingResult == -1) {

        // The name already exists in the domain.
        QString newName = name + "_New";
        //QMessageBox::warning(0, tr("Warning"), QString("An object with the name \"%1\" already exists. Rts will use the name \"%2\" instead.").arg(name).arg(newName), QMessageBox::Ok | QMessageBox::Default);
        qCritical() << "Warning: An object with the name" << name << "already exists. Rts will use the name" << newName << "instead.";
        int result = createObject(newName, className);
        if (result < 0 ) {
            return -1;
        }
        return 1;
    }
    else if (namingResult < -1) {

        // The name contains illegal characters.
        return -1;
    }
    else return -1;
}


QObjectList RDomain::getObjectList(QString passedClassName)
{
    //Dont remove the spaces, makes it more readable when you add the key to the registry
    QString className = passedClassName;
    QObjectList result;

    QList<RObject*> factoryObjectList = QList<RObject*>::fromStdList(factory.getObjects.getObjectList(className.toStdString(),this));

    if(!factoryObjectList.empty())
        for (int i = 0; i < factoryObjectList.count(); i++) {
            result << factoryObjectList[i];
        }

    // CLASSES NOT APPEARING IN THE OBJECTS PANE, BUT THAT WE MAY WANT TO CREATE LISTS OF... THE PARENT CLASSES OTHERS ARE SUBCLASSED FROM
    if (className == QString("Parameter").remove(' ')) {
        QList<RParameter *> objectList = this->findChildren<RParameter *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    else if (className == QString("Random Variable").remove(' ')) {
        QList<RRandomVariable *> objectList = this->findChildren<RRandomVariable *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    else if (className == QString("Design Variable").remove(' ')) {
        QList<RDecisionVariable *> objectList = this->findChildren<RDecisionVariable *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    else if (className == QString("Model").remove(' ')) {
        QList<RModel *> objectList = this->findChildren<RModel *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    else if (className == QString("Response").remove(' ')) {
        QList<RResponse *> objectList = this->findChildren<RResponse *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    else if (className == QString("Search Directions").remove(' ')) {
        QList<RSearchDirection *> objectList = this->findChildren<RSearchDirection *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    else if (className == QString("Probability Transformations").remove(' ')) {
        QList<RProbabilityTransformation *> objectList = this->findChildren<RProbabilityTransformation *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    else if (className == QString("Convergence Checks").remove(' ')) {
        QList<RConvergenceCheck *> objectList = this->findChildren<RConvergenceCheck *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    else if (className == QString("Merit Functions").remove(' ')) {
        QList<RMeritFunction *> objectList = this->findChildren<RMeritFunction *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    else if (className == QString("Linear Solvers").remove(' ')) {
        QList<RLinearSolver *> objectList = this->findChildren<RLinearSolver *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }

    sort(&result);
    return result;
}


void RDomain::updateObjectTree()
{
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(this->parent());
    mainWindow->updateTree("PARAMETERS");
}


QStringList RDomain::getDynamicPropertyList()
{
    QStringList result;
    result << "PerturbationFactor" << "EfficientPerturbation" << "XPoints" << "PDFPoints";
    return result;
}


QStringList RDomain::getPropertyStringList(QString propertyName)
{
    // Here we tell the user which parameters are available when he/she needs to enter a parameter in the Properties Pane
    // What is returned here must match one of the items in ::getObjectList above

    QStringList parameterPropertyList;

    parameterPropertyList << "Mean" << "StandardDeviation" << "CoefficientOfVariation" << "InitialValue" << "UpperBound" << "LowerBound" << "Parameter1" << "Parameter2" << "Parameter3" << "Parameter4" << "IncrementalCost" << "Magnitude";

    if (parameterPropertyList.contains(propertyName)) {
        return getStringList(getObjectList("Parameter"));
    }
    else if (propertyName.contains("RandomVariable")) {
        return getStringList(getObjectList("Random Variables Continuous"));
    }
    else if (propertyName == QString("Location")) {
        return getStringList(getObjectList("Map Locations"));
    }
    else if (propertyName == QString("Point")) {
        return getStringList(getObjectList("Points"));
    }
    else if (propertyName == QString("DistributionType")) {
        return QStringList() << "Normal (mean, stdv)" << "Exponential (mu, xo)" << "Bounded Exponential (mu, xo, min, max)" << "Linear (a, b)" << "Laplace (a, xo)" << "Rayleigh (sigma, xo)" << "Gamma (a, b)" << "Uniform (a, b)" << "Lognormal (zeta, sigma)" << "Chi-square (nu, xo)" << "t-distribution (nu, xo)" << "Beta (a, b, min, max)" << "Logistic (a, xo)" << "Weibull (a, b)" << "Gumbel (a, b)" << "User Defined";
    }
    else if (propertyName == QString("Model")) {
        return getStringList(getObjectList("Model"));
    }
    else if (propertyName == QString("Function") || propertyName == QString("LimitStateFunction")) {
        return getStringList(getObjectList("Function"));
    }
    else if (propertyName == QString("TimeParameter")) {
        return getStringList(getObjectList("Times"));
    }
    else if (propertyName == QString("RandomNumberGenerator")) {
        return getStringList(getObjectList("Random Number Generators"));
    }
    else if (propertyName == QString("SearchDirection")) {
        return getStringList(getObjectList("Search Directions"));
    }
    else if (propertyName == QString("ProbabilityTransformation")) {
        return getStringList(getObjectList("Probability Transformations"));
    }
    else if (propertyName == QString("LinearSolver")) {
        return getStringList(getObjectList("Linear Solvers"));
    }
    else if (propertyName == QString("ConvergenceCheck")) {
        return getStringList(getObjectList("Convergence Checks"));
    }
    else if (propertyName == QString("MeritFunction")) {
        return getStringList(getObjectList("Merit Functions"));
    }
    else {
        return QStringList();
    }
}


QObject * RDomain::getLastAddedObject() const
{
    return theLastAddedObject;
}


QString RDomain::getProperty(const QObject *passedObject, QString propertyName) const
{
    QByteArray byteArrayName = propertyName.toLatin1();
    const char * charName = byteArrayName.data();
    int index = passedObject->metaObject()->indexOfProperty(charName);
    QVariant valueVariant;
    if (index >= 0) {
        QMetaProperty metaProperty = passedObject->metaObject()->property(index);
        QVariant::Type type = metaProperty.type();

        valueVariant = QVariant::fromValue(passedObject->property(charName));
        if (metaProperty.isEnumType()) {
            valueVariant = metaProperty.enumerator().valueToKey(valueVariant.toInt());
        }
        //const char * typeName = passedObject->metaObject()->property(i).typeName();

        if (type == QVariant::Invalid) {
            valueVariant = "";
        } else if (type == QMetaType::QObjectStar) {
            QObject *propertyObject = passedObject->property(charName).value<QObject *>();
            if (propertyObject) {
                valueVariant = propertyObject->objectName();
            } else {
                valueVariant = "";
            }
            //const char * propertyType = QVariant::fromValue(propertyObject).typeName();
        } else if (type == QVariant::Double) {
            if (!metaProperty.isWritable()) {
                bool ok;
                double tempDouble = valueVariant.toDouble(&ok);
                if (ok) {
                    QString tempString = QString("%1").arg(tempDouble, 0, 'g', 7);
                    valueVariant = tempString;
                }
            } else {
                bool ok;
                double tempDouble = valueVariant.toDouble(&ok);
                if (ok) {
                    QString tempString = QString("%1").arg(tempDouble, 0, 'g', 13);
                    valueVariant = tempString;
                }
            }
        }
    } else {
        valueVariant = passedObject->property(charName);

        if (valueVariant.type() == QVariant::Invalid) {
            return "No such property";
        }
    }

    QString result = valueVariant.toString();
    return result;
}


int RDomain::setProperty(QObject *passedObject, QString propertyName, QString propertyValue)
{
    QByteArray byteArrayName = propertyName.toLatin1();
    const char * charName = byteArrayName.data();
    int index = passedObject->metaObject()->indexOfProperty(charName);
    QVariant::Type type = passedObject->metaObject()->property(index).type();

    bool isStaticProperty = false;

    if (type == QMetaType::QObjectStar) {
        QObject *propertyObject = findChild<QObject *>(propertyValue);
        if (propertyObject) {
            isStaticProperty = passedObject->setProperty(charName, QVariant::fromValue(propertyObject));
        } else {
            if (!propertyValue.isEmpty()) {
                qCritical() << "Error in" << passedObject->objectName() << ": Could not find an object named" << propertyValue << "in the domain.";
                return 0;
            }
            QObject *nullPointer = nullptr;
            isStaticProperty = passedObject->setProperty(charName, qVariantFromValue(nullPointer));
        }
    } else {
        if (propertyName == QString("objectName")) {
            // Updating the object pane if the objectName has been changed.
            int result = renameObject(passedObject, propertyValue);
            if (result == 0) {
                // If the renaming was successful, then 1 is returned, so that the property widget sends a command to the tree widget to refresh the tree
                return 1;
            }
        } else {
            QMetaProperty metaProperty = passedObject->metaObject()->property(index);
            if (metaProperty.isEnumType()) {
                int enumValue = metaProperty.enumerator().keyToValue(propertyValue.toLatin1());
                if (enumValue < 0) {
                    qCritical() << "Error: Could not set the property" << propertyName << "in the object" << passedObject->objectName() << ". The value" << propertyValue << "is invalid.";
                }
            }
            isStaticProperty = passedObject->setProperty(charName, QVariant::fromValue(propertyValue));
        }
    }
    if (isStaticProperty) {
        return 0;
    } else {
        // Returns -1 so that the main window knows that a property with this name is not a static property
        return -1;
    }
}


int RDomain::nameIsValid(QString name, bool checkForDuplicate)
{
    QRegExp regExp("[A-Za-z0-9]+[A-Za-z0-9_{}\/\"-:]*");  // Steve add - allow uid hash to be recognized as a domain name
    if (!regExp.exactMatch(name)) {
        QMessageBox::critical(0, tr("Error"), QString(tr("The object name \"%1\" is unacceptable. It should start with a letter and contain no spaces.")).arg(name), QMessageBox::Ok | QMessageBox::Default);
        return -2;
    }

    if (checkForDuplicate) {
        // ATTENTION: THE FOLLOWING LINE MAKES THE PROCESS OF OPENING LARGE INPUT FILES VERY LONG
        QObject *theObject = this->findChild<QObject *>(name);
        if (theObject) {
            return -1;
        }
    }

    return 0;
}


void RDomain::setInputFileBeingOpened(bool value)
{
    isInputFileBeingOpened = value;
}


int RDomain::deleteObject(QString name)
{
    QObject *object = this->findChild<QObject *>(name);
    if (object) {

        bool isParameter =qobject_cast<RParameter*>(object);

        disconnect(object, nullptr, nullptr, nullptr);
        delete object;
        object = nullptr;

        // Make sure object trees are updated (notice the name is no longer used)
        //Also notice how it will now check if it is a parameter first before updating the tree
        //updating the tree takes a lot of time, this will makre sure that does not happen for everything that gets deleted
        if(isParameter){
            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(this->parent());
            if(mainWindow)
                mainWindow->updateTree("PARAMETERS");
        }

        return 0;
    } else {
        return -1;
    }
}


int RDomain::renameObject(QObject *passedObject, QString name)
{
    int result = nameIsValid(name);
    if (result == 0) {
        // The name is fine.
        QString oldName = passedObject->objectName();
        RObject *rObject = qobject_cast<RObject *>(passedObject);
        rObject->setObjectName(name);
        return 0;
    } else if (result == -1) {
        // The name already exists in the domain.
        QMessageBox::critical(nullptr, tr("Error"), QString("Could not rename the object. An object with the name \"%1\" already exists.").arg(name), QMessageBox::Ok | QMessageBox::Default);
        return -1;
    } else if (result < -1) {
        // The name contains illegal characters.
        return -1;
    }

    return 0;
}


void RDomain::print(QVector< QVector<double> > *matrix, QString outputFile)
{
    int row = matrix->size();
    int column = matrix[0][0].size();

    QString startAndEndChar = "|";
    //QString startAndEndChar = QString(QChar(0x2502));  // Unicode number of the graphic sign

    QFile *file = nullptr;
    if (!outputFile.isNull()) {
        file = new QFile(outputFile);
        if (!file) {
            return;
        }
        if (file->exists()) {
            file->remove();
        }
        if (!file->open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical() << "Warning: Could not create the file" << outputFile;
            return;
        }
        startAndEndChar.clear();
    }

    for (int i = 0; i < row; i++) {
        QString line = "";
        line += startAndEndChar;
        for (int j = 0; j < column; j++) {
            if (file) {
                line += QString("%1").arg((*matrix)[i][j], 0, 'g', 12);
            } else {
                // Is this enough precession?
                line += QString("%1").arg((*matrix)[i][j], 12, 'g', -1);
            }
            line += " ";
        }
        line += startAndEndChar;
        if (file) {
            line += "\n";
            QByteArray byteArray;
            byteArray.append(line);
            file->write(byteArray);
        } else {
            qDebug() << qPrintable(line);
        }
    }

    if (file) {
        file->flush();
        file->close();
        delete file;
        file = nullptr;
    }
}


void RDomain::print(QVector<double> *vector, QString outputFile)
{
    // Get the length of the vector
    int theSize = vector->size();

    // Create a temporary matrix to store the vector, so we can use the other ::print method
    QVector< QVector<double> > *tempmatrix = new QVector< QVector<double> >(theSize, QVector<double>(1));

    // Copy the vector into that temporary matrix
    for (int i=0; i<theSize; i++) {
        (*tempmatrix)[i][0] = vector->at(i);
    }

    // Call the other ::print method
    print(tempmatrix, outputFile);

    // Delete the temporary matrix
    delete tempmatrix;
}


int RDomain::readMatrixFromFile(QVector< QVector<double> > *matrix, QString inputFile)
{
    if (!matrix) {
        qCritical() << "Error: The passed matrix pointer is null";
        return -1;
    }
    int rowCount = matrix->size();
    int colCount = matrix[0][0].size();


    // Read the file here
    QFile file(inputFile);
    if (!file.open(QFile::ReadOnly)) {
        // Error
        qCritical() << "Error in RDomain: Could not open the file" << inputFile;
        return -1;
    }

    // Read the file content into a QString
    QTextStream stream(&file);
    stream.seek(0);
    //QString fileContentString = in.readAll();

    // Tokenize the QString of file contents by "\n" and create a string list of input file lines
    QString verticalRegExpString = QString("[n\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator));
    QRegExp verticalRegExp = QRegExp(verticalRegExpString);
    //QStringList fileContentStringList = fileContentString.split(verticalRegExp, QString::SkipEmptyParts);

    QStringList fileContentStringList;
    QString line;
    QStringList lineList;
    stream.setAutoDetectUnicode(true);
    do {
        line = stream.readLine();
        if (!line.isEmpty()) {
            lineList = line.split(verticalRegExp, QString::SkipEmptyParts);
            fileContentStringList.append(lineList);
        }
        //QCoreApplication::processEvents();
    } while (!line.isNull());

    int fileRowCount = fileContentStringList.count();

    if (fileRowCount != rowCount) {
        qCritical() << "Error: The number of rows in the file" << inputFile << "does not match the number of rows of the passed matrix/vector";
        return -1;
    }

    QRegExp regExp = QRegExp("[ ,;\t]");
    int fileColCount = 0;

    for (int i = 0; i < fileContentStringList.count(); i++) {
        QStringList valueList = fileContentStringList[i].split(regExp, QString::SkipEmptyParts);

        if (i == 0) {
            fileColCount = valueList.count();
            if (fileColCount != colCount) {
                qCritical() << "Error: The number of columns in the file" << inputFile << "does not match the number of columns of the passed matrix/vector";
                return -1;
            }
        } else {
            if (fileColCount != valueList.count()) {
                qCritical() << "Error: The number of data at the line" << i + 1 << "is different than the line 1 in the file" << inputFile;
                return -1;
            }
        }

        for (int j = 0; j < valueList.count(); j++) {
            QString tempStr = valueList[j].trimmed();
            bool ok;
            double tempValue = tempStr.toDouble(&ok);
            if (!ok) {
                qCritical() << "Error: Could not read the value number" << j + 1 << "at the line" << i + 1 << "in the file" << inputFile;
                return -1;
            }

            (*matrix)[i][j] = tempValue;
        }
    }
    file.close();
    return 0;
}


int RDomain::readVectorFromFile(QVector<double> *vector, QString inputFile)
{
    if (!vector) {
        qCritical() << "Error: The passed vector pointer is null";
        return -1;
    }

    // Create a temporary matrix so that the "read matrix from file" method can be used, to avoid code duplication
    int a = vector->size();
    QVector< QVector<double> > *tempMatrix = new QVector< QVector<double> >(a, QVector<double>(1));
    int result = readMatrixFromFile(tempMatrix, inputFile);
    if (result < 0) {
        return -1;
    }

    // Now revert back to a vector again, by picking up the first column of the matrix
    for (int i=0; i<tempMatrix->size(); i++) {
        (*vector)[i] = (*tempMatrix)[i][0];
    }

    return 0;
}


QStringList RDomain::getStringList(QObjectList objectList)
{
    QStringList result;
    for (int i = 0; i < objectList.count(); i++) {
        result << objectList[i]->objectName();
    }
    return result;
}


int RDomain::sort(QObjectList *passedList)
{
    QObjectList searchList = *passedList;

    int beforeCount = searchList.count();
    QStringList nameList;
    QMap<QString, QObject *> map;
    for (int i = 0; i < searchList.count(); i++) {
        map[searchList[i]->objectName()] = searchList[i];
    }
    *passedList = map.values();
    int afterCount = map.count();
    if (afterCount < beforeCount) {
        // means a duplicated name has been removed
        return -1;
    } else {
        return 0;
    }

    /*bool duplicate = false;
    double count = passedList->count();
    bool swapped = true;
    while (swapped) {
    swapped = false;
    count--;
    for (int i = 0; i < count; i++) {
    QObject *first = passedList->value(i);
    QObject *second = passedList->value(i+1);
    QString name1 = first->objectName();
    QString name2 = second->objectName();
    if (name1 > name2) {
    passedList->replace(i, second);
    passedList->replace(i + 1, first);
    swapped = true;
    } else if (name1 == name2) {
    duplicate = true;
    }
    }
    }
    if (duplicate) {
    // means there is a duplicated name
    return -1;
    } else {
    return 0;
    }*/

    /*for (int i = 0; i < searchList.count(); i++) {
    nameList << searchList[i]->objectName();
    }
    qSort(nameList.begin(), nameList.end());
    passedList->clear();
    for (int i = 0; i < nameList.count(); i++) {
    for (int j = 0; j < searchList.count(); j++) {
    if (searchList[j]->objectName() == nameList[i]) {
    passedList->append(searchList[j]);
    searchList.removeAt(j);
    continue;
    }
    }
    }*/
}


int RDomain::sort(QList<RModel *> *passedList)
{
    QObjectList objectList;
    for (int i = 0; i < passedList->count(); i++) {
        QObject *qObject = passedList->value(i);
        objectList << qObject;
    }
    int result = sort(&objectList);
    passedList->clear();
    for (int i = 0; i < objectList.count(); i++) {
        RModel *object = qobject_cast<RModel *>(objectList[i]);
        passedList->append(object);
    }

    return result;
}


int RDomain::sort(QList<RParameter *> *passedList)
{
    QObjectList objectList;
    for (int i = 0; i < passedList->count(); i++) {
        QObject *qObject = passedList->value(i);
        objectList << qObject;
    }
    int result = sort(&objectList);
    passedList->clear();
    for (int i = 0; i < objectList.count(); i++) {
        RParameter *object = qobject_cast<RParameter *>(objectList[i]);
        passedList->append(object);
    }

    return result;
}


int RDomain::sort(QList<RRandomVariable *> *passedList, QString sortingMeasure)
{
    int result = 0;
    if (sortingMeasure == QString("Name")) {
        QObjectList objectList;
        for (int i = 0; i < passedList->count(); i++) {
            QObject *qObject = passedList->value(i);
            objectList << qObject;
        }
        int result = sort(&objectList);
        passedList->clear();
        for (int i = 0; i < objectList.count(); i++) {
            RRandomVariable *object = qobject_cast<RRandomVariable *>(objectList[i]);
            passedList->append(object);
        }
    } else {
        int count = passedList->count();
        bool swapped = true;
        while (swapped) {
            swapped = false;
            count--;
            for (int i = 0; i < count; i++) {
                RContinuousRandomVariable *firstVariable = qobject_cast<RContinuousRandomVariable *>(passedList->value(i));
                RContinuousRandomVariable *secondVariable = qobject_cast<RContinuousRandomVariable *>(passedList->value(i+1));
                double importance1;
                double importance2;
                if (sortingMeasure == QString("Alpha")) {
                    importance1 = firstVariable->getAlphaImportanceMeasure();
                    importance2 = secondVariable->getAlphaImportanceMeasure();
                } else if (sortingMeasure == QString("Gamma")) {
                    importance1 = firstVariable->getGammaImportanceMeasure();
                    importance2 = secondVariable->getGammaImportanceMeasure();
                }

                if (qAbs(importance1) < qAbs(importance2)) {
                    passedList->replace(i, secondVariable);
                    passedList->replace(i + 1, firstVariable);
                    swapped = true;
                }
            }
        }
    }

    return result;
}


int RDomain::sort(QList<RDecisionVariable *> *passedList, QString sortingMeasure)
{
    int result = 0;
    if (sortingMeasure == QString("Name")) {
        QObjectList objectList;
        for (int i = 0; i < passedList->count(); i++) {
            QObject *qObject = passedList->value(i);
            objectList << qObject;
        }
        int result = sort(&objectList);
        passedList->clear();
        for (int i = 0; i < objectList.count(); i++) {
            RDecisionVariable *object = qobject_cast<RDecisionVariable *>(objectList[i]);
            passedList->append(object);
        }
    }
    else if (sortingMeasure == QString("Sensitivity")) {
        int count = passedList->count();
        bool swapped = true;
        while (swapped) {
            swapped = false;
            count--;
            for (int i = 0; i < count; i++) {
                RContinuousDecisionVariable *firstVariable = qobject_cast<RContinuousDecisionVariable *>(passedList->value(i));
                RContinuousDecisionVariable *secondVariable = qobject_cast<RContinuousDecisionVariable *>(passedList->value(i+1));
                double importance1;
                double importance2;
                importance1 = firstVariable->getBetaSensitivityWrtCost();
                importance2 = secondVariable->getBetaSensitivityWrtCost();

                if (importance1 < importance2) {
                    passedList->replace(i, secondVariable);
                    passedList->replace(i + 1, firstVariable);
                    swapped = true;
                }
            }
        }
    }

    return result;
}


int RDomain::sort(QList<RConstant *> *passedList)
{
    QObjectList objectList;
    for (int i = 0; i < passedList->count(); i++) {
        QObject *qObject = passedList->value(i);
        objectList << qObject;
    }
    int result = sort(&objectList);
    passedList->clear();
    for (int i = 0; i < objectList.count(); i++) {
        RConstant *object = qobject_cast<RConstant *>(objectList[i]);
        passedList->append(object);
    }

    return result;
}


int RDomain::sort(QList<RResponse *> *passedList)
{
    QObjectList objectList;
    for (int i = 0; i < passedList->count(); i++) {
        QObject *qObject = passedList->value(i);
        objectList << qObject;
    }
    int result = sort(&objectList);
    passedList->clear();
    for (int i = 0; i < objectList.count(); i++) {
        RResponse *object = qobject_cast<RResponse *>(objectList[i]);
        passedList->append(object);
    }

    return result;
}


QString RDomain::getFileName(QString filePath)
{
    return QFileInfo(filePath).completeBaseName();
}


QString RDomain::getAbsoluteFolderPath(QString filePath)
{
    return QFileInfo(filePath).absolutePath();
}


QString RDomain::stringToFilePath(QString string)
{
    string.remove('"');
    return QFileInfo(string).filePath();
}


QString RDomain::getCurrentFilePath() const
{
    return theCurrentFilePath;
}


void RDomain::setCurrentFilePath(QString value)
{
    theCurrentFilePath = value;

    QString workingPath = theCurrentFilePath;
    bool workingPathIsSet = QDir::setCurrent(workingPath);
}


void RDomain::clearAnalyzedFlags()
{
    QList<RModel *> theModelList = findChildren<RModel *>();
    for (int i=0; i<theModelList.size(); i++) {
        theModelList[i]->isAnalyzed = false;
    }
}


int RDomain::checkAllProperties()
{
    // Notice that this only checks for input that is a QObject...

    QList<RObject *> theObjectList = findChildren<RObject *>();

    bool allGood = true;

    for (int i=0; i<theObjectList.size(); i++) {

        if (theObjectList.at(i)->checkProperties() < 0) {

            allGood = false;
        }
    }

    if (allGood) {
        return 0;
    }
    else {
        return -1;
    }
}


void RDomain::addToDDMList(RParameter* theParameter)
{
    if(theParameter)
    {
        if(!theDomainDDMParameterList.contains(theParameter))
        {
            theDomainDDMParameterList.append(theParameter);
        }
    }
}


QList<RParameter*> RDomain::getDDMParameterList()
{
    return theDomainDDMParameterList;
}


void RDomain::registerHeadersAsClasses()
{


    // *******************************************************************************
    // *******************************************************************************
    //                                     PARAMETERS
    // *******************************************************************************
    // *******************************************************************************

    factory.registerClass<RConstant>("RConstant");
    factory.registerClass<RConstant>("PARAMETERS Constants");
    factory.registerClass<RContinuousRandomVariable>("RContinuousRandomVariable");
    factory.registerClass<RContinuousRandomVariable>("PARAMETERS Random Variables");
    factory.registerClass<RContinuousDecisionVariable>("RContinuousDecisionVariable");
    factory.registerClass<RContinuousDecisionVariable>("PARAMETERS Design Variables");
    factory.registerClass<RCorrelation>("RCorrelation");
    factory.registerClass<RCorrelation>("PARAMETERS Correlation");
    factory.registerClass<RLocation>("RLocation");
    factory.registerClass<RLocation>("PARAMETERS Map Locations");
    factory.registerClass<RTime>("RTime");
    factory.registerClass<RTime>("PARAMETERS Time Instants");
    factory.registerClass<RTimeSeries>("RTimeSeries");
    factory.registerClass<RTimeSeries>("PARAMETERS Time Series");
    factory.registerClass<RGenericResponse>("RGenericResponse");
    factory.registerClass<RGenericResponse>("PARAMETERS Generic Responses");
    factory.registerClass<RFileResponse>("RFileResponse");
    factory.registerClass<RFileResponse>("PARAMETERS File Responses");
    factory.registerClass<RCommandResponse>("RCommandResponse");
    factory.registerClass<RCommandResponse>("PARAMETERS Command Responses");
    factory.registerClass<RLocationResponse>("RLocationResponse");
    factory.registerClass<RLocationResponse>("PARAMETERS Location Responses");
    factory.registerClass<RInfoRichResponse>("RInfoRichResponse");
    factory.registerClass<RInfoRichResponse>("PARAMETERS Information Rich Responses");
    factory.registerClass<RTimeSeriesResponse>("RTimeSeriesResponse");
    factory.registerClass<RTimeSeriesResponse>("PARAMETERS Time Series Responses");

    // *******************************************************************************
    // *******************************************************************************
    //                                     METHODS
    // *******************************************************************************
    // *******************************************************************************

    factory.registerClass<RCholeskyLinearSolver>("RCholeskyLinearSolver");
    factory.registerClass<RCholeskyLinearSolver>("METHODS Linear Solvers Cholesky");
    factory.registerClass<RLULinearSolver>("RLULinearSolver");
    factory.registerClass<RLULinearSolver>("METHODS Linear Solvers LU");
    factory.registerClass<RQRLinearSolver>("RQRLinearSolver");
    factory.registerClass<RQRLinearSolver>("METHODS Linear Solvers QR");
    factory.registerClass<RGaussJordanLinearSolver>("RGaussJordanLinearSolver");
    factory.registerClass<RGaussJordanLinearSolver>("METHODS Linear Solvers Gauss Jordan");
    factory.registerClass<RMatrixInverseLinearSolver>("RMatrixInverseLinearSolver");
    factory.registerClass<RMatrixInverseLinearSolver>("METHODS Linear Solvers Matrix Inverse");
    factory.registerClass<RInHouseLinearSolver>("RInHouseLinearSolver");
    factory.registerClass<RInHouseLinearSolver>("METHODS Linear Solvers InHouse");
    factory.registerClass<RBasicCholeskyDecomposition>("RBasicCholeskyDecomposition");
    factory.registerClass<RBasicCholeskyDecomposition>("METHODS Cholesky Decompositions Basic");
    factory.registerClass<RStandardFORMConvergenceCheck>("RStandardFORMConvergenceCheck");
    factory.registerClass<RStandardFORMConvergenceCheck>("METHODS Convergence Checks Standard FORM");
    factory.registerClass<RGradientNormConvergenceCheck>("RGradientNormConvergenceCheck");
    factory.registerClass<RGradientNormConvergenceCheck>("METHODS Convergence Checks Gradient Norm");
    factory.registerClass<RADKZhangMeritFunction>("RADKZhangMeritFunction");
    factory.registerClass<RADKZhangMeritFunction>("METHODS Merit Functions ADK Zhang");
    factory.registerClass<RBasicMeritFunction>("RBasicMeritFunction");
    factory.registerClass<RBasicMeritFunction>("METHODS Merit Functions Basic");
    factory.registerClass<RDummyMeritFunction>("RDummyMeritFunction");
    factory.registerClass<RDummyMeritFunction>("METHODS Merit Functions Dummy");
    factory.registerClass<RObjectiveMeritFunction>("RObjectiveMeritFunction");
    factory.registerClass<RObjectiveMeritFunction>("METHODS Merit Functions Objective");
    factory.registerClass<RInHouseProbabilityDistributions>("RInHouseProbabilityDistributions");
    factory.registerClass<RInHouseProbabilityDistributions>("METHODS Probability Distributions InHouse");
    factory.registerClass<RIndependenceProbabilityTransformation>("RIndependenceProbabilityTransformation");
    factory.registerClass<RIndependenceProbabilityTransformation>("METHODS Probability Transformations Independence");
    factory.registerClass<RNatafProbabilityTransformation>("RNatafProbabilityTransformation");
    factory.registerClass<RNatafProbabilityTransformation>("METHODS Probability Transformations Nataf");
    factory.registerClass<RInHouseRandomNumberGenerator>("RInHouseRandomNumberGenerator");
    factory.registerClass<RInHouseRandomNumberGenerator>("METHODS Random Number Generators InHouse");
    factory.registerClass<RHLRFSearchDirection>("RHLRFSearchDirection");
    factory.registerClass<RHLRFSearchDirection>("METHODS Search Directions HLRF");
    factory.registerClass<RSteepestDescentSearchDirection>("RSteepestDescentSearchDirection");
    factory.registerClass<RSteepestDescentSearchDirection>("METHODS Search Directions Steepest Descent");
    factory.registerClass<RQuasiNewtonSearchDirection>("RQuasiNewtonSearchDirection");
    factory.registerClass<RQuasiNewtonSearchDirection>("METHODS Search Directions Quasi Newton");
    factory.registerClass<RComboSearchDirection>("RComboSearchDirection");
    factory.registerClass<RComboSearchDirection>("METHODS Search Directions Combo");
    factory.registerClass<RConjugateGradientSearchDirection>("RConjugateGradientSearchDirection");
    factory.registerClass<RConjugateGradientSearchDirection>("METHODS Search Directions Conjugate Gradient");
    factory.registerClass<RFixedLineSearchAlgorithm>("RFixedLineSearchAlgorithm");
    factory.registerClass<RFixedLineSearchAlgorithm>("METHODS Line Search Algorithms Fixed");
    factory.registerClass<RBisectionLineSearchAlgorithm>("RBisectionLineSearchAlgorithm");
    factory.registerClass<RBisectionLineSearchAlgorithm>("METHODS Line Search Algorithms Bisection");
    factory.registerClass<RNewtonLineSearchAlgorithm>("RNewtonLineSearchAlgorithm");
    factory.registerClass<RNewtonLineSearchAlgorithm>("METHODS Line Search Algorithms Newton");
    factory.registerClass<RSecantLineSearchAlgorithm>("RSecantLineSearchAlgorithm");
    factory.registerClass<RSecantLineSearchAlgorithm>("METHODS Line Search Algorithms Secant");
    factory.registerClass<RGoldenSectionLineSearchAlgorithm>("RGoldenSectionLineSearchAlgorithm");
    factory.registerClass<RGoldenSectionLineSearchAlgorithm>("METHODS Line Search Algorithms Golden Section");
    factory.registerClass<RArmijoLineSearchAlgorithm>("RArmijoLineSearchAlgorithm");
    factory.registerClass<RArmijoLineSearchAlgorithm>("METHODS Line Search Algorithms Armijo");
    factory.registerClass<RPlottingLineSearchAlgorithm>("RPlottingLineSearchAlgorithm");
    factory.registerClass<RPlottingLineSearchAlgorithm>("METHODS Line Search Algorithms Plotting");
    factory.registerClass<RDirectionalLineSearchOptimizationAlgorithm>("RDirectionalLineSearchOptimizationAlgorithm");
    factory.registerClass<RDirectionalLineSearchOptimizationAlgorithm>("METHODS Optimization Algorithms Directional Line Search");
    factory.registerClass<RDownhillSimplexOptimizationAlgorithm>("RDownhillSimplexOptimizationAlgorithm");
    factory.registerClass<RDownhillSimplexOptimizationAlgorithm>("METHODS Optimization Algorithms Downhill Simplex");

    // *******************************************************************************
    // *******************************************************************************
    //                                     MODELS
    // *******************************************************************************
    // *******************************************************************************

    factory.registerClass<RFORMModel>("RFORMModel");
    factory.registerClass<RFORMModel>("MODELS FORM");
    factory.registerClass<RFOSMModel>("RFOSMModel");
    factory.registerClass<RFOSMModel>("MODELS FOSM");
    factory.registerClass<RSOSMModel>("RSOSMModel");
    factory.registerClass<RSOSMModel>("MODELS SOSM");
    factory.registerClass<RSamplingModel>("RSamplingModel");
    factory.registerClass<RSamplingModel>("MODELS Sampling");
    factory.registerClass<RRiskModel>("RRiskModel");
    factory.registerClass<RRiskModel>("MODELS Risk");
    factory.registerClass<ROptimizationModel>("ROptimizationModel");
    factory.registerClass<ROptimizationModel>("MODELS Optimization");
    factory.registerClass<RScriptModel>("RScriptModel");
    factory.registerClass<RScriptModel>("MODELS Script");
    factory.registerClass<RAlgebraicExpressionModel>("RAlgebraicExpressionModel");
    factory.registerClass<RAlgebraicExpressionModel>("MODELS Expression");
    factory.registerClass<R1DOptimizationModel>("R1DOptimizationModel");
    factory.registerClass<R1DOptimizationModel>("MODELS 1D Optimization");
    factory.registerClass<RRootFindingModel>("RRootFindingModel");
    factory.registerClass<RRootFindingModel>("MODELS Root Finding");
    factory.registerClass<RScenarioModel>("RScenarioModel");
    factory.registerClass<RScenarioModel>("MODELS Scenarios");
    factory.registerClass<RHardcodedExpressionModel>("RHardcodedExpressionModel");
    factory.registerClass<RHardcodedExpressionModel>("MODELS Hardcoded Expressions");
    factory.registerClass<RRandomVariableWithRandomParametersModel>("RRandomVariableWithRandomParametersModel");
    factory.registerClass<RRandomVariableWithRandomParametersModel>("MODELS Random Variable With Random Parameters");

#ifdef DETAILEDBUILDINGS
#include "CreateDetailedBuildingsObjects.h"
#endif

#ifdef SIMPLEBUILDINGS
#include "CreateSimpleBuildingObjects.h"
#endif

#ifdef NETWORKS
#include "CreateNetworkObjects.h"
#endif

#ifdef HAZARDS
#include "CreateHazardObjects.h"
#endif

#ifdef GSL
#include "CreateGSLObjects.h"
#endif

#ifdef OPENSEES
#include "CreateOpenSeesObjects.h"
#endif

}

#ifdef OPENSEES
Domain *RDomain::getTheOpenSeesDomain() const
{
    return theOpenSeesDomain;
}

void RDomain::setTheOpenSeesDomain(Domain *value)
{
    theOpenSeesDomain = value;
}
#endif

#ifdef DETAILEDBUILDINGS
RStatistics* RDomain::getStatsMethod(void)
{
    return theStatsMethod.get();
}
#endif

