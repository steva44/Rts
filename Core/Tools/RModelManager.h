#ifndef RModelManager_H
#define RModelManager_H
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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RMethod.h"
#include "RDomain.h"
#include "RMainWindow.h"
#include "RFlowchartWidget.h"
#ifdef NETWORKS
#include "RNetworkModel.h"
#include "RPacket.h"
#include "RPacketResponse.h"
#include "RRoute.h"
#endif

#include <QProcess>
#include "qcustomplot.h"

class RModelManager : public RMethod
{

public:
    RModelManager(RDomain *domain);
    ~RModelManager();

    int createModelMap(QList<QPointer<RParameter> > parameterList, QList<RModel *> functionModelList);
    int plotFlowchart();
    QList<QPair<double, QString> > *getTriggerTimes(double startTime, double endTime, double maxTimeGap);

#ifdef NETWORKS
    int createLocationDiagram();
    //int assignRoutes();
    //int assignMaritimeRoutes();
    int establishPaths();
    int removeClosedPaths();
    int printPaths();
    int setPaths();
    void setPathMatrix(QVector<QVector<RModel *> > originalPathMatrix);
    QVector<QVector<RModel *> > retrievePathMatrix();
#endif

protected:

    QVector<QVector<RModel *> > pathMatrix;

private:

     QMap<QPointer<QObject>, QObjectList> objectMap;
     QList<QObjectList> objectListList;
     QList<QPair<double, QString> > triggerTimes;
     QPointer<QCustomPlot> customPlotForLocations;
     bool theEventOccurrence;

};

#endif



