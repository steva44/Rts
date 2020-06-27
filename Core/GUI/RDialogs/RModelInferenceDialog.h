#ifndef RModelInferenceDialog_H
#define RModelInferenceDialog_H
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

#include "ui_RModelInferenceDialog.h"
#include <QDialog>
#include <QPointer>

class RDomain;

using namespace Ui;

class RModelInferenceDialog : public QDialog, public RModelInferenceDialogUi
{
        Q_OBJECT

public:
        RModelInferenceDialog(QWidget *parent = 0, RDomain *passedDomain = nullptr);
        ~RModelInferenceDialog();

private slots:

        void onOkButtonClicked();
        void browse();

private:

        int analyze();
        int carryOutInference(QVector<double> *mean_theta, QVector<double> *cov_theta, QVector< QVector<double> > *correlationMatrix, double &dmean_sigma, double &cov_sigma, double &R);
        void createResidualPlot(QVector<double> *mean_theta);
        void createModelPredictionPlot(QVector<double> *mean_theta, QString plotTitle);
        void createModelObsRatioPlot(QVector<double> *mean_theta);
        void createNormalityPlot(QVector<double> *mean_theta, double mean_sigma);
        void createRandomVariablesAndModel(QVector<double> *mean_theta, QVector<double> *cov_theta, double mean_sigma, double cov_sigma);
        void searchForExplanatoryFunctions(QVector<double> *mean_theta, QVector<double> *cov_theta);
        int incrementCombinationVector(QVector<double> *cValues);

        RDomain *theDomain;

        QVector< QVector<double> > *X;
        QVector<double> *y;

        int n, k;
        QStringList stringList;

		double power;

};

#endif // RModelInferenceDialog_H
