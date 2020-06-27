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

#include "RModelInferenceDialog.h"
#include "RMainWindow.h"
#include "RDomain.h"
#include "RContinuousRandomVariable.h"
#include "RConstant.h"
#include "RAlgebraicExpressionModel.h"
#include "RRandomVariableWithRandomParametersModel.h"
#include "RMatrixOperations.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QtDebug>
#include <QString>
#include <QStringList>
#include <math.h>


RModelInferenceDialog::RModelInferenceDialog(QWidget *parent, RDomain *passedDomain)
    : QDialog(parent)
{
    setupUi(this);

    theDomain = passedDomain;

    X = new QVector< QVector<double> >(1, QVector<double>(1));
    y = new QVector<double>(1);

    power = 3.0;

    connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
    connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}


RModelInferenceDialog::~RModelInferenceDialog()
{
    delete X;
    delete y;
}


void RModelInferenceDialog::browse()
{
    // Get the file name from the user using QFileDialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Input file"), QDir::currentPath(), "All Files (*.*)");
    if (fileName.isEmpty()) {
        return;
    }
    lineEdit->setText(fileName);
}


void RModelInferenceDialog::onOkButtonClicked()
{	
    QString filePath = lineEdit->text();
    filePath.remove('"');

    if (filePath.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Please choose enter a file name and try again."), QMessageBox::Ok | QMessageBox::Default);
        return;
    }
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(this->parent());

    accept();

    mainWindow->raiseDock("Output");


    // Read the file here
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly)) {
        // Error
        qCritical() << "Error: Could not open the file" << filePath << ".";
        return;
    }

    // Read the file content into a QString
    QTextStream stream(&file);
    stream.seek(0);
    //QString fileContentString = in.readAll();

    // Tokenize the QString of file contents by "\n" and create a string list of input file lines
    QString verticalRegExpString = QString("[n\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator));
    QRegExp verticalRegExp = QRegExp(verticalRegExpString);
    //QStringList fileContentStringList = fileContentString.split(verticalRegExp, QString::SkipEmptyParts);

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
        //QCoreApplication::processEvents();
    } while (!line.isNull());

    int size = fileContentStringList.count();

    QRegExp regExp = QRegExp("[ ,;\t]");
    int columnCount = 0;

    for (int i = 0; i < fileContentStringList.count(); i++) {
        QStringList valueList = fileContentStringList[i].split(regExp, QString::SkipEmptyParts);

        if (i == 0) {
            columnCount = valueList.count();
            if (columnCount < 2) {
                qCritical() << "Error: The number of columns should be more than 1 in the database file.";
                return;
            }


            delete X;
            delete y;
            X = new QVector< QVector<double> >(size, QVector<double>(columnCount-1));
            y = new QVector<double>(size);
            n = size;
            k = columnCount-1;


        } else {
            if (columnCount != valueList.count()) {
                qCritical() << "Error: The number of data at the line" << i + 1 << "is different than the line 1 in the database file.";
                return;
            }
        }

        for (int j = 0; j < valueList.count(); j++) {
            QString tempStr = valueList[j].trimmed();
            bool ok;
            double tempValue = tempStr.toDouble(&ok);
            if (!ok) {
                qCritical() << "Error: Could not read the value number" << j + 1 << "at the line" << i + 1 << "in the database file.";
                return;
            }

            // The first value in the line is the output

            if (j == 0) {
                (*y)[i] = tempValue;

                // The from 1 to "valueList.count()" values in the line
            } else if (j > 0) {
                (*X)[i][j-1] = tempValue;
            }
        }
    }
    file.close();



    qDebug() << (X->size()) << "data points were read to obtain the following y-vector and X-matrix:";
    theDomain->print(y);
    qDebug() << "";
    theDomain->print(X);
    qDebug() << "";



    // Carry out the inference analysis
    int result = this->analyze();
    if (result < 0) {
        qCritical() << "Error: The variable inference analysis did not converge because of errors.";
        return;
    }
    mainWindow->updateTree("Domain");
}


