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
*   - Stevan Gavrilovic                                              *
*   - Rodrigo Costa                                                  *
*                                                                    *
*********************************************************************/

#include "RMainWindow.h"
#include "RPropertyWidget.h"
#include "RObjectWidget.h"
#include "RAggregationObjectWidget.h"
#include "RAboutDialog.h"
#include "RDomain.h"
#include "RParameter.h"
#include "REuclidsAlgorithm.h"
#include "RLocation.h"
#ifdef HAZARDS
#include "RPolygonalAreaSourceModel.h"
#endif
#ifdef GIS
#include "RGISWidget.h"
#include "RFixedNetworkModel.h"
#include "RModelManager.h"
#include "RObject.h"
#include "RRefineryModel.h"
#include "RGenericStorageFacilityModel.h"
#include "RFuelPipelineModel.h"
#include "RBoundedExponentialMagnitudeModel.h"
#endif
#ifdef OSG
#include "ROSGOrbitCameraManipulator.h"
#include "ROSGSystem.h"
#include "ROSGPlotter.h"
#endif
#ifdef IFC
#include "RBIM.h"
#include "TabReadWrite.h"
#include "IfcPPObject.h"
#include "IfcPPModel.h"
#include "IfcPlusPlusSystem.h"
#endif

#include <QtGui>
#include <QDockWidget>
#include <QMdiArea>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QProgressBar>
#include <QPlainTextEdit>
#include <QMdiSubWindow>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <qcustomplot.h>

bool RMainWindow::noWarning = false;

RMainWindow::RMainWindow(QWidget *parent) : QMainWindow(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    setAcceptDrops(true);
    createCentralWidget();
    createActions();
    createMenus();
    showHideToolBars();
    createStatusBar();
    createDockWindows();
    createConnections();
    readSettings(false);
    propertiesDock->raise();
    //setWindowIcon(QIcon(":RIcon/RtsIcon.iconset/icon_256x256.png"));   // Changed by Terje
    setWindowTitle(QApplication::applicationName());
    theDomain = nullptr;
    closeWithoutWarning = true; // Changed by Terje
    resetView(); // Added by Terje

#ifdef OSG
    OSGSystem = nullptr;
    theOSGWidget = nullptr;
    theOrbitCameraManipulator = nullptr;
    thePlotter = std::make_unique<ROSGPlotter>(this);
#endif

#ifdef IFC
    RIFCTabReadWrite = nullptr;
    theIFCSystem = nullptr;
    osgWidgetIFC = nullptr;
#endif

    // Keep this below for testing - when the code below is on, it automatically loads the last previous file when some code in RMain is commented out as well.
    // resetDomain();
    // QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    // QStringList files = settings.value("recentFiles/recentFileList").toStringList();
    // if(!files.empty())
    //     openFile(files[0]);

    // createGLViewWidget();
}


RMainWindow::~RMainWindow()
{

}


RDomain* RMainWindow::getDomain(void)
{
    return theDomain;
}


void RMainWindow::help()
{
    qDebug() << "*****************************************************************";
    qDebug() << "To obtain information about a partiular object, right-click on it";
    qDebug() << "and select Help.";
    qDebug() << "*****************************************************************";
}


void RMainWindow::createConnections()
{
    QList<RObjectWidget *> objectWidgetList = findChildren<RObjectWidget *>();
    for (int i = 0; i < objectWidgetList.count(); i++) {
        connect(objectWidgetList[i], SIGNAL(currentObjectChanged(QString &)), propertyList, SLOT(viewProperties(QString &)));
        connect(objectWidgetList[i], SIGNAL(currentObjectChanged(QString &)), this, SLOT(raisePropertieDock()));
    }

}


void RMainWindow::createCentralWidget()
{
    mdiArea = new QMdiArea(this);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setDocumentMode(true);
    setCentralWidget(mdiArea);
    //centralWidget()->setMinimumSize(400, 300);

    QTabBar *tabBar = this->findChild<QTabBar *>();
    tabBar->setTabsClosable(true);
}


