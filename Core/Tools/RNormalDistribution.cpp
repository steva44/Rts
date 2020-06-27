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

#include "RNormalDistribution.h"

#include <math.h>
#include <float.h>
#include <QtDebug>

RNormalDistribution::RNormalDistribution(RDomain *domain)
    : RMethod(domain, nullptr)
{

}


RNormalDistribution::~RNormalDistribution()
{

}


double RNormalDistribution::getNormalPDF(double x, double sigma)
{
    const double pi = 4.0 * atan(1.0);
    double result;
    result = 1.0/sqrt(2 * pi * pow(sigma,2)) * exp(-1.0/2.0 * pow((x/sigma),2));
    return result;
}


double RNormalDistribution::getNormalCDF(double x, double sigma)
{
    double result = standardNormalPhi(x/sigma);

    if (result > 1.0) {
        qCritical() << "Error: The CDF is" << result << "It must be between zero and unity";
    }

    return result;
}


double RNormalDistribution::getInverseNormalCDF(double x, double sigma)
{
    double result = sigma * standardNormalInversePhi(x);
    return result;
}


// ************************************************
//    THE "PHI" METHODS
// ************************************************
double RNormalDistribution::standardNormalPhi(double uValue)
{
    return 0.5 * erfc(-uValue / sqrt(2.0));
}



double RNormalDistribution::standardNormalInversePhi(double probValue)
{
    /*
    double trval = probValue;
    if (trval <= 0.0) {
        // technically this should return negative infinity
        //opserr << "WARNING: Invalid probability value (" << trval << ") input <= 0 to RandomVariable::standardNormalInversePhi()" << endln;
        trval = 2.0*DBL_EPSILON;
    } else if (trval >= 1.0) {
        // technically this should return positive infinity
        //opserr << "WARNING: Invalid probability value (" << trval << ") input >= 1 to RandomVariable::standardNormalInversePhi()" << endln;
        trval = 1.0-2.0*DBL_EPSILON;
    }
    */

    return sqrt(2.0) * inverseErrorFunction(2.0*probValue-1.0);
}


// ************************************************
//    THE ERROR FUNCTION AND ITS INVERSE
// ************************************************
double RNormalDistribution::errorFunction(double x)
{
    const double pi = 4.0 * atan(1.0);
    // ErrorFunction(x) = 2/sqrt(pi) * integral from 0 to x of exp(-t^2) dt.
    double a1,a2,a3,a4,a5;
    double b1,b2,b3,b4;
    double c1,c2,c3,c4,c5,c6,c7,c8,c9;
    double d1,d2,d3,d4,d5,d6,d7,d8;
    double p1,p2,p3,p4,p5,p6;
    double q1,q2,q3,q4,q5;
    double y,z,xnum,xden,del,result;
    //double pi = acos(-1.0);

    // Evaluate  errorFunction  for  |x| <= 0.46875
    if ( fabs(x) <= 0.46875 )
    {
        a1 = 3.16112374387056560e00;
        a2 = 1.13864154151050156e02;
        a3 = 3.77485237685302021e02;
        a4 = 3.20937758913846947e03;
        a5 = 1.85777706184603153e-1;
        b1 = 2.36012909523441209e01;
        b2 = 2.44024637934444173e02;
        b3 = 1.28261652607737228e03;
        b4 = 2.84423683343917062e03;
        y = fabs(x);
        z = y * y;
        xnum = a5 * z;
        xden = z;
        xnum = (xnum + a1) * z;
        xden = (xden + b1) * z;
        xnum = (xnum + a2) * z;
        xden = (xden + b2) * z;
        xnum = (xnum + a3) * z;
        xden = (xden + b3) * z;
        result = x * (xnum + a4) / (xden + b4);
    }


    // Evaluate  errorFunction  for 0.46875 <= |x| <= 4.0
    else if ( fabs(x) > 0.46875 && fabs(x) <= 4.0 )
    {
        c1 = 5.64188496988670089e-1;
        c2 = 8.88314979438837594e00;
        c3 = 6.61191906371416295e01;
        c4 = 2.98635138197400131e02;
        c5 = 8.81952221241769090e02;
        c6 = 1.71204761263407058e03;
        c7 = 2.05107837782607147e03;
        c8 = 1.23033935479799725e03;
        c9 = 2.15311535474403846e-8;
        d1 = 1.57449261107098347e01;
        d2 = 1.17693950891312499e02;
        d3 = 5.37181101862009858e02;
        d4 = 1.62138957456669019e03;
        d5 = 3.29079923573345963e03;
        d6 = 4.36261909014324716e03;
        d7 = 3.43936767414372164e03;
        d8 = 1.23033935480374942e03;
        y = fabs(x);
        xnum = c9 * y;
        xden = y;
        xnum = (xnum + c1) * y;
        xden = (xden + d1) * y;
        xnum = (xnum + c2) * y;
        xden = (xden + d2) * y;
        xnum = (xnum + c3) * y;
        xden = (xden + d3) * y;
        xnum = (xnum + c4) * y;
        xden = (xden + d4) * y;
        xnum = (xnum + c5) * y;
        xden = (xden + d5) * y;
        xnum = (xnum + c6) * y;
        xden = (xden + d6) * y;
        xnum = (xnum + c7) * y;
        xden = (xden + d7) * y;
        result = (xnum + c8) / (xden + d8);
        z = floor(y*16)/16;
        del = (y-z) * (y+z);
        result = exp(-z * z) * exp(-del) * result;
    }


    // Evaluate  erfc  for |x| > 4.0
    else if ( fabs(x) > 4.0 )
    {
        p1 = 3.05326634961232344e-1;
        p2 = 3.60344899949804439e-1;
        p3 = 1.25781726111229246e-1;
        p4 = 1.60837851487422766e-2;
        p5 = 6.58749161529837803e-4;
        p6 = 1.63153871373020978e-2;
        q1 = 2.56852019228982242e00;
        q2 = 1.87295284992346047e00;
        q3 = 5.27905102951428412e-1;
        q4 = 6.05183413124413191e-2;
        q5 = 2.33520497626869185e-3;
        y = fabs(x);
        z = 1 / (y * y);
        xnum = p6 * z;
        xden = z;
        xnum = (xnum + p1) * z;
        xden = (xden + q1) * z;
        xnum = (xnum + p2) * z;
        xden = (xden + q2) * z;
        xnum = (xnum + p3) * z;
        xden = (xden + q3) * z;
        xnum = (xnum + p4) * z;
        xden = (xden + q4) * z;
        result = z * (xnum + p5) / (xden + q5);
        result = (1/sqrt(pi) -  result) / y;
        z = floor(y*16)/16;
        del = (y-z) * (y+z);
        result = exp(-z * z) * exp(-del) * result;
        //if ( isnan(result) )
        //  result = 0;
    }

    else {
        // should never reach here unless the input argument was nan
        qCritical() << "RandomVariable::errorFunction WARNING: invalid input (" << x << ")";
        result = 0;
    }


    // Final computations
    if ( x > 0.46875 )
        result = (0.5 - result) + 0.5;
    if ( x < -0.46875 )
        result = (-0.5 + result) - 0.5;

    return result;
}


