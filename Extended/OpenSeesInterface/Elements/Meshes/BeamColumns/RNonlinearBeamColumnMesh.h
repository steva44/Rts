#ifndef RNonlinearBeamColumnMesh_H
#define RNonlinearBeamColumnMesh_H
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

#include "RBeamColumnMesh.h"
#include "RMaterialKnowledge.h"
#include "RSectionKnowledge.h"
#include "RParameter.h"
#include "RPoint.h"

class RElement;

class RNonlinearBeamColumnMesh : public RBeamColumnMesh
{

    Q_OBJECT

public:
    RNonlinearBeamColumnMesh(RDomain *domain,
                             QString parentName,
                             QString elementType,
                             QList<RElement*>& elementList,
                             QList<RParameter*>& theParameterList,
                             int numEl,
                             RSectionKnowledge *crossSection,
                             RPoint *point1,
                             RPoint *point2,
                             RParameter* orientation);

    ~RNonlinearBeamColumnMesh();

    int createMesh(void);
    double getMaxStress();
    double getAxialLoad(int elementNumber);

    QString getTheIntegrationType() const;
    void setTheIntegrationType(const QString &value);

    RParameter *getTheRhoParameter() const;

    void setTheRhoParameter(RParameter *value);

    QString getCrdTransformationType() const;
    void setCrdTransformationType(const QString &value);

    QString getIntegrationType() const;
    void setIntegrationType(const QString &value);

    RSectionKnowledge *getTheCrossSection() const;
    void setTheCrossSection(RSectionKnowledge *value);

private:

    QString crdTransformationType;
    QString integrationType;

    RSectionKnowledge *theCrossSection;
    QString theParentName;
    RParameter* theOrientationParameter;
    RParameter* theRhoParameter;
    int numElements;
    QString theElementType;

};

#endif
