#ifndef RFixedSourceModel_H	
#define RFixedSourceModel_H	
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
*                                                                    *
*********************************************************************/
	
#include "RModel.h"	
	
class RParameter;
class RLocation;
class RLocationResponse;
	
class RFixedSourceModel : public RModel	
{	
	Q_OBJECT

    Q_PROPERTY(QObject *Location READ getLocation WRITE setLocation)
    Q_PROPERTY(QObject *Depth READ getDepth WRITE setDepth)

public:	
	RFixedSourceModel(QObject *parent, QString name);
	~RFixedSourceModel();

	int evaluateModel(RGradientType theGradientType);

    QObject *getLocation() const;
    void setLocation(QObject *value);

    QObject *getDepth() const;
    void setDepth(QObject *value);

	QStringList getActionList();

private slots:

	void displayonMap();
	void removefromMap();

private:	

    QPointer<RParameter> theLocation;
    RLocationResponse *theHypocentreLocation;
    QPointer<RParameter> theDepth;
};

#endif	
