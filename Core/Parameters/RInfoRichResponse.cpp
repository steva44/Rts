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

#include "RInfoRichResponse.h"

RInfoRichResponse::RInfoRichResponse(QObject *parent, QString name)
    : RResponse(parent, name)
{

}


RInfoRichResponse::~RInfoRichResponse()
{

}


void RInfoRichResponse::clearResponses(void)
{
    theResponseMap.clear();
}


bool RInfoRichResponse::isEmpty(void)
{
    return theResponseMap.empty();
}


void RInfoRichResponse::setCurrentValue(QString name, double value)
{
    theResponseMap.insert(name, value);
}


double RInfoRichResponse::getCurrentValue(QString name) const
{
    // Return 0.0 if the response can not be found
    return theResponseMap.value(name, 0.0);
}


int RInfoRichResponse::numResponses(void)
{
    return theResponseMap.size();
}
