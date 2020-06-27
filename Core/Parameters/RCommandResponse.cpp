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

#include "RCommandResponse.h"
#include "RModel.h"
#include "RMainWindow.h"

#include <QProcess>

RCommandResponse::RCommandResponse(QObject *parent, QString name)
	: RResponse(parent, name)
{

}


RCommandResponse::~RCommandResponse()
{

}


QStringList RCommandResponse::getActionList()
{
    QStringList actionList;
    actionList << "&Help";
    return actionList;
}


QString RCommandResponse::getCommand() const
{
	return theCommand;
}


void RCommandResponse::setCommand(QString value)
{
	theCommand = value;
}


bool RCommandResponse::isAbsolute() const
{
	return theAbsolute;
}


void RCommandResponse::setAbsolute(bool value)
{
	theAbsolute = value;
}


int RCommandResponse::updateValue()
{
	RModel *model = qobject_cast<RModel *>(this->getModel());
	if (!model) {
		qCritical() << "Error: The response" << objectName() << "is not given any models.";
		return -1;
	}

	QProcess *process = model->getProcess();
	if (!process) {
		qCritical() << "Error: The response" << objectName() << "cannot get the process from the model" << model->objectName();
		return -1;
	}

	process->write(qPrintable(theCommand + "\n"));
	// Wait until something is ready to be read or 3000 milliseconds, which ever comes first
	
	bool ok;

	ok = process->waitForReadyRead(3000);
	if (!ok) {
		qCritical() << "Error: Could not get the value of the command response" << objectName() << "which is the output of the model" << model->objectName();
		return -3;
	}

	QByteArray tempBA = process->readAllStandardOutput();
	QString tempStr = QString(tempBA).trimmed();
	

	double value = tempStr.toDouble(&ok);
	if (theAbsolute) {
		value = qAbs(value);
	}
	
	if (ok) {
		this->setCurrentValue(value);
	} else {
		qCritical() << "Error: Could not update the value of the command response" << objectName() << "which is the output of the model" << model->objectName();
		return -3;
	}

	return 0;
}