void RMainWindow::createActions()
{
    clearAllObjectAction = new QAction(tr("&Clear All"), this);
    //clearAllObjectAction->setIcon(QIcon(":/RImages/new.png"));
    clearAllObjectAction->setShortcut(tr("Ctrl+N"));
    clearAllObjectAction->setStatusTip(tr("Clears all objects."));
    connect(clearAllObjectAction, SIGNAL(triggered()), this, SLOT(clearAllObjects()));

    openAction = new QAction(tr("&Open..."), this);
    //openAction->setIcon(QIcon(":/RImages/open.png"));
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip(tr("Opens a file."));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save..."), this);
    //saveAction->setIcon(QIcon(":/RImages/save.png"));
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(tr("Saves a file."));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    exitAction = new QAction(tr("E&xit"), this);
    //exitAction->setIcon(QIcon(":/RImages/exit.png"));
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exits the application."));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    cutAction = new QAction(tr("&Cut Output"), this);
    //cutAction->setIcon(QIcon(":/RImages/cut.png"));
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("Cuts the selected text in the output pane."));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cutText()));

    copyAction = new QAction(tr("C&opy Output"), this);
    //copyAction->setIcon(QIcon(":/RImages/copy.png"));
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip(tr("Copies the selected text in the output pane."));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyText()));

    pasteAction = new QAction(tr("&Paste Output"), this);
    //pasteAction->setIcon(QIcon(":/RImages/paste.png"));
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("Pastes the clipboard text in the output pane."));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(pasteText()));

    deleteAllAction = new QAction(tr("Clear Output"), this);
    //deleteAllAction->setIcon(QIcon(":/RImages/remove.png"));
    deleteAllAction->setStatusTip(tr("Deletes the entire text in the output pane."));
    connect(deleteAllAction, SIGNAL(triggered()), this, SLOT(deleteAllText()));

    // Terje
    expandTreesAction = new QAction(tr("Expand Object Tree"), this);
    expandTreesAction->setStatusTip(tr("Expands all the branches of the object trees."));
    connect(expandTreesAction, SIGNAL(triggered()), this, SLOT(expandAllTrees()));

    collapseTreesAction = new QAction(tr("Collapse Object Tree"), this);
    collapseTreesAction->setStatusTip(tr("Closes all the branches of the object trees."));
    connect(collapseTreesAction, SIGNAL(triggered()), this, SLOT(collapseAllTrees()));

    viewMapAction = new QAction(tr("View &Map"), this);
    //viewMapAction->setIcon(QIcon(":/RImages/map.png"));
    viewMapAction->setStatusTip(tr("Opens the Google Maps."));
    connect(viewMapAction, SIGNAL(triggered()), this, SLOT(viewMap()));

    overlayImageOnMapAction = new QAction(tr("&Overlay Image on Map"), this);
    overlayImageOnMapAction->setStatusTip(tr("Overlays an image on the Google Maps."));
    connect(overlayImageOnMapAction, SIGNAL(triggered()), this, SLOT(overlayImageOnMap()));

    createLocationsFromMapAction = new QAction(tr("&Create Locations from Map"), this);
    createLocationsFromMapAction->setStatusTip(tr("Creates multiple locations by clicking on the Google Maps."));
    connect(createLocationsFromMapAction, SIGNAL(triggered()), this, SLOT(createLocationsFromMap()));

    toggleAllPanesAction = new QAction(tr("Toggle All"), this);
    //toggleAllPanesAction->setIcon(QIcon(":/RImages/showhideallpanes.png"));
    toggleAllPanesAction->setStatusTip(tr("Shows/hides all the panes."));
    connect(toggleAllPanesAction, SIGNAL(triggered()), this, SLOT(togglePanes()));

    toggleAllToolBarsAction = new QAction(tr("Toggle All"), this);
    //toggleAllToolBarsAction->setIcon(QIcon(":/RImages/showhidealltoolbars.png"));
    toggleAllToolBarsAction->setStatusTip(tr("Shows/hides all the panes."));
    connect(toggleAllToolBarsAction, SIGNAL(triggered()), this, SLOT(showHideToolBars()));

    resetViewAction = new QAction(tr("&Reset View"), this);
    //resetViewAction->setIcon(QIcon(":/RImages/resetview.png"));
    resetViewAction->setStatusTip(tr("Resets the position of menus."));
    connect(resetViewAction, SIGNAL(triggered()), this, SLOT(resetView()));

    setDefaultViewAction = new QAction(tr("&Set Default View"), this);
    //setDefaultViewAction->setIcon(QIcon(":/RImages/setdefaultview.png"));
    setDefaultViewAction->setStatusTip(tr("Sets the current positions of menus as default."));
    connect(setDefaultViewAction, SIGNAL(triggered()), this, SLOT(setDefaultView()));

    tileWindowsAction = new QAction(tr("&Tile Windows"), this);
    //tileWindowsAction->setIcon(QIcon(":/RImages/tilewindows.png"));
    tileWindowsAction->setStatusTip(tr("Tiles windows."));
    connect(tileWindowsAction, SIGNAL(triggered()), this, SLOT(tileWindows()));

    cascadeWindowsAction = new QAction(tr("&Cascade Windows"), this);
    //cascadeWindowsAction->setIcon(QIcon(":/RImages/cascadewindows.png"));
    cascadeWindowsAction->setStatusTip(tr("Cascades windows."));
    connect(cascadeWindowsAction, SIGNAL(triggered()), this, SLOT(cascadeWindows()));

    closeAllAction = new QAction(tr("&Close Windows"), this);
    //closeAllAction->setIcon(QIcon(":/RImages/close.png"));
    closeAllAction->setStatusTip(tr("Closes all windows."));
    connect(closeAllAction, SIGNAL(triggered()), this, SLOT(closeAll()));

    helpAction = new QAction(tr("&Help"), this);
    //helpAction->setIcon(QIcon(":/RImages/help.png"));
    helpAction->setShortcut(tr("F1"));
    helpAction->setStatusTip(tr("Opens the InRisk website for help."));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));

    aboutAction = new QAction(tr("&About ")+QApplication::applicationName()+"...", this);
    //aboutAction->setIcon(QIcon(":/RImages/icon.png"));
    aboutAction->setStatusTip(tr("Shows the application about box."));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

}


void RMainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(clearAllObjectAction);
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    recentFilesMenu = fileMenu->addMenu(tr("&Recent Files"));
    //recentFilesMenu->setIcon(QIcon(":/RImages/recentfiles.png"));
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFilesMenu->addAction(recentFileActions[i]);
    }
    updateRecentFileActions();
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAllAction);


    viewMenu = menuBar()->addMenu(tr("&View"));
    // Terje viewMenu->addAction(viewMapAction);
    // Terje viewMenu->addAction(overlayImageOnMapAction);
    // Terje viewMenu->addAction(createLocationsFromMapAction);
    // Terje viewMenu->addSeparator();
    panesMenu = viewMenu->addMenu(tr("Toggle &Panes"));
    panesMenu->addAction(toggleAllPanesAction);
    panesMenu->addSeparator();

    /*
    toolBarsMenu = viewMenu->addMenu(tr("&Toggle Toolbars"));
    toolBarsMenu->addAction(toggleAllToolBarsAction);
    toolBarsMenu->addSeparator();
    viewMenu->addSeparator();
    */

    viewMenu->addSeparator();
    viewMenu->addAction(resetViewAction);
    viewMenu->addAction(setDefaultViewAction);

    viewMenu->addSeparator();
    viewMenu->addAction(collapseTreesAction);
    viewMenu->addAction(expandTreesAction);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    windowMenu->addAction(tileWindowsAction);
    windowMenu->addAction(cascadeWindowsAction);
    windowMenu->addSeparator();
    windowMenu->addAction(closeAllAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);
}


void RMainWindow::closeEvent(QCloseEvent *event)
{
    if (!closeWithoutWarning) {
        int response = QMessageBox::question(this, tr("Confirmation"), tr("Are you sure you want to exit the program?"), QMessageBox::Yes | QMessageBox::Default, QMessageBox::No);
        if (response == QMessageBox::Yes){
            writeSettings(false);
            event->accept();
        } else{
            event->ignore();
        }
    } else {
        event->accept();
    }
}


void RMainWindow::createStatusBar()
{

#ifdef OSG
    statusBarOne = new QStatusBar(this);
    statusBarOne->showMessage("Ready");
    setStatusBar( statusBarOne );
#endif

    // RStatusCursorLabel2->setText("Ready");
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(1000);
    statusBar()->addPermanentWidget(progressBar);
    //progressBar->resize(progressBar->width(), progressBar->height() * 3 / 4.0);
    statusBar()->removeWidget(progressBar);

}


