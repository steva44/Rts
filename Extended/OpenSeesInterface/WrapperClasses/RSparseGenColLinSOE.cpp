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

#include "RSparseGenColLinSOE.h"
#include "SparseGenColLinSOE.h"
#include "SparseGenColLinSolver.h"
#include "SuperLU.h"
//#include "ThreadedSuperLU.h"
//#include "DistributedSuperLU.h"
#include "LinearSOE.h"


RSparseGenColLinSOE::RSparseGenColLinSOE(QObject *parent, QString name)
    : RLinearSOE(parent, name)
{
    theSparseGenColLinSOE = nullptr;
    theSparseGenColLinSolver = nullptr;
}


RSparseGenColLinSOE::~RSparseGenColLinSOE()
{

}


LinearSOE* RSparseGenColLinSOE::getTheLinearSOE()
{
    if(!theSparseGenColLinSOE)
    {
#ifdef _THREADS
        //theSparseGenColLinSolver = new ThreadedSuperLU();
        theSparseGenColLinSolver = new SuperLU();

        //        theSparseGenColLinSolver = new DistributedSuperLU();
        //TODO: when osx supports thread.h or some library/other fix is implemented
#else
        theSparseGenColLinSolver = new SuperLU();
#endif
        theSparseGenColLinSOE = std::make_unique<SparseGenColLinSOE>(*theSparseGenColLinSolver);
    }
    
    return theSparseGenColLinSOE.get();
}



