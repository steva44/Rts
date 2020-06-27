#ifndef RLocationResponse_H
#define RLocationResponse_H
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

class RLocation;

class RLocationResponse : public RResponse
{
	Q_OBJECT
    Q_PROPERTY(QObject *Location READ getLocation WRITE setLocation) // Sets the RLocation object that the response "carries"

public:
    RLocationResponse(QObject *parent, QString name);
    ~RLocationResponse();
	
	// getter/setter methods for meta-properties
    QObject *getLocation() const;
    void setLocation(QObject *value);

    // Specific implementation of the setObjectName, which sets the object name for the response and its auto-generated RLocation object
	void setObjectName(const QString &name);

	// Backs up the current values of the response (latitude, longitude, and altitude)
	void backup();

	// Backs up the current values of the response (latitude, longitude, and altitude)
	void restore();

	// Returns the vector of response values, used in DDM computations; It has three elements: latitude, longitude, and altitude
	QVector<double> getValueVector();

	// Sets the vector of response values as explained above
	void setValueVector(QVector<double> valueVector);

	// Returns the backup vector of response values as explained above
	QVector<double> getBackupVector();

    QStringList getActionList();

private:
	// Data member for meta-properties
    RLocation *theLocation;

	// Data member for the backup vector of response values as explained above
	QVector<double> backupVector;
	
};

#endif // RLocationResponse_H
