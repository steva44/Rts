#ifndef RVariableInferenceDialog_H
#define RVariableInferenceDialog_H
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
// Last modified by Mojtaba on June 30, 2012

#include "ui_RVariableInferenceDialog.h"
#include "RDomain.h"
#include <QDialog>

using namespace Ui;

class RVariableInferenceDialog : public QDialog, public RVariableInferenceDialogUi
{
	Q_OBJECT

public:
    RVariableInferenceDialog(QWidget *parent = nullptr, RDomain *passedDomain = nullptr);
	~RVariableInferenceDialog();

	void setDomain(RDomain *passedDomain);

	double getInput();

private slots:
	void onOkButtonClicked();	
	void browse();

private:
	int analyze();
	void deleteVectors();

	RDomain *theDomain;
    QVector< QVector<double> > *X;

};

#endif
