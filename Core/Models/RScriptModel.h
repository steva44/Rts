#ifndef RScriptModel_H
#define RScriptModel_H
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

#include "RModel.h"
#include <QtScript>

class RParameter;
class RResponse;

class RScriptModel : public RModel
{
	Q_OBJECT
	Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)
	Q_PROPERTY(QString ScriptFile READ getScriptFile WRITE setScriptFile)
	Q_PROPERTY(bool DDM READ getDDM WRITE setDDM)

public:
	RScriptModel(QObject *parent, QString name);
	~RScriptModel();

	// Define the mandatory read/write methods for all QProperties
	QString getInputParameterList();
	void setInputParameterList(QString value);
	QString getScriptFile();
	void setScriptFile(QString value);
	bool getDDM();
	void setDDM(bool value);

	// Define the method where the actual model is coded
	int evaluateModel(RGradientType theGradientType);

	QStringList getActionList();
	void reset();

private slots:
	void debug();

private:
	// Declare the list of input data members
	QList<QPointer<RParameter> > theInputParameterList;
	QString theScriptFile;
	bool theDDM;

	QScriptEngine theEngine;
	QString theScriptText;

};

#endif // RScriptModel_H
