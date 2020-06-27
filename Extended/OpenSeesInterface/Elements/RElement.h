#ifndef RElement_H
#define RElement_H
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
*   - Terje Haukaas                                                  *
*                                                                    *
*********************************************************************/

// The base class for all finite elements in Rts

#include "RNode.h"
#include "RMaterialKnowledge.h"

#include <functional>

class Element;
class RDomain;
class RTimeHistoryRecorder;
class RDamageRegion;

class RElement : public RObject
{
    Q_OBJECT

public:
    RElement(QObject *parent, QString &name);
    virtual ~RElement();

    // Virtual methods implemented by each element type
    virtual int getNumNodes() = 0;
    virtual RNode* getNode(int i) const = 0;
    virtual int setNode(int i, RNode *theNode) = 0;

    // Updates the element, i.e., the paramter values, or destroys and creates a new one where applicable
    virtual int update(void) = 0;

    // OpenSees functionality - where you can print the properties of an element
    virtual void print(void);

    // Gets the OpenSees element
    virtual Element* getTheOpenSeesElement(void);

    // Gets the nodes of an element
    std::vector<RNode*> getNodes(void);

    // A recorder that stores the element responses for each time step in a transiet time history analysis
    RTimeHistoryRecorder* getTheTransientRecorder() const;

    // Stores the damage region where the element resides
    RDamageRegion *getTheDamageRegion() const;
    void setTheDamageRegion(RDamageRegion *value);

protected:
    // Stores the damage region where the element resides
    RDamageRegion* theDamageRegion;

    // A recorder that stores the element responses for each time step in a transiet time history analysis
    std::unique_ptr<RTimeHistoryRecorder> theTransientRecorder;

};

#endif
