#ifndef RMainWindow_H
#define RMainWindow_H
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

#include <QtCore/qglobal.h>
#include <QMainWindow>
#include <QPointer>
#include <QLabel>
#include <QSplitter>
#include <QToolBar>
#include <QTableWidget>
#include <QDockWidget>

#ifdef OSG
class ROSGSystem;
class ROSGOrbitCameraManipulator;
class ROSGPlotter;

#include <osg/Group>
#include "osg/ref_ptr"
#include "OSGWidget.h"
#endif

#ifdef IFC
class IfcTreeWidget;
class TabReadWrite;
class IfcPlusPlusSystem;
class OrbitCameraManipulator;
class IfcPPEntity;
class RBIM;

#include "TabReadWrite.h"
#include <boost/unordered_map.hpp>
#endif

class QAction;
class QListWidget;
class QTreeWidget;
class QWidget;
class QToolBox;
class QPlainTextEdit;
class QTabBar;
class QMdiArea;
class QMdiSubWindow;
class QProgressBar;
class RPropertyWidget;
class RObjectWidget;
class RAggregationObjectWidget;
class RDomain; 

#ifdef GIS
class RGISWidget;
class RLocation;
#endif

class RMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    RMainWindow(QWidget *parent = nullptr);
    virtual ~RMainWindow();

    static bool noWarning;

    RDomain* getDomain(void);

#ifdef OSG
    ROSGSystem* getOSGSystem();
    void createGLViewWidget();
    OSGWidget* getGLViewWidget();
    ROSGPlotter* getOSGPlotter();
#endif

#ifdef GIS
    void createGISWidget();
    void displayLocationsOnMap();
    void displayAvailabilityOnMap();
    void displayNetworkLinks();
    void displayEQOccurrence(QVector<double> theCoordinates);
    void displayHazardAreaSources();
    QPointer<RGISWidget> theGISMapWidget;
    QPointer<QSplitter>	 splitWindowGIS = nullptr;
#endif

#if IFC
    inline QPointer<OSGWidget> getIFCOSGWidget(){return osgWidgetIFC;}
    void initializeIFC(bool isRtsFile);
#endif

public slots:

    void raiseDock(QString dockName);
    void updateTree(QString treeName);
    void collapseTree(QString treeName);
    void expandTree(QString treeName);
    void updatePropertyList();
    void clearAllObjects(bool showDialogBox = true);

    //This function receives a file and decides which route to take when opening it; whether it is an IFC or Rts Text File
    void openFile(QString fileName, bool StInputFile = false);
    QObjectList openRtsFile(QString fileName, bool StInputFile);
    void addSubWindow(QWidget *widget, QString windowTitle, bool tiled = false);
    QMdiSubWindow *getSubWindow(QWidget *widget);
    void setActiveSubWindow(QWidget *widget);
    void viewMap();
    void collapseAllTrees();
    void expandAllTrees();
    //This function closes the properties box on the main screen
    void closePropertyBox();
    void closeOutputBox();

#ifdef IFC
    IfcPlusPlusSystem* getIFCSystem();
    void openIFCFile(QString fileName);
    void importBIMFromIFC(RBIM* theBIMModel = nullptr);
#endif

signals:

    void subWindowAdded(QWidget *subWindow);

private slots:	

    void closeEvent(QCloseEvent *event);
    void raisePropertieDock();
    void about();
    void togglePanes();
    void showHideToolBars();
    void writeSettings(bool defaultFlag);
    void readSettings(bool defaultFlag);
    void setDefaultView();
    void resetView();
    void overlayImageOnMap();
    void createLocationsFromMap();
    void open();
    void save();
    void openRecentFile();
    void cutText();
    void copyText();
    void pasteText();
    void deleteAllText();
    void tileWindows();
    void cascadeWindows();
    void closeAll();
    void help();
    void closeActiveSubWindow();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void updateStatusBarMouseCoordinates(float &x,float &y);
#ifdef OSG
    void clearGLViewWidget();
#endif

private:

    void createCentralWidget();
    void createActions();
    void createMenus();
    void createStatusBar();
    void createDockWindows();
    void createConnections();
    void resetDomain();
    void onFileOpenError(QObjectList *objectList, QString oldFilePath);
    void onMapIsNotOpenError();
    void updateApplicationTitle(QString currentTitle);
    void updateRecentFileList(QString newFileName);
    void updateRecentFileActions();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QMenu *panesMenu;
    QMenu *unitsMenu;
    QMenu *toolBarsMenu;
    QMenu *recentFilesMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *viewToolBar;
    QToolBar *windowToolBar;
    QToolBar *helpToolBar;

    QAction *clearAllObjectAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *exitAction;
    QAction *cutAction;
    QAction *setUnitsNmmkgsAction;
    QAction *setUnitskNmkgsAction;
    QAction *setUnitslbsinsAction;
    QAction *setUnitskipsftsAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAllAction;
    QAction *toggleAllPanesAction;
    QAction *toggleAllToolBarsAction;
    QAction *resetViewAction;
    QAction *setDefaultViewAction;

    QAction *collapseTreesAction;
    QAction *expandTreesAction;
    QAction *viewMapAction;
    QAction *overlayImageOnMapAction;
    QAction *createLocationsFromMapAction;
    QAction *variableInferenceAction;
    QAction *modelInferenceAction;
    QAction *tileWindowsAction;
    QAction *cascadeWindowsAction;
    QAction *closeAllAction;
    QAction *helpAction;
    QAction *aboutAction;
    enum {MaxRecentFiles = 10};
    QAction *recentFileActions[MaxRecentFiles];

    QDockWidget *propertiesDock;
    QDockWidget *consoleDock;
    QDockWidget *objectsDock;
    RPropertyWidget *propertyList;
    RObjectWidget *objectsTree;
    QPlainTextEdit *console;

    QMdiArea *mdiArea;
    QProgressBar *progressBar;

    RDomain *theDomain;
    bool closeWithoutWarning;

    QStatusBar*     statusBarOne;

#ifdef IFC
    std::unique_ptr<TabReadWrite> RIFCTabReadWrite;
    QPointer<IfcPlusPlusSystem> theIFCSystem;
    QPointer<OSGWidget> osgWidgetIFC;
#endif

#ifdef OSG
    QPointer<ROSGSystem> OSGSystem;
    osg::ref_ptr<ROSGOrbitCameraManipulator> theOrbitCameraManipulator;
    QPointer<OSGWidget> theOSGWidget;
    std::unique_ptr<ROSGPlotter> thePlotter;
#endif

};

#endif