void RMainWindow::createDockWindows()
{
    setDockOptions(AnimatedDocks | AllowTabbedDocks /*| VerticalTabs*/ | AllowNestedDocks);
    setDockNestingEnabled(true);

    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);

    const char *objectsStr = "Objects";
    objectsDock = new QDockWidget(tr(objectsStr), this);
    objectsDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    objectsDock->setObjectName(objectsStr);
    objectsTree = new RObjectWidget(objectsDock);
    objectsDock->setWidget(objectsTree);
    objectsTree->createHeader(objectsStr);
    addDockWidget(Qt::LeftDockWidgetArea, objectsDock);
    panesMenu->addAction(objectsDock->toggleViewAction());
    objectsDock->raise();

    const char *propertiesStr = "Properties";
    propertiesDock = new QDockWidget(tr(propertiesStr), this);
    propertiesDock->setObjectName(propertiesStr);
    propertiesDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    propertyList = new RPropertyWidget(propertiesDock);
    propertiesDock->setWidget(propertyList);
    addDockWidget(Qt::BottomDockWidgetArea, propertiesDock);
    panesMenu->addAction(propertiesDock->toggleViewAction());

    const char *outputStr = "Output";
    consoleDock = new QDockWidget(tr(outputStr), this);
    consoleDock->setObjectName(outputStr);
    consoleDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    console = new QPlainTextEdit(consoleDock);
    console->setReadOnly(false);
    console->setLineWrapMode(QPlainTextEdit::NoWrap);
    consoleDock->setWidget(console);
    QFont consoleFont = console->font();
    //consoleFont.setFamily("Courier New");
    consoleFont.setFamily("Lucida Console");
    console->setFont(consoleFont);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);
    panesMenu->addAction(consoleDock->toggleViewAction());

    //this->tabifyDockWidget(propertiesDock, consoleDock);
    propertiesDock->raise();

}


void RMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}


void RMainWindow::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls()) {
        QList<QUrl> urlList = event->mimeData()->urls();
        for (int i = 0; i < urlList.count(); i++) {
            openFile(urlList[i].toLocalFile());
        }
    }
}


void RMainWindow::addSubWindow(QWidget *widget, QString windowTitle, bool tiled)
{
    QMdiSubWindow *subWindow = getSubWindow(widget);
    if (!subWindow) {
        subWindow = mdiArea->addSubWindow(widget);

        // Set the parent of the widget of the subWindow, so that you can access
        // the subWindow from within the widget using parentWidget() method to, e.g., resize the subwindow from within the plot widget
        //widget->setParent(subWindow);

        double width = centralWidget()->width();
        double height = centralWidget()->height();
        double factor = 0.8;
        double x = (1 - factor) * width / 2;
        double y = (1 - factor) * height / 2;
        subWindow->setGeometry(x, y, factor * width, factor * height);

        subWindow->setAttribute(Qt::WA_DeleteOnClose);

        widget->setWindowTitle(windowTitle);

        //subWindow->setWindowTitle(windowTitle);

        emit subWindowAdded(widget);

    }

    subWindow->showMaximized();

    mdiArea->setActiveSubWindow(subWindow);

    QList<QMdiSubWindow *> subWindowList = mdiArea->subWindowList();

    if ((tiled) && (subWindowList.count() > 1)) {
        mdiArea->tileSubWindows();
    } else {
        subWindow->showMaximized();
        //        widget->showMaximized();
    }
}


QMdiSubWindow * RMainWindow::getSubWindow(QWidget *widget)
{
    QList<QMdiSubWindow *> subWindowList = mdiArea->subWindowList();
    for (int i = 0; i < subWindowList.count(); i++) {
        if (subWindowList[i]->widget() == widget) {
            return subWindowList[i];
        }
    }

    return nullptr;
}


void RMainWindow::setActiveSubWindow(QWidget *widget)
{
    QMdiSubWindow *subWindow = getSubWindow(widget);

    if (subWindow) {

        mdiArea->setActiveSubWindow(subWindow);
    }
}


void RMainWindow::closeActiveSubWindow()
{
    QMdiSubWindow *subWindow = mdiArea->activeSubWindow();

    mdiArea->closeActiveSubWindow();
    if (subWindow) {
        delete subWindow;
    }
}


void RMainWindow::closeAll()
{
    QList<QMdiSubWindow *> subWindowList = mdiArea->subWindowList();
    if (subWindowList.count() == 0) {
        return;
    }
    int response = QMessageBox::Yes;//QMessageBox::question(this, tr("Confirmation"), tr("Are you sure you want to close all windows?"), QMessageBox::Yes | QMessageBox::Default, QMessageBox::No);
    if (response == QMessageBox::Yes) {

        QListIterator<QMdiSubWindow *> subWinList(subWindowList);
        while (subWinList.hasNext())
        {
            subWinList.next()->close();
        }

    }
}


void RMainWindow::readSettings(bool defaultFlag)
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    int desktopWidth = QApplication::desktop()->width();     // returns desktop width
    int desktopHeight = QApplication::desktop()->height();    // returns desktop height
    setMinimumSize(0.5*desktopWidth, 0.5*desktopHeight);
    double ratio = 0.7;
    QString pathStr;
    if (defaultFlag) {
        pathStr = "view/default/";
    }
    else {
        pathStr = "view/";
    }
    //if (!(settings.contains("default/pos"))) {QTimer::singleShot(1000, this, SLOT(writeSettings(true)));}
    QPoint pos = settings.value(pathStr+"pos", QPoint(0.5*desktopWidth*(1-ratio), 0.5*desktopHeight*(1-ratio))).toPoint();
    QSize size = settings.value(pathStr+"size", QSize(ratio*desktopWidth, ratio*desktopHeight)).toSize();
    bool firstTime = !(settings.contains(pathStr+"maximized"));
    if (!firstTime) {
        bool maximized = settings.value(pathStr+"maximized", false).toBool();
        if (maximized == true) {
            showMaximized();
        }
        else {
            showNormal();
            move(pos);
            resize(size);
        }
    }
    //    else {
    //        showMaximized();
    //    }
    QByteArray state = settings.value(pathStr+"toolbars").toByteArray();
    restoreState(state,0);
    //centralWidget()->restoreGeometry(settings.value(pathStr+"geometry").toByteArray());
}


void RMainWindow::writeSettings(bool defaultFlag)
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    QString pathStr;
    if (defaultFlag) {
        pathStr = "view/default/";
    }
    else {
        pathStr = "view/";
    }
    settings.setValue(pathStr+"pos", pos());
    settings.setValue(pathStr+"size", size());
    settings.setValue(pathStr+"maximized", isMaximized());
    settings.setValue(pathStr+"toolbars", saveState(0));
    //settings.setValue(pathStr+"geometry", centralWidget()->saveGeometry());
}


void RMainWindow::resetView()
{
    readSettings(true);
}


void RMainWindow::clearAllObjects(bool showDialogBox)
{
    closeAll();

    resetDomain();

    collapseTree("Objects");

    console->clear();

    theDomain->setCurrentFilePath(QApplication::applicationDirPath());

#ifdef OSG
    this->clearGLViewWidget();
#endif
}


void RMainWindow::closePropertyBox()
{

    if(propertiesDock)
    {
        propertiesDock->close();
    }

}


void RMainWindow::closeOutputBox()
{

    if(consoleDock)
    {
        consoleDock->close();
    }

}


