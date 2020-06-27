#ifndef RMesh_H
#define RMesh_H
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

// This class stores and manages the finite element mesh of a component

#include "RMethod.h"
#include "RDomain.h"
#include "RSmartPointer.h"

class RElement;

#ifdef OSG
#include <RTopoMesh.h>
#endif

class RMesh : public RMethod
{
    Q_OBJECT

public:
    RMesh(RDomain *theDomain, QString name);
    ~RMesh();

    virtual int getNumberOfElements();

    virtual int createMesh(void) = 0;

    virtual void reset(void);

    virtual int commitState(void);

    // Get the current drift value of the mesh
    virtual double getDrift();

    // Gets the maximum drift value of the mesh (set in a time history structural analysis)
    double getMaxDrift() const;

    // Data members for visualization - topo mesh is for visualizing damage
#ifdef OSG
    inline void setTopoMesh(shared_ptr<RTopoMesh>& topoMesh){theTopoMesh = topoMesh;}

    virtual int createTopologicalMesh(void);

    shared_ptr<RTopoMesh> getTopologicalMesh();
#endif

    // Sets and returns the finite elements for this mesh
    QList<RElement *> *getTheElementList() const;

    void setTheElementList(QList<RElement *> *value);

protected:
    QList<RElement*>* theElementList;

#ifdef OSG
    shared_ptr<RTopoMesh> theTopoMesh;
#endif

    QString theComponentName;

    double maxDrift;
};

#endif