int RModelInferenceDialog::analyze()
{
    // Inform the user that the sampling analysis has started
    qDebug() << "##### STARTING VARIABLE INFERENCE ANALYSIS ...";
    qDebug() << "";

    QTime time;
    time.start();


    // PLAY AROUND WITH NEW DATA
    //playAroundWithNewData();



    // CARRY OUT THE INFERENCE ANALYSIS
    QVector<double> mean_theta(k);
    QVector<double> cov_theta(k);
    QVector< QVector<double> > correlationMatrix(k, QVector<double>(k));

    double mean_sigma = 0.0;
    double cov_sigma = 0.0;
    double R = 0.0;
    carryOutInference(&mean_theta, &cov_theta, &correlationMatrix, mean_sigma, cov_sigma, R);


    // PRINT RESULTS TO THE OUTPUT PANE
    qDebug() << "Mean of the model parameters:";
    theDomain->print(&mean_theta);
    qDebug() << "";
    qDebug() << "Coefficient of variation (in percent) of the model parameters:";
    theDomain->print(&cov_theta);
    qDebug() << "";
    qDebug() << "Correlation matrix of the model parameters:";
    theDomain->print(&correlationMatrix);
    qDebug() << "";
    qDebug() << "R-factor:" << R;
    qDebug() << "Mean of sigma:" << mean_sigma;
    qDebug() << "";


    // PLOT RESIDUALS VERSUS REGRESSORS
    if (ResiudalPlotCheckBox->isChecked()) {
        createResidualPlot(&mean_theta);
    }

    QCoreApplication::processEvents(); //Mojtaba


    // PLOT MODEL PREDICTIONS VERSUS THE OBSERVED DATA
    if (ModelPredictionPlotCheckBox->isChecked()) {
        QString plotTitle = "Model Predictions Vs. Observations";
        createModelPredictionPlot(&mean_theta, plotTitle);
    }

    QCoreApplication::processEvents(); //Mojtaba


    // PLOT THE RATIO OF THE MODEL PREDICTION AND OBSERVATIONS, AS A FUNCTION OF THE OBSERVED VALUE
    if (ModelObsRatioPlotCheckBox->isChecked()) {
        createModelObsRatioPlot(&mean_theta);
    }

    QCoreApplication::processEvents(); //Mojtaba


    // DISPLAY A NORMALITY PLOT
    if (NormalityPlotCheckBox->isChecked()) {
        createNormalityPlot(&mean_theta, mean_sigma);

    }

    QCoreApplication::processEvents(); //Mojtaba


    // SEARCH FOR EXPLANATORY FUNCTIONS (Perhaps best do this OUTSIDE Rt?)
    if (SearchCheckBox->isChecked()) {
        searchForExplanatoryFunctions(&mean_theta, &cov_theta);

    }

    QCoreApplication::processEvents(); //Mojtaba


    // CREATING RANDOM VARIABLES AND THE MODEL
    if (CreateRVCheckBox->isChecked()) {
        createRandomVariablesAndModel(&mean_theta, &cov_theta, mean_sigma, cov_sigma);
    }


    qDebug() << "##### MODEL INFERENCE ANALYSIS DONE IN" << (double)(1000.0*time.elapsed())/1000000.0 << "SECONDS.";
    qDebug() << "";
    qDebug() << "";

    return 0;
}




