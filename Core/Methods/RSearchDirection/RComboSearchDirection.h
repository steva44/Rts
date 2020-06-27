#ifndef RComboSearchDirection_H
#define RComboSearchDirection_H
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

class RComboSearchDirection : public RSearchDirection
{
	Q_OBJECT
    Q_PROPERTY(QObject *SearchDirection1 READ getSearchDirection1 WRITE setSearchDirection1)
    Q_PROPERTY(QObject *SearchDirection2 READ getSearchDirection2 WRITE setSearchDirection2)
    Q_PROPERTY(int SwitchStep READ getSwitchStep WRITE setSwitchStep)

public:
	RComboSearchDirection(QObject *parent, QString name);
	~RComboSearchDirection();

    QObject *getSearchDirection1() const;
    void setSearchDirection1(QObject *value);

    QObject *getSearchDirection2() const;
    void setSearchDirection2(QObject *value);

    int getSwitchStep() const;
    void setSwitchStep(int value);

    int getSearchDirection(QVector<double> const *point, double function, QVector<double> const *gradient, QVector<double> *result);

private:

    QPointer<RSearchDirection> theSearchDirection1;
    QPointer<RSearchDirection> theSearchDirection2;
    int stepNumber;
    int theSwitchStep;
};

#endif
