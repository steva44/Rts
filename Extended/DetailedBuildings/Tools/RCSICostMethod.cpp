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

#include "RCSICostMethod.h"

#include <QFile>

RCSICostMethod::RCSICostMethod(RDomain *theDomain, QString name)
    : RMethod(theDomain, name)
{	
    this->initializeDatabase();

    // Demand surge due to scarcity in labour, materials, etc., after an earthquake
    demandSurge = 1.0;
}


RCSICostMethod::~RCSICostMethod()
{

}


bool RCSICostMethod::initializeDatabase()
{
    // Files are added in RtsBuilding.qrc and can be updated on the fly without setting any paths here
    QFile file(":Tools/Database/costDatabase.csv");

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return false;
    }

    QString data = file.readAll();
    data.remove( QRegExp("\r"));

    // First get each line from the spreadsheet, each line is a CSI item except for the first line which contains the headings of the cells
    QStringList costList;

    costList.append(data.split('\n'));

    QString firstLine = costList.front();

    QStringList CSIProperties;
    CSIProperties.append(firstLine.split(','));

    // Pop the front item off that describes the various headings of the cells
    costList.pop_front();

    for(auto& it : costList)
    {
        CSIItem newCostItem;

        QStringList costPropertiesList;

        // The regexp here is to ensure that the commas between the quotes do not get registered
        costPropertiesList.append(it.split(QRegExp(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)")));

        int sizePropList = costPropertiesList.size();

        if(sizePropList != 18)
        {
            qDebug()<<"Warning a line item was imported in "<<this->objectName()<<" that does not have the correct number of properties, it expects 18 but has"<<sizePropList;
            continue;
        }

        bool ok = true;

        auto csiCode = costPropertiesList[0];
        newCostItem.setCSIcode(csiCode);
        //skip the "eco" column
        newCostItem.setNotes(costPropertiesList[2]);
        newCostItem.setDescription(costPropertiesList[3]);
        newCostItem.setUnit(costPropertiesList[4]);
        newCostItem.setCrew(costPropertiesList[5]);

        // First the cost properties
        if(!costPropertiesList[6].isEmpty())
        {
            newCostItem.setDailyOutput(costPropertiesList[6].toDouble(&ok));

            if(!ok)
            {
                qDebug()<<"Error, something happened when importing the RVs in "<<__FUNCTION__;
                ok = true;
            }
        }

        // Then the labour rates
        auto labourHrsVal = 0.0;
        if(!costPropertiesList[7].isEmpty())
        {
            labourHrsVal = costPropertiesList[7].toDouble(&ok);
            newCostItem.setLaborHours(labourHrsVal);

            if(!ok)
            {
                qDebug()<<"Error, something happened when importing the RVs in "<<__FUNCTION__;
                ok = true;
            }
        }

        auto bareMaterialVal = 0.0;

        // It could potentially be given as a % of some value....
        if(costPropertiesList[8].contains("%"))
        {
            costPropertiesList[8].remove("%");

            bareMaterialVal = costPropertiesList[8].toDouble(&ok);

            if(ok)
                bareMaterialVal /= 100.0; //turn it into a fraction
            else
            {
                qDebug()<<"Error getting the number from a percent string in"<<__FUNCTION__;
                ok = true;
            }
        }
        else if(!costPropertiesList[8].isEmpty())
        {
            bareMaterialVal = costPropertiesList[8].toDouble(&ok);

            if(!ok)
            {
                qDebug()<<"Error, something happened when importing the RVs in "<<__FUNCTION__;
                ok = true;
            }
        }
        newCostItem.setBareMaterial(bareMaterialVal);

        auto bareLabourVal = 0.0;

        // A cost could potentially be given as a % of some value....
        if(costPropertiesList[9].contains("%"))
        {
            costPropertiesList[9].remove("%");

            bareLabourVal = costPropertiesList[9].toDouble(&ok);

            if(ok)
                bareLabourVal /= 100.0; //turn it into a fraction
            else
            {
                qDebug()<<"Error getting the number from a percent string in"<<__FUNCTION__;
                ok = true;
            }
        }
        else if(!costPropertiesList[9].isEmpty())
        {
            bareLabourVal = costPropertiesList[9].toDouble(&ok);

            if(!ok)
            {
                qDebug()<<"Error, something happened when importing the RVs in "<<__FUNCTION__;
                ok = true;
            }
        }
        newCostItem.setBareLabour(bareLabourVal);

        auto bareEquipmentVal = 0.0;

        if(!costPropertiesList[10].isEmpty())
        {
            bareEquipmentVal = costPropertiesList[10].toDouble(&ok);
            newCostItem.setBareEquipment(bareEquipmentVal);

            if(!ok)
            {
                qDebug()<<"Error, something happened when importing the RVs in "<<__FUNCTION__;
                ok = true;
            }
        }
        newCostItem.setBareTotal(costPropertiesList[11].toDouble(&ok));
        newCostItem.setTotalOP(costPropertiesList[12].toDouble(&ok));
        newCostItem.setHourlyOperCost(costPropertiesList[13].toDouble(&ok));
        newCostItem.setRentPerDay( costPropertiesList[14].toDouble(&ok));
        newCostItem.setRentPerWeek(costPropertiesList[15].toDouble(&ok));
        newCostItem.setRentPerMonth(costPropertiesList[16].toDouble(&ok));

        auto cov = costPropertiesList[17];
        newCostItem.setCov(cov);

        if(bareMaterialVal != 0.0)
            newCostItem.setTheMaterialRV(this->createRV("RVMaterialCostForCode:"+csiCode,bareMaterialVal,cov));

        if(bareLabourVal != 0.0)
            newCostItem.setTheLabourRV(this->createRV("RVLabourCostForCode:"+csiCode,bareLabourVal,cov));

        if(bareEquipmentVal != 0.0)
            newCostItem.setTheEquipmentRV(this->createRV("RVEquipmentCostForCode:"+csiCode,bareEquipmentVal,cov));

        if(labourHrsVal != 0.0)
            newCostItem.setTheLabourHoursRV(this->createRV("RVLabourRateForCode:"+csiCode,labourHrsVal,cov));


        theCSICostMap.insert(csiCode,newCostItem);
    }

    return true;
}


double RCSICostMethod::getCostMaterial(QString& csiCode)
{
    auto item = this->getCSIItem(csiCode);

    if(item == nullptr)
        return 0.0;

    auto matRV = item->getTheMaterialRV();

    if(matRV)
    {
        auto markUp = this->getMarkup(csiCode);

        auto val = matRV->getCurrentValue();

        return val*demandSurge*markUp;
    }
    else
        qDebug()<<"The material RV for CSI code: "<<csiCode<<" was not found. Returning a cost of 0.0";

    return 0.0;
}


double RCSICostMethod::getCostLabour(QString& csiCode)
{
    auto item = this->getCSIItem(csiCode);

    if(item == nullptr)
        return 0.0;

    auto labourRV = item->getTheLabourRV();

    if(labourRV)
    {
        auto markUp = this->getMarkup(csiCode);

        auto val = labourRV->getCurrentValue();

        return val*demandSurge*markUp;
    }
    else
        qDebug()<<"The labour RV for CSI code: "<<csiCode<<" was not found. Returning a cost of 0.0";

    return 0.0;
}


double RCSICostMethod::getCostEquipment(QString& csiCode)
{
    auto item = this->getCSIItem(csiCode);

    if(item == nullptr)
        return 0.0;

    auto equipRV = item->getTheEquipmentRV();

    if(equipRV)
    {
        auto markUp = this->getMarkup(csiCode);

        auto val = equipRV->getCurrentValue();

        return val*demandSurge*markUp;
    }

    return 0.0;
}


RContinuousRandomVariable* RCSICostMethod::createRV(const QString& name,const double& meanValue,const QString& cov)
{
    RContinuousRandomVariable* theRV = nullptr;

    theDomain->createObject(name,"RContinuousRandomVariable");

    theRV = qobject_cast<RContinuousRandomVariable*>(theDomain->getLastAddedObject());

    if(!theRV || cov == "NULL")
    {
        qCritical()<<"Error creating the random variable"<<name<<"in "<<__FUNCTION__;
        return nullptr;
    }

    theRV->setDistributionType(QString("Lognormal (zeta, sigma)"));
    theRV->setMeanValue(meanValue);
    theRV->setCurrentValue(meanValue);
    theRV->setCoefficientOfVariation(cov);

    return theRV;
}


double RCSICostMethod::getDemandSurge() const
{
    return demandSurge;
}


void RCSICostMethod::setDemandSurge(double value)
{
    demandSurge = value;
}


double RCSICostMethod::getLabourCosts(QVector<std::tuple<QString,double,QString>>& labourQuantityCSICodes)
{
    auto labourCost = 0.0;

    for(auto it: labourQuantityCSICodes)
    {
        auto CSIcode = std::get<0>(it);
        auto CSIquantity = std::get<1>(it);
        auto CSIdescription = std::get<2>(it);

        auto CSIcost = this->getCostLabour(CSIcode);

        if(CSIcost == 0.0)
        {
            qDebug()<<"Warning: a zero cost came into "<<__FUNCTION__<<" that should probably not be zero";
        }

        // qDebug()<<"LABOUR Description: "<<CSIdescription<<", quantity: "<<CSIquantity<<", unit cost: "<<CSIcost<<", total cost: "<<CSIcost*CSIquantity;

        auto cost = CSIcost*CSIquantity;

        labourCost += cost;
    }

    return labourCost;
}


RContinuousRandomVariable* RCSICostMethod::getLabourHours(QString& CSICode)
{
    auto item = this->getCSIItem(CSICode);

    if(item == nullptr)
    {
        qDebug()<<"Error finding CSI Item "<<CSICode<<" in "<<__FUNCTION__;
        return nullptr;
    }

    auto labourRV = item->getTheLabourHoursRV();

    return labourRV;
}


double RCSICostMethod::getMaterialCosts(QVector<std::tuple<QString,double,QString>>& materialQuantityCSICodes)
{
    auto materialCosts = 0.0;

    for(auto it: materialQuantityCSICodes)
    {
        auto CSIcode = std::get<0>(it);
        auto CSIquantity = std::get<1>(it);
        auto CSIdescription = std::get<2>(it);

        //Dont include mark up and demand surge here
        auto CSIcost = this->getCostMaterial(CSIcode);

        // qDebug()<<"MATERIAL Description: "<<CSIdescription<<", quantity: "<<CSIquantity<<", unit cost: "<<CSIcost<<", total cost: "<<CSIcost*CSIquantity;

        materialCosts+=CSIcost*CSIquantity;
    }

    return materialCosts;
}


double RCSICostMethod::getEquipmentCosts(QVector<std::tuple<QString,double,QString>>& equipmentQuantityCSICodes)
{
    auto equipmentCosts = 0.0;

    for(auto it: equipmentQuantityCSICodes)
    {
        auto CSIcode = std::get<0>(it);
        auto CSIquantity = std::get<1>(it);
        auto CSIdescription = std::get<2>(it);

        // Dont include mark up and demand surge here
        auto CSIcost = this->getCostEquipment(CSIcode);

        equipmentCosts+=CSIcost*CSIquantity;
    }

    return equipmentCosts;
}


double RCSICostMethod::getMarkup(const QString& csiCode)
{
    // The RSMeans costs do not include profit and overhead-they are bare costs, as a result need to add a profit margin if the costs will be paid by a building stakeholder
    auto markUp = 1.0;

    auto item = this->getCSIItem(csiCode);

    if(item == nullptr)
    {
        qDebug()<<"Cannot find the item in the database in "<<__FUNCTION__;
        return markUp;
    }

    auto bareTotal = item->getBareTotal();
    auto totalOP = item->getTotalOP();

    markUp = totalOP/bareTotal;

    if(markUp > 2.0)
    {
        qDebug()<<"markup for code"<<csiCode<<" is unusually high at "<<markUp;
    }

    return markUp;
}


void CSIItem::setCSIcode(const QString &value)
{
    CSIcode = value;
}


QString CSIItem::getCSIcode() const
{
    return CSIcode;
}


QString CSIItem::getDescription() const
{
    return description;
}


void CSIItem::setDescription(const QString &value)
{
    description = value;
}


QString CSIItem::getCov() const
{
    return cov;
}


QString CSIItem::getNotes() const
{
    return notes;
}


void CSIItem::setNotes(const QString &value)
{
    notes = value;
}


QString CSIItem::getUnit() const
{
    return unit;
}


void CSIItem::setUnit(const QString &value)
{
    unit = value;
}


QString CSIItem::getCrew() const
{
    return crew;
}


void CSIItem::setCrew(const QString &value)
{
    crew = value;
}


double CSIItem::getDailyOutput() const
{
    return dailyOutput;
}


void CSIItem::setDailyOutput(double value)
{
    dailyOutput = value;
}


double CSIItem::getLaborHours() const
{
    return laborHours;
}


void CSIItem::setLaborHours(double value)
{
    laborHours = value;
}


double CSIItem::getBareMaterial() const
{
    return bareMaterial;
}


void CSIItem::setBareMaterial(double value)
{
    bareMaterial = value;
}


double CSIItem::getBareLabour() const
{
    return bareLabour;
}


void CSIItem::setBareLabour(double value)
{
    bareLabour = value;
}


double CSIItem::getBareEquipment() const
{
    return bareEquipment;
}


void CSIItem::setBareEquipment(double value)
{
    bareEquipment = value;
}


double CSIItem::getBareTotal() const
{
    return bareTotal;
}


void CSIItem::setBareTotal(double value)
{
    bareTotal = value;
}


double CSIItem::getTotalOP() const
{
    return totalOP;
}


void CSIItem::setTotalOP(double value)
{
    totalOP = value;
}


double CSIItem::getHourlyOperCost() const
{
    return hourlyOperCost;
}


void CSIItem::setHourlyOperCost(double value)
{
    hourlyOperCost = value;
}


double CSIItem::getRentPerDay() const
{
    return rentPerDay;
}


void CSIItem::setRentPerDay(double value)
{
    rentPerDay = value;
}


double CSIItem::getRentPerWeek() const
{
    return rentPerWeek;
}


void CSIItem::setRentPerWeek(double value)
{
    rentPerWeek = value;
}


double CSIItem::getRentPerMonth() const
{
    return rentPerMonth;
}


void CSIItem::setRentPerMonth(double value)
{
    rentPerMonth = value;
}


RContinuousRandomVariable *CSIItem::getTheMaterialRV() const
{
    return theMaterialRV;
}


void CSIItem::setTheMaterialRV(RContinuousRandomVariable *value)
{
    theMaterialRV = value;
}


RContinuousRandomVariable *CSIItem::getTheLabourRV() const
{
    return theLabourRV;
}


void CSIItem::setTheLabourRV(RContinuousRandomVariable *value)
{
    theLabourRV = value;
}


RContinuousRandomVariable *CSIItem::getTheEquipmentRV() const
{
    return theEquipmentRV;
}


void CSIItem::setTheEquipmentRV(RContinuousRandomVariable *value)
{
    theEquipmentRV = value;
}


RContinuousRandomVariable *CSIItem::getTheLabourHoursRV() const
{
    return theLabourHoursRV;
}


void CSIItem::setTheLabourHoursRV(RContinuousRandomVariable *value)
{
    theLabourHoursRV = value;
}


void CSIItem::setCov(const QString& value)
{
    cov = value;
}


CSIItem*  RCSICostMethod::getCSIItem(const QString& csiCode)
{

    CSIItem& item = theCSICostMap[csiCode];

    if(item.getDescription() == "NULL")
    {
        qDebug()<<"The cost value for CSI code: "<<csiCode<<" was not found. Returning a cost of 0.0";
        return nullptr;
    }

    return &item;
}