int RModelInferenceDialog::carryOutInference(QVector<double> *mean_theta, QVector<double> *cov_theta, QVector< QVector<double> > *correlationMatrix, double &mean_sigma, double &cov_sigma, double &R)
{
    /*

        // Get number of regressors and observations
        int nu = n-k;

        // Give warning if there is little data
        if (nu<=0) {
                qCritical() << "You've got too few observations (data) compared with the number of regressors!";
        }
        else if (n<2*k) {
                qDebug() << "You've got quite few observations (data) compared with the number of regressors.";
        }

        // Compute the Ordinary Least Squares estimate; the mean of the thetas
        gsl_matrix *XX = gsl_matrix_calloc(k,k);
        gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, X, X, 0.0, XX);

        gsl_matrix *XXcopy = gsl_matrix_calloc(k, k);
        gsl_matrix_memcpy(XXcopy, XX);
        gsl_matrix *XXinv = gsl_matrix_calloc(k, k);
        int signum = 0;
        gsl_permutation *p = gsl_permutation_alloc(k);
        gsl_linalg_LU_decomp(XXcopy, p, &signum);

        double determinant = gsl_linalg_LU_det(XXcopy,signum);
        if (determinant==0.0) {
                qCritical() << "Error: Could not invert X*X^T in model inference analysis.";
        }

        gsl_linalg_LU_invert(XXcopy, p, XXinv);
        gsl_permutation_free(p);


        // Warn about collinearity
        for (int i=0; i<(XXinv->size1); i++) {
                if (  gsl_matrix_get(XXinv,i,i)<0.0  ) {
                        qCritical() << "You've got problems with collinearity in your data!";
                }
        }


        gsl_matrix *XXinvX = gsl_matrix_calloc(k,n);
        gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1.0, XXinv, X, 0.0, XXinvX);

        gsl_blas_dgemv(CblasNoTrans, 1.0, XXinvX, y, 0.0, mean_theta);


        // Compute the R-factor, namely the correlation between observed and predicted results
        double modelPrediction, observation;
        double sumPred = 0.0;
        double sumPredSquared = 0.0;
        double sumObs = 0.0;
        double sumObsSquared = 0.0;
        double sumObsPred = 0.0;

        for (int i=0; i<n; i++) {

                observation = gsl_vector_get(y,i);

                modelPrediction = 0.0;

                for (int j=0; j<k; j++) {
                        modelPrediction += gsl_vector_get(mean_theta,j)*gsl_matrix_get(X,i,j);
                }

                sumPred += modelPrediction;
                sumPredSquared += modelPrediction*modelPrediction;

                sumObs += observation;
                sumObsSquared += observation*observation;

                sumObsPred += modelPrediction*observation;
        }

        double predictionMean = 1.0/((double)n) * sumPred;
        double predictionVariance = 1.0/((double)(n-1)) * (sumPredSquared - n*predictionMean*predictionMean);
        double observationMean = 1.0/((double)n) * sumObs;
        double observationVariance = 1.0/((double)(n-1)) * (sumObsSquared - n*observationMean*observationMean);
        R = 1.0/((double)(n-1)) * (sumObsPred - n*predictionMean*observationMean) / (sqrt(predictionVariance) * sqrt(observationVariance));


        // Compute X*theta
        gsl_vector *Xtheta = gsl_vector_calloc(n);
        gsl_blas_dgemv(CblasNoTrans, 1.0, X, mean_theta, 0.0, Xtheta);


        // Compute the Ordinary Least Squares error variance
        gsl_vector *y_minus_Xtheta = gsl_vector_calloc(n);
        gsl_vector_memcpy(y_minus_Xtheta, y);
        gsl_vector_sub(y_minus_Xtheta, Xtheta);
        double dot_product_y_minus_Xtheta;
        gsl_blas_ddot (y_minus_Xtheta, y_minus_Xtheta, &dot_product_y_minus_Xtheta);
        double s_squared = dot_product_y_minus_Xtheta/nu;


        // Compute the covariance matrix for the model parameters
        gsl_matrix *covarianceMatrix = gsl_matrix_calloc(k,k);
        gsl_matrix_memcpy(covarianceMatrix, XXinv);
        double multiplicationFactor = s_squared; // Used to be:  double multiplicationFactor = nu*s_squared/(nu-2.0);
        gsl_matrix_scale(covarianceMatrix, multiplicationFactor);
        //for (int i=0; i<k; i++) {
        //	for (int j=0; j<k; j++) {
        //		gsl_matrix_set(covarianceMatrix,i,j,   (multiplicationFactor*gsl_matrix_get(covarianceMatrix,i,j)));
        //	}
        //}



        // Collect the standard deviations from the covariance matrix
        gsl_vector *stdv_theta = gsl_vector_calloc(k);
        for (int i=0; i<k; i++) {
                gsl_vector_set(stdv_theta, i, sqrt(gsl_matrix_get(covarianceMatrix,i,i)) );
        }


        // Collect the coefficient of variation in a vector (in percent!)
        for (int i=0; i<k; i++) {
                gsl_vector_set(cov_theta, i, fabs(100.0*gsl_vector_get(stdv_theta,i)/gsl_vector_get(mean_theta,i)) );
        }


        // Place 1/stdvs on the diagonal of the D^-1 matrix and extract the correlation matrix
        gsl_matrix *Dinv = gsl_matrix_calloc(k,k);
        for (int i=0; i<k; i++) {
                gsl_matrix_set(Dinv, i, i, 1.0/gsl_vector_get(stdv_theta,i));
        }
        gsl_matrix *DinvSigma = gsl_matrix_calloc(k,k);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, Dinv, covarianceMatrix, 0.0, DinvSigma);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, DinvSigma, Dinv, 0.0, correlationMatrix);


        // Compute statistics for the epsilon variable
        double mean_sigma_squared = s_squared;
        mean_sigma = sqrt(mean_sigma_squared);

        //double variance_sigma_squared = nu*nu*s_squared*s_squared*(2.0/((nu-2.0)*(nu-2.0)*(nu-4.0)));
        //double variance_sigma = variance_sigma_squared/(4.0*mean_sigma_squared);

        double variance_sigma = s_squared/(2.0*(nu-4.0));

        double stdv_sigma = sqrt(variance_sigma);
        cov_sigma = fabs(stdv_sigma/mean_sigma);


        // Free vectors and matrices
        gsl_matrix_free(XX);
        gsl_matrix_free(XXcopy);
        gsl_matrix_free(XXinv);
        gsl_matrix_free(XXinvX);

        gsl_vector_free(Xtheta);
        gsl_vector_free(y_minus_Xtheta);
        gsl_vector_free(stdv_theta);

        */


    return 0;
}

