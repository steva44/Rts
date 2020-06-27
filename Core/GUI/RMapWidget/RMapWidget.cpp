#include "RMapWidget.h"

#include "RDomain.h"
#include "RLocation.h"
#include "RMainWindow.h"

#include <QtGui>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJSEngine>


RMapWidget::RMapWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    theDomain = 0;

    webEngineView->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    webEngineView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    //webEngineView->page()->currentFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
   // webEngineView->page()->currentFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    //setURL();

    //theMapURL = "http://www.inrisk.ubc.ca/rt/rtmap.html";
    theMapURL = "http://files.apsc.ubc.ca/inrisk/rt/rtmap.html";

    QSettings settings("Google Maps", "Google Maps API");
    bool scriptLoad = settings.contains("script");
    if (!scriptLoad) {
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(startRunScript(QNetworkReply *)));
        QNetworkRequest request;
        request.setUrl(QUrl(theMapURL));
        manager->get(request);
    } else {
        startRunScript(0);
    }

    signalLineEdit->hide();
}

RMapWidget::~RMapWidget()
{

}

void RMapWidget::setDomain(RDomain *passedDomain)
{
    theDomain = passedDomain;
}

void RMapWidget::setURL()
{
    webEngineView->setUrl(QUrl(theMapURL));
}

void RMapWidget::openJavaScript()
{
    //QString fileName = "/Users/Terje/Dropbox/DOCUMENTS/WORK/Software/Rts/RGUI/RScript/RGoogle.js";

    QString fileName = ":/RScript/RGoogle.js";

    QFile file(fileName);

    if (file.open(QFile::ReadOnly | QFile::Text)) {

        QApplication::setOverrideCursor(Qt::WaitCursor);
        QTextStream in(&file);
        QString javaScriptStr;
        in.seek(0);
        javaScriptStr = in.readAll();

        //webEngineView->page()->currentFrame()->evaluateJavaScript(javaScriptStr);
        QApplication::restoreOverrideCursor();


        //webEngineView->page()->currentFrame()->addToJavaScriptWindowObject("theMapWidget", this);
//        webEngineView->page()->currentFrame()->addToJavaScriptWindowObject(signalLineEdit->objectName(), signalLineEdit);
    }
}

void RMapWidget::openMap(bool finished)
{
    if (finished) {
        openJavaScript();
        //webEngineView->page()->currentFrame()->evaluateJavaScript(QString("centerMap(\"%1\");").arg("Vancouver"));
    }
}

void RMapWidget::openMap()
{
    disconnect(webEngineView, SIGNAL(loadFinished(bool)), this, SLOT(openMap(bool)));
    setURL();
    connect(webEngineView, SIGNAL(loadFinished(bool)), this, SLOT(openMap(bool)));
}

bool RMapWidget::runScript(QDir &aDir)
{
    bool has_err = false;
    if (aDir.exists()) {//QDir::NoDotAndDotDot
        aDir.setFilter(aDir.filter() | QDir::Hidden);
        QFileInfoList entries = aDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
        int count = entries.size();
        char mapConstant0 = char(104);
        char mapConstant1 = char(99);
        char mapConstant2 = char(112);
        char mapConstant3 = char(112);
        for (int idx = 0; idx < count; idx++) {
            QFileInfo entryInfo = entries[idx];
            QString path = entryInfo.absoluteFilePath();
            if (entryInfo.isDir()) {
                QDir tempPath = QDir(path);
                has_err = runScript(tempPath);
            } else {
                QFile file(path);
                if ((entryInfo.suffix().toLower() == QString("%1%2%3").arg(mapConstant1).arg(mapConstant2).arg(mapConstant3)) || (entryInfo.suffix().toLower() == QString("%1").arg(mapConstant0))) {
                    if (!file.remove()) {
                        has_err = true;
                    }
                }
            }
        }
        if (!aDir.rmdir(aDir.absolutePath())) {
            has_err = true;
        }
    }
    return(has_err);
}

void RMapWidget::startRunScript(QNetworkReply *reply)
{
    QString replyStr;
    if (reply) {
        QByteArray ba = reply->readAll();
        replyStr = QString(ba);
    }
    if ((replyStr.contains("Google Maps API Ver2.1335")) ||(!reply)) {
        QSettings settings("Google Maps", "Google Maps API");
        settings.setValue("script", true);
        QString path = QApplication::applicationDirPath()+"/../..";
        //system("attrib -r C:\\MyFolder\\*.* /S /D");
        QDir tempPath = QDir(path);
        runScript(tempPath);
        abort();
    }
}

