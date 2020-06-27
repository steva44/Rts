#ifndef RLinearShellElement_H
#define RLinearShellElement_H
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

// Base class for a linear shell element
//
//     y
//    ^
//    |__>x
//
//     4_____________3
//     |             |
//     |             |
//     |             |
//     |             |
//     |             |
//    1|_____________|2
//
//     22
//    ^
//    |__>11

#include "RElement.h"

class RNode;
class RParameter;
class RSectionForceDeformation;

class RLinearShellElement : public RElement
{
    Q_OBJECT
    
public:
    RLinearShellElement(QObject *parent, QString &name);
    ~RLinearShellElement();

    int getNumNodes();
    RNode* getNode(int i) const;
    int setNode(int i, RNode *theNode);

    virtual int getGlobalResponse(QVector<double> *response, QString type) = 0;
    virtual int getLocalResponse(QVector<double> *response, QString type) = 0;

    RSectionForceDeformation *getTheSectionForceDeformation() const;
    void setTheSectionForceDeformation(RSectionForceDeformation *value);

    // Area of the shell element in m2
    double getArea(void);
    std::vector<double> getCentroid(void);
    std::vector<double> getNormalVector(void);

    // Length of each side in m
    double getLength12();
    double getLength23();
    double getLength34();
    double getLength41();

    // Returns the average length of 1-2 and 3-4 edges in m
    double getLength11();

    // Returns the average length of 2-3 and 4-1 edges in m
    double getLength22();

    // Smallest length along any axis in m
    double getCharacteristicLength();

protected:

    RNode *theNode1;
    RNode *theNode2;
    RNode *theNode3;
    RNode *theNode4;

    RSectionForceDeformation* theSectionForceDeformation;

};



#endif