void RMainWindow::open()
{
    // Get the file name from the user using QFileDialog
    QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("Open Rt Input Files"), QDir::currentPath(), "Text Files (*.txt);;All Files (*.*)");
    if (fileNameList.isEmpty()) {
        return;
    }
    // Open files

    for (int i = 0; i < fileNameList.count(); i++) {
        openFile(fileNameList[i]);
    }
}


void RMainWindow::openFile(QString fileName, bool StInputFile)
{

    //If it is a typical rts "text" input file
    if (fileName.contains(".txt"))
    {
        openRtsFile(fileName, StInputFile);

    }

    //Else its a RtsIFC file
    else if (fileName.contains(".ifc") ||fileName.contains(".stp") )
    {

#ifdef IFC
        //closePropertyBox();

        //closeOutputBox();

        theDomain->createObject(this->objectName() + "BIM","RBIM");
        RBIM* theBIM = qobject_cast<RBIM*>(theDomain->getLastAddedObject());
        theBIM->setIfcFile(fileName);

        this->updateTree("tree");
        updateRecentFileList(fileName);
        //openIFCFile(fileName);
#endif IFC

    }

    //Else the file type is not supported
    else
    {
        QMessageBox::critical(this, tr("Error"), QString("Could not open; file %1 type is not supported").arg(fileName), QMessageBox::Ok | QMessageBox::Default);
    }

}


void RMainWindow::updateApplicationTitle(QString currentFileName)
{
    QString windowTitle = currentFileName + " - " + QApplication::applicationName();
    setWindowTitle(windowTitle);
}


void RMainWindow::resetDomain()
{
    //Things that should be done in case of "New" or "Open" file
    if (theDomain) {
        delete theDomain;
        theDomain = 0;
    }
    QString name = "Domain";
    theDomain = new RDomain(this, name);
    propertyList->setDomain(theDomain);
    objectsTree->setDomain(theDomain);

    objectsTree->updateView();
    propertyList->removeAll();

}


void RMainWindow::onFileOpenError(QObjectList *objectList, QString oldFilePath)
{
    for (int i = 0; i < objectList->count(); i++) {
        delete objectList->value(i);
    }
    // Set the current file path of the program to the oldFilePath
    theDomain->setCurrentFilePath(RDomain::getAbsoluteFolderPath(oldFilePath));

    QApplication::restoreOverrideCursor();
    statusBar()->removeWidget(progressBar);
    statusBar()->showMessage("File open operation failed.", 2000);

    RMainWindow::noWarning = false;
}


void RMainWindow::viewMap()
{



}


void RMainWindow::collapseAllTrees()
{
    collapseTree("Objects");
}


void RMainWindow::expandAllTrees()
{
    expandTree("Objects");
}


void RMainWindow::save()
{
    // Get the file name from the user using QFileDialog
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save an Rt Input File"), QDir::currentPath(), "Text Files (*.txt)");
    if (fileName.isEmpty()) {
        return;
    }

    if (QFileInfo(fileName).suffix().toLower() != QString("txt").toLower()) {
        fileName += ".txt";
    }

    // Open the file
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        // Error
        QMessageBox::critical(this, tr("Error"), QString("Could not create the file %1.").arg(fileName), QMessageBox::Ok | QMessageBox::Default);
        return;
    }
    // Set cursor status to wait
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Save the file
    QObjectList objectList = theDomain->findChildren<QObject *>();

    int count = objectList.count();
    statusBar()->addPermanentWidget(progressBar);
    progressBar->show();
    //progressBar->resize(progressBar->width(), progressBar->height() * 3 / 4.0);

    for (int i = 0; i < count; i++) {
        progressBar->setValue(i / count * (progressBar->maximum() - progressBar->minimum()));

        // A line to keep the GUI responsive
        QCoreApplication::processEvents();

        QObject *object = objectList[i];

        RObject *rObject = qobject_cast<RObject *>(object);
        if (!rObject) {
            qCritical() << "Error in saving file!";
        }
        if (rObject->isAutoGenerated()) {
            continue;
        }

        QString string = object->metaObject()->className();

        for (int i = 0; i < object->metaObject()->propertyCount(); i++) {
            QMetaProperty metaProperty = object->metaObject()->property(i);
            QString propertyName = metaProperty.name();
            QString propertyValue = theDomain->getProperty(object, propertyName);

            if ((propertyName == QString("Type")) || (propertyValue.isEmpty()) || !metaProperty.isWritable()) {
                continue;
            }

            string += " |" + (propertyList->propertyToName(propertyName)).remove(' ') + ": " + propertyValue;
        }

        QList<QByteArray> dynamicPropertyList = object->dynamicPropertyNames();
        for (int i = 0; i < dynamicPropertyList.count(); i++) {
            QString propertyName = QString(dynamicPropertyList[i]);
            QString propertyValue = theDomain->getProperty(object, propertyName);

            string += " |" + (propertyList->propertyToName(propertyName)).remove(' ') + ": " + propertyValue;
        }

        string += "\n";

        QByteArray byteArray;
        byteArray.append(string);

        file.write(byteArray);
        file.flush();
    }

    // Close the file
    file.close();

    updateRecentFileList(fileName);

    // Update the file path of the application
    theDomain->setCurrentFilePath(RDomain::getAbsoluteFolderPath(fileName));

    // Restore the cursor to
    QApplication::restoreOverrideCursor();

    statusBar()->removeWidget(progressBar);
    statusBar()->showMessage("File saved.", 2000);
}


void RMainWindow::about()
{
    RAboutDialog *dialog = new RAboutDialog(this);
    if (dialog->exec()) { }
    delete dialog;
}


void RMainWindow::togglePanes()
{
    bool hideFlag = true;
    QList<QDockWidget *> dockWidgetsList = findChildren<QDockWidget *>();
    for (int i = 0; i < dockWidgetsList.count(); i++) {
        if (dockWidgetsList[i]->isHidden()) {
            hideFlag = false;
        }
    }
    for (int i = 0; i < dockWidgetsList.count(); i++) {
        dockWidgetsList[i]->setHidden(hideFlag);
    }
}


void RMainWindow::showHideToolBars()
{
    bool hideFlag = true;
    QList<QToolBar *> toolBarList = findChildren<QToolBar *>();
    for (int i = 0; i < toolBarList.count(); i++) {
        if (toolBarList[i]->isHidden()) {
            hideFlag = false;
        }
    }
    for (int i = 0; i < toolBarList.count(); i++) {
        toolBarList[i]->setHidden(hideFlag);
    }
}


void RMainWindow::setDefaultView()
{
    writeSettings(true);
}


void RMainWindow::onMapIsNotOpenError()
{
    viewMap();
    QMessageBox::information(nullptr, tr("Information"), tr("The map is not open. Rts will now open the map. Please try again when the loading of the map is complete"), QMessageBox::Ok | QMessageBox::Default);
}