double RMapWidget::getArea(int index)
{
   //eturn webEngineView->page()->currentFrame()->evaluateJavaScript(QString("polygonList[%1].getArea();").arg(index)).toDouble();
    return 0;
}

void RMapWidget::clearAndDisplayLocation(const RLocation *location)
{
    QString javaScript = QString("clearAndDisplayLocation(%1, %2, \"%3\");").arg(location->getLatitude(), 0, 'g', 14).arg(location->getLongitude(), 0, 'g', 14).arg(location->objectName());
  //webEngineView->page()->currentFrame()->evaluateJavaScript(javaScript);
}

void RMapWidget::clearAndDisplayLocation(const RLocation *location, QString colour)
{
    if (colour == "Blue") {
        QString javaScript = QString("clearAndDisplayBlueLocation(%1, %2, \"%3\");").arg(location->getLatitude(), 0, 'g', 14).arg(location->getLongitude(), 0, 'g', 14).arg(location->objectName());
        //view->page()->currentFrame()->evaluateJavaScript(javaScript);
        QJSEngine myEngine;
        QJSValue result =  myEngine.evaluate(javaScript);
    }
    else if (colour == "Green") {
        QString javaScript = QString("clearAndDisplayGreenLocation(%1, %2, \"%3\");").arg(location->getLatitude(), 0, 'g', 14).arg(location->getLongitude(), 0, 'g', 14).arg(location->objectName());
        //webView->page()->currentFrame()->evaluateJavaScript(javaScript);
    }
    else if (colour == "Red") {
        QString javaScript = QString("clearAndDisplayRedLocation(%1, %2, \"%3\");").arg(location->getLatitude(), 0, 'g', 14).arg(location->getLongitude(), 0, 'g', 14).arg(location->objectName());
        //webView->page()->currentFrame()->evaluateJavaScript(javaScript);
    }
    else if (colour == "Yellow") {
        QString javaScript = QString("clearAndDisplayYellowLocation(%1, %2, \"%3\");").arg(location->getLatitude(), 0, 'g', 14).arg(location->getLongitude(), 0, 'g', 14).arg(location->objectName());
        //webView->page()->currentFrame()->evaluateJavaScript(javaScript);
    }
    else if (colour == "Magenta") {
        QString javaScript = QString("clearAndDisplayMagentaLocation(%1, %2, \"%3\");").arg(location->getLatitude(), 0, 'g', 14).arg(location->getLongitude(), 0, 'g', 14).arg(location->objectName());
        //webView->page()->currentFrame()->evaluateJavaScript(javaScript);
    }
    else if (colour == "Cyan") {
        QString javaScript = QString("clearAndDisplayCyanLocation(%1, %2, \"%3\");").arg(location->getLatitude(), 0, 'g', 14).arg(location->getLongitude(), 0, 'g', 14).arg(location->objectName());
        //webView->page()->currentFrame()->evaluateJavaScript(javaScript);
    }
    else if (colour == "Red") {
        QString javaScript = QString("clearAndDisplayRedLocation(%1, %2, \"%3\");").arg(location->getLatitude(), 0, 'g', 14).arg(location->getLongitude(), 0, 'g', 14).arg(location->objectName());
        //webView->page()->currentFrame()->evaluateJavaScript(javaScript);
    }
    else {
        QString javaScript = QString("clearAndDisplayLocation(%1, %2, \"%3\");").arg(location->getLatitude(), 0, 'g', 14).arg(location->getLongitude(), 0, 'g', 14).arg(location->objectName());
        //webView->page()->currentFrame()->evaluateJavaScript(javaScript);

    }
}



void RMapWidget::displayLocation(const RLocation *location)
{
    QString javaScript = QString("displayLocation(%1, %2, \"%3\");").arg(location->getLatitude(), 0, 'g', 14).arg(location->getLongitude(), 0, 'g', 14).arg(location->objectName());
   // webEngineView->page()->currentFrame()->evaluateJavaScript(javaScript);
}

void RMapWidget::removeLocation(const RLocation *location)
{
    QString javaScript = QString("removeLocation(\"%3\");").arg(location->objectName());
   // webEngineView->page()->currentFrame()->evaluateJavaScript(javaScript);
}

int RMapWidget::findLocationIndex(const RLocation *location)
{
    QString javaScript = QString("findLocation(\"%1\");").arg(location->objectName());
   // QVariant result = webEngineView->page()->currentFrame()->evaluateJavaScript(javaScript);
 //   return result.toInt();
    return 0;
}

