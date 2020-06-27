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

#include "RGISWidget.h"
#include "RLocation.h"
#include "RPortModel.h"
#include "RRefineryModel.h"
#include "RGenericSourceModel.h"
#include "RTankFarmModel.h"
#include "RFuelPipelineModel.h"
#include "RWaterStorageTankModel.h"
#include "RWaterPipelineModel.h"
#include "RWaterReservoirModel.h"
#include "RWaterLiftStationModel.h"
#include "RPowerSourceModel.h"
#include "RPowerSubstationModel.h"
#include "RPowerLineModel.h"
#include "RGenericStorageFacilityModel.h"
#include "RTruckModel.h"
#include "RBridgeModel.h"
#include "RAirportModel.h"
#include "RHospitalModel.h"
#include "RFoodStorageFacilityModel.h"
#include "RShelterModel.h"
#include "RGasCompressorStationModel.h"
#include "RGasPipelineModel.h"

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
#include <PictureMarkerSymbol.h>

using namespace Esri::ArcGISRuntime;

RGISWidget::RGISWidget(QWidget *parent):
    QWidget(parent),
    m_map(nullptr),
    m_mapView(nullptr)
{
    // Create a map using the topographic BaseMap
    // m_map = new Map(Basemap::topographic(this), this);
    // the coordinates define the center of the map when it is opened
    // the number after is the zoom level
    m_map = new Map(BasemapType::Topographic, 49.263384, -123.245223, 11, this);

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);
    //m_mapView->setWrapAroundMode(WrapAroundMode::Disabled);
    //m_mapView = new MapGraphicsView(m_map, this);

    // Set map to map view
    m_mapView->setMap(m_map);

    // set the mapView as the central widget
    // setCentralWidget(m_mapView);

    // create a new graphics overlay
    graphicsOverlay = new GraphicsOverlay(this);

    // add the overlay to the mapview
    m_mapView->graphicsOverlays()->append(graphicsOverlay);
}


RGISWidget::~RGISWidget()
{

}


void RGISWidget::openMap()
{
    // Create the map UI
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setMargin(10);

    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(m_mapView);
    setLayout(vBoxLayout);
    this->show();
}


void RGISWidget::createPoints(QVector<QVector <double>> coordinates)
{
    QList<Point> pointsList;
    for (int i = 0; i < coordinates.size(); i++){
        double theLatitude = coordinates[i][0];
        double theLongtitude = coordinates[i][1];

        pointsList << Point(theLongtitude, theLatitude, SpatialReference::wgs84());
    }

    // create the symbology for the points
    SimpleMarkerSymbol* sms = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::X, QColor("black"), 20, this);

    // create a graphic and add each of them to the overlay
    foreach (const Point &buoyPoint, pointsList) {
        Graphic* graphic = new Graphic(buoyPoint, this);
        graphic->setSymbol(sms);
        graphicsOverlay->graphics()->append(graphic);
    }
}


void RGISWidget::createLabelledPoint(QVector <double> coordinates, QString colour,  QString label)
{
    // create the point geometry
    Point pointGeometry(coordinates[1], coordinates[0], SpatialReference::wgs84());

    // create the point symbol
    SimpleMarkerSymbol* pointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::X, QColor("red"), 20, this);

    // Point graphic
    Graphic* pointGraphic = new Graphic(pointGeometry, this);

    // create a graphic overlay to display the point graphic
    GraphicsOverlay* pointGraphicOverlay = new GraphicsOverlay(this);
    // set the renderer of the overlay to be the marker symbol
    pointGraphicOverlay->setRenderer(new SimpleRenderer(pointSymbol, this));
    // add the graphic to the overlay
    pointGraphicOverlay->graphics()->append(pointGraphic);
    // add the overlay to the mapview
    m_mapView->graphicsOverlays()->append(pointGraphicOverlay);

    // add a label to the point
    addText(coordinates, label, colour);
}


void RGISWidget::createPoint(QVector<double> coordinates, RNetworkModel *theModel)
{
    // create the point geometry
    Point pointGeometry(coordinates[1], coordinates[0], SpatialReference::wgs84());

    // create the point symbol
    SimpleMarkerSymbol* pointSymbol;

    // Set the colour of the marker symbol
    QString theColour = this->setColour(theModel);

    // Set the style of the marker symbol
    pointSymbol = this->setMarkerSymbol(theModel, theColour);

    // Create a point graphic
    Graphic* pointGraphic = new Graphic(pointGeometry, this);

    // create a graphic overlay to display the point graphic
    GraphicsOverlay* pointGraphicOverlay = new GraphicsOverlay(this);
    // set the renderer of the overlay to be the marker symbol
    pointGraphicOverlay->setRenderer(new SimpleRenderer(pointSymbol, this));
    // add the graphic to the overlay
    pointGraphicOverlay->graphics()->append(pointGraphic);
    // add the overlay to the mapview
    m_mapView->graphicsOverlays()->append(pointGraphicOverlay);

    // add a label to the point
    QString label = theModel->objectName();
    QString labelColour = QString("red");

    addText(coordinates, label, labelColour);
}


