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
*   - Mojtaba Mahsuli                                                *
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RInHouseRandomNumberGenerator.h"
#include "RProbabilityTransformation.h"
#include "RRandomVariable.h"
#include "RDomain.h"
#include "RNormalDistribution.h"

#include <stdlib.h>

RInHouseRandomNumberGenerator::RInHouseRandomNumberGenerator(QObject *parent, QString name)
    : RRandomNumberGenerator(parent, name)
{
    theSeed = 0;
    theGeneratedNumbers = nullptr;
}


RInHouseRandomNumberGenerator::~RInHouseRandomNumberGenerator()
{
    if (theGeneratedNumbers) {
        delete theGeneratedNumbers;
        theGeneratedNumbers = nullptr;
    }
}


int RInHouseRandomNumberGenerator::getSeed()
{
    return theSeed;
}


void RInHouseRandomNumberGenerator::setSeed(int value)
{
    theSeed = value;
}


int RInHouseRandomNumberGenerator::generateStandardNormalNumbers(QVector<double> *u)
{
    // Check that a proper vector came in
    if (u==nullptr) {
        qCritical() << "The default GSL random number generator did not get a proper vector to fill with numbers.";
        return 0;
    }

    // Check the size of the vector
    double count = u->size();

    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);

    // Initial declarations
    int randomNumberBetween0AndRAND_MAX;
    double randomNumberBetween0And1;

    // Set seed
    if (theSeed != 0)
    {
        srand(theSeed);
    }
    else
    {
        srand(time(nullptr));
    }


    // Iterate "count" times
    for (int i=0; i<count; i++) {


        // Generate a number between 0 and RAND_MAX
        randomNumberBetween0AndRAND_MAX = rand();

        // Modify it so that the value lies between 0 and 1
        randomNumberBetween0And1 = (double)randomNumberBetween0AndRAND_MAX/RAND_MAX;

        // Treat two special cases
        if (randomNumberBetween0And1 == 0.0) {
            randomNumberBetween0And1 = 0.0000001;
        }
        if (randomNumberBetween0And1 == 1.0) {
            randomNumberBetween0And1 = 0.9999999;
        }


        // Transform that number into a standard normal variable
        //    Phi(z) = F(x)
        //    z = invPhi( F(x) )
        //       where F(x) for the uniform distribution
        //       from 0 to 1 in fact is equal to x itself.
        (*u)[i] = theNormalDistribution.getInverseNormalCDF(randomNumberBetween0And1, 1.0);
    }


    // Reset seed (from Paolo?)
    //seed = randomNumberBetween0AndRAND_MAX;

    return 0;
}


void RInHouseRandomNumberGenerator::reset()
{
}


