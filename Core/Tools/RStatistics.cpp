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

#include "RStatistics.h"

#ifdef XLINT
#include <xlnt/xlnt.hpp>
#include <xlnt/workbook/workbook.hpp>
#endif

#include <QDebug>

RStatistics::RStatistics()
{

}


void RStatistics::accumulate(QString objectName,const double& value)
{
    objectName.append("SUM");

    //Check if one exists, if not create one
    if(accumulateMap.contains(objectName))
    {
        auto valueVec = accumulateMap.find(objectName);
        valueVec->push_back(value);
    }
    else {

        std::vector<double> newObj={value};
        accumulateMap.insert(objectName,newObj);
    }

}


double RStatistics::getCurrentMeanValue(QString objectName)
{

    //Check if one exists, if not create one
    if(samplingMap.contains(objectName))
    {
        auto pdist = samplingMap.find(objectName);
        pdist.value().mean();
    }

    return 0.0;
}


void RStatistics::update(QString objectName,const double& value)
{
    //Check if one exists, if not create one
    if(samplingMap.contains(objectName))
    {
        auto pdist = samplingMap.find(objectName);
        pdist.value().addSample(value);
    }
    else {

        ProbDist newObj(objectName);
        newObj.addSample(value);
        samplingMap.insert(objectName,newObj);
    }

}


ProbDist* RStatistics::getDistribution(const QString& object)
{
    //Check if one exists, if not create one
    if(samplingMap.contains(object))
    {
        auto pdist = samplingMap.find(object);
        return &pdist.value();
    }
    else
        return nullptr;

}


QMap<QString, ProbDist> RStatistics::getSamplingMap() const
{
    return samplingMap;
}


#ifdef XLINT
void RStatistics::outputToExcel(const QList<QString>& theResponses)
{

    xlnt::workbook wb;
    xlnt::worksheet ws = wb.active_sheet();

    std::uint32_t i = 0;
    for(auto&& it : theResponses)
    {

        if(!samplingMap.contains(it))
        {
            qDebug()<<"The stats for "<<it<<" are not found in the recorder";
            continue;
        }

        auto probDist = samplingMap.value(it);

        auto freqs = probDist.getRelativeFrequencyDiagram();
        auto ticks = probDist.getHistogramTicks();

        for (std::uint32_t j = 0; j < freqs.size(); ++j)
        {
            xlnt::column_t col = i*2+1;
            xlnt::row_t row = j+1;

            if(row == 1)
            {
                auto name = probDist.getName().toStdString();
                ws.cell(col,row).value(name);
                ws.cell(col+1,row).value(name);
                continue;
            }

            ws.cell(col,row).value(ticks[j]);
            ws.cell(col+1,row).value(freqs[j]);
        }

        ++i;

    }

    //    auto worksheet2 = wb.create_sheet();
    //    worksheet2.cell("C3").value(69.5);
    //    worksheet2.cell("B2").value("string data");

    //    ws.cell("A1").value(5);
    //    ws.cell("B2").value("string data");
    //    ws.cell("C3").formula("=RAND()");
    //    ws.merge_cells("C3:C4");
    //    ws.freeze_panes("B2");

#ifdef __APPLE__
    wb.save("/Users/steve/Desktop/distributions.xlsx");
#elif __linux__
    wb.save("/PATH_WHERE_TO_SAVE/distributions.xlsx");
#else

#endif

}
#endif


void RStatistics::dumpAllDescriptors(const int numSamples)
{

    for(auto&& it : samplingMap)
    {
        qDebug()<<"Name: "<<it.getName();
        qDebug()<<"Mean: "<<it.mean();
        qDebug()<<"Std.Dev.: "<<it.stdDev();
        qDebug()<<"Num Samples: "<<it.getNumberSamples();
        qDebug()<<"Maximum: "<<it.getMax();
        qDebug()<<"Minimum: "<<it.getMin();

        auto values = it.getValues();
    }

    for(auto&& it : samplingMap)
    {
        QString myString = it.getName()+","+ QString::number(it.mean());
        qDebug().noquote() << myString;
    }

    qDebug()<<"Accumulated Values [item ; total average (total sum/num. samples) ; item average (total sum/num. items)]. The number of samples is: "<<numSamples;

    QMapIterator<QString, std::vector<double>> i(accumulateMap);
    while (i.hasNext()) {
        i.next();

        auto vec = i.value();

        auto sum = 0.0;
        for(auto&& it : vec)
        {
            sum +=it;
        }

        if(sum != 0.0)
            qDebug().noquote()<< i.key()+","+ QString::number(sum/static_cast<double>(numSamples));

    }


}



