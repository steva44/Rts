#ifndef RInfoRichResponse_H
#define RInfoRichResponse_H
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

#include "RResponse.h"

class RInfoRichResponse : public RResponse
{
	Q_OBJECT

public:
    RInfoRichResponse(QObject *parent, QString name);
    ~RInfoRichResponse() override;

    void setCurrentValue(QString name, double value) override;

    double  getCurrentValue(QString name) const override;

    void clearResponses(void);

    bool isEmpty(void);

    int numResponses(void);

private:

    QMap<QString, double> theResponseMap;
	
};

#endif // RInfoRichResponse_H
