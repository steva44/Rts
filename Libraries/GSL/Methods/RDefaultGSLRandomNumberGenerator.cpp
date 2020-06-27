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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

#include "RDefaultGSLRandomNumberGenerator.h"
#include "RProbabilityTransformation.h"
#include "RRandomVariable.h"
#include "RDomain.h"

#include <gsl/gsl_cdf.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_rng.h>
#include <math.h>

RDefaultGSLRandomNumberGenerator::RDefaultGSLRandomNumberGenerator(QObject *parent, QString name)
    : RRandomNumberGenerator(parent, name)
{
    theSeed = 0;
    theGeneratedNumbers = 0;

    theRandomGeneratorType = 0;
    theRandomGenerator = 0;

    theRandomGeneratorType = gsl_rng_default;
    theRandomGenerator = gsl_rng_alloc(theRandomGeneratorType);
    gsl_rng_set(theRandomGenerator, theSeed);
}


RDefaultGSLRandomNumberGenerator::~RDefaultGSLRandomNumberGenerator()
{
    if (theGeneratedNumbers) {
        delete theGeneratedNumbers;
        theGeneratedNumbers = 0;
    }

    if (theRandomGenerator) {
        gsl_rng_free(theRandomGenerator);
        theRandomGenerator = 0;
        theRandomGeneratorType = 0;
    }
}


int RDefaultGSLRandomNumberGenerator::getSeed()
{
    return theSeed;
}


void RDefaultGSLRandomNumberGenerator::setSeed(int value)
{
    theSeed = value;
}


int RDefaultGSLRandomNumberGenerator::generateStandardNormalNumbers(QVector<double> *u)
{
    // Check that a proper vector came in
    if (u==0) {
        qCritical() << "The default GSL random number generator did not get a proper vector to fill with numbers.";
        return 0;
    }


    // Check the size of the vector
    double count = u->size();


    // Iterate "count" times
    for (int i = 0; i<count; i++) {

        double stdNormalRandomNumber = gsl_ran_ugaussian(theRandomGenerator);

        // Add the random number to the vector theGeneratedNumbers
        (*u)[i] = stdNormalRandomNumber;
    }

    return 0;
}


void RDefaultGSLRandomNumberGenerator::reset()
{
    if (theRandomGenerator) {
        gsl_rng_free(theRandomGenerator);
        theRandomGenerator = 0;
        theRandomGeneratorType = 0;
    }

    // Create a random number generator
    gsl_rng_env_setup();
    theRandomGeneratorType = gsl_rng_default;
    theRandomGenerator = gsl_rng_alloc(theRandomGeneratorType);
    gsl_rng_set(theRandomGenerator, theSeed);
}

