#ifndef RQuasiNewtonSearchDirection_H
#define RQuasiNewtonSearchDirection_H
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

#include "RSearchDirection.h"

class RQuasiNewtonSearchDirection : public RSearchDirection
{
    Q_OBJECT
    Q_PROPERTY(QString Type READ getType WRITE setType)

public:
    RQuasiNewtonSearchDirection(QObject *parent, QString name);
    ~RQuasiNewtonSearchDirection();

    QString getType();
    void setType(QString value);

    int getSearchDirection(QVector<double> const *point, double function, QVector<double> const *gradient, QVector<double> *result);

private:

    QString theType;
    QVector<double> *oldPoint;
    QVector<double> *oldGradient;
    QVector< QVector<double> > *HinvOld;
};

#endif
