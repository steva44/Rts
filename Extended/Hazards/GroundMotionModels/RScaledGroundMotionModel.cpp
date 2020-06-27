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

#include "RScaledGroundMotionModel.h"
#include "RResponse.h"
#include "RDomain.h"
#include "RTimeSeries.h"

#include <QDir>

RScaledGroundMotionModel::RScaledGroundMotionModel(QObject *parent, QString name)
    : REarthquakeBaseClass(parent, name)
{
    //Do not scale if not set
    //scalingFactor = 1.0;
    scalingFactor = -9.80665;

    theInputFile = "";

    // Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RTimeSeriesResponse");
    theGroundMotionResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theGroundMotionResponse->setModel(this);

    // Instantiating a time series to contain the file content
    theDomain->createObject(objectName() + "InputGroundMotion", "RTimeSeries");
    theInputTimeSeries = qobject_cast<RTimeSeries *>(theDomain->getLastAddedObject());

}


RScaledGroundMotionModel::~RScaledGroundMotionModel()
{

}


bool RScaledGroundMotionModel::isOn()
{
    return true;
}


QString RScaledGroundMotionModel::getInputFile() const
{
    return theInputFile;
}


void RScaledGroundMotionModel::setScalingFactor(QString value)
{
    bool OK;

    auto dblVal = value.toDouble(&OK);

   if(OK)
       scalingFactor = dblVal;
   else
       qCritical()<<"In the model "<<this->objectName()<<", the scaling factor must me a number";
}


void RScaledGroundMotionModel::setInputFile(QString value)
{
    theInputFile = RDomain::stringToFilePath(value);
}


QStringList RScaledGroundMotionModel::getActionList()
{
    // Remember that the "run" and "help" methods are implemented in base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator" << "&Help";
    if(theInputTimeSeries)
        actionList <<"Separator" << "&PlotX"<< "&PlotY"<< "&PlotZ"<< "&Print Time Series";


    return actionList;
}


int RScaledGroundMotionModel::evaluateModel(RGradientType theGradientType)
{

    // Clean the input time series
    theInputTimeSeries->clean();


    // Check that there is any magnitude
    if (!isOn()) {
        return 0;
    }


    // Open the file
    theInputFile.remove('"');
    if (theInputFile.isEmpty()) {
        qCritical() << "The scaled ground motion model" << this->objectName() << "needs an input file name.";
        return -1;
    }
    QFile file(theInputFile);
    if (!file.open(QFile::ReadOnly /*| QFile::Text*/)) {
        qCritical() << "Error: Could not open the file" << theInputFile << ".";
        return -1;
    }


    // Read the file content into a QString
    QTextStream stream(&file);
    stream.seek(0);


    // Tokenize the entire input file, i.e., the string, by creating a line break at "\n" and create a string list LINES
    QString verticalRegExpString = QString("[n\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator));
    QRegExp verticalRegExp = QRegExp(verticalRegExpString);
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
    } while (!line.isNull());


    // Get the number of lines
    int numLines = fileContentStringList.count();


    // Tokenize each line
    QRegExp regExp = QRegExp("[ ,;\t]");
    int columnCount = 0;
    for (int i = 0; i < numLines; i++) {

        QStringList valueList = fileContentStringList[i].split(regExp, QString::SkipEmptyParts);


        columnCount = valueList.count();

        if (columnCount != 4) {
            qCritical() << "The ground motion file given to" << this->objectName() << "does not have 4 columns.";
        }


        // Declare the vector that temporarily stores the numbers in one line
        QVector<double> numbersInReadLine(columnCount);


        // Read number by number in this line (line number i)
        for (int j = 0; j < columnCount; j++) {
            QString tempStr = valueList[j].trimmed();
            bool ok;
            double tempValue = tempStr.toDouble(&ok);
            if (!ok) {
                qCritical() << "Error: Could not read the value number" << j + 1 << "at the line" << i + 1 << "in the input file.";
                return -1;
            }

            numbersInReadLine[j] = tempValue;
        }

        theInputTimeSeries->setXYZvalues(numbersInReadLine[0], numbersInReadLine[1], numbersInReadLine[2], numbersInReadLine[3]);
    }

    file.close();


    // Getting pointer to the response time series, and clean it
    RTimeSeries *theOutputTimeSeries = parameterToTimeSeries(theGroundMotionResponse);
    if (!theOutputTimeSeries) {
        qCritical() << "Error: The response object" << theGroundMotionResponse->objectName() << "does not contain a times series object.";
        return -1;
    }
    theOutputTimeSeries->clean();


    // Do something with the input time series, such as scaling it, and then save it to the response
    int numEntries = theInputTimeSeries->getNumEntries();
    double t, x, y, z;


    for (int i=0; i<numEntries; i++) {

        t = theInputTimeSeries->getTimeItem(i);
        x = theInputTimeSeries->getXitem(i);
        y = theInputTimeSeries->getYitem(i);
        z = theInputTimeSeries->getZitem(i);


        x=scalingFactor*x;
        y=scalingFactor*y;
        z=scalingFactor*z;

        theOutputTimeSeries->setXYZvalues(t, x, y, z);
    }


    return 0;
}


void RScaledGroundMotionModel::plotX()
{
    this->evaluateModel(RGradientType::NONE);
    theInputTimeSeries->plotX();
}


void RScaledGroundMotionModel::plotY()
{
    this->evaluateModel(RGradientType::NONE);
    theInputTimeSeries->plotY();
}


void RScaledGroundMotionModel::plotZ()
{
    this->evaluateModel(RGradientType::NONE);
    theInputTimeSeries->plotZ();
}


void RScaledGroundMotionModel::printTimeSeries()
{
    this->evaluateModel(RGradientType::NONE);
    theInputTimeSeries->print();
}