void RGISWidget::createPoint(QVector<double> coordinates, RNetworkModel *theModel, bool availability)
{
    // create the point geometry
    Point pointGeometry(coordinates[1], coordinates[0], SpatialReference::wgs84());

    // create the point symbol
    SimpleMarkerSymbol* pointSymbol;

    QString theColour;

    // Define the colour based on the availability
    if (availability){
        theColour = QString("green");
    }
    else {
        theColour = QString("red");
    }

    // Set the style of the marker symbol
    pointSymbol = this->setMarkerSymbol(theModel, theColour);


    // Point graphic
    Graphic* pointGraphic = new Graphic(pointGeometry, this);

    // create a graphic overlay to display the point graphic
    GraphicsOverlay* pointGraphicOverlay = new GraphicsOverlay(this);
    // set the renderer of the overlay to be the marker symbol
    pointGraphicOverlay->setRenderer(new SimpleRenderer(pointSymbol, this));
    // add the graphic to the overlay
    pointGraphicOverlay->graphics()->append(pointGraphic);
    // add the overlay to the mapview
    m_mapView->graphicsOverlays()->append(pointGraphicOverlay);

}


void RGISWidget::createPolyline(QVector<QVector<double>> coordinates)
{
    // create line geometry
    PolygonBuilder polylineBuilder(SpatialReference::wgs84());

    // build the polyline
    for (int i = 0; i < coordinates.size(); i++){
        polylineBuilder.addPoint(coordinates[i][1], coordinates[i][0]);
    }

    // create a line symbol
    SimpleLineSymbol* sls = new SimpleLineSymbol(SimpleLineSymbolStyle::Dash, QColor("blue"), 2, this);

    // create a line graphic
    Graphic* lineGraphic = new Graphic(polylineBuilder.toGeometry(), this);

    // create a graphic overlay to display the line graphic
    GraphicsOverlay* lineGraphicOverlay = new GraphicsOverlay(this);

    // set the renderer of the graphic overlay to be the line symbol
    lineGraphicOverlay->setRenderer(new SimpleRenderer(sls, this));

    // add the graphic to the overlay
    lineGraphicOverlay->graphics()->append(lineGraphic);

    // add the overlay to the mapview
    m_mapView->graphicsOverlays()->append(lineGraphicOverlay);
}


void RGISWidget::createPolyline(QVector<QVector<double>> coordinates, RNetworkModel *theModel)
{
    // create line geometry
    PolygonBuilder polylineBuilder(SpatialReference::wgs84());

    // build the polyline
    for (int i = 0; i < coordinates.size(); i++){
        polylineBuilder.addPoint(coordinates[i][1], coordinates[i][0]);
    }

    // Define the colour of the network link
    QString theColour = this->setColour(theModel);

    // create a line symbol
    SimpleLineSymbol* sls = new SimpleLineSymbol(SimpleLineSymbolStyle::Dash, QColor(theColour), 2, this);

    // create a line graphic
    Graphic* lineGraphic = new Graphic(polylineBuilder.toGeometry(), this);

    // create a graphic overlay to display the line graphic
    GraphicsOverlay* lineGraphicOverlay = new GraphicsOverlay(this);

    // set the renderer of the graphic overlay to be the line symbol
    lineGraphicOverlay->setRenderer(new SimpleRenderer(sls, this));

    // add the graphic to the overlay
    lineGraphicOverlay->graphics()->append(lineGraphic);

    // add the overlay to the mapview
    m_mapView->graphicsOverlays()->append(lineGraphicOverlay);
}


void RGISWidget::createPolygon(QVector<QVector<double>> boundaries)
{
    // create the polygon geometry
    PolygonBuilder polygonBuilder(SpatialReference::wgs84());

    for (int i = 0; i < boundaries.size(); i++){
        polygonBuilder.addPoint(boundaries.at(i).at(1),boundaries.at(i).at(0));
    }

    SimpleFillSymbol* sfs = new SimpleFillSymbol(SimpleFillSymbolStyle::Cross, QColor(255, 0, 0, 100), this);
    Graphic* polygonGraphic = new Graphic(polygonBuilder.toGeometry(), this);

    GraphicsOverlay* polygonGraphicsOverlay = new GraphicsOverlay(this);
    polygonGraphicsOverlay->setRenderer(new SimpleRenderer(sfs, this));

    polygonGraphicsOverlay->graphics()->append(polygonGraphic);
    m_mapView->graphicsOverlays()->append(polygonGraphicsOverlay);
}


