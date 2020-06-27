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

#include "RInHouseProbabilityDistributions.h"
#include "RDomain.h"

#include "math.h"
#include "float.h"

RInHouseProbabilityDistributions::RInHouseProbabilityDistributions(QObject *parent, QString name)
    : RProbabilityDistributions(parent, name)
{

}


RInHouseProbabilityDistributions::~RInHouseProbabilityDistributions()
{

}


double RInHouseProbabilityDistributions::gammaFunction(double x)
{
    const double pi = 4.0 * atan(1.0);
    double res;
    //Abbas
    //#ifdef _LINUX
    //      return tgamma(x);
    //#endif
    //static const double pi = std::acos(-1.0);

    if (x==0 || ( x < 0.0 && floor(x)==x ) )  {
        qCritical() << "Invalid input to the gamma function";
    }
    else {
        QVector<double> *p = new QVector<double>(9);
        QVector<double> *q = new QVector<double>(9);
        QVector<double> *c = new QVector<double>(8);

        (*p)[0] = 0.0;
        (*p)[1] = -1.71618513886549492533811e+0;
        (*p)[2] = 2.47656508055759199108314e+1;
        (*p)[3] = -3.79804256470945635097577e+2;
        (*p)[4] = 6.29331155312818442661052e+2;
        (*p)[5] = 8.66966202790413211295064e+2;
        (*p)[6] = -3.14512729688483675254357e+4;
        (*p)[7] = -3.61444134186911729807069e+4;
        (*p)[8] = 6.64561438202405440627855e+4;

        (*q)[0] = 0.0;
        (*q)[1] = -3.08402300119738975254353e+1;
        (*q)[2] = 3.15350626979604161529144e+2;
        (*q)[3] = -1.01515636749021914166146e+3;
        (*q)[4] = -3.10777167157231109440444e+3;
        (*q)[5] = 2.25381184209801510330112e+4;
        (*q)[6] = 4.75584627752788110767815e+3;
        (*q)[7] = -1.34659959864969306392456e+5;
        (*q)[8] = -1.15132259675553483497211e+5;

        (*c)[0] = 0.0;
        (*c)[1] = -1.910444077728e-03;
        (*c)[2] = 8.4171387781295e-04;
        (*c)[3] = -5.952379913043012e-04;
        (*c)[4] = 7.93650793500350248e-04;
        (*c)[5] = -2.777777777777681622553e-03;
        (*c)[6] = 8.333333333333333331554247e-02;
        (*c)[7] = 5.7083835261e-03;

        double y, y1;
        double fact;
        double x1, xn;
        double ysq;
        double sum;
        double spi;
        bool flag01 = false;
        bool flag1_12 = false;
        bool flagNegative = false;

        // If x is negative
        if (x<0.0) {
            y = -x;
            y1 = floor(y);
            res = y - y1;
            fact = -pi / sin(pi*res) * (1 - 2*fmod(y1,2));
            x = y + 1;
            flagNegative = true;
        }
        // Now x is positive

        // Map x in interval [0,1] to [1,2]
        if (x<1.0) {
            x1 = x;
            x = x1 + 1.0;
            flag01 = true;
        }

        // Map x in interval [1,12] to [1,2]
        if (x<12.0) {
            xn = floor(x) - 1;
            x = x - xn;
            // Evaluate approximation for 1 < x < 2
            double z = x - 1.0;
            double xnum = 0.0;
            double xden = xnum + 1.0;


            for (int i = 1 ; i<=8; i++ ) {
                xnum = (xnum + p->at(i)) * z;
                xden = xden * z + q->at(i);
            }

            res = xnum / xden + 1.0;
            flag1_12 = true;
        }

        // Adjust result for case  0.0 < x < 1.0
        if (flag01)  {
            res = res / x1;
        }
        else if (flag1_12){   // Adjust result for case  2.0 < x < 12.0
            double max_xn = xn;
            for (int m=1;  m<=max_xn; m++) {
                res = res * x;
                x = x + 1;
                xn = xn - 1;
            }
        }

        // Evaluate approximation for x >= 12
        if (x>=12.0) {
            y = x;
            ysq = y * y;
            sum = c->at(7);
            for (int i = 1; i<=6; i++ ) {
                sum = sum / ysq + c->at(i);
            }

            spi = 0.9189385332046727417803297;
            sum = sum / y - y + spi;
            sum = sum + (y-0.5)*log(y);
            res = exp(sum);
        }

        if (flagNegative) {
            res = fact / res;
            if(p)
            {
                delete p;
                p = 0;
            }
            if(q)
            {
                delete q;
                q = 0;
            }
            if(c)
            {
                delete c;
            }
        }
    }


    return res;
}


