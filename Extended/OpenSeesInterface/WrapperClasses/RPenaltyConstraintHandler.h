#ifndef RPenaltyConstraintHandler_H
#define RPenaltyConstraintHandler_H
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

// Wrapper class for OpenSees PenaltyConstraintHandler
// Stevan Gavrilovic 12/2018

#include "RConstraintHandler.h"
#include "PenaltyConstraintHandler.h"

class RPenaltyConstraintHandler : public RConstraintHandler
{
    Q_OBJECT

    Q_PROPERTY(double SPConstraint READ getAlphaSP  WRITE setAlphaSP)
    Q_PROPERTY(double MPConstraint READ getAlphaMP  WRITE setAlphaMP)

public:
    RPenaltyConstraintHandler(QObject *parent, QString name);
    ~RPenaltyConstraintHandler();

    ConstraintHandler* getTheConstraintHandler();

    double getAlphaSP() const;
    void setAlphaSP(double value);

    double getAlphaMP() const;
    void setAlphaMP(double value);

protected:

    std::unique_ptr<PenaltyConstraintHandler> theConstraintHandler;

    double alphaSP;
    double alphaMP;

};

#endif