void RGISWidget::addText(QVector<double> coordinates, QString text, QString colour)
{
    // text symbol
    TextSymbol* textSymbol = new TextSymbol(QString(text), QColor(colour), 20, HorizontalAlignment::Center, VerticalAlignment::Bottom, this);

    // geometry for the graphics
    // giving a very small offset in the north direction so text and symbol do not overlap
    coordinates[0] += 0.001;
    Point textPoint(coordinates[1], coordinates[0], SpatialReference::wgs84());

    // create the graphics and set their symbology
    Graphic* graphic = new Graphic(textPoint, this);
    graphic->setSymbol(textSymbol);

    // add the graphics to the map
    GraphicsOverlay *textGraphicsOverlay = new GraphicsOverlay(this);

    // set the renderer of the graphic overlay to be the text symbol
    textGraphicsOverlay->setRenderer(new SimpleRenderer(textSymbol,this));

    // add the graphic to the overlay
    textGraphicsOverlay->graphics()->append(graphic);

    // add the overlay to the mapview
    m_mapView->graphicsOverlays()->append(textGraphicsOverlay);
}


SimpleMarkerSymbol* RGISWidget::setMarkerSymbol(RNetworkModel *theModel, QString theColour)
{
    // Creating the means to check what model has been passed

    // Fuel network models
    RPortModel *thePort = qobject_cast<RPortModel *>(theModel);;
    RRefineryModel *theRefinery = qobject_cast<RRefineryModel *>(theModel);
    RGenericSourceModel *theGenericSource = qobject_cast<RGenericSourceModel *>(theModel);
    RTankFarmModel *theTankFarm = qobject_cast<RTankFarmModel *>(theModel);
    RFuelPipelineModel *theFuelPipeline = qobject_cast<RFuelPipelineModel *>(theModel);

    RGenericStorageFacilityModel *theGenericStorage =  qobject_cast<RGenericStorageFacilityModel *>(theModel);

    // Water network models
    RWaterStorageTankModel *theWaterTank = qobject_cast<RWaterStorageTankModel *>(theModel);
    RWaterLiftStationModel *theWaterLiftStation = qobject_cast<RWaterLiftStationModel *>(theModel);
    RWaterReservoirModel *theWaterReservoir = qobject_cast<RWaterReservoirModel *>(theModel);
    RWaterPipelineModel *theWaterPipeline = qobject_cast<RWaterPipelineModel *>(theModel);

    // Power network models
    RPowerSubstationModel *thePowerSubstation = qobject_cast<RPowerSubstationModel *>(theModel);
    RPowerSourceModel *thePowerSource = qobject_cast<RPowerSourceModel *>(theModel);
    RPowerLineModel *thePowerLine = qobject_cast<RPowerLineModel *>(theModel);

    // Transportation (?) network models
    RTruckModel *theTruck = qobject_cast<RTruckModel *>(theModel);
    RAirportModel *theAirport = qobject_cast<RAirportModel *>(theModel);
    RBridgeModel *theBridge = qobject_cast<RBridgeModel *>(theModel);

    // Medical care network models
    RHospitalModel *theHospital = qobject_cast<RHospitalModel *>(theModel);

    RShelterModel *theShelter = qobject_cast<RShelterModel *>(theModel);

    // Gas network
    RGasCompressorStationModel *theCompressorStation = qobject_cast<RGasCompressorStationModel *>(theModel);
    RGasPipelineModel *theGasPipeline = qobject_cast<RGasPipelineModel *>(theModel);

    RFoodStorageFacilityModel *theFoodStorage = qobject_cast<RFoodStorageFacilityModel *>(theModel);

    // Sources = circle
    // Intermediate = triangle
    // Storage = square

    // Fuel = black
    // Power = orange
    // Water = blue
    // Medical care = red
    // Gas = green

    // create the point symbol
    SimpleMarkerSymbol* markerSymbol;

    // If the model is a port
    if (thePort){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Diamond, QColor(theColour), 15, this);
    }


    // If the model is a bridge
    else if (theBridge){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Diamond, QColor(theColour), 15, this);
    }


    // If the model is an airport
    else if (theAirport){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Cross, QColor(theColour), 15, this);
    }


    // If the model is a shelter
    else if (theShelter){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Diamond, QColor(theColour), 15, this);
    }


    // If the model is a food storage
    else if (theFoodStorage){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Square, QColor(theColour), 15, this);
    }


    // If the model is a generic storage model
    else if (theGenericStorage){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Square, QColor(theColour), 15, this);
    }


    // Gas infrastructure
    else if (theCompressorStation){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(theColour), 15, this);
    }


    // Fuel infrastructure
    else if (theRefinery || theGenericSource){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(theColour), 15, this);
    }

    else if (theTankFarm){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Square, QColor(theColour), 15, this);
    }


    // Water infrastructure
    else if (theWaterTank){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Square, QColor(theColour), 15, this);
    }

    else if (theWaterLiftStation){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Triangle, QColor(theColour), 15, this);
    }

    else if (theWaterReservoir){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(theColour), 15, this);
    }


    // Power infrastructure
    else if (thePowerSubstation){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Triangle, QColor(theColour), 15, this);
    }

    else if (thePowerSource){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Diamond, QColor(theColour), 15, this);
    }


    // Medical care infrastructure
    else if (theHospital){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Cross, QColor(theColour), 30, this);
    }

    // Line models are not plotted at this time
    else if (thePowerLine || theWaterPipeline){
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Diamond, QColor(theColour), 1, this);
    }


    else {
        markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Triangle, QColor(theColour), 7, this);
    }

    return markerSymbol;

}