void RMainWindow::overlayImageOnMap()
{
}


void RMainWindow::createLocationsFromMap()
{
}


void RMainWindow::raiseDock(QString dockName)
{
    QList<QDockWidget *> dockWidgetlist = this->findChildren<QDockWidget *>();
    for (int i = 0; i < dockWidgetlist.count(); i++) {
        if (dockWidgetlist[i]->objectName() == dockName) {
            dockWidgetlist[i]->raise();
            return;
        }
    }
}


void RMainWindow::raisePropertieDock()
{
    propertiesDock->raise();
}


void RMainWindow::updateTree(QString treeName)
{
    objectsTree->updateView();
    objectsTree->expandPopulatedBranches();
}


void RMainWindow::collapseTree(QString treeName)
{
    objectsTree->collapseAll();
}


void RMainWindow::expandTree(QString treeName)
{
    objectsTree->expandAll();
}


void RMainWindow::updatePropertyList()
{
    propertyList->updateView();
}


void RMainWindow::tileWindows()
{
    mdiArea->tileSubWindows();
}


void RMainWindow::cascadeWindows()
{
    mdiArea->cascadeSubWindows();
}


void RMainWindow::cutText()
{
    console->cut();
}


void RMainWindow::copyText()
{
    console->copy();
}


void RMainWindow::pasteText()
{
    console->paste();
}


void RMainWindow::deleteAllText()
{
    console->clear();
}


void RMainWindow::updateRecentFileActions()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    QStringList files = settings.value("recentFiles/recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());
        recentFileActions[i]->setText(text);
        recentFileActions[i]->setData(files[i]);
        recentFileActions[i]->setVisible(true);
    }
    for (int i = numRecentFiles; i < MaxRecentFiles; ++i) {
        recentFileActions[i]->setVisible(false);
    }
}


void RMainWindow::updateRecentFileList(QString newFileName)
{
    QString fileName = RDomain::stringToFilePath(newFileName);

    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    QString key = "recentFiles/recentFileList";
    QStringList files = settings.value(key).toStringList();
    files.removeAll(newFileName);
    files.prepend(newFileName);
    while (files.size() > MaxRecentFiles) {
        files.removeLast();
    }

    settings.setValue(key, files);
    updateRecentFileActions();
}


void RMainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        openFile(action->data().toString());
    }
}


