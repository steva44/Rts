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

#include "REuclidsAlgorithm.h"
#include "RDomain.h"

REuclidsAlgorithm::REuclidsAlgorithm(RDomain *theDomain)
    : RMethod(theDomain, nullptr)
{

}


REuclidsAlgorithm::~REuclidsAlgorithm()
{

}


int REuclidsAlgorithm::findGreatestCommonDivisor(int a, int b)
{
    // First make sure a is the largest of the two numbers
    if (a<b) {
        double c = a;
        a = b;
        b = c;
    }

    // Then loop until convergence
    bool convergence = false;
    while (!convergence) {

        // Subtract as many multiples of b from a
        // as possible and check the remainder
        double remainder = a%b;
        if (remainder == 0.0) {
            convergence = true;
            return b;
        }
        else {

            // Let b be the new big number, let the remainder be
            // the new small number, and try again
            a = b;
            b = remainder;
        }
    }

    return 0;
}


