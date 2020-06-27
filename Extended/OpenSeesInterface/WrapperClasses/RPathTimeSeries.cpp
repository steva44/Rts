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

#include "RPathTimeSeries.h"
#include "PathTimeSeries.h"
#include "TimeSeries.h"
#include "PathSeries.h"
#include "RParameter.h"
#include "RDomain.h"

#include <QFileInfo>

RPathTimeSeries::RPathTimeSeries(QObject *parent, QString name)
    : ROpenSeesTimeSeries(parent, name)
{

    // $cFactor	optional, a factor to multiply load factors by (default = 1.0)
    cFactor = 1.0;

    // $tStart	optional, to provide a start time for provided load factors (default = 0.0)
    startTime = 0.0;

    // -useLast	optional, to use last value after the end of the series (default = 0.0)
    useLast = false;

    // -prependZero	optional, to prepend a zero value to the series of load factors (default = false). See NOTES.
    prependZero = false;

}


RPathTimeSeries::~RPathTimeSeries()
{

}


TimeSeries* RPathTimeSeries::getTheTimeSeries()
{
    if(!theTimeSeries)
        this->createThePathTimeSeries();

    return theTimeSeries.get();
}


int RPathTimeSeries::createThePathTimeSeries(void)
{

    bool filePathExists = this->fileExists(FileName);

    bool fileTimeExists = this->fileExists(FileTimeName);

    if (filePathExists && !fileTimeExists && dt != 0.0)
    {
        // PathSeries(int tag,
        // const char *fileName,
        // double pathTimeIncr = 1.0,
        // double cfactor = 1.0,
        // bool useLast = false,
        // bool prependZero = false,
        // double startTime = 0.0);

        QByteArray byteArrayName = FileName.toLatin1();
        const char * fileName = byteArrayName.data();

        theTimeSeries = std::make_unique<PathSeries>(objectTag, fileName, dt, cFactor,
                                                     useLast, prependZero, startTime);
    }
    else if (filePathExists)
    {
        QByteArray byteArrayName = FileName.toLatin1();
        const char * fileName = byteArrayName.data();

        // PathTimeSeries(int tag,
        // const char *fileName,
        // double cfactor = 1.0,
        // bool useLast = false);
        theTimeSeries = std::make_unique<PathTimeSeries>(objectTag, fileName, cFactor, useLast);
    }
    else if(filePathExists &&  fileTimeExists)
    {
        QByteArray byteArrayName = FileName.toLatin1();
        const char * fileNamePath = byteArrayName.data();

        QByteArray byteArrayNameTime = FileTimeName.toLatin1();
        const char * fileNameTime = byteArrayNameTime.data();

        // PathTimeSeries(int tag,
        // const char *fileNamePath,
        // const char *fileNameTime,
        // double cfactor = 1.0,
        // bool useLast = false);
        theTimeSeries = std::make_unique<PathTimeSeries>(objectTag, fileNamePath, fileNameTime, cFactor, useLast);
    }
    else if (theLoadPath.Size() != 0 && theTimePath.Size() == 0 && dt != 0.0)
    {

        // PathSeries(int tag,
        // const Vector &thePath,
        // double pathTimeIncr = 1.0,
        // double cfactor = 1.0,
        // bool useLast = false,
        // bool prependZero = false,
        // double startTime = 0.0);
        theTimeSeries = std::make_unique<PathSeries>(objectTag, theLoadPath, dt, cFactor, useLast, prependZero, startTime);
    }
    else if(theLoadPath.Size() != 0 || theTimePath.Size() != 0)
    {

        // PathTimeSeries(int tag,
        // const Vector &thePath,
        // const Vector &theTime,
        // double cfactor = 1.0,
        // bool useLast = false);
        theTimeSeries = std::make_unique<PathTimeSeries>(objectTag, theLoadPath, theTimePath, cFactor,useLast);
    }
    else
    {
        qDebug()<< "WARNING choice of options for Path Series invalid - valid options for Path are\n";
        qDebug()<< "-fileT fileTimeName -fileP filePathName \n";
        qDebug()<< "-dt constTimeIncr -file filePathName\n";
        qDebug()<< "-dt constTimeIncr -values {list of points on path}\n";
        qDebug()<< "-time {list of time points} -values {list of points on path}\n";
        return -1;
    }

    return 0;
}


bool RPathTimeSeries::getUseLast() const
{
    return useLast;
}


void RPathTimeSeries::setUseLast(bool value)
{
    useLast = value;
}


Vector RPathTimeSeries::getTheTimePath() const
{
    return theTimePath;
}


void RPathTimeSeries::setTheTimePath(const Vector &value)
{
    theTimePath = value;
}


Vector RPathTimeSeries::getTheLoadPath() const
{
    return theLoadPath;
}


void RPathTimeSeries::setTheLoadPath(const Vector &value)
{
    theLoadPath = value;
}


bool RPathTimeSeries::getPrependZero() const
{
    return prependZero;
}


void RPathTimeSeries::setPrependZero(bool value)
{
    prependZero = value;
}


double RPathTimeSeries::getCFactor() const
{
    return cFactor;
}


void  RPathTimeSeries::setCFactor(double value)
{
    cFactor = value;
}


double RPathTimeSeries::getTStart() const
{
    return startTime;
}


void RPathTimeSeries::setTStart(double value)
{
    startTime = value;
}


QString RPathTimeSeries::getFileTime() const
{
    return fileTime;
}


void RPathTimeSeries::setFileTime(const QString &value)
{
    fileTime = value;
}


QString RPathTimeSeries::getFilePath() const
{
    return filePath;
}


void RPathTimeSeries::setFilePath(const QString &value)
{
    filePath = value;
}


QString RPathTimeSeries::getFileTimeName() const
{
    return FileTimeName;
}


void RPathTimeSeries::setFileTimeName(const QString &value)
{
    FileTimeName = value;
}


QString RPathTimeSeries::getFileName() const
{
    return FileName;
}


void RPathTimeSeries::setFileName(const QString &value)
{
    FileName = value;
}


bool RPathTimeSeries::fileExists(QString file)
{
    std::ifstream theFile2;
    theFile2.open(file.toStdString(),std::ios::in);
    if (theFile2.bad() || !theFile2.is_open()) {
        return false;
    }

    return true;
}