double RInHouseProbabilityDistributions::incompleteGammaFunction(double a, double x)
{
    double b = x;
    if (x == 0.0) {
        b = 0.0;
    }
    if (a == 0.0) {
        b = 1.0;
    }
    // Series expansion for x < a+1
    if ( (a != 0.0) && (x != 0.0) && (x < a+1.0) ) {
        double ap = a;
        double del = 1.0;
        double sum = del;
        while (fabs(del) >= 1.0e-12*fabs(sum)) {
            ap = ap + 1.0;
            del = x * del / ap;
            sum = sum + del;
        }
        b = sum * exp(-x + a*log(x) - log(gammaFunction(a+1)) );
        if ( x > 0 && b > 1 )
            b = 1.0;

    } else {
        // Continued fraction for x >= a+1
        double a0 = 1.0;
        double a1 = x;
        double b0 = 0.0;
        double b1 = a0;
        double fac = 1.0/a1;
        double n = 1;
        double g = b1*fac;
        double gold = b0;
        while (fabs(g-gold) >= 1.0e-12*fabs(g))  {
            gold = g;
            double ana = n - a;
            a0 = (a1 + a0 *ana) * fac;
            b0 = (b1 + b0 *ana) * fac;
            double anf = n*fac;
            a1 = x * a0 + anf * a1;
            b1 = x * b0 + anf * b1;
            fac = 1.0 / a1;
            g = b1 * fac;
            n = n + 1.0;
        }
        b = 1.0 - exp(-x + a*log(x) - log(gammaFunction(a)) ) * g;
    }

    return b;
}