void RModelInferenceDialog::createResidualPlot(QVector<double> *mean_theta)
{
    /*

        RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent());

        //QList<QPointer<QwtPlot> > thePlotList;
        double epsilon, regressorValue, xMin, xMax, epsMax;

        for (int i=0; i<k; i++) {

                QCoreApplication::processEvents(); //Mojtaba

                QwtPlot *thePlot = new RPlotWidget(mainWindow);
                QString str = QString("x%1").arg(i+1);
                mainWindow->addSubWindow(thePlot, " Residual versus regressor " + str, true);
                thePlot->setAxisTitle(QwtPlot::xBottom, str);
                thePlot->setAxisTitle(QwtPlot::yLeft, "Residual");


                // Get the value of X*theta, namely the model prediction
                gsl_vector *Xtheta = gsl_vector_calloc(n);
                gsl_blas_dgemv(CblasNoTrans, 1.0, X, mean_theta, 0.0, Xtheta);



                // Plot the points
                QwtSymbol theSymbol;
                theSymbol.setStyle(QwtSymbol::Ellipse);
                for (int j=0; j<n; j++) {
                        epsilon = gsl_vector_get(y,j) - gsl_vector_get(Xtheta,j);
                        regressorValue = gsl_matrix_get(X,j,i);

                        if (j==0) {
                                xMin = regressorValue;
                                xMax = regressorValue;
                                epsMax = epsilon;
                        }
                        else {
                                if ( fabs(epsilon)>epsMax ) {
                                        epsMax = fabs(epsilon);
                                }
                                if ( regressorValue > xMax ) {
                                        xMax = regressorValue;
                                }
                                if ( regressorValue < xMin ) {
                                        xMin = regressorValue;
                                }
                        }

                        QwtPlotMarker *theMarker = new QwtPlotMarker();
                        theMarker->setLineStyle(QwtPlotMarker::NoLine);
                        theMarker->setSymbol(theSymbol);
                        theMarker->setValue(regressorValue, epsilon);
                        theMarker->attach(thePlot);
                }

                if (xMin==xMax) {
                        xMin -= 1.0;
                        xMax += 1.0;
                }
                thePlot->setAxisScale(QwtPlot::xBottom, xMin-0.1*(xMax-xMin), xMax+0.1*(xMax-xMin));
                thePlot->setAxisScale(QwtPlot::yLeft, -epsMax-0.1*epsMax, epsMax+0.1*epsMax);

                // Take away the legends
                QwtLegend *myLegend = thePlot->legend();
                myLegend->clear();

                thePlot->replot();
        }
*/
}

void RModelInferenceDialog::createModelPredictionPlot(QVector<double> *mean_theta, QString plotTitle)
{
    /*
        RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent());

        // Create a plot
        QPointer<RPlotWidget> thePredictionPlot = new RPlotWidget(mainWindow);
        mainWindow->addSubWindow(thePredictionPlot, plotTitle, true);
        thePredictionPlot->setAxisTitle(QwtPlot::xBottom, "Observations");
        thePredictionPlot->setAxisTitle(QwtPlot::yLeft, "Model Predictions");


        // Plot the points
        double modelPrediction, observation, minModel, maxModel;
        QwtSymbol theSymbol;
        theSymbol.setStyle(QwtSymbol::Ellipse);
        for (int i=0; i<n; i++) {

                observation = gsl_vector_get(y,i);
                modelPrediction = 0.0;
                for (int j=0; j<k; j++) {
                        modelPrediction += gsl_vector_get(mean_theta,j)*gsl_matrix_get(X,i,j);
                }

                if (i==0) {
                        if (observation>modelPrediction) {
                                minModel = modelPrediction;
                                maxModel = observation;
                        }
                }

                if (modelPrediction>maxModel) {
                        maxModel = modelPrediction;
                }
                if (observation>maxModel) {
                        maxModel = observation;
                }
                if (modelPrediction<minModel) {
                        minModel = modelPrediction;
                }
                if (observation<minModel) {
                        minModel = observation;
                }

                QwtPlotMarker *theMarker = new QwtPlotMarker();
                theMarker->setLineStyle(QwtPlotMarker::NoLine);
                theMarker->setSymbol(theSymbol);
                theMarker->setValue(observation, modelPrediction);
                theMarker->attach(thePredictionPlot);
        }

        // Create the zero to one curve
        QwtPlotCurve *curve = new QwtPlotCurve();
        QPen thePen;
        thePen.setColor(QColor(Qt::black));
        curve->setPen(thePen);
        curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
        curve->attach(thePredictionPlot);
        QVector<double> theZeroOneVector;
        theZeroOneVector << minModel-0.1*(maxModel-minModel) << maxModel+0.1*(maxModel-minModel);
        curve->setData(theZeroOneVector, theZeroOneVector);


        // Take away the legends
        QwtLegend *myLegend = thePredictionPlot->legend();
        myLegend->clear();


        thePredictionPlot->replot();
*/
}

