#ifndef RJob_H
#define RJob_H
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

// This class contains the worker crew codes, it creates and returns the crews required for a job. This class also contains the total material quantities for the entire job.
// If two different tasks require the same crew, then only one crew is created, i.e., the same crew is used for multiple tasks

#include "RSchedulingEnums.h"
#include "RCrew.h"

#include <QString>
#include <vector>
#include <memory>

class RJob
{

public:
    RJob();
    ~RJob();

    // Number of crews on the job
    size_t getNumberOfCrews(void);

    // Add a crew to the job - returns a crew if it already exists
    RCrew* addCrew(const RCrewCode crewCode);
    RCrew* addCrew(const std::shared_ptr<RCrew>& crew);

    // Get all of the crews on the job
    const std::vector<std::shared_ptr<RCrew>> &getCrewsOnJob() const;

    // Unique job code
    QString getJobCode() const;
    void setJobCode(const QString &value);

    // Reset the total job-wide material quantities
    void reset(void);

    // Job-wide material quantities
    double& getTonsReinfNo3toNo7();
    double& getTonsReinfNo8toNo18();
    double& getYrdConcrete();
    double& getYrdWood();
    double& getCfEpoxy();

private:

    // Create a crew, or return the crew if it already exists
    RCrew createCrew(const RCrewCode& crewCode);

    // Unique job code
    QString jobCode;

    // Vector of crews on the job
    std::vector<std::shared_ptr<RCrew>> crewsOnJob;

    // Job-wide material quantities
    double tonsReinfNo3toNo7;
    double tonsReinfNo8toNo18;
    double YrdConcrete;
    double YrdWood;
    double cfEpoxy;

};

#endif // RJob_H
