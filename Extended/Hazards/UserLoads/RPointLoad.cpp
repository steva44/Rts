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

#include "RPointLoad.h"
#include "RPoint.h"
#include "RDomain.h"

RPointLoad::RPointLoad(QObject *parent, QString name)
    : RLoadBaseClass(parent, name)
{

}


RPointLoad::~RPointLoad()
{

}


double RPointLoad::getFOGradient()
{
  return 1.0;
}


double RPointLoad::getSOGradient()
{
   return 0.0;
}
	

void RPointLoad::setParameter(QObject *value)
{	
    RParameter *theParam = qobject_cast<RParameter *>(value);

    if(!theParam)
    {
        qCritical()<<"The Object that came into the pointload is not a parameter!";
    }
    else
    {
        theLoadValueParameter = theParam;
    }
}	
	

