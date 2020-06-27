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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RTimeSeriesDialog.h"
#include "RMainWindow.h"
#include "RDomain.h"
#include "RContinuousRandomVariable.h"
#include "RTimeSeries.h"
#include "math.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QtDebug>

RTimeSeriesDialog::RTimeSeriesDialog(QWidget *parent, RDomain *passedDomain, RTimeSeries *passedTimeSeries)
    : QDialog(parent)
{
    setupUi(this);

    theTimeSeries = passedTimeSeries;

    X = 0;

    connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
    connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}


RTimeSeriesDialog::~RTimeSeriesDialog()
{

}


void RTimeSeriesDialog::browse()
{
    // Get the file name from the user using QFileDialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Input file"), QDir::currentPath(), "All Files (*.*)");
    if (fileName.isEmpty()) {
        return;
    }
    lineEdit->setText(fileName);
}


void RTimeSeriesDialog::onOkButtonClicked()
{
    // Get file name
    QString filePath = lineEdit->text();
    filePath.remove('"');
    if (filePath.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Please choose enter a file name and try again."), QMessageBox::Ok | QMessageBox::Default);
        return;
    }


    // Get a pointer to the main window
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(this->parent());
    accept();
    mainWindow->raiseDock("Output");


    // Open the file
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly /*| QFile::Text*/)) {
        // Error
        qCritical() << "Error: Could not open the file" << filePath << ".";
        return;
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
    double DT = 0.0;
    int printerCounter = 0;
    for (int i = 0; i < numLines; i++) {

        QStringList valueList = fileContentStringList[i].split(regExp, QString::SkipEmptyParts);



        // When reading the first line, get the number of columns, and check that it is consistent with the user's check boxes
        if (i == 0) {


            if (PEERCheckBox->isChecked()) {

                // When this option is selected, then the column count does not matter
                columnCount = 0;

                // If the PEER box is checked, we can only allow ONE more box to be checked, and it cannot be the time box
                if (TimeCheckBox->isChecked()) {
                    qCritical() << "When the PEER format is selected, then the Time box cannot be checked.";
                }

                int userChecks = 0;
                if (XValuesCheckBox->isChecked()) {
                    userChecks++;
                }
                if (YValuesCheckBox->isChecked()) {
                    userChecks++;
                }
                if (ZValuesCheckBox->isChecked()) {
                    userChecks++;
                }
                if (userChecks > 1) {
                    qCritical() << "When the PEER format is selected, then only one other box can be checked.";
                }

            }
            else if (!PEERCheckBox->isChecked()) {


                // Get the number of columns
                columnCount = valueList.count();


                // Investigate how many check-boxes the user has checkd
                int userChecks = 0;
                if (TimeCheckBox->isChecked()) {
                    userChecks++;
                }
                else {
                    qCritical() << "Unless the PEER format is selected, the first column in the file must be Time (please check that box).";
                }
                if (XValuesCheckBox->isChecked()) {
                    userChecks++;
                }
                if (YValuesCheckBox->isChecked()) {
                    userChecks++;
                }
                if (ZValuesCheckBox->isChecked()) {
                    userChecks++;
                }


                // Give an error message if the number of clicked boxes does not match the number of columns in the file
                if (columnCount != userChecks) {
                    qCritical() << "The number of boxes checked is different from the number of columns in the file.";
                }
                else if (columnCount < 2) {
                    qCritical() << "There must be at least TWO columns in the file, and the first column must be Time.";
                }

            }

        } // End of actions for the first line







        if (PEERCheckBox->isChecked()) {


            // Get the column count for this line
            columnCount = valueList.count();


            // Declare a string that will contains the numbers, as strings, before they are converted into numbers
            QString tempStr;
            bool ok;
            double tempValue;
            bool lineIsOk = true;



            // Check if this line contains the word "NPTS"
            if (valueList.contains("NPTS")) {


                // Pick up the value we think is DT
                tempStr = valueList[1].trimmed();
                tempValue = tempStr.toDouble(&ok);
                if (ok) {

                    DT = tempValue;
                    //qDebug() << "The time step in the loaded ground motion is" << DT;

                }
                else {

                    qCritical() << "The value thought to be the time increment cannot be converted to a number.";
                    return;

                }

            }



            // Read item by item in this line, and check that they are all numbers
            for (int j = 0; j < columnCount; j++) {

                tempStr = valueList[j].trimmed();
                tempValue = tempStr.toDouble(&ok);
                if (!ok) {
                    lineIsOk = false;
                }
            }



            // Read all items again, if line was found to be ok
            if (lineIsOk) {

                // Read number by number in this line (line number i)
                for (int j = 0; j < columnCount; j++) {


                    tempStr = valueList[j].trimmed();
                    tempValue = tempStr.toDouble(&ok);


                    // Set values in the Time Series object
                    if (XValuesCheckBox->isChecked()) {

                        theTimeSeries->setXvalues(DT*(printerCounter + j), tempValue);
                    }
                    else if (YValuesCheckBox->isChecked()) {

                        theTimeSeries->setYvalues(DT*(printerCounter + j), tempValue);
                    }
                    else if (ZValuesCheckBox->isChecked()) {

                        theTimeSeries->setZvalues(DT*(printerCounter + j), tempValue);
                    }
                }

                printerCounter += columnCount;

            }








        }
        else { // Now read the file the non-PEER-format way


            // Declare the vector that temporarily stores the numbers in one line
            QVector<double> numbersInReadLine(columnCount);




            // Read number by number in this line (line number i)
            for (int j = 0; j < columnCount; j++) {
                QString tempStr = valueList[j].trimmed();
                bool ok;
                double tempValue = tempStr.toDouble(&ok);
                if (!ok) {
                    qCritical() << "Error: Could not read the value number" << j + 1 << "at the line" << i + 1 << "in the input file.";
                    return;
                }

                numbersInReadLine[j] = tempValue;
            }



            if (columnCount==2) {

                if (XValuesCheckBox->isChecked()) {

                    theTimeSeries->setXvalues(numbersInReadLine[0], numbersInReadLine[1]);
                }
                else if (YValuesCheckBox->isChecked()) {

                    theTimeSeries->setYvalues(numbersInReadLine[0], numbersInReadLine[1]);
                }
                else if (ZValuesCheckBox->isChecked()) {

                    theTimeSeries->setZvalues(numbersInReadLine[0], numbersInReadLine[1]);
                }

            }
            else {

                if (XValuesCheckBox->isChecked() && YValuesCheckBox->isChecked() && ZValuesCheckBox->isChecked()) {

                    theTimeSeries->setXYZvalues(numbersInReadLine[0], numbersInReadLine[1], numbersInReadLine[2], numbersInReadLine[3]);
                }
                else if (XValuesCheckBox->isChecked() && YValuesCheckBox->isChecked()) {

                    theTimeSeries->setXYZvalues(numbersInReadLine[0], numbersInReadLine[1], numbersInReadLine[2], 0.0);
                }
                else if (XValuesCheckBox->isChecked() && ZValuesCheckBox->isChecked()) {

                    theTimeSeries->setXYZvalues(numbersInReadLine[0], numbersInReadLine[1], 0.0, numbersInReadLine[2]);
                }
                else if (YValuesCheckBox->isChecked() && ZValuesCheckBox->isChecked()) {

                    theTimeSeries->setXYZvalues(numbersInReadLine[0], 0.0, numbersInReadLine[1], numbersInReadLine[2]);
                }
            }
        }




    } // End for loop over lines


    qDebug() << "Done loading time series file.";

    file.close();


    // Execute the 'analyze' method
    //	int result = this->analyze();
    mainWindow->updateTree("Domain");
}
