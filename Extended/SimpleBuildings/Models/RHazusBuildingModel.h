#ifndef RHazusBuildingModel_H
#define RHazusBuildingModel_H
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
#include "RProbabilityDistributions.h"

class RParameter;
class RResponse;

class RHazusBuildingModel : public RModel
{
	Q_OBJECT

	// Define a QProperty for each input (parameter) data member. The type of the QProperty is "QObject *"
	Q_PROPERTY(RBuildingType BuildingType READ getBuildingType WRITE setBuildingType)
	Q_PROPERTY(ROccupancyClass OccupancyClass READ getOccupancyClass WRITE setOccupancyClass)
	Q_PROPERTY(RCodeDesignLevel CodeDesignLevel READ getCodeDesignLevel WRITE setCodeDesignLevel)
	Q_PROPERTY(QObject *Sa1 READ getSa1 WRITE setSa1)
	Q_PROPERTY(QObject *Sa2 READ getSa2 WRITE setSa2)
	Q_PROPERTY(QObject *Magnitude READ getMagnitude WRITE setMagnitude)
	Q_PROPERTY(double NumberOfStoreys READ getNumberOfStoreys WRITE setNumberOfStoreys)
	Q_PROPERTY(double FootprintArea READ getFootprintArea WRITE setFootprintArea)
    Q_PROPERTY(QObject *ProbabilityDistributions READ getProbabilityDistributions WRITE setProbabilityDistributions)


	Q_ENUMS(RBuildingType)
	Q_ENUMS(ROccupancyClass)
	Q_ENUMS(RCodeDesignLevel)

public:
	// Define the constructor and destructor
	RHazusBuildingModel(QObject *parent, QString name);
	~RHazusBuildingModel();

	enum RBuildingType{W1, W2, S1L, S1M, S1H, S2L, S2M, S2H, S3, S4L, S4M, S4H, S5L, S5M, S5H, C1L, C1M, C1H, C2L, C2M, C2H, C3L, C3M, C3H, PC1, PC2L, PC2M, PC2H, RM1L, RM1M, RM2L, RM2M, RM2H, URML, URMM, MH};
	enum ROccupancyClass{RES1, RES2, RES3A, RES3B, RES3C, RES3D, RES3E, RES3F, RES4, RES5, RES6, COM1, COM2, COM3, COM4, COM5, COM6, COM7, COM8, COM9, COM10, IND1, IND2, IND3, IND4, IND5, IND6, AGR1, REL1, GOV1, GOV2, EDU1, EDU2};
	enum RCodeDesignLevel{High, Moderate, Low, Pre};

	// Define the mandatory read/write methods for all QProperties
	QObject *getMagnitude() const;
	void setMagnitude(QObject *value);
	QObject *getSa1() const;
	void setSa1(QObject *value);
	QObject *getSa2() const;
	void setSa2(QObject *value);
	RBuildingType getBuildingType() const;
	void setBuildingType(RBuildingType value);
	ROccupancyClass getOccupancyClass() const;
	void setOccupancyClass(ROccupancyClass value);
	RCodeDesignLevel getCodeDesignLevel() const;
	void setCodeDesignLevel(RCodeDesignLevel value);
	double getFootprintArea() const;
	void setFootprintArea(double value);
	double getNumberOfStoreys() const;
	void setNumberOfStoreys(double value);
    QObject *getProbabilityDistributions() const;
    void setProbabilityDistributions(QObject *value);

	// Define the method where the actual model is coded
	int evaluateModel(RGradientType theGradientType);

private:
	// Declare input data members
	QPointer<RParameter> theSa1;
	QPointer<RParameter> theSa2;
	QPointer<RParameter> theMagnitude;
	RBuildingType theBuildingType;
	ROccupancyClass theOccupancyClass;
	RCodeDesignLevel theCodeDesignLevel;
	double theFootprintArea;
	double theNumberOfStoreys;
    QPointer<RProbabilityDistributions> theProbabilityDistributions;

	
	// Declare responses
	RResponse *theDamageFactor;

};

#endif // RHazusBuildingModel_H
