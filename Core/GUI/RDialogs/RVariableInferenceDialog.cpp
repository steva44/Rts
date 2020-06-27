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

#include "RVariableInferenceDialog.h"
#include "RMainWindow.h"
#include "RDomain.h"
#include "RContinuousRandomVariable.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QtDebug>

#include "math.h"

RVariableInferenceDialog::RVariableInferenceDialog(QWidget *parent, RDomain *passedDomain)
: QDialog(parent)
{
	setupUi(this);

	theDomain = passedDomain;
	X = 0;

	connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}


RVariableInferenceDialog::~RVariableInferenceDialog()
{
	deleteVectors();
}


void RVariableInferenceDialog::browse()
{
	// Get the file name from the user using QFileDialog
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Input file"), QDir::currentPath(), "All Files (*.*)");
	if (fileName.isEmpty()) {
		return;
	}
	lineEdit->setText(fileName);
}


void RVariableInferenceDialog::setDomain(RDomain *passedDomain)
{
	theDomain = passedDomain;
}


void RVariableInferenceDialog::onOkButtonClicked()
{	
    /*


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
    // Terje removed a comment insert in the following if-statement
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

			X = gsl_matrix_calloc(size, columnCount);
		} else {
			if (columnCount != valueList.count()) {
				qCritical() << "Error: The number of data at the line" << i + 1 << "is different than the line 1 in the database file.";
				deleteVectors();
				return;
			}
		}

		for (int j = 0; j < valueList.count(); j++) {
			QString tempStr = valueList[j].trimmed();
			bool ok;
			double tempValue = tempStr.toDouble(&ok);
			if (!ok) {
				qCritical() << "Error: Could not read the value number" << j + 1 << "at the line" << i + 1 << "in the database file.";
				deleteVectors();
				return;
			}

			// The first value in the line is the output

			gsl_matrix_set(X, i, j, tempValue);
		}
	}

	file.close();


	// Execute the 'analyze' method
	int result = this->analyze();
	if (result < 0) {
		qCritical() << "Error: The variable inference analysis did not converge because of errors.";
		return;
	}
	mainWindow->updateTree("Domain");


    */
}