QObjectList RMainWindow::openRtsFile(QString fileName, bool StInputFile)
{

    QRegExp regExp("([[//]%#])");
    QChar propertyChar('|');
    QChar defineChar(':');

    // Get the list of dynamic QProperties that has been used in all classes from the domain
    QStringList dynamicPropertyList = RDomain::getDynamicPropertyList();

    int result = 0;

    // Open the file
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly /*| QFile::Text*/)) {
        // Error
        QMessageBox::critical(this, tr("Error"), QString("Could not open the file %1.").arg(fileName), QMessageBox::Ok | QMessageBox::Default);
        return QObjectList();
    }

    QString oldPath = theDomain->getCurrentFilePath();
    // Change the current path of the program
    if (!StInputFile) {
        theDomain->setCurrentFilePath(RDomain::getAbsoluteFolderPath(fileName));
    }
    // Set cursor status to wait
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Delete the current domain and re-instantiate it
    //resetDomain();

    // Read the file content into a QString
    QTextStream stream(&file);
    stream.seek(0);

    // Tokenize the QString of file contents by "\n" and create a string list of input file lines
    QString verticalRegExpString = QString("[\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator));
    QRegExp verticalRegExp = QRegExp(verticalRegExpString);

    QStringList fileContentStringList;
    QString line;
    QStringList lineList;
    stream.setAutoDetectUnicode(true);
    do {
        line = stream.readLine();
        lineList = line.split(verticalRegExp/*, QString::SkipEmptyParts*/);
        fileContentStringList.append(lineList);
    } while (!line.isNull());



    statusBar()->addPermanentWidget(progressBar);
    progressBar->show();
    //progressBar->resize(progressBar->width(), progressBar->height() * 3 / 4.0);

    QList<QStringList> propertyNamesList;
    QList<QStringList> propertyValuesList;
    QObjectList objectList;

    // A list to store pointer to RParameters that are used in the St input file
    QObjectList stParameterList;

    // A string list to store a list of commands, such as "Analyze", "Output", or "Exit"
    QStringList commandStringList;

    // Inform theDomain that an input file is being opened
    theDomain->setInputFileBeingOpened(true);

    // First stage: Instantiate objects
    int count = fileContentStringList.count();

    // Loop over the string list of all lines in the input file
    for (int i = 0; i < count; i++) {
        progressBar->setValue(i / 2.0 / count * (progressBar->maximum() - progressBar->minimum()));

        // A line to keep the GUI responsive
        QCoreApplication::processEvents();

        // Reading the current line
        QString syntax = fileContentStringList[i].trimmed();
        fileContentStringList[i] = syntax;

        // Ignore empty lines
        if (syntax.isEmpty()) {
            continue;
        }

        // Ignore the comment lines (starting with //, %, and #)
        if ((syntax.startsWith("//")) || (syntax.startsWith('%')) || (syntax.startsWith('#')) || syntax.isEmpty()) {
            //fileContentStringList.removeAt(i);
            //i--;
            continue;
        }


        if (syntax.startsWith("Analyze") || syntax.startsWith("Output") || syntax.startsWith("Exit")) {
            commandStringList << syntax;
            continue;
        }

        // Split the current line from the locations of propertyChar, which is "|"
        QStringList parameterList = syntax.split(propertyChar, QString::SkipEmptyParts);

        // The first part after the split is the object's base class name
        QString objectClass = parameterList[0].trimmed();
        if (objectClass.isEmpty()) {
            // Error
            QMessageBox::critical(this, tr("Error"), QString("Error in line %1: The object defined at this line does not have the object type specified.").arg(i+1), QMessageBox::Ok | QMessageBox::Default);
            onFileOpenError(&objectList, oldPath);
            return QObjectList();
        }

        QString objectName;

        QStringList propertyNameList;
        QStringList propertyValueList;

        bool hasObjectName = false;

        // Loop over splitted parts of this line
        for (int j = 1; j < parameterList.count(); j++) {
            QString parameter = parameterList[j].trimmed();

            // Split this part form the location of the first occurrence of the character defineChar (':')
            int separatorIndex = parameter.indexOf(defineChar);
            if (separatorIndex < 0) {
                // Error
                QMessageBox::critical(this, tr("Error"), QString("Error in line %1: Could not read the properties of the object due to incorrect arguments.").arg(i+1), QMessageBox::Ok | QMessageBox::Default);
                onFileOpenError(&objectList, oldPath);
                return QObjectList();
            }

            // Find the property name and its value
            propertyNameList += parameter.left(separatorIndex).trimmed().remove(' ');
            propertyValueList += parameter.right(parameter.length() - separatorIndex - 1).trimmed();

            if (propertyNameList[j-1] == QString("ObjectName")) {
                objectName = propertyValueList[j-1];
                result = theDomain->nameIsValid(objectName, false);
                if (result < 0) {
                    // Error
                    onFileOpenError(&objectList, oldPath);
                    return QObjectList();
                }
                if (!objectName.isEmpty()) {
                    hasObjectName = true;
                }
            }
        }
        propertyNamesList.append(propertyNameList);
        propertyValuesList.append(propertyValueList);

        if (!hasObjectName) {
            // Error
            QMessageBox::critical(this, tr("Error"), QString("Error in line %1: The object defined at this line does not have an object name specified.").arg(i+1), QMessageBox::Ok | QMessageBox::Default);
            onFileOpenError(&objectList, oldPath);
            return QObjectList();
        }

        // Instantiate the object
        if ((!StInputFile) || ((StInputFile) && (objectClass != QString("RConstant")))) {
            result = theDomain->createObject(objectName, objectClass, false);
        } else {
            // Parameters should not be opened in St input files
            if ((StInputFile) && (objectClass == QString("RConstant"))) {
                RParameter *parameter = theDomain->findChild<RParameter *>(objectName);
                if (parameter) {
                    stParameterList.append(parameter);
                }
                propertyNamesList.takeLast();
                propertyValuesList.takeLast();
            }
            continue;
        }

        if (result < 0) {
            // Error
            QMessageBox::critical(this, tr("Error"), QString("Error in line %1: Could not create the object called %2 due to errors.").arg(i+1).arg(objectName), QMessageBox::Ok | QMessageBox::Default);
            onFileOpenError(&objectList, oldPath);
            return QObjectList();
        }
        objectList.append(theDomain->getLastAddedObject());
    }
    // Now, all lines in the input file are looped over

    // Inform theDomain that the operation of opening an input file is over
    theDomain->setInputFileBeingOpened(false);

    // Checking for object name duplicates
    QObjectList allObjectList;
    if (!StInputFile) {
        allObjectList = theDomain->findChildren<QObject *>();
    } else {
        // In case of opening an St file, do the name checking only for recently instantiated St objects.
        allObjectList = objectList;
    }
    QMap<QString, QObject *> map;
    for (int i = 0; i < allObjectList.count(); i++) {
        if (map.contains(allObjectList[i]->objectName())) {
            // Error
            QMessageBox::critical(this, tr("Error"), QString("Error: Object with the name \"%1\" already exists.").arg(allObjectList[i]->objectName()), QMessageBox::Ok | QMessageBox::Default);
            onFileOpenError(&objectList, oldPath);
            return QObjectList();
        } else {
            map[allObjectList[i]->objectName()] = allObjectList[i];
        }
    }

    RMainWindow::noWarning = true;

    // Second stage: Assign QProperties
    count = objectList.count();
    // Loop over the objectList
    for (int i = 0; i < count; i++) {
        progressBar->setValue((i / 2.0 / count + 0.5) * (progressBar->maximum() - progressBar->minimum()));

        // A line to keep the GUI responsive
        QCoreApplication::processEvents();

        QObject *object = objectList[i];
        if (!object) {
            // Error
            QMessageBox::critical(this, tr("Error"), QString("Could not find the object \"%2\" in the domain.").arg(object->objectName()), QMessageBox::Ok | QMessageBox::Default);
            onFileOpenError(&objectList, oldPath);
            return QObjectList();
        }

        QStringList propertyNameList = propertyNamesList[i];
        QStringList propertyValueList = propertyValuesList[i];

        // Loop over splitted parts of this line
        for (int j = 0; j < propertyNameList.count(); j++) {

            if (propertyNameList[j] == QString("ObjectName")) {
                continue;
            }

            QString propertyName = propertyNameList[j];

            // Set the property
            int result = theDomain->setProperty(object, propertyNameList[j], propertyValueList[j]);
            bool isStaticProperty = !(result);

            // If the property that has just been set is not a static (hard-coded) property and is not listed in the list of dynamic properties, then issue and error
            if ((!isStaticProperty) && (!dynamicPropertyList.contains(propertyNameList[j]))) {
                QByteArray byteArrayName = propertyNameList[j].toLatin1();
                const char *propertyNameChar = byteArrayName.data();
                object->setProperty(propertyNameChar, QVariant());

                // Error
                QMessageBox::critical(this, tr("Error"), QString("The object \"%2\" has a problem with the property called \"%3\".").arg(object->objectName()).arg(propertyNameList[j])+" Also, check the registry to make sure the classes match the key.", QMessageBox::Ok | QMessageBox::Default);
                onFileOpenError(&objectList, oldPath);
                return QObjectList();
            }
        }
    }
    // Now, all object in the objectList are looped over

    RMainWindow::noWarning = false;


    if (!StInputFile) {
        updateRecentFileList(fileName);

        // Refresh the object trees and expand them
        objectsTree->updateView();

        objectsTree->expandPopulatedBranches();

        // Change the current directory path of the program to the path of the input file
        //QString currentTitle = RDomain::stringToFilePath(fileName);
        //updateApplicationTitle(RDomain::getFileName(currentTitle));
        theDomain->setCurrentFilePath(RDomain::getAbsoluteFolderPath(fileName));
    } else {
        // In case of opening St input files, append stParameterList to objectList
        objectList.append(stParameterList);
    }

    // Restore the cursor to
    QApplication::restoreOverrideCursor();

    statusBar()->removeWidget(progressBar);
    statusBar()->showMessage("File opened.", 2000);

    // Third stage: run commands
    QRegExp horizontalRegExp("[ \t]");
    for (int i = 0; i < commandStringList.count(); i++) {
        if (commandStringList[i].startsWith("Analyze")) {

            /*
            QStringList commandOptionsList = commandStringList[i].split(horizontalRegExp, QString::SkipEmptyParts);
            if (commandOptionsList.count() != 2) {
                qCritical() << "Error: The command" << commandStringList[i] << "is not recognized and is not run";
                continue;
            }
            RAnalyzer *analyzer = theDomain->findChild<RAnalyzer *>(commandOptionsList[1]);
            if (!analyzer) {
                qCritical() << "Error: The command" << commandStringList[i] << "is not recognized and is not run";
                continue;
            }
            int status = analyzer->analyze();
            if (status < 0) {
                qCritical() << "Error in" << analyzer->objectName() << ": The analysis did not converge because of errors";
            }
            */

        } else if (commandStringList[i].startsWith("Output")) {
            //closeWithoutWarning = true;
            //this->close();

            // Dumping the entire output pane in a text file
            QStringList commandOptionsList = commandStringList[i].split('|', QString::SkipEmptyParts);
            if (commandOptionsList.count() < 2) {
                continue;
            }
            QString outputFileName = commandOptionsList[1];
            QFile *outputFile = new QFile(outputFileName);
            if (outputFile->exists()) {
                outputFile->remove();
            }
            if (!outputFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
                qCritical() << "Warning: Could not create the file" << outputFileName << "to store the Rt output before exiting";
            }

            QByteArray consoleText = console->toPlainText().toLatin1();
            outputFile->write(consoleText);
            outputFile->flush();

            outputFile->close();
            delete outputFile;
        } else if (commandStringList[i].startsWith("Exit")) {
            //closeWithoutWarning = true;
            //this->close();

            exit(0);
        }
    }

    return objectList;
}


