#ifndef RCSISchedulingMethod_H
#define RCSISchedulingMethod_H
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

// This class provides the time it takes to complete a set of tasks given as CSI codes

#include "RMethod.h"
#include "RContinuousRandomVariable.h"
#include "RSchedulingEnums.h"
#include "RCrew.h"
#include "RLabourer.h"
#include "REquipment.h"

#include <QMap>

class RCSICostMethod;
class RJob;
class RTask;

class RCSISchedulingMethod : public RMethod
{
    Q_OBJECT

public:
    RCSISchedulingMethod(RDomain *theDomain, QString name);
    ~RCSISchedulingMethod();

    // Add a job to the scheduler
    void addJob(RJob& job);

    // Clear the jobs
    void reset(void);

    // Total worker hours to complete all jobs
    double getTotalWorkHours(void);

    // Number of workers to complete all jobs
    int getNumberOfWorkers(void);

    // Number of labourers to complete all jobs
    int getNumberOfCrews(void);

    // Number of jobs
    int getNumJobs(void);

    // Number of tasks from all jobs
    int getNumTasks(void);

    // Vector of tasks from all jobs
    std::vector<RTask> getAllTasks(void);

    // Critical path, i.e., the number of hours to complete all jobs
    double getCriticalPath(void);

    // Method to calcualate the costs from CSI codes
    RCSICostMethod *getTheCostMethod() const;
    void setTheCostMethod(RCSICostMethod *value);

private:

    // Method to calcualate the costs from CSI codes
    RCSICostMethod* theCostMethod;

    // Vector of the jobs
    QVector<RJob> theJobs;

    // Sorts the task according to their CSI codes
    QVector<RTask> sortTasks(std::vector<RTask>* origTaskVec);
};

#endif // RCSISchedulingMethod_H
