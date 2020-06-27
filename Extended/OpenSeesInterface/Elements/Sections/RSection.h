#ifndef RSection_H
#define RSection_H
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

#include "RObject.h"
#include "R2DPoint.h"

class RNode;
class RFibreDamageRegion;

class RSection : public RObject
{
    Q_OBJECT

public:
    RSection(QObject *parent, QString &name);
    ~RSection();

    std::vector<R2DPoint> getSectionVertexLoop() const;

    void setSectionVertexLoop(const std::vector<R2DPoint> &value);

    std::shared_ptr<RFibreDamageRegion>& getSegmentDamageRegion();

    void setTheFibreContainer(const std::shared_ptr<RFibreDamageRegion> &value);

protected:

    // For visual damage
    std::shared_ptr<RFibreDamageRegion> theFiberContainer;

private:

    std::vector<R2DPoint> sectionVertexLoop;

};

#endif