double RInHouseProbabilityDistributions::errorFunction(double x)
{
    //Abbas
    //#ifdef _LINUX
    //       return erf(x);
    //#endif

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


double RInHouseProbabilityDistributions::standardNormalPhi(double uValue)
{
    return 0.5 * erfc(-uValue / sqrt(2.0));
}


double RInHouseProbabilityDistributions::standardNormalInversePhi(double probValue)
{
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
    static const double rootTwo = sqrt(2.0);
    double result = rootTwo * inverseErrorFunction(2.0*trval-1.0);
    return result;
}


double RInHouseProbabilityDistributions::inverseErrorFunction(double y)
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


// Methods for the Normal distribution:
double RInHouseProbabilityDistributions::getNormalPDF(double x, double sigma)
{
    //Abbas: The expression for the Normal PDF function in the Opensees
    //does not have "sigma" in its denominator! As a result I have changed the PDF, CDF and inverse CDF accordingly.
    const double pi = 4.0 * atan(1.0);
    double result;
    result = 1.0/sqrt(2 * pi * pow(sigma,2)) * exp(-1.0/2.0 * pow((x/sigma),2));
    return result;
}


double RInHouseProbabilityDistributions::getNormalCDF(double x, double sigma)
{
    return standardNormalPhi(x/sigma);
}


double RInHouseProbabilityDistributions::getInverseNormalCDF(double x, double sigma)
{

    double result = sigma * sqrt(2.0) * inverseErrorFunction(2.0*x-1.0);

    //double result = sigma * (x*sigma); Why was this here? :)

    return result;
}


// Methods for the Number GenerationstandardNormalInversePhi
double RInHouseProbabilityDistributions::generateStandardNormalNumber()
{
    qCritical() << "The normal random number generator is not yet available in the in-house probability distributions";
    return 0;
}


double RInHouseProbabilityDistributions::generateBinomialNumber(double p, int n)
{
    qCritical() << "The normal random number generator is not yet available in the in-house probability distributions";
    return 0;
}


double RInHouseProbabilityDistributions::generateExponentialNumber(double mu)
{
    qCritical() << "The normal random number generator is not yet available in the in-house probability distributions";
    return 0;
}


// Methods for the Exponential distribution:
double RInHouseProbabilityDistributions::getExponentialPDF(double x, double mu)
{
    double result;
    if ( 0.0 < x ){
        result = (1.0/mu) * exp(-(1.0/mu)*x);
    }
    else {
        result = 0.0;
    }
    return result;
}


double RInHouseProbabilityDistributions::getExponentialCDF(double x, double mu)
{
    double result;
    if ( 0.0 < x){
        result = 1.0 - exp(-(1.0/mu)*x);
    }
    else {
        result = 0.0;
    }
    return result;
}


double RInHouseProbabilityDistributions::getInverseExponentialCDF(double x, double mu)
{
    double result;

    result = -mu * log(1.0 - x);

    return result;
}


// Methods for the Laplace distribution:
double RInHouseProbabilityDistributions::getLaplacePDF(double x, double a)
{
    //Assumed that Mean=0.0
    double result;
    result = 1.0/2.0/a * exp(-fabs(x/a));
    return result;
}


double RInHouseProbabilityDistributions::getLaplaceCDF(double x, double a)
{
    double result;
    if (x < 0.0)  {
        result = 0.5*exp(-1.0/a*fabs(x));
    }
    else  {
        result = 1.0 - 0.5*exp(-1.0/a*fabs(x));
    }
    return result;
}


double RInHouseProbabilityDistributions::getInverseLaplaceCDF(double x, double a)
{
    return 0.0;
}


// Methods for the Rayleigh distribution:
double RInHouseProbabilityDistributions::getRayleighPDF(double x, double sigma)
{
    // Abbas: Please note that the Opensees Rayleigh distribution is twice Rts one.
    double result;
    if ( 0.0 < x ) {
        result = x/(sigma*sigma) * exp(-0.5 * pow(x/sigma,2));
    }
    else {
        result = 0.0;
    }
    return result;
}


double RInHouseProbabilityDistributions::getRayleighCDF(double x, double sigma)
{
    double result;
    if ( 0.0 < x ) {
        result = 1-exp(-0.5 * pow(x/sigma,2));
    }
    else {
        result = 0.0;
    }
    return result;
}


double RInHouseProbabilityDistributions::getInverseRayleighCDF(double x, double sigma)
{
    return sigma*sqrt(-2*log(1-x));
}


// Methods for the Gamma distribution:
double RInHouseProbabilityDistributions::getGammaPDF(double x, double a, double b)
{
    double result;
    if ( 0.0 < x ) {
        result = (1.0/b)*pow((1.0/b*x),(a-1.0))*exp(-1.0/b*x) / gammaFunction(a);
    }
    else {
        result = 0.0;
    }

    return result;
}


double RInHouseProbabilityDistributions::getGammaCDF(double x, double a, double b)
{
    double result;
    if ( 0.0 < x ) {
        result = incompleteGammaFunction(a,(1.0/b*x));
    }
    else {
        result = 0.0;
    }

    return result;
}


double RInHouseProbabilityDistributions::getInverseGammaCDF(double x, double a, double b)
{
    //Abbas: I am not sure how to deal with this part:
    // Here we want to solve the nonlinear equation:
    //         x = getCDFvalue(result)
    // with respect to x.
    // A Newton scheme to find roots - f(x)=0 - looks something like:
    //         x(i+1) = x(i) - f(xi)/f'(xi)
    // In our case the function f(x) is: f(x) = x - getCDFvalue(x)

    // KRM -- Note to writer of above: Newton does not converge for all cases, particularly those
    // that look like CDFs when the start point is not close to the zero point.
    // Bootstrap with estimate from equivalent normal distribution

    //Abbas: More specifically, we need to change this parts:
    //static NormalRV normRV( 1, getMean(), getStdv());
    double x_old = this->getInverseNormalCDF(x-a*b,sqrt(a)*b)            ;//normRV.getInverseCDFvalue(x);

    double tol = 1.0e-6;
    double x_new = x_old;
    double dx = x_old;
    int step = 1;
    int nmax = 50;

    if (x > 1.0e-3) {
        while ( (step < nmax) && (fabs(dx/x_old) > tol) ) {
            // compute dx
            dx = (this->getGammaCDF(x_new,a,b) - x) / getGammaPDF(x_new,a,b);

            // Take a Newton step
            x_new = x_new - dx;
            step++;
        }
    }

    // use bisection for small probabilities (x > 0 always for gamma). Here we (arbitrarily)
    // limit the left extreme x to be 0.1
    else {
        double left = 0.1;
        double right = 2.5*x_old;
        int change = 0;
        double fa = 0;
        double fc = 0;

        while (change == 0 && step <= nmax) {
            x_new = (left+right)/2.0;
            fc = getGammaCDF(x_new,a,b) - x;
            fa = getGammaCDF(left,a,b) - x;
            if (fabs(fc) < tol || (right-left)/2.0 < tol)
                change = 1;

            if ( fa>0 && fc>0 )
                left = x_new;
            else if ( fa<0 && fc<0 )
                left = x_new;
            else
                right = x_new;

            step++;
        }
    }

    if ( step >= 50 ) {
        qCritical() << "WARNING: GammaRV did not converge to find inverse CDF!";
        // decide later what to do with this terminal case
    }

    return x_new;
}


// Methods for the Uniform distribution:
double RInHouseProbabilityDistributions::getUniformPDF(double x, double a, double b)
{
    double result;
    if ( x >= a && x <= b ) {
        result = 1.0/(b-a);
    }
    else {
        result = 0.0;
    }
    return result;
}


double RInHouseProbabilityDistributions::getUniformCDF(double x, double a, double b)
{
    double result;
    if ( x >= a && x <= b ) {
        result = (x-a)/(b-a);
    }
    else if (x > b) {
        result = 1.0;
    }
    else {
        result = 0.0;
    }
    return result;
}


double RInHouseProbabilityDistributions::getInverseUniformCDF(double x, double a, double b)
{
    return x * (b-a) + a;
}


// Methods for the Lognormal distribution:
double RInHouseProbabilityDistributions::getLognormalPDF(double x, double zeta, double sigma)
{
    double result;
    const double pi = 4.0 * atan(1.0);
    if ( 0.0 < x ) {
        result = 1.0/(sqrt(2.0*pi)*sigma*x) * exp(-0.5* pow ( (log(x)-zeta) / sigma, 2.0 )  );
    }
    else {
        result = 0.0;
    }
    return result;
}


double RInHouseProbabilityDistributions::getLognormalCDF(double x, double zeta, double sigma)
{
    return standardNormalPhi( (log(x)-zeta)/sigma );
}


double RInHouseProbabilityDistributions::getInverseLognormalCDF(double x, double zeta, double sigma)
{
    double inverseNormal = 0;

    // now use base class functions
    inverseNormal = standardNormalInversePhi(x);
    return exp(inverseNormal*sigma + zeta);

}


// Methods for the Chi-squared distribution:
double RInHouseProbabilityDistributions::getChiSquaredPDF(double x, double nu)
{
    double result;
    if ( 0.0 < x ) {
        double a = gammaFunction(0.5*nu);
        result = pow(0.5*x,0.5*nu-1.0)*exp(-0.5*x)/(2.0*a);
    }
    else {
        result = 0.0;
    }
    return result;
}


double RInHouseProbabilityDistributions::getChiSquaredCDF(double x, double nu)
{
    double result;
    if ( 0.0 < x ) {
        result = incompleteGammaFunction(0.5*nu,0.5*x);
    }
    else {
        result = 0.0;
    }
    return result;
}


double RInHouseProbabilityDistributions::getInverseChiSquaredCDF(double x, double nu)
{
    return 0.0;
}


// Methods for the t-distribution distribution:
double RInHouseProbabilityDistributions::getTDistributionPDF(double x, double nu)
{
    qCritical() << "The t-distribution is not yet available in the in-house probability distributions";
    return 0;
}


double RInHouseProbabilityDistributions::getTDistributionCDF(double x, double nu)
{
    qCritical() << "The t-distribution is not yet available in the in-house probability distributions";
    return 0;
}


double RInHouseProbabilityDistributions::getInverseTDistributionCDF(double x, double nu)
{
    qCritical() << "The t-distribution is not yet available in the in-house probability distributions";
    return 0;
}


// Methods for the Beta distribution:
double RInHouseProbabilityDistributions::getBetaPDF(double x, double a, double b)
{
    qCritical() << "The beta distribution is not yet available in the in-house probability distributions";
    return 0;

}


double RInHouseProbabilityDistributions::getBetaCDF(double x, double a, double b)
{
    qCritical() << "The beta distribution is not yet available in the in-house probability distributions";
    return 0;
}


double RInHouseProbabilityDistributions::getInverseBetaCDF(double x, double a, double b)
{
    qCritical() << "The beta distribution is not yet available in the in-house probability distributions";
    return 0;
}


// Methods for the Logistic distribution:
double RInHouseProbabilityDistributions::getLogisticPDF(double x, double a)
{
    double result;
    result= exp(-x/a)/a/pow((1+exp(-x/a)),2);
    return result;
}


double RInHouseProbabilityDistributions::getLogisticCDF(double x, double a)
{
    double result;
    result= -1.0/(exp(x/a)+1)+1;
    return result;
}


double RInHouseProbabilityDistributions::getInverseLogisticCDF(double x, double a)
{
    double result;
    result= a*(log(-1.0/(x-1.0)-1.0));
    return result;
}


// Methods for the Weibull distribution:
double RInHouseProbabilityDistributions::getWeibullPDF(double x, double a, double b)
{
    double result;
    if ( 0.0 < x) {
        result = b/a * pow((x/a),b-1) * exp(-pow((x/a),b));
    }
    else {
        result = 0.0;
    }
    return result;
}


double RInHouseProbabilityDistributions::getWeibullCDF(double x, double a, double b)
{
    double result;
    if ( 0.0 < x) {
        result = 1.0 - exp(-pow(x/a,b));
    }
    else {
        result = 0.0;
    }
    return result;
}


double RInHouseProbabilityDistributions::getInverseWeibullCDF(double x, double a, double b)
{
    return a * pow(-log(1-x),1/b);
}


// Methods for the Gumbel distribution:
double RInHouseProbabilityDistributions::getGumbelPDF(double x, double a, double b)
{
    return a * b * exp(-a * (x) - b * exp(-a*(x)));
}


double RInHouseProbabilityDistributions::getGumbelCDF(double x, double a, double b)
{
    return exp(-b * exp(-a * x));
}


double RInHouseProbabilityDistributions::getInverseGumbelCDF(double x, double a, double b)
{

    return -1.0/a*log(-1.0/b*log(x));
}