#ifdef IFC
void RMainWindow::openIFCFile(QString fileName)
{ 
    // Initializes the IFC docks and trees
    initializeIFC(false);

    // Open the file
    QFile file(fileName);

    if (file.open(QFile::ReadOnly /*| QFile::Text*/))
    {
        // Change the current path of the program
        theDomain->setCurrentFilePath(RDomain::getAbsoluteFolderPath(fileName));

        // QString path(arg1.c_str());
        RIFCTabReadWrite->slotLoadIfcFile( fileName );

        // Refresh the object trees and expand them
        objectsTree->updateView();

        objectsTree->expandPopulatedBranches();

        // Change the current directory path of the program to the path of the input file
        theDomain->setCurrentFilePath(RDomain::getAbsoluteFolderPath(fileName));
    }
    else
    {
        // Error
        QMessageBox::critical(this, tr("Error"), QString("Could not open the file %1.").arg(fileName), QMessageBox::Ok | QMessageBox::Default);
    }

}
#endif


#ifdef OSG
void RMainWindow::createGLViewWidget()
{
    // Return if it already exists
    if(OSGSystem != nullptr)
        return;

    OSGSystem = new ROSGSystem(this);
    theOrbitCameraManipulator = new ROSGOrbitCameraManipulator(OSGSystem.data());

    OSGSystem->setOrbitCameraManipulator(theOrbitCameraManipulator.get());

    // Create an OSG Widget to display 3D main scene
    theOSGWidget = new OSGWidget(this,OSGSystem->getRootNode(),theOrbitCameraManipulator.get(),Qt::WindowType::SubWindow);

    connect(theOSGWidget, SIGNAL(destroyed()), this, SLOT(clearGLViewWidget()));

    // Assign attributes to OSG Widget
    theOSGWidget->getMainView()->addEventHandler(OSGSystem);

    addSubWindow(theOSGWidget,"Rts",true);

}


void RMainWindow::clearGLViewWidget()
{
    thePlotter->clearPlotter();
}


OSGWidget* RMainWindow::getGLViewWidget()
{
    // Create an OSG Widget to display 3D main scene
    return theOSGWidget;
}


ROSGSystem* RMainWindow::getOSGSystem()
{
    return OSGSystem.data();
}


ROSGPlotter* RMainWindow::getOSGPlotter()
{
    return thePlotter.get();
}
#endif


#ifdef GIS
void RMainWindow::createGISWidget()
{

    if (!theGISMapWidget){
        theGISMapWidget = new RGISWidget(this);

        addSubWindow(theGISMapWidget, QString("Map"));

        theGISMapWidget->openMap();
        theGISMapWidget->show();
    }


    // This is another way of plotting the map
    /*
    //Create the split window
    if(!splitWindowGIS)
    {
        splitWindowGIS = new QSplitter( Qt::Vertical );
        splitWindowGIS->setContentsMargins( 0, 0, 0, 0 );
        splitWindowGIS->addWidget( theGISMapWidget );
        addSubWindow(splitWindowGIS.data(),"Rts",false);
    }
    else
    {
        splitWindowGIS->addWidget( theGISMapWidget );
        addSubWindow(splitWindowGIS.data(),"Rts",false);
    }
    */
}


void RMainWindow::displayLocationsOnMap()
{
    // If the map was not previously created, create it
    if (!theGISMapWidget){
        this->createGISWidget();
    }


    // Get a list of all the models
    QList<RFixedNetworkModel *> modelList = theDomain->findChildren<RFixedNetworkModel *>();
    RLocation *theLocationsAsPointer;

    for (int i = 0; i < modelList.size(); i++){

        // Get the location of the model
        theLocationsAsPointer = qobject_cast<RLocation *>(modelList.at(i)->getLocation());
        QVector<double> theCoordinates;

        // If the model has a location, pass it to the map widget
        if (theLocationsAsPointer){

            theCoordinates << theLocationsAsPointer->getLatitude() << theLocationsAsPointer->getLongitude();
            theGISMapWidget->createPoint(theCoordinates, modelList.at(i));
        }
    }
}

void RMainWindow::displayAvailabilityOnMap()
{
    // If the map was not previously created, create it
    if (!theGISMapWidget){
        this->createGISWidget();
    }


    // This method works but it is still a work-in-progress
    // Get a list of all the models
    QList<RFixedNetworkModel *> modelList = theDomain->findChildren<RFixedNetworkModel *>();
    RLocation *theLocationsAsPointer;

    for (int i = 0; i < modelList.size(); i++){

        RRefineryModel *theStorageModel = qobject_cast<RRefineryModel *>(modelList.at(i));
        RFuelPipelineModel *thePipeline = qobject_cast<RFuelPipelineModel *>(modelList.at(i));

        if (!theStorageModel && !thePipeline){

            bool myAvailability = modelList.at(i)->getAvailability();

            // Get the location of the model
            theLocationsAsPointer = qobject_cast<RLocation *>(modelList.at(i)->getLocation());
            QVector<double> theCoordinates;

            // If the model has a location, pass it to the map widget
            if (theLocationsAsPointer){

                theCoordinates << theLocationsAsPointer->getLatitude() << theLocationsAsPointer->getLongitude();
                theGISMapWidget->createPoint(theCoordinates, modelList.at(i), myAvailability);
            }
        }
    }
}


void RMainWindow::displayEQOccurrence(QVector<double> theCoordinates)
{
    // If the map was not previously created, create it
    if (!theGISMapWidget){
        this->createGISWidget();
    }


    RBoundedExponentialMagnitudeModel *theMagnitudeModel = theDomain->findChild<RBoundedExponentialMagnitudeModel *>();

    if (theMagnitudeModel){
        // Get the magnitude as string
    }

    theGISMapWidget->createLabelledPoint(theCoordinates, QString("red"), QString("Hypocentre"));
}


