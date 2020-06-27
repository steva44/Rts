#ifndef RCommandResponse_H
#define RCommandResponse_H
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

#include "RResponse.h"

class RCommandResponse : public RResponse
{
	Q_OBJECT
	// Sets the command that is issued to the external software application to get the response value
	Q_PROPERTY(QString Command READ getCommand WRITE setCommand)
	// Indicates if the response value should be set to the absolute value of what is returned by the external software application
	Q_PROPERTY(bool Absolute READ isAbsolute WRITE setAbsolute)

public:
	RCommandResponse(QObject *parent, QString name);
	~RCommandResponse();
	
	// getter/setter methods for meta-properties
	QString getCommand() const;
	void setCommand(QString value);
	bool isAbsolute() const;
	void setAbsolute(bool value);

	// Invokes the operation of updating the response value by issuing the command to the external software application
	int updateValue();

    QStringList getActionList();

private:
	// Data member for meta-properties
	QString theCommand;
	bool theAbsolute;
	
};

#endif // RCommandResponse_H
