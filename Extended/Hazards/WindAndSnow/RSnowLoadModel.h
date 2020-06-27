#ifndef RSnowLoadModel_H
#define RSnowLoadModel_H
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

#include "RLoadPatternModel.h"

class RSnowLoadModel : public RLoadPatternModel
{
	Q_OBJECT

public:
    RSnowLoadModel(QObject *parent, QString name);
    virtual ~RSnowLoadModel() override;

    virtual QList<QPointer<RLoadPattern> > getTheLoadPatterns()  override;

protected:

RLoadPattern* theLoadPattern;

int initialize(void);
	
};

#endif // RSnowLoadModel_H