void RModelInferenceDialog::createModelObsRatioPlot(QVector<double> *mean_theta)
{
    /*
        RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent());

        // Create a plot
        QPointer<RPlotWidget> theRatioPlot = new RPlotWidget(mainWindow);
        mainWindow->addSubWindow(theRatioPlot, "Prediction/Observation Ratio", true);
        theRatioPlot->setAxisTitle(QwtPlot::xBottom, "Predicted Value");
        theRatioPlot->setAxisTitle(QwtPlot::yLeft, "Prediction/Observation");


        // Plot the points
        double modelPrediction, observation, minModel, maxModel, ratio;
        double maxRatio = 0.0;
        QwtSymbol theSymbol;
        theSymbol.setStyle(QwtSymbol::Ellipse);
        for (int i=0; i<n; i++) {

                observation = gsl_vector_get(y,i);
                modelPrediction = 0.0;
                for (int j=0; j<k; j++) {
                        modelPrediction += gsl_vector_get(mean_theta,j)*gsl_matrix_get(X,i,j);
                }

                if (i==0) {
                        if (observation>modelPrediction) {
                                minModel = modelPrediction;
                                maxModel = observation;
                        }
                }

                if (modelPrediction>maxModel) {
                        maxModel = modelPrediction;
                }
                if (observation>maxModel) {
                        maxModel = observation;
                }
                if (modelPrediction<minModel) {
                        minModel = modelPrediction;
                }
                if (observation<minModel) {
                        minModel = observation;
                }

                QwtPlotMarker *theMarker = new QwtPlotMarker();
                theMarker->setLineStyle(QwtPlotMarker::NoLine);
                theMarker->setSymbol(theSymbol);
                ratio = modelPrediction/observation;
                if (ratio>maxRatio) {
                        maxRatio = ratio;
                }
                theMarker->setValue(modelPrediction, ratio);
                theMarker->attach(theRatioPlot);
        }

        // Create flat curve at 1
        QwtPlotCurve *curve = new QwtPlotCurve();
        QPen thePen;
        thePen.setColor(QColor(Qt::black));
        curve->setPen(thePen);
        curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
        curve->attach(theRatioPlot);
        QVector<double> theFlatVector;
        QVector<double> theOneToOneVector;
        theOneToOneVector << 1.0 << 1.0;
        theFlatVector << minModel-0.1*(maxModel-minModel) << maxModel+0.1*(maxModel-minModel);
        curve->setData(theFlatVector, theOneToOneVector);

        theRatioPlot->setAxisScale(QwtPlot::xBottom, minModel-0.1*(maxModel-minModel) , maxModel+0.1*(maxModel-minModel));
        theRatioPlot->setAxisScale(QwtPlot::yLeft, 0.0, maxRatio+1.0);


        // Take away the legends
        QwtLegend *myLegend = theRatioPlot->legend();
        myLegend->clear();


        theRatioPlot->replot();
*/
}

void RModelInferenceDialog::createNormalityPlot(QVector<double> *mean_theta, double mean_sigma)
{
    /*
        RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent());

        // Create a plot
        QPointer<RPlotWidget> theNormalityPlot = new RPlotWidget(mainWindow);
        mainWindow->addSubWindow(theNormalityPlot, "Normality Plot", true);
        theNormalityPlot->setAxisTitle(QwtPlot::xBottom, "Normal theoretical quantiles");
        theNormalityPlot->setAxisTitle(QwtPlot::yLeft, "Normal data quantiles");


        // Create a random number generator
        const gsl_rng_type * T;
        gsl_rng * r;
        gsl_rng_env_setup();
        T = gsl_rng_default;
        r = gsl_rng_alloc(T);


        // Generate the outcome of a standard normal random variable
        //gsl_vector *normalEpsilons = gsl_vector_calloc(n);
        //for (int i=0; i<n; i++) {
        //	gsl_vector_set(normalEpsilons,i, gsl_ran_gaussian(r, 3.0));
        //}
        //gsl_sort_vector(normalEpsilons);


        // Compute X*theta, namely the model prediction
        gsl_vector *Xtheta = gsl_vector_calloc(n);
        gsl_blas_dgemv(CblasNoTrans, 1.0, X, mean_theta, 0.0, Xtheta);


        // Store the epsilon values in a vector
        gsl_vector *epsilons = gsl_vector_calloc(n);
        for (int i=0; i<n; i++) {
                gsl_vector_set(epsilons,i, gsl_vector_get(y,i) - gsl_vector_get(Xtheta,i));
        }
        gsl_sort_vector(epsilons);


        // Plot the points
        double normalEpsilon;
        QwtSymbol theSymbol;
        theSymbol.setStyle(QwtSymbol::Ellipse);
        for (int i=0; i<n; i++) {

                normalEpsilon = gsl_cdf_gaussian_Pinv(  ((i+1)/(double)n) , mean_sigma);

                QwtPlotMarker *theMarker = new QwtPlotMarker();
                theMarker->setLineStyle(QwtPlotMarker::NoLine);
                theMarker->setSymbol(theSymbol);
                //theMarker->setValue(gsl_vector_get(normalEpsilons,i), gsl_vector_get(epsilons,i));
                theMarker->setValue(normalEpsilon, gsl_vector_get(epsilons,i));
                theMarker->attach(theNormalityPlot);
        }


        // Create the zero to one curve
        QwtPlotCurve *curve = new QwtPlotCurve();
        QPen thePen;
        thePen.setColor(QColor(Qt::black));
        curve->setPen(thePen);
        curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
        curve->attach(theNormalityPlot);
        QVector<double> theZeroOneVector;
        double epsMin = gsl_vector_min(epsilons);
        double epsMax = gsl_vector_max(epsilons);
        theZeroOneVector << epsMin-0.2*(epsMax-epsMin) << epsMax+0.2*(epsMax-epsMin);
        curve->setData(theZeroOneVector, theZeroOneVector);

        // Take away the legends
        QwtLegend *myLegend = theNormalityPlot->legend();
        myLegend->clear();



        theNormalityPlot->replot();

        //gsl_vector_free(epsilons);
        //gsl_vector_free(normalEpsilons);
*/
}

