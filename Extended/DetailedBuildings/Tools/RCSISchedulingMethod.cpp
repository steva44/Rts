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

#include "RCSISchedulingMethod.h"
#include "RCSICostMethod.h"
#include "RComponent.h"
#include "RJob.h"
#include "RTask.h"

#include <numeric>
#include <QFile>

RCSISchedulingMethod::RCSISchedulingMethod(RDomain *theDomain,  QString name)
    : RMethod(theDomain, name)
{
    theCostMethod = nullptr;
}


RCSISchedulingMethod::~RCSISchedulingMethod()
{

}


int RCSISchedulingMethod::getNumberOfWorkers(void)
{
    int totalNumWorkers = 0;

    for(auto job : theJobs)
    {
        auto crewVector = job.getCrewsOnJob();

        for(auto& crew : crewVector)
        {
            auto numberOfWorkersOnCrew = crew->getNumberOfLabourers();

            // qDebug()<<"Crew code"<<crew->getCrewCode();

            totalNumWorkers += numberOfWorkersOnCrew;
        }
    }

    // qCritical()<<"***********TOTAL NUMBER WORKERS************"<<totalNumWorkers;

    return totalNumWorkers;
}


int RCSISchedulingMethod::getNumberOfCrews(void)
{
    int totalNumCrew = 0;

    for(auto job : theJobs)
    {
        auto crewVector = job.getCrewsOnJob();

        totalNumCrew += crewVector.size();
    }

    // qCritical()<<"***********TOTAL NUMBER CREWS************"<<totalNumCrew;

    return totalNumCrew;
}


double RCSISchedulingMethod::getTotalWorkHours(void)
{
    auto totalHours = 0.0;

    for(auto job : theJobs)
    {
        auto crewVector = job.getCrewsOnJob();

        for(auto& crew : crewVector)
        {
            auto  taskVector = crew->getTheTaskVector();

            auto numberOfWorkers = crew->getNumberOfLabourers();

            for(auto task : taskVector)
            {
                auto CSICode = task.getCSICode();

                auto numberOfWorkUnits = task.getQuantityOfUnits();

                auto crewRate = theCostMethod->getLabourHours(CSICode);

                if(crewRate == nullptr)
                {
                    qCritical()<<"Error, a null crew rate was returned for task "<<task.getDescription()<<" in "<<__FUNCTION__;
                    continue;
                }

                auto workRate = crewRate->getCurrentValue();

                if(workRate == 0.0)
                {
                    qCritical()<<"Error, the crew rate is zero for task "<<task.getDescription()<<" in "<<__FUNCTION__;
                    continue;
                }

                auto workHrsForTask = numberOfWorkUnits * workRate * numberOfWorkers;

                totalHours += workHrsForTask;
            }
        }
    }

    // qCritical()<<"***********TOTAL WORK HOURS************"<<totalHours;

    return totalHours;
}


double RCSISchedulingMethod::getCriticalPath(void)
{
    std::vector<RTask> taskVec;

    // First collect all of the tasks, the tasks will be sorted according to the CSI codes, those with a lower code need to be completed before the next task with a higher code can begin
    for(auto job : theJobs)
    {
        auto crewVector = job.getCrewsOnJob();

        for(auto& crew : crewVector)
        {
            auto  crewTaskVec = crew->getTheTaskVector();

            taskVec.insert( taskVec.end(), crewTaskVec.begin(), crewTaskVec.end() );

        }
    }

    // Sort the tasks
    auto sortedTasks = this->sortTasks(&taskVec);

    // Estimate the critical path
    auto criticalTime = 0.0;

    for(auto task : sortedTasks)
    {
        auto CSICode = task.getCSICode();

        auto crewRate = theCostMethod->getLabourHours(CSICode);

        if(crewRate == nullptr)
        {
            qCritical()<<"Error, a nullptr for crew rate was returned for task "<<task.getDescription()<<" in "<<__FUNCTION__;
            continue;
        }

        auto workRate = crewRate->getCurrentValue();

        if(workRate == 0.0)
        {
            qCritical()<<"Error, the crew rate is zero for task "<<task.getDescription()<<" in "<<__FUNCTION__;
            continue;
        }


        criticalTime += workRate*task.getQuantityOfUnits();
    }

    return criticalTime;
}


void RCSISchedulingMethod::addJob(RJob& job)
{
    theJobs.push_back(job);
}


QVector<RTask> RCSISchedulingMethod::sortTasks(std::vector<RTask>* origTaskVec)
{

    auto listSize = origTaskVec->size();

    std::vector<int> indices(listSize);
    std::vector<double> CSICodes;

    // Fill with 0, 1, ..., listSize
    std::iota (std::begin(indices), std::end(indices), 0);

    for(auto it : *origTaskVec)
    {
        CSICodes.push_back(it.getCSICodeAsDouble());
    }

    // Sort task list according to the order defined of CSI codes
    std::vector<std::pair<double, int>> pairt(listSize);

    // Storing the respective array elements in pairs
    for (size_t i = 0; i < listSize; ++i)
    {
        pairt[i].first = CSICodes[i];
        pairt[i].second = indices[i];
    }

    // Sorting the pair array
    std::sort(pairt.begin(), pairt.end());

    // Modifying original arrays
    for (size_t i = 0; i < listSize; ++i)
    {
        CSICodes[i] = pairt[i].first;
        indices[i] = pairt[i].second;
    }

    QVector<RTask> newTaskVector;

    // Sorted task list
    for(auto it : indices)
    {
        newTaskVector.push_back(origTaskVec->at(it));
    }

    return newTaskVector;
}


void RCSISchedulingMethod::reset(void)
{
    theJobs.clear();
}


RCSICostMethod *RCSISchedulingMethod::getTheCostMethod() const
{
    return theCostMethod;
}


void RCSISchedulingMethod::setTheCostMethod(RCSICostMethod *value)
{
    theCostMethod = value;
}


int RCSISchedulingMethod::getNumJobs(void)
{
    return theJobs.size();
}


int RCSISchedulingMethod::getNumTasks(void)
{
    int numTasks = 0;

    for(auto job : theJobs)
    {
        auto crewVector = job.getCrewsOnJob();

        for(auto crew : crewVector)
        {
            auto tasksVec = crew->getTheTaskVector();

            numTasks += tasksVec.size();
        }
    }

    return numTasks;
}


std::vector<RTask> RCSISchedulingMethod::getAllTasks(void)
{
    std::vector<RTask> allTaskVec;

    for(auto job : theJobs)
    {
        auto crewVector = job.getCrewsOnJob();

        for(auto crew : crewVector)
        {
            auto tasksVec = crew->getTheTaskVector();

            allTaskVec.insert( allTaskVec.end(), tasksVec.begin(), tasksVec.end() );
        }
    }

    return allTaskVec;
}


