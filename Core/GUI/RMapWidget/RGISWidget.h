#ifndef RGISWidget_H
#define RGISWidget_H
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
*   - Rodrigo Costa                                                  *
*                                                                    *
*********************************************************************/

#include "Map.h"
#include "MapGraphicsView.h"
#include "Viewpoint.h"
#include "SpatialReference.h"
#include "Point.h"
#include "PolylineBuilder.h"
#include "Polyline.h"
#include "PolygonBuilder.h"
#include "Polygon.h"
#include "Graphic.h"
#include "GraphicsOverlay.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"
#include "TextSymbol.h"
#include "SimpleRenderer.h"
#include <QVBoxLayout>
#include <QList>
#include "ArcGISRuntimeEnvironment.h"

#include "RModel.h"
#include "RNetworkModel.h"

namespace Esri {
namespace ArcGISRuntime {
class Map;
class MapGraphicsView;
class Viewpoint;
class GraphicsOverlay;
class Point;
}
}

#include "Geometry.h"
#include <QWidget>
#include "Point.h"


class RGISWidget : public QWidget
{
    Q_OBJECT

public:
    RGISWidget(QWidget *parent = 0);
    ~RGISWidget();

    // Creates the map
    void openMap();

    // Create a list of points in a single overlay
    void createPoints(QVector<QVector<double>> coordinates);

    // Create one labelled point
    void createLabelledPoint(QVector <double> coordinates, QString colour,  QString label);

    // Create one point customized by the model it is representing
    void createPoint(QVector<double> coordinates, RNetworkModel *theModel);

    // Create one point customized by the model it is representing and its availability (green for available, red for unavailable)
    void createPoint(QVector<double> coordinates, RNetworkModel *theModel, bool availability);

    // Create a polyline between two points passed on a vector of vectors
    void createPolyline(QVector<QVector<double>> coordinates);

    // Create a polyline between two points passed on a vector of vectors with coloured based on the models
    void createPolyline(QVector<QVector<double>> coordinates, RNetworkModel *theModel);

    // Create a polygon based on a vector of vectors containing the coordinates of each point
    void createPolygon(QVector<QVector<double>> boundaries);

    // Create a text element on the map
    void addText(QVector<double> coordinates, QString text, QString colour);

    // Define the symbol marker of an element based on the model type
    Esri::ArcGISRuntime::SimpleMarkerSymbol* setMarkerSymbol(RNetworkModel *theModel, QString theColour);

    // Define the colour of an element based on the model type
    QString setColour(RNetworkModel *theModel);


public slots:

private:
    Esri::ArcGISRuntime::Map*                   m_map;
    Esri::ArcGISRuntime::MapGraphicsView*       m_mapView;
    Esri::ArcGISRuntime::GraphicsOverlay*       graphicsOverlay;

};

#endif //RGISWidget_H

