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
*   - Stevan Gavrilovic                                              *
*   - Terje Haukaas                                                  *
*   - Rodrigo Costa                                                  *
*                                                                    *
*********************************************************************/

#include "RMainWindow.h"

#include <QApplication>
#include <QPointer>
#include <QPlainTextEdit>
#include <QSurfaceFormat>
#include <QDebug>

#ifdef GIS
#include "RGISWidget.h"
#include "ArcGISRuntimeEnvironment.h"
#endif

#ifdef _OPENMPI
#include <mpi.h>
#endif

QPointer<QPlainTextEdit> console;

// Custom message handler, which redirects standard output and standard error channels to the output pane
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!console) {
        return;
    }
    QString msgStr;
    switch (type) {
    case QtDebugMsg:
        console->appendPlainText(msg);
        break;
    case QtWarningMsg:
        break;
    case QtCriticalMsg:
        msgStr = QString("<font color=%1>").arg("red") + QString(msg) + QString("</font>") + QString("<font color=%1>").arg("black") + QString("&nbsp;") + QString("</font>");
        console->appendHtml(msgStr);
        break;
    case QtFatalMsg:
        msgStr = QString("<font color=%1>").arg("green") + QString(msg) + QString("</font>") + QString("<font color=%1>").arg("black") + QString("&nbsp;") + QString("</font>");
        console->appendHtml(msgStr);
        abort();
    }
    console->ensureCursorVisible();
}


int main(int argc, char *argv[])
{
    // Qt settings needed for modern GL rendering, must be declared before QApplication
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(0);
    // format.setSamples(4);    // Set the number of samples used for multisampling
#ifdef Q_OS_UNIX
    format.setProfile(QSurfaceFormat::CoreProfile);
#else
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
#endif
    QSurfaceFormat::setDefaultFormat(format);

    QApplication theApplication(argc, argv);

    // Set the name and version of the application
    theApplication.setApplicationName("Rts");
    theApplication.setApplicationVersion("2.0");
    theApplication.setOrganizationName("UBC");
    theApplication.setOrganizationDomain("http://www.inrisk.ubc.ca");

    // Needed for the GIS widget
#ifdef GIS

#ifdef Q_OS_WIN
    // Force usage of OpenGL ES through ANGLE on Windows
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
#endif

    Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::setLicense("runtimelite,1000,rud7787112932,none,GB2PMD17JYJ1R6EJM240");

#endif

    // Declare the main window
    RMainWindow theMainWindow;

    // Install the custom message handler that is defined above
    console = theMainWindow.findChild<QPlainTextEdit *>();
    qInstallMessageHandler(myMessageOutput);
    theMainWindow.show();

    // Establish signal-slot connections
    theApplication.connect(&theApplication, SIGNAL(lastWindowClosed()), &theApplication, SLOT(quit()));

    // Open the input files that are passed in argv
    if (argc == 1) {
        theMainWindow.clearAllObjects(false); //Comment this out ***** To automatically load last file
    } else if (argc >= 2) {
        theMainWindow.clearAllObjects(false); //Comment this out ***** To automatically load last file
        for (int i = 1; i < argc; i++) {
            // theMainWindow.openFile(argv[i]);
        }
    }


    // Set up MPI for parallel solving
#ifdef _OPENMPI
    // int numP; // number of processes started
    // int me;  // the integer id of the process .. 0 through numP-1

    MPI_Init(&argc, &argv);
    // MPI_Comm_rank(MPI_COMM_WORLD, &me);
    // MPI_Comm_size(MPI_COMM_WORLD, &numP);

    // qDebug()<<"Number of MPI Processes: "<<numP;
#endif

    // Run the Rt application
    return theApplication.exec();
}

