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

#include "RTimeSeriesResponse.h"
#include "RDomain.h"
#include "RMainWindow.h"

RTimeSeriesResponse::RTimeSeriesResponse(QObject *parent, QString name)
	: RResponse(parent, name)
{
	RDomain *theDomain = qobject_cast<RDomain *>(parent);

    // Instantiate an RTimeSeries
    theDomain->createObject(objectName() + "TimeSeries", "RTimeSeries");
    theTimeSeries = qobject_cast<RTimeSeries *>(theDomain->getLastAddedObject());
}


RTimeSeriesResponse::~RTimeSeriesResponse()
{
    if (theTimeSeries) {
        delete theTimeSeries;
	}
    theTimeSeries = nullptr;
}


QStringList RTimeSeriesResponse::getActionList()
{
    QStringList actionList;
    actionList << "&Help";
    return actionList;
}


RTimeSeries* RTimeSeriesResponse::getTimeSeries() const
{
    return theTimeSeries;
}


void RTimeSeriesResponse::setTimeSeries(QObject *value)
{
    theTimeSeries = qobject_cast<RTimeSeries *>(value);
}


void RTimeSeriesResponse::setObjectName(const QString &name)
{
	QString oldName = objectName();
	QObject *object = qobject_cast<QObject *>(this);
	object->setObjectName(name);

    QString TimeSeriesName = theTimeSeries->objectName();
    if (TimeSeriesName.contains(oldName)) {
        QString newTimeSeriesName = TimeSeriesName.replace(oldName, name);
        theTimeSeries->setObjectName(newTimeSeriesName);
	}
}


void RTimeSeriesResponse::backup()
{
	backupVector.clear();
}


void RTimeSeriesResponse::restore()
{
	if (backupVector.count() != 3) {
		qCritical() << "Error: There is no backup vector for the response" << objectName() << ".";
		return;
	}

}


QVector<double> RTimeSeriesResponse::getValueVector()
{
    return theValueVector;
}


QVector<double> RTimeSeriesResponse::getBackupVector()
{
	return backupVector;
}


void RTimeSeriesResponse::setValueVector(QVector<double> valueVector)
{
    theValueVector.clear();
    theValueVector = valueVector;

    /*
	if (valueVector.count() != 3) {
		qCritical() << "Error: The value vector passed to the response" << objectName() << "does not include enough elements.";
		return;
	}
    */
}