void RModelInferenceDialog::createRandomVariablesAndModel(QVector<double> *mean_theta, QVector<double> *cov_theta, double mean_sigma, double cov_sigma)
{
    /*

        // Create the model parameters, theta
        RContinuousRandomVariable *rv;
        for (int i=0; i<k; i++) {

                QString myString = QString("theta%1").arg(i+1);
                theDomain->createObject(myString, "RContinuousRandomVariable", false);
                rv = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
                rv->setDistributionType("Normal (mean, stdv)");
                rv->setMean(QString("%1").arg(gsl_vector_get(mean_theta,i), 0, 'g', 14));
                rv->setStandardDeviation(QString("%1").arg(gsl_vector_get(cov_theta,i) / 100 *fabs(gsl_vector_get(mean_theta,i)), 0, 'g', 14)); // COV is divided by 100
        }

        // Create the model parameter, epsilon, and the random sigma
        theDomain->createObject("Epsilon", "RRandomVariableWithRandomParametersModel", false);
        RRandomVariableWithRandomParametersModel *epsilon = qobject_cast<RRandomVariableWithRandomParametersModel *>(theDomain->getLastAddedObject());

        theDomain->createObject("Sigma", "RContinuousRandomVariable", false);
        RContinuousRandomVariable *sigma = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        sigma->setDistributionType("Normal (mean, stdv)");
        sigma->setMean(QString("%1").arg(mean_sigma, 0, 'g', 14));
        sigma->setCoefficientOfVariation(QString("%1").arg(cov_sigma, 0, 'g', 14));

        theDomain->createObject("EpsilonUncertainty", "RContinuousRandomVariable", false);
        RContinuousRandomVariable *epsilonUncertainty = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
        epsilonUncertainty->setDistributionType("Normal (mean, stdv)");
        epsilonUncertainty->setMean(QString("%1").arg(0.0, 0, 'g', 14));
        epsilonUncertainty->setStandardDeviation(QString("%1").arg(1.0, 0, 'g', 14));

        theDomain->createObject("ZeroConstant", "RConstant", false);
        RConstant *ZeroConstant = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
        ZeroConstant->setCurrentValue(0.0);

        epsilon->setStandardNormalRandomVariable(epsilonUncertainty);
        epsilon->setParameterList("ZeroConstant, Sigma");


        // Create the explanatory variables (they are case specific, so set them as Constants equal to 1.0)
        RConstant *xVariable;
        for (int i=0; i<k; i++) {

                QString myString = QString("x%1").arg(i+1);
                theDomain->createObject(myString, "RConstant", false);
                xVariable = qobject_cast<RConstant *>(theDomain->getLastAddedObject());
                xVariable->setCurrentValue(1.0);
        }


        // Create a generic model (if a search has been carried out, then use the new explanatory functions)
        theDomain->createObject("theProbabilisticModel", "RAlgebraicExpressionModel", false);
        RAlgebraicExpressionModel *theExprModel = qobject_cast<RAlgebraicExpressionModel *>(theDomain->getLastAddedObject());
        QString theExpressionString = "theta1 * (";
        if (SearchCheckBox->isChecked()) {
                theExpressionString.append( stringList[0] + ")");
                for (int i=2; i<=k; i++) {
                        theExpressionString.append(QString(QString(" + theta%1 * (").arg(i) + stringList[i-1]) + ")");
                }
        }
        else {
                theExpressionString.clear();
                theExpressionString.append("theta1*x1");
                for (int i=2; i<=k; i++) {
                        theExpressionString.append(QString(" + theta%1*x%1").arg(i));
                }
        }
        theExpressionString.append(" + EpsilonResponse");
        theExprModel->setExpression(theExpressionString);
*/
}

