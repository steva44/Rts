#ifndef RCSICostMethod_H
#define RCSICostMethod_H
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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

// This class provides the methods to calculate the material, labour, and equipment costs from CSI codes

#include "RMethod.h"
#include "RContinuousRandomVariable.h"

#include <QMap>

// This struct is used by the database to store the RSMeans cost, labour-hours, etc., values corresponding to each CSI code
struct CSIItem
{
public:
    void setCSIcode(const QString &value);
    void setDescription(const QString &value);

    QString getCSIcode() const;
    QString getDescription() const;

    void setCov(const QString& value);
    QString getCov() const;

    QString getNotes() const;
    void setNotes(const QString &value);

    QString getUnit() const;
    void setUnit(const QString &value);

    QString getCrew() const;
    void setCrew(const QString &value);

    double getDailyOutput() const;
    void setDailyOutput(double value);

    double getLaborHours() const;
    void setLaborHours(double value);

    // Bare costs do not include profit and overhead, i.e., they are the costs a contractor would pay
    double getBareMaterial() const;
    void setBareMaterial(double value);

    double getBareLabour() const;
    void setBareLabour(double value);

    double getBareEquipment() const;
    void setBareEquipment(double value);

    double getBareTotal() const;
    void setBareTotal(double value);

    // Total cost of equipment, labour, and material INCLUDING profit and overhead, i.e., they are the costs an owner would pay
    double getTotalOP() const;
    void setTotalOP(double value);

    double getHourlyOperCost() const;
    void setHourlyOperCost(double value);

    double getRentPerDay() const;
    void setRentPerDay(double value);

    double getRentPerWeek() const;
    void setRentPerWeek(double value);

    double getRentPerMonth() const;
    void setRentPerMonth(double value);

    // Random variables for the material, labour, and equipment costs
    RContinuousRandomVariable *getTheMaterialRV() const;
    void setTheMaterialRV(RContinuousRandomVariable *value);

    RContinuousRandomVariable *getTheLabourRV() const;
    void setTheLabourRV(RContinuousRandomVariable *value);

    RContinuousRandomVariable *getTheEquipmentRV() const;
    void setTheEquipmentRV(RContinuousRandomVariable *value);

    // Random variables for labour hours to complete a unit of the CSI item
    RContinuousRandomVariable *getTheLabourHoursRV() const;
    void setTheLabourHoursRV(RContinuousRandomVariable *value);

private:
    QString CSIcode= "NULL";
    QString eco = "NULL";
    QString notes = "NULL";
    QString description = "NULL";
    QString unit = "NULL";
    QString crew = "NULL";
    double dailyOutput = 0.0;
    double laborHours = 0.0;
    double bareMaterial = 0.0;
    double bareLabour = 0.0;
    double bareEquipment = 0.0;
    double bareTotal = 0.0;
    double totalOP = 0.0;
    double hourlyOperCost = 0.0;
    double rentPerDay = 0.0;
    double rentPerWeek = 0.0;
    double rentPerMonth = 0.0;
    QString cov = "NULL";

    RContinuousRandomVariable* theMaterialRV = nullptr;
    RContinuousRandomVariable* theLabourRV = nullptr;
    RContinuousRandomVariable* theEquipmentRV = nullptr;
    RContinuousRandomVariable* theLabourHoursRV = nullptr;

};


class RCSICostMethod : public RMethod
{
    Q_OBJECT

public:
    RCSICostMethod(RDomain *theDomain, QString name);
    ~RCSICostMethod();

    // Functions to provide the various costs given a CSI code
    // Cost of material
    double getCostMaterial(QString& csiCode);

    // Cost of labour
    double getCostLabour(QString& csiCode);

    // Cost of equipment
    double getCostEquipment(QString& csiCode);

    // Labour costs of a vector of CSI codes
    double getLabourCosts(QVector<std::tuple<QString,double,QString>>& labourQuantityCSICodes);

    // Material costs of a vector of CSI codes
    double getMaterialCosts(QVector<std::tuple<QString,double,QString>>& materialQuantityCSICodes);

    // Equipment costs of a vector of CSI codes
    double getEquipmentCosts(QVector<std::tuple<QString,double,QString>>& equipmentQuantityCSICodes);

    // Labor-Hours column is the amount of labor hours required to perform one unit of work
    RContinuousRandomVariable* getLabourHours(QString& CSICode);

    // Demand surge due to scarcity in labour, materials, etc., after an earthquake
    double getDemandSurge() const;
    void setDemandSurge(double value);

private:

    // Returns a CSI item given its code
    CSIItem*  getCSIItem(const QString& csiCode);

    // The RSMeans costs do not include profit and overhead-they are bare costs,
    // As a result need to add a profit markup if the costs will be paid by a building stakeholder
    // This function calculates the markup
    double getMarkup(const QString& csiCode);

    // Imports the RSMeans properties for each CSI item
    // Also, imports the costs, labour rates, etc., as random variables
    bool initializeDatabase();

    // Function to create a random variable
    RContinuousRandomVariable* createRV(const QString& name,const double& meanValue,const QString& cov);

    // Map of CSI items
    QMap<QString, CSIItem> theCSICostMap;

    // Demand surge for profit markup or due to increased demand for resources after an earthquake
    double demandSurge;

};

#endif // RCSICostMethod_H
