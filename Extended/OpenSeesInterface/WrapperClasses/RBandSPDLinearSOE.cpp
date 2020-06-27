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

#include "RBandSPDLinearSOE.h"
#include "BandSPDLinSOE.h"
#include "BandSPDLinLapackSolver.h"
#include "BandSPDLinSolver.h"
#include "BandSPDLinThreadSolver.h"
#include "LinearSOE.h"

RBandSPDLinearSOE::RBandSPDLinearSOE(QObject *parent, QString name)
    : RLinearSOE(parent, name)
{
    theBandSPDLinearSOE = nullptr;
    theBandSPDLinSolver = nullptr;
}


RBandSPDLinearSOE::~RBandSPDLinearSOE()
{
}


LinearSOE* RBandSPDLinearSOE::getTheLinearSOE()
{
    if(!theBandSPDLinearSOE)
    {
#ifdef _THREADS
        //theBandSPDLinSolver = new BandSPDLinThreadSolver();
        theBandSPDLinSolver = new BandSPDLinLapackSolver();     //TODO: when osx supports thread.h or some library/other fix is implemented
#else
        theBandSPDLinSolver = new BandSPDLinLapackSolver();
#endif
        theBandSPDLinearSOE = std::make_unique<BandSPDLinSOE>(*theBandSPDLinSolver);
    }
    
    return theBandSPDLinearSOE.get();
}