void RMainWindow::displayNetworkLinks()
{
    // If the map was not previously created, create it
    if (!theGISMapWidget){
        this->createGISWidget();
    }


    // Get a list of all models
    QList<RNetworkModel *> listOfModels = theDomain->findChildren<RNetworkModel *>();

    for (int i = 0; i < listOfModels.size(); i++){

        // Creating the vector to receive the coordinates
        QVector<QVector<double>> coordinates;

        // Getting the location of the first model
        RLocation *theLocationOftheFirstModel = qobject_cast<RLocation *>(listOfModels.at(i)->getLocation());

        if (theLocationOftheFirstModel){
            // Passing the coordinates of the first model to the vector that will create the polyline
            QVector<double> theCoordinatesOfTheFirstModel;
            theCoordinatesOfTheFirstModel << theLocationOftheFirstModel->getLatitude() << theLocationOftheFirstModel->getLongitude();
            coordinates << theCoordinatesOfTheFirstModel;


            // Getting hold of all the responses coming in to this model
            QList<QPointer<RParameter> > theResponseList = listOfModels.at(i)->getInputResponseList();

            if (theResponseList.size() > 0){
                for (int j = 0; j < theResponseList.size(); j++){

                    // Cast the response parameter as a response object
                    RResponse *theResponseObject = qobject_cast<RResponse *>(theResponseList.at(j));

                    // Get the model of the response object as a network model
                    RNetworkModel *theSecondModel = qobject_cast<RNetworkModel *>(theResponseObject->getModel());

                    // Get the location of the second model
                    RLocation *theLocationOfTheSecondModel = qobject_cast<RLocation *>(theSecondModel->getLocation());

                    // Pass the coordinates of the second model to the vector that will create the polyline
                    QVector<double> theCoordinatesOfTheSecondModel;
                    theCoordinatesOfTheSecondModel << theLocationOfTheSecondModel->getLatitude() << theLocationOfTheSecondModel->getLongitude();
                    coordinates << theCoordinatesOfTheSecondModel;

                    // Create the polyline
                    theGISMapWidget->createPolyline(coordinates, theSecondModel);

                    // Clear the second model coordinates from the coordinates vector
                    coordinates.removeAt(1);
                }
            }
        }
    }
}

void RMainWindow::displayHazardAreaSources()
{
    /*
    QVector<QVector<double>> boundaries;
    RPolygonalAreaSourceModel *theAreaSource = theDomain->findChild<RPolygonalAreaSourceModel *>();
    QString theBoundaryPointsList = theAreaSource->getBoundaryLocationList();
    QList<QPointer<RParameter>> theBoundariesAsParametersList = stringToParameterList(theBoundaryPointsList);
    QList<RLocation *> theBoundariesAsPointersList = parameterListToLocationList(theBoundariesAsParametersList, true);

    for (int i = 0; i < theBoundariesAsPointersList.size(); i++){
        QVector<double> theBoundariesAsCoordinates;
        theBoundariesAsCoordinates << theBoundariesAsPointersList.at(i)->getLatitude() << theBoundariesAsPointersList.at(i)->getLongitude();
        boundaries << theBoundariesAsCoordinates;
        theGISMapWidget->createPolygon(boundaries);
    }
    */


    /*
    QVector<double> CASR_Point01;
    QVector<double> CASR_Point02;
    QVector<double> CASR_Point03;
    QVector<double> CASR_Point04;
    QVector<double> CASR_Point05;
    QVector<double> CASR_Point06;
    QVector<double> CASR_Point07;
    QVector<double> CASR_Point08;
    QVector<double> CASR_Point09;
    QVector<double> CASR_Point10;

    CASR_Point01 << 51.900 << -123.600;
    CASR_Point02 << 51.200 << -122.000;
    CASR_Point03 << 48.700 << -120.000;
    CASR_Point04 << 47.000 << -120.000;
    CASR_Point05 << 47.000 << -124.030;
    CASR_Point06 << 47.560 << -123.900;
    CASR_Point07 << 48.030 << -124.230;
    CASR_Point08 << 49.540 << -126.710;
    CASR_Point09 << 49.740 << -126.400;
    CASR_Point10 << 49.910 << -126.710;

    QVector<QVector<double>> CASR;
    CASR << CASR_Point01 << CASR_Point02 << CASR_Point03 << CASR_Point04 << CASR_Point05 << CASR_Point06 << CASR_Point07 << CASR_Point08 << CASR_Point09 << CASR_Point10;
    theGISMapWidget->createPolygon(CASR);
    */
}
#endif


#ifdef IFC
void RMainWindow::initializeIFC(bool isRtsFile)
{
    if(theIFCSystem != nullptr)
        return;

    // Define the IFC++ system before you initialize connections... otherwise will crash due to signal slot initialization
    theIFCSystem = new IfcPlusPlusSystem();

    // This is the read-and write tab
    RIFCTabReadWrite = std::make_unique<TabReadWrite>(theIFCSystem.data());

    // Global style sheet definitions
    QFile file(":styles.css");
    file.open(QFile::ReadOnly);

    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);

}


void RMainWindow::importBIMFromIFC(RBIM* theBIMModel)
{
    // Check for null pointers
    if(theIFCSystem == nullptr)
    {
        qCritical()<<"The IFC system does not exist, a null ptr in createMeshFromIFC";
        return;
    }

    // Get the IfcPP model to pass to RBIM
    shared_ptr<IfcPPModel> theIFCppmodel = theIFCSystem->getIFCModel();

    // Check for null pointers
    if(theIFCppmodel == nullptr)
    {
        qCritical()<<"Cannot create a FEM model, no IFC model exists.";
        return;
    }

    // Get the model name
    QString theModelName = QString::fromStdWString(theIFCppmodel->getFileName());

    // Create a new RDomain if one does not exist
    if(theDomain == nullptr)
        theDomain = new RDomain(this,"Domain");

    if(theBIMModel==nullptr)
    {
        // Create a new detailed BIM
        theDomain->createObject("DetailedBimModel","RBIM");
        // Get created object from RDomain
        theBIMModel = qobject_cast<RBIM*>(theDomain->getLastAddedObject());
    }

    // Give it the IFC system
    theBIMModel->setIfcPPSystem(theIFCSystem);

    // Perform the conversion
    theBIMModel->createComponentsFromBIM();


    // Refresh the object trees
    objectsTree->updateView();

}


IfcPlusPlusSystem* RMainWindow::getIFCSystem()
{
    return theIFCSystem.data();
}
#endif


void RMainWindow::updateStatusBarMouseCoordinates(float &x,float &y)
{

    statusBarOne->clearMessage();

    statusBarOne->showMessage(QString::number(x,'g',4)+ ","+ QString::number(y,'g',4),500);
}




