#ifndef RParametricPoint_H
#define RParametricPoint_H
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

// Class for a point whose coordinates depend on some parameter

#include "RParameter.h"
#include "muParser.h"

class RParametricPoint : public RParameter
{
    Q_OBJECT

    Q_PROPERTY(QString Expression READ getExpression WRITE setExpression)

public:
    // Define the constructor and destructor
    RParametricPoint(QObject *parent, QString name);
    ~RParametricPoint();

    QString getExpression() const;
    void setExpression(QString value);

    double evaluateExpression(RGradientType theGradientType);

    inline QMap<RParameter*,double> getFOFDMParameterMap(){return theFOFDMMap;}
    inline QMap<RParameter*,double> getSOFDMParameterMap(){return theSOFDMMap;}
    inline QList<RParameter *> getExplicitParameterList(){return theExplicitParameterList;}

private:

    int parseExpression();
    void reset();

    QString theExpression;

    std::unique_ptr<mu::Parser> theParser = nullptr;
    QList<RParameter *> theExplicitParameterList;

    QMap<RParameter*,double> theFOFDMMap;

    QMap<RParameter*,double> theSOFDMMap;

};

#endif
