#ifndef RShellMITC4_H
#define RShellMITC4_H
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

// Wrapper class for OpenSees ShellMITC4
// Stevan Gavrilovic 12/2018

#include "RLinearShellElement.h"
#include "ShellMITC4.h"

class RNode;
class RParameter;
class ShellMITC4;

class RShellMITC4 : public RLinearShellElement
{
    Q_OBJECT

public:
    RShellMITC4(QObject *parent, QString name);
    ~RShellMITC4();

    ShellMITC4* getTheOpenSeesElement(void);

    int update(void);

    void print(void);

    int getGlobalResponse(QVector<double> *response, QString type);

    int getLocalResponse(QVector<double> *response, QString type);

    int generateOpenSeesInput(std::string& object, int flag);

private:

    // Raw ptr; OpenSees controls the destruction, otherwise program crashes on exit
    ShellMITC4* theOpenSeesElement;

    int createShellMITC4Element(void);
    int initialize(void);
    int getGlobalDisplacementResponse(QVector<double> *response);
    int getGlobalForceResponse(QVector<double> *response);
    int getLocalForceResponse(QVector<double> *response);
    int getLocalStressResponse(QVector<double> *response);
    int getLocalStrainResponse(QVector<double> *response);
};

#endif
