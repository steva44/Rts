#ifndef RNodalLoad_H
#define RNodalLoad_H
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

#include "RLoadBaseClass.h"
#include "RPoint.h"

class NodalLoad;
class Node;
class Load;
class RLoadPattern;

class RNodalLoad : public RLoadBaseClass
{
    Q_OBJECT
    Q_PROPERTY(QObject* InputParameter READ getParameter WRITE setParameter) 
    
public:
    RNodalLoad(QObject *parent, QString name);
    ~RNodalLoad();
    
    Load* getOpenSeesLoad();
    
    int update();
    
    RLoadPattern* getTheLoadPattern();
    void setTheLoadPattern(RLoadPattern* value);

    std::vector<double> getLoadDirectionVector(void);
    void setLoadDirectionVector(std::vector<double> value);

    void setNode(Node* value);
    Node* getNode(void);

    // Get the type of parameter. This could be a constant, random variable, decision variable etc.
    void setParameter(QObject *parameter);
    
    inline void setPoint(RPoint* point){thePointList.append(point);}
    inline QList<RPoint*> getPointList(){return thePointList;}
    
    double getFOGradient();
    double getSOGradient();
    
private:

    std::vector<double> loadDirectionVector;
    
    Node* theNode;
    QPointer<RLoadPattern> theLoadPattern;    
    
    int createNodalLoad(void);
    
    NodalLoad* theOpenSeesLoad;
    
    //  The same load can be applied to multiple points
    QList<RPoint*> thePointList;
};

#endif