void RModelInferenceDialog::searchForExplanatoryFunctions(QVector<double> *mean_theta, QVector<double> *cov_theta)
{
    /*
    // Create a matrix operations tool
    RMatrixOperations theMatrixOperations;


    QString plotTitle = "Before Search";
    createModelPredictionPlot(mean_theta, plotTitle);

    // Initial declarations
    double trialFunction;
    int indexOfWorstExplanatoryFunction;
    double valueOfWorstExplanatoryFunction;
    QVector< QVector<double> > correlationMatrix(k, QVector<double>(k));
    double mean_sigma = 0.0;
    double cov_sigma = 0.0;
    double R = 0.0;


    // Check if the first column is intercept
    bool firstColumnIsIntercept = true;
    for (int i=0; i<n; i++) {

        if ((*X)[i][0] != 1.0) {
            firstColumnIsIntercept = false;
            break;
        }
    }


    // Establish a new X matrix of explanatory functions
    QVector< QVector<double> > Xnew(n, QVector<double>(k));
    theMatrixOperations.scaleMatrix(1.0, *X, &Xnew);


    // Also have a trial X matrix to play around with
    QVector< QVector<double> > Xtrial(n, QVector<double>(k));


    // Set the combination vector (the vector of c-values)
    QVector<double> cValues(k);
    for (int i=0; i<k; i++) {
        cValues[i] = -power;
    }


    // Initialize the list of regressors
    if (!stringList.isEmpty()) {
        stringList.clear();
    }

    QString regressorString = "";
    for (int i=0; i<k; i++) {
        regressorString.clear();
        regressorString.append( QString("x%1").arg(i+1) );
        stringList << regressorString;
    }


    // Allocate vectors and matrices for the quick inference analysis
    gsl_matrix *XX = gsl_matrix_calloc(k,k);
    gsl_matrix *XXcopy = gsl_matrix_calloc(k, k);
    gsl_matrix *XXinv = gsl_matrix_calloc(k, k);
    gsl_matrix *XXinvX = gsl_matrix_calloc(k,n);
    gsl_vector *Xtheta = gsl_vector_calloc(n);
    gsl_vector *y_minus_Xtheta = gsl_vector_calloc(n);
    gsl_matrix *covarianceMatrix = gsl_matrix_calloc(k,k);
    gsl_vector *cov_theta_trial = gsl_vector_calloc(k);
    gsl_permutation *p = gsl_permutation_alloc(k);
    int signum = 0;
    double dot_product_y_minus_Xtheta;
    double s_squared;
    double multiplicationFactor;
    double determinant;
    double Xminimum;
    double Xmaximum;
    double covarianceValue;
    double cSum;
    int c;
    bool negativeCovariance;
    bool oneElementInXisOne;


    // Loop over all combinations
    int numCombinations = (int)gsl_pow_int(2*power+1,k); //Mojtaba: 5.0
    qDebug() << "*************************";
    qDebug() << " ";
    qDebug() << " ";
    qDebug() << "Number of combinations in search for explanatory functions:" << numCombinations;
    for (int m=0; m<numCombinations; m++) {
        //Mojtaba
        if (m % 50 == 0) {
            QCoreApplication::processEvents();
            //qDebug() << "Processing number" << m;
        }

        negativeCovariance = 0;
        oneElementInXisOne = 0;

        // Don't do the trial functions that are already in X
        cSum = 0.0;
        for (int i=0; i<k; i++) {
            cSum += gsl_vector_get(cValues, i);
        }
        for (int i=0; i<k; i++) {
            if (gsl_vector_get(cValues,i)==1.0) {
                oneElementInXisOne = 1;
                break;
            }
        }
        if (!(oneElementInXisOne && cSum==1.0)) {

            // Update the trial matrix
            gsl_matrix_memcpy(Xtrial, Xnew);

            // Identify the regressor with highest coefficient of variation
            indexOfWorstExplanatoryFunction = gsl_vector_max_index(cov_theta);
            valueOfWorstExplanatoryFunction = gsl_vector_get(cov_theta, indexOfWorstExplanatoryFunction);

            // Insert the candidate explanatory function into the trial X matrix
            for (int i=0; i<n; i++) {
                trialFunction = 1.0;
                for (int j=0; j<k; j++) {
                    trialFunction = trialFunction * pow( gsl_matrix_get(X,i,j) , gsl_vector_get(cValues,j) );
                }
                gsl_matrix_set( Xtrial, i, indexOfWorstExplanatoryFunction, trialFunction);
            }

            Xmaximum = gsl_matrix_max(Xtrial);
            Xminimum = gsl_matrix_min(Xtrial);
            if (Xminimum>1.0e-16 || Xmaximum<1.0e+16) {

                // Do a "quick inference analysis
                int nu = n-k;
                gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, Xtrial, Xtrial, 0.0, XX);
                gsl_matrix_memcpy(XXcopy, XX);
                signum = 0;
                gsl_linalg_LU_decomp(XXcopy, p, &signum);
                determinant = gsl_linalg_LU_det(XXcopy,signum);

                if (determinant != 0.0) {

                    gsl_linalg_LU_invert(XXcopy, p, XXinv);
                    gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1.0, XXinv, Xtrial, 0.0, XXinvX);
                    gsl_blas_dgemv(CblasNoTrans, 1.0, XXinvX, y, 0.0, mean_theta);
                    gsl_blas_dgemv(CblasNoTrans, 1.0, Xtrial, mean_theta, 0.0, Xtheta);
                    gsl_vector_memcpy(y_minus_Xtheta, y);
                    gsl_vector_sub(y_minus_Xtheta, Xtheta);
                    gsl_blas_ddot (y_minus_Xtheta, y_minus_Xtheta, &dot_product_y_minus_Xtheta);
                    s_squared = dot_product_y_minus_Xtheta/nu;
                    gsl_matrix_memcpy(covarianceMatrix, XXinv);
                    multiplicationFactor = nu*s_squared/(nu-2.0);
                    for (int i=0; i<k; i++) {
                        for (int j=0; j<k; j++) {
                            gsl_matrix_set(covarianceMatrix,i,j,   (multiplicationFactor*gsl_matrix_get(covarianceMatrix,i,j)));
                        }
                    }

                    for (int i=0; i<k; i++) {
                        covarianceValue = gsl_matrix_get(covarianceMatrix,i,i);
                        if (covarianceValue<0.0) {
                            negativeCovariance = 1;
                            break;
                        }
                        else {
                            gsl_vector_set(cov_theta_trial, i, fabs(100.0*(sqrt(covarianceValue))/gsl_vector_get(mean_theta,i)) );
                        }
                    }


                    if (!negativeCovariance) {

                        // Check the COV for the new variable
                        if (gsl_vector_get(cov_theta_trial,indexOfWorstExplanatoryFunction)<valueOfWorstExplanatoryFunction) {

                            // Update the matrix of regressor values
                            gsl_matrix_memcpy(Xnew, Xtrial);
                            gsl_vector_memcpy(cov_theta, cov_theta_trial);

                            // Update the string of regressors
                            stringList[indexOfWorstExplanatoryFunction].clear();
                            for (int i=0; i<k; i++) {
                                c = (int)gsl_vector_get(cValues,i);
                                if ((i==0) && (!firstColumnIsIntercept)) {
                                    if (c==0) {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString("1").arg(i+1) );
                                    }
                                    else if (c==1) {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString("x%1").arg(i+1) );
                                    }
                                    else if (c==-1) {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString("1/x%1").arg(i+1) );
                                    }
                                    else if (c<0) {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString("1/x%1^%2").arg(i+1).arg(-c) );
                                    }
                                    else {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString("x%1^%2").arg(i+1).arg(c) );
                                    }
                                }
                                else if ((i==0) && (firstColumnIsIntercept)) {
                                }
                                else if ((i==1) && (firstColumnIsIntercept)) {
                                    if (c==0) {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString("1").arg(i+1) );
                                    }
                                    else if (c==1) {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString("x%1").arg(i+1) );
                                    }
                                    else if (c==-1) {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString("1 / x%1").arg(i+1) );
                                    }
                                    else if (c<0) {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString("1 / x%1^%2").arg(i+1).arg(-c) );
                                    }
                                    else {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString("x%1^%2").arg(i+1).arg(c) );
                                    }
                                }
                                else {
                                    if (c==0) {
                                    }
                                    else if (c==1) {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString(" * x%1").arg(i+1) );
                                    }
                                    else if (c==-1) {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString(" / x%1").arg(i+1) );
                                    }
                                    else if (c<0) {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString(" / x%1^%2").arg(i+1).arg(-c) );
                                    }
                                    else {
                                        stringList[indexOfWorstExplanatoryFunction].append( QString(" * x%1^%2").arg(i+1).arg(c) );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Increment c-values
        incrementCombinationVector(cValues);
    }


    // Now the original X matrix is replaced by the new
    gsl_matrix_memcpy(X, Xnew);

    // Carry out a full inference analysis
    carryOutInference(mean_theta, cov_theta, correlationMatrix, mean_sigma, cov_sigma, R);

    // Print results
    qDebug() << "";
    qDebug() << "Regressors after search:";
    for (int i=0; i<k; i++) {
        qDebug() << stringList[i];
    }
    qDebug() << "";
    qDebug() << "Mean of the model parameters (after search):";
    theDomain->print(mean_theta);
    qDebug() << "";
    qDebug() << "Coefficient of variation (in percent) of the model parameters (after search):";
    theDomain->print(cov_theta);
    qDebug() << "";
    qDebug() << "Correlation matrix of the model parameters (after search):";
    theDomain->print(correlationMatrix);
    qDebug() << "";
    qDebug() << "R-factor (after search):" << R;
    qDebug() << "Mean of sigma (after search):" << mean_sigma;
    qDebug() << "";



    // Plot the quality of the new model
    plotTitle = "After Search";
    createModelPredictionPlot(mean_theta, plotTitle);


    gsl_permutation_free(p);
    gsl_vector_free(cValues);
    gsl_matrix_free(Xtrial);
    gsl_matrix_free(XX);
    gsl_matrix_free(XXcopy);
    gsl_matrix_free(XXinv);
    gsl_matrix_free(XXinvX);
    gsl_vector_free(Xtheta);
    gsl_vector_free(y_minus_Xtheta);
    gsl_matrix_free(covarianceMatrix);

    */

}

int RModelInferenceDialog::incrementCombinationVector(QVector<double> *cValues)
{
    /*

        for (int i=0; i<k; i++) {
                if (gsl_vector_get(cValues,i)==power) { //Mojtaba: 2.0
                        gsl_vector_set(cValues, i, -power );//Mojtaba: -2.0
                }
                else {
                        gsl_vector_set(cValues, i, (gsl_vector_get(cValues,i)+1.0) );
                        return 0;
                }
        }


        */
    return 0;
}
