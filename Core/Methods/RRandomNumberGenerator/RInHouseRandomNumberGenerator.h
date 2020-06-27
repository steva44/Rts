#ifndef RInHouseRandomNumberGenerator_H
#define RInHouseRandomNumberGenerator_H
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

#include "RRandomNumberGenerator.h"

class RProbabilityTransformation;

class RInHouseRandomNumberGenerator : public RRandomNumberGenerator
{
	Q_OBJECT
    Q_PROPERTY(int Seed READ getSeed WRITE setSeed)

public:
	RInHouseRandomNumberGenerator(QObject *parent, QString name);
	~RInHouseRandomNumberGenerator();

    int getSeed();
    void setSeed(int value);

    int generateStandardNormalNumbers(QVector<double> *u);
    void reset();

private:

    int theSeed;
    QVector<double> *theGeneratedNumbers;

};

#endif