double RNormalDistribution::inverseErrorFunction(double y)
{
    const double pi = 4.0 * atan(1.0);

    double a1,a2,a3,a4;
    double b1,b2,b3,b4;
    double c1,c2,c3,c4;
    double d1,d2;
    double x,z;

    // Coefficients in rational approximations.
    a1 = 0.886226899;
    a2 = -1.645349621;
    a3 = 0.914624893;
    a4 = 0.140543331;
    b1 = -2.118377725;
    b2 = 1.442710462;
    b3 = -0.329097515;
    b4 = 0.012229801;
    c1 = -1.970840454;
    c2 = -1.624906493;
    c3 = 3.429567803;
    c4 = 1.641345311;
    d1 = 3.543889200;
    d2 = 1.637067800;

    // Central range
    if ( fabs(y) <= 0.7 )
    {
        z = y * y;
        x = y * (((a4*z+a3)*z+a2)*z+a1) / ((((b4*z+b3)*z+b2)*z+b1)*z+1);
    }

    // Near end-points of range
    else if ( y > 0.7  &&  y < 1 )
    {
        z = sqrt(-log((1-y)/2));
        x = (((c4*z+c3)*z+c2)*z+c1) / ((d2*z+d1)*z+1);
    }

    else if ( y < -0.7  &&  y > -1 )
    {
        z = sqrt(-log((1+y)/2));
        x = -(((c4*z+c3)*z+c2)*z+c1) / ((d2*z+d1)*z+1);
    }

    // domain input errors: either abs(y) >= 1, or nan was input
    else
    {
        // this is the -1 limit
        if ( y < 0 )
            x = -DBL_MAX;

        // this is the +1 limit
        else if ( y > 0 )
            x = DBL_MAX;

        else {
            qCritical() << "RandomVariable::inverseErrorFunction WARNING: input (" << y << ") outside of [-1,1] domain.";
            x = 0;
        }

        // do not perform Newton on these boundary values
        return x;
    }

    //static const double pi = acos(-1.0);
    static const double twoOverRootPi = 2.0/sqrt(pi);
    // Two steps of Newton-Raphson correction to full accuracy.
    // Without these steps, erfinv(y) would be about 3 times
    // faster to compute, but accurate to only about 6 digits.
    //x = x - (errorFunction(x) - y) / (2.0/sqrt(pi) * exp(-pow(x,2.0)));
    //x = x - (errorFunction(x) - y) / (2.0/sqrt(pi) * exp(-pow(x,2.0)));
    x = x - (errorFunction(x) - y) / (twoOverRootPi * exp(-pow(x,2.0)));
    x = x - (errorFunction(x) - y) / (twoOverRootPi * exp(-pow(x,2.0)));

    return x;

}


