#ifndef RSTATISTICS_H
#define RSTATISTICS_H
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

#include "REmpiricalProbabilityDistribution.h"

#include <QMap>

using ProbDist = REmpiricalProbabilityDistribution;

class RStatistics
{
public:
    RStatistics();

    template <typename Type>
    struct TObjectSizeCalculator
    {
        enum {size = sizeof(Type)};
    };

    template <typename Type>
    struct TObjectSizeCalculator<Type*>
    {
        enum {size = sizeof(Type)};
    };

    template <typename Container, typename type>
    unsigned int getMemorySize(Container& container)
    {
        const unsigned int elrementMemoryUse = TObjectSizeCalculator<type>::size;
        return container.size() * elrementMemoryUse;
    }

    // Adds a sample to the data
    void accumulate(QString objectName,const double& value);

    // Adds a sample to the distribution and updates the descriptors
    void update(QString objectName,const double& value);

    // Get the empirical distribution
    ProbDist* getDistribution(const QString& object);

    // Get a container of all the samples
    QMap<QString, ProbDist> getSamplingMap() const;

#ifdef XLINT
    // Outputs the relative frequency diagrams of the distributions to excel
    void outputToExcel(const QList<QString>& theResponses);
#endif

    // Prints the empirical distribution descriptors, i.e., mean, std. dev., num samples, etc.
    void dumpAllDescriptors(const int numSamples);

    // Gets the mean value of the response
    double getCurrentMeanValue(QString objectName);

private:

    QMap<QString,ProbDist> samplingMap;
    QMap<QString,std::vector<double>> accumulateMap;
    QMap<QString,ProbDist> accumulatedDistMap;

};

#endif // RSTATISTICS_H