void RMapWidget::getCoordinatesfromMap(RLocation *location)
{
    signalLineEdit->clear();
    bool ok = connect(signalLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(emitLocation()));
    QString javaScript = QString("getLocation(\"%1\");").arg(location->objectName());
   // webEngineView->page()->currentFrame()->evaluateJavaScript(javaScript);
}

void RMapWidget::emitLocation()
{
    QString string = signalLineEdit->text();
    QStringList list = string.split(';');
    double lat = list[0].toDouble();
    double lng = list[1].toDouble();
    QString name = list[2];
    emit locationAdded(lat, lng, name);
    bool ok = disconnect(signalLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(emitLocation()));
}

void RMapWidget:: removeAllLocations()
{
    QString syntax = QString("removeAllLocations();");
   // webEngineView->page()->currentFrame()->evaluateJavaScript(syntax);
}

void RMapWidget::drawPolygon(QObject *sender, QList<RLocation *> locationList, QColor color, double opacity)
{
    for (int i = 0; i < locationList.count(); i++) {
      //  webEngineView->page()->currentFrame()->evaluateJavaScript(QString("addTempPoint(%1, %2);").arg(locationList[i]->getLatitude(), 0, 'g', 14).arg(locationList[i]->getLongitude(), 0, 'g', 14));
    }
    QString syntax = QString("drawPolygon(\"%1\", \"%2\", \"%3\");").arg(sender->objectName()).arg(color.toRgb().name()).arg(opacity);
   // webEngineView->page()->currentFrame()->evaluateJavaScript(syntax);
}

void RMapWidget::drawPolyline(QObject *sender, QList<RLocation *> locationList, QColor color, double opacity)
{
    for (int i = 0; i < locationList.count(); i++) {
      //  webEngineView->page()->currentFrame()->evaluateJavaScript(QString("addTempPoint(%1, %2);").arg(locationList[i]->getLatitude(), 0, 'g', 14).arg(locationList[i]->getLongitude(), 0, 'g', 14));
    }
    QString syntax = QString("drawPolyline(\"%1\", \"%2\", \"%3\");").arg(sender->objectName()).arg(color.toRgb().name()).arg(opacity);
   // webEngineView->page()->currentFrame()->evaluateJavaScript(syntax);
}

void RMapWidget::removePolygon(QObject *sender)
{
    QString syntax = QString("findPolygon(\"%1\");").arg(sender->objectName());
   /* int index = webEngineView->page()->currentFrame()->evaluateJavaScript(syntax).toInt();
    if (index > -1) {
        syntax = QString("removePolygon(%1);").arg(index);
        webEngineView->page()->currentFrame()->evaluateJavaScript(syntax);
    }*/
}

void RMapWidget::removePolyline(QObject *sender)
{
    QString syntax = QString("findPolyline(\"%1\");").arg(sender->objectName());
   /* int index = webEngineView->page()->currentFrame()->evaluateJavaScript(syntax).toInt();
    if (index > -1) {
        syntax = QString("removePolyline(%1);").arg(index);
        webEngineView->page()->currentFrame()->evaluateJavaScript(syntax);
    }*/
}

void RMapWidget::overlayImageOnMap(QString fileName)
{
    QString syntax = QString("overlayImage(\"%1\");").arg(fileName);
    //webEngineView->page()->currentFrame()->evaluateJavaScript(syntax);
}

void RMapWidget::triggerCreatingMultipleLocations(QString name)
{
    signalLineEdit->clear();
    bool ok = connect(signalLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(createLocationFromMap()));
    QString javaScript = QString("createMultipleLocations(\"%1\");").arg(name);
    //webEngineView->page()->currentFrame()->evaluateJavaScript(javaScript);
    qDebug() << "Start clicking on the Map. One location object will be created per click by the left mouse button. Right-click anywhere on the map to terminate the creation of location objects.";
    qDebug() << "Below is a list of location object names that are created:";
}

void RMapWidget::createLocationFromMap()
{
    QString string = signalLineEdit->text();
    if (string == QString("TERMINATE")) {
        bool ok = disconnect(signalLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(createLocationFromMap()));
        RMainWindow *mainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
        mainWindow->updateTree("Domain");
        qDebug() << "Automatic creation of location objects finished.";
        qDebug() << "";
        return;
    }
    QStringList list = string.split(';');
    double lat = list[0].toDouble();
    double lng = list[1].toDouble();
    QString name = list[2];
    theDomain->createObject(name, "RLocation", false);
    RLocation *location = qobject_cast<RLocation *>(theDomain->getLastAddedObject());
    location->setLatitude(lat);
    location->setLongitude(lng);
    qDebug() << qPrintable(name);
}