QString RGISWidget::setColour(RNetworkModel *theModel){

    QString theColour;

    // Creating the means to check what model has been passed

    // Fuel network models
    RPortModel *thePort = qobject_cast<RPortModel *>(theModel);;
    RRefineryModel *theRefinery = qobject_cast<RRefineryModel *>(theModel);
    RGenericSourceModel *theGenericSource = qobject_cast<RGenericSourceModel *>(theModel);
    RTankFarmModel *theTankFarm = qobject_cast<RTankFarmModel *>(theModel);
    RFuelPipelineModel *theFuelPipeline = qobject_cast<RFuelPipelineModel *>(theModel);

    RGenericStorageFacilityModel *theGenericStorage =  qobject_cast<RGenericStorageFacilityModel *>(theModel);

    // Water network models
    RWaterStorageTankModel *theWaterTank = qobject_cast<RWaterStorageTankModel *>(theModel);
    RWaterLiftStationModel *theWaterLiftStation = qobject_cast<RWaterLiftStationModel *>(theModel);
    RWaterReservoirModel *theWaterReservoir = qobject_cast<RWaterReservoirModel *>(theModel);
    RWaterPipelineModel *theWaterPipeline = qobject_cast<RWaterPipelineModel *>(theModel);

    // Power network models
    RPowerSubstationModel *thePowerSubstation = qobject_cast<RPowerSubstationModel *>(theModel);
    RPowerSourceModel *thePowerSource = qobject_cast<RPowerSourceModel *>(theModel);
    RPowerLineModel *thePowerLine = qobject_cast<RPowerLineModel *>(theModel);

    // Transportation (?) network models
    RTruckModel *theTruck = qobject_cast<RTruckModel *>(theModel);
    RAirportModel *theAirport = qobject_cast<RAirportModel *>(theModel);
    RBridgeModel *theBridge = qobject_cast<RBridgeModel *>(theModel);

    // Medical care network models
    RHospitalModel *theHospital = qobject_cast<RHospitalModel *>(theModel);

    RShelterModel *theShelter = qobject_cast<RShelterModel *>(theModel);

    // Gas network
    RGasCompressorStationModel *theCompressorStation = qobject_cast<RGasCompressorStationModel *>(theModel);
    RGasPipelineModel *theGasPipeline = qobject_cast<RGasPipelineModel *>(theModel);

    RFoodStorageFacilityModel *theFoodStorage = qobject_cast<RFoodStorageFacilityModel *>(theModel);


    // Sources = circle
    // Intermediate = triangle
    // Storage = square

    // Fuel = black
    // Power = orange
    // Water = blue
    // Medical care = red
    // Gas = green


    // If the model belongs to the fuel network
    if (thePort || theRefinery || theGenericSource || theTankFarm || theFuelPipeline || theTruck){
        theColour = QString("black");
    }

    // If the model belongs to the water network
    else if (theWaterReservoir || theWaterLiftStation || theWaterTank || theWaterPipeline){
        theColour = QString("blue");
    }

    else if (thePowerSource || thePowerSubstation || thePowerLine){
        theColour = QString("orange");
    }

    else if (theHospital){
        theColour = QString("red");
    }

    else if (theCompressorStation || theGasPipeline){
        theColour = QString("green");
    }

    else {
        theColour =  QString("gray");
    }

    return theColour;
}
















