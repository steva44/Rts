#ifndef RSlaveNode_H
#define RSlaveNode_H
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

#include "RNode.h"
#include "RPoint.h"

class RSlaveNode : public RNode
{
    Q_OBJECT

    Q_PROPERTY(RPoint* MasterPoint WRITE setMasterPoint)

public:
    RSlaveNode(QObject *parent, QString &name);
    ~RSlaveNode();

    inline void setMasterNode(RNode* node)
    {
        masterNode = node;
        masterNode->theSlaveNode=this;
    }

    inline RNode* getMasterNode(){return masterNode;}

    inline void setMasterPoint(RPoint* point)
    {
        masterNode = point->getAssociatedNode();
        masterNode->theSlaveNode=this;
    }

    void intializeNode(RNode* theNode);

private:

    RNode* masterNode = nullptr;

};

#endif