int RVariableInferenceDialog::analyze()
{
    /*

	// Inform the user that the sampling analysis has started
	qDebug() << "##### STARTING VARIABLE INFERENCE ANALYSIS ...";
	qDebug() << "";


	// Start recording the time
	QDateTime startTime = QDateTime::currentDateTime();


	// Get a hold of the main window, for plotting purposes
	RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent());


	// Get number of random variables and observations
	int N = X->size1;
	int numRVs = X->size2;

	// Allocate a vector that will be used in the single-variable inference
	gsl_vector *variableVector = gsl_vector_calloc(N);

	// Allocate vectors to store means and standard deviations
	gsl_vector *means = gsl_vector_calloc(numRVs);
	gsl_vector *stdvs = gsl_vector_calloc(numRVs);

	// Allocate vectors to store min and max values
	gsl_vector *mins = gsl_vector_calloc(numRVs);
	gsl_vector *maxs = gsl_vector_calloc(numRVs);


	for (int j=0; j<numRVs; j++) {

		// Collect the variable vector from X
		for (int i=0; i<N; i++) {
			gsl_vector_set(variableVector,i,gsl_matrix_get(X,i,j));
		}

		// Get max and min values for later plots
		gsl_vector_set(mins,j,(gsl_vector_min(variableVector)));
		gsl_vector_set(maxs,j,(gsl_vector_max(variableVector)));


		// Create a plot
		QPointer<RPlotWidget> thePlot = new RPlotWidget(mainWindow);
		QString str = QString("Cumulative Frequency Diagram for Column %1").arg(j+1);
		mainWindow->addSubWindow(thePlot, str, true);
		thePlot->setAxisTitle(QwtPlot::xBottom, "Random Variable");
		thePlot->setAxisTitle(QwtPlot::yLeft, "Cumulative Frequency");


		// Create the x and y vectors and the curve to be plotted
		QVector<double> xValues;
		QVector<double> yValues;
		QwtPlotCurve *theCDFcurve = new QwtPlotCurve(QString("Data"));


		// Set properties of the line and attach it to the plot
		QPen thePen;
		thePen.setColor(QColor(Qt::black));
		theCDFcurve->setPen(thePen);
		theCDFcurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
		theCDFcurve->attach(thePlot);


		// Sort the elements of the vector into ascending order
		gsl_sort_vector(variableVector);


		// Compute the sum, the sum squared, and send data points to the plot curve
		double value;
		double vectorSum = 0.0;
		double vectorSumSquared = 0.0;
		for (int i=0; i<N; i++) {
			value = gsl_vector_get(variableVector,i);
			vectorSum += value;
			vectorSumSquared += value*value;
			xValues << value;
			yValues << (i+1)/(double)N;
		}
		theCDFcurve->setData(xValues, yValues);


		// Compute the mean (and create a QString with its value)
		double mean = vectorSum/(double)N;
		gsl_vector_set(means, j, mean);


		// Compute the variance
		double variance = 1.0/(N-1.0) * (vectorSumSquared - N * gsl_pow_2(mean));


		// Compute the standard deviation
		double stdv = sqrt(variance);
		gsl_vector_set(stdvs, j, stdv);


		// Print results to the screen
		qDebug() << "";
		qDebug() << "The mean of the data in column" << (j+1) << "is:" << mean;
		qDebug() << "The standard deviation of the data in column" << (j+1) << "is:" << stdv;
		qDebug() << "Consequently, the coefficient of variation (in percent) the data in column" << (j+1) << "is:" << 100.0*stdv/mean;


		// For the next steps, get the max and min of the vector
		double vectorMin = gsl_vector_min(variableVector);
		double vectorMax = gsl_vector_max(variableVector);
		int numPoints = 30;


		// Declare a random variable to be used in the following plotting
		RContinuousRandomVariable theRV(0, "theRandomVariable");
		theRV.setMean(QString("%1").arg(mean, 0, 'g', 14));
		theRV.setStandardDeviation(QString("%1").arg(stdv, 0, 'g', 14));
		QVector<double> xDistrValues;
		QVector<double> yDistrValues;
		for (int i=0; i<numPoints+1; i++) {
			value = vectorMin + (vectorMax-vectorMin)*i/(double)numPoints;
			xDistrValues << value;
		}


		// Add a NORMAL line to the plot
		if (NormalPlotCheckBox->isChecked()) {
			yDistrValues.clear();
			theRV.setDistributionType("Normal (mean, stdv)");
			QwtPlotCurve *theNormalCDFcurve = new QwtPlotCurve(QString("Normal"));
			QPen theNormalPen;
			theNormalPen.setColor(QColor(Qt::red));
			theNormalCDFcurve->setPen(theNormalPen);
			theNormalCDFcurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
			theNormalCDFcurve->attach(thePlot);
			for (int i=0; i<numPoints+1; i++) {
				yDistrValues << theRV.getCDFValue(xDistrValues[i]);
			}
			theNormalCDFcurve->setData(xDistrValues, yDistrValues);
		}


		// Add a LOGNORMAL line to the plot
		if (LognormalPlotCheckBox->isChecked()) {
			yDistrValues.clear();
			theRV.setDistributionType("Lognormal (zeta, sigma)");
			QwtPlotCurve *theLognormalCDFcurve = new QwtPlotCurve(QString("Lognormal"));
			QPen theLognormalPen;
			theLognormalPen.setColor(QColor(Qt::blue));
			theLognormalCDFcurve->setPen(theLognormalPen);
			theLognormalCDFcurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
			theLognormalCDFcurve->attach(thePlot);
			for (int i=0; i<numPoints+1; i++) {
				yDistrValues << theRV.getCDFValue(xDistrValues[i]);
			}
			theLognormalCDFcurve->setData(xDistrValues, yDistrValues);
		}


		// Add a EXPONENTIAL line to the plot
		if (ExponentialPlotCheckBox->isChecked()) {
			yDistrValues.clear();
			theRV.setDistributionType("Exponential (mu, xo)");
			QwtPlotCurve *theExponentialCDFcurve = new QwtPlotCurve(QString("Exponential"));
			QPen theExponentialPen;
			theExponentialPen.setColor(QColor(Qt::darkRed));
			theExponentialCDFcurve->setPen(theExponentialPen);
			theExponentialCDFcurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
			theExponentialCDFcurve->attach(thePlot);
			for (int i=0; i<numPoints+1; i++) {
				yDistrValues << theRV.getCDFValue(xDistrValues[i]);
			}
			theExponentialCDFcurve->setData(xDistrValues, yDistrValues);
		}


		// Add a UNIFORM line to the plot
		if (UniformPlotCheckBox->isChecked()) {
			yDistrValues.clear();
			theRV.setDistributionType("Uniform (a, b)");
			QwtPlotCurve *theUniformCDFcurve = new QwtPlotCurve(QString("Uniform"));
			QPen theUniformPen;
			theUniformPen.setColor(QColor(Qt::green));
			theUniformCDFcurve->setPen(theUniformPen);
			theUniformCDFcurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
			theUniformCDFcurve->attach(thePlot);
			for (int i=0; i<numPoints+1; i++) {
				yDistrValues << theRV.getCDFValue(xDistrValues[i]);
			}
			theUniformCDFcurve->setData(xDistrValues, yDistrValues);
		}

		// Add a LAPLACE line to the plot
		if (LaplacePlotCheckBox->isChecked()) {
			yDistrValues.clear();
			theRV.setDistributionType("Laplace (a, xo)");
			QwtPlotCurve *theLaplaceCDFcurve = new QwtPlotCurve(QString("Laplace"));
			QPen theLaplacePen;
			theLaplacePen.setColor(QColor(Qt::darkGreen));
			theLaplaceCDFcurve->setPen(theLaplacePen);
			theLaplaceCDFcurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
			theLaplaceCDFcurve->attach(thePlot);
			for (int i=0; i<numPoints+1; i++) {
				yDistrValues << theRV.getCDFValue(xDistrValues[i]);
			}
			theLaplaceCDFcurve->setData(xDistrValues, yDistrValues);
		}


		// Add a RAYLEIGH line to the plot
		if (RayleighPlotCheckBox->isChecked()) {
			yDistrValues.clear();
			theRV.setDistributionType("Rayleigh (sigma, xo)");
			QwtPlotCurve *theRayleighCDFcurve = new QwtPlotCurve(QString("Rayleigh"));
			QPen theRayleighPen;
			theRayleighPen.setColor(QColor(Qt::darkBlue));
			theRayleighCDFcurve->setPen(theRayleighPen);
			theRayleighCDFcurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
			theRayleighCDFcurve->attach(thePlot);
			for (int i=0; i<numPoints+1; i++) {
				yDistrValues << theRV.getCDFValue(xDistrValues[i]);
			}
			theRayleighCDFcurve->setData(xDistrValues, yDistrValues);
		}


		// Add a GAMMA line to the plot
		if (GammaPlotCheckBox->isChecked()) {
			yDistrValues.clear();
			theRV.setDistributionType("Gamma (a, b)");
			QwtPlotCurve *theGammaCDFcurve = new QwtPlotCurve(QString("Gamma"));
			QPen theGammaPen;
			theGammaPen.setColor(QColor(Qt::cyan));
			theGammaCDFcurve->setPen(theGammaPen);
			theGammaCDFcurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
			theGammaCDFcurve->attach(thePlot);
			for (int i=0; i<numPoints+1; i++) {
				yDistrValues << theRV.getCDFValue(xDistrValues[i]);
			}
			theGammaCDFcurve->setData(xDistrValues, yDistrValues);
		}


		// Add a CHI SQUARE line to the plot
		if (ChiSquarePlotCheckBox->isChecked()) {
			yDistrValues.clear();
			theRV.setDistributionType("Chi-square (nu, xo)");
			QwtPlotCurve *theChiSquaredCDFcurve = new QwtPlotCurve(QString("Chi-square"));
			QPen theChiSquaredPen;
			theChiSquaredPen.setColor(QColor(Qt::magenta));
			theChiSquaredCDFcurve->setPen(theChiSquaredPen);
			theChiSquaredCDFcurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
			theChiSquaredCDFcurve->attach(thePlot);
			for (int i=0; i<numPoints+1; i++) {
				yDistrValues << theRV.getCDFValue(xDistrValues[i]);
			}
			theChiSquaredCDFcurve->setData(xDistrValues, yDistrValues);
		}


		// Add a LOGISTIC line to the plot
		if (LogisticPlotCheckBox->isChecked()) {
			yDistrValues.clear();
			theRV.setDistributionType("Logistic (a, xo)");
			QwtPlotCurve *theLogisticCDFcurve = new QwtPlotCurve(QString("Logistic"));
			QPen theLogisticPen;
			theLogisticPen.setColor(QColor(Qt::darkMagenta));
			theLogisticCDFcurve->setPen(theLogisticPen);
			theLogisticCDFcurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
			theLogisticCDFcurve->attach(thePlot);
			for (int i=0; i<numPoints+1; i++) {
				yDistrValues << theRV.getCDFValue(xDistrValues[i]);
			}
			theLogisticCDFcurve->setData(xDistrValues, yDistrValues);
		}


		// Add a GUMBEL line to the plot
		if (GumbelPlotCheckBox->isChecked()) {
			yDistrValues.clear();
			theRV.setDistributionType("Gumbel (a, b)");
			QwtPlotCurve *theGumbelCDFcurve = new QwtPlotCurve(QString("Gumbel"));
			QPen theGumbelPen;
			theGumbelPen.setColor(QColor(Qt::gray));
			theGumbelCDFcurve->setPen(theGumbelPen);
			theGumbelCDFcurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
			theGumbelCDFcurve->attach(thePlot);
			for (int i=0; i<numPoints+1; i++) {
				yDistrValues << theRV.getCDFValue(xDistrValues[i]);
			}
			theGumbelCDFcurve->setData(xDistrValues, yDistrValues);
		}


		// Display the plot
		thePlot->replot();
	}


	// Compute correlation matrix
	if (numRVs>1) {
		double productSum;
		double value;
		double mini, maxi, minj, maxj;
		gsl_matrix *R = gsl_matrix_calloc(numRVs, numRVs);
		for (int i=0; i<numRVs; i++) {
			gsl_matrix_set(R,i,i, 1.0);
		}
		for (int i=0; i<numRVs; i++) {
			for (int j=0; j<i; j++) {

				// Create scatter plots
				QPointer<RPlotWidget> thePlot = new RPlotWidget(mainWindow);
				QString str = QString("Scatter Plot for Columns %1 and %2").arg(j+1).arg(i+1);
				mainWindow->addSubWindow(thePlot, str, true);
				thePlot->setAxisTitle(QwtPlot::xBottom, QString("Data in Column %1").arg(j+1));
				thePlot->setAxisTitle(QwtPlot::yLeft, QString("Data in Column %1").arg(i+1));

				QwtSymbol theSymbol;
				theSymbol.setStyle(QwtSymbol::Ellipse);

				for (int k=0; k<N; k++) {
					QwtPlotMarker *theMarker = new QwtPlotMarker();
					theMarker->setLineStyle(QwtPlotMarker::NoLine);
					theMarker->setSymbol(theSymbol);
					theMarker->setValue(gsl_matrix_get(X,k,j), gsl_matrix_get(X,k,i));
					theMarker->attach(thePlot);
				}

				mini = gsl_vector_get(mins,i);
				maxi = gsl_vector_get(maxs,i);
				minj = gsl_vector_get(mins,j);
				maxj = gsl_vector_get(maxs,j);

				thePlot->setAxisScale(QwtPlot::yLeft, mini-0.1*(maxi-mini), maxi+0.1*(maxi-mini));
				thePlot->setAxisScale(QwtPlot::xBottom, minj-0.1*(maxj-minj), maxj+0.1*(maxj-minj));


				// Compute the correlation coefficient
				productSum = 0.0;
				for (int k=0; k<N; k++) {
					productSum += gsl_matrix_get(X,k,i) * gsl_matrix_get(X,k,j);
				}
				value = 1.0/(N-1.0) * (productSum - N * gsl_vector_get(means,i) * gsl_vector_get(means,j)  ) / (gsl_vector_get(stdvs,i) * gsl_vector_get(stdvs,j));
				gsl_matrix_set(R,i,j, value);
				gsl_matrix_set(R,j,i, value);
			}
		}
		qDebug() << "";
		qDebug() << "The correlation matrix is:";
		theDomain->print(R);
		gsl_matrix_free(R);
	}


	// Free vectors/matrices
	gsl_vector_free(mins);
	gsl_vector_free(maxs);
	gsl_vector_free(variableVector);
	gsl_vector_free(means);
	gsl_vector_free(stdvs);


	QDateTime finishTime = QDateTime::currentDateTime();
	double numDays = startTime.daysTo(finishTime);
	double numMilliSecs = startTime.time().msecsTo(finishTime.time());
	double numSecs = numDays * 86400.0 + numMilliSecs / 1000.0;

	qDebug() << "";
	qDebug() << "##### VARIABLE INFERENCE ANALYSIS DONE IN" << numSecs << "SECONDS.";
	qDebug() << "";
	qDebug() << "";
*/



	return 0;
}


void RVariableInferenceDialog::deleteVectors()
{
}
