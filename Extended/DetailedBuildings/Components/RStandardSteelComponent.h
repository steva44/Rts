#ifndef RStandardSteelComponent_H
#define RStandardSteelComponent_H
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

//   Point1                           Point2
//           1       2       3      ..n
//       .-------.-------.-------.---->
//

// This is a wrapper class for any standard steel component that can be found in the AISC database
// It will parse the type of section it is; then it will create that type with information from the database
// This has to be done this way since W sections are different than C, than L, etc. so it will create the
// appropriate subclass and import cross-section information as needed

#include "RSteelSectionImporter.h"
#include "RComponent.h"
#include "RPoint.h"

class RSteelSectionImporter;

class RStandardSteelComponent : public RComponent
{
    Q_OBJECT

    Q_PROPERTY(QString CrossSectionType WRITE setCrossSectionType)
    Q_PROPERTY(QObject *Point1 WRITE setPoint1)
    Q_PROPERTY(QObject *Point2 WRITE setPoint2)
    Q_PROPERTY(QString Orientation WRITE setOrientation)

public:
    RStandardSteelComponent(QObject *parent, QString &name);
    ~RStandardSteelComponent();

    // Set the cross-section type
    void setCrossSectionType(const QString& value){ theCrossSectionType = value;
                                                    createComponent();}

    // Set the end points
    inline void setPoint1(QObject *value){ thePoint1 = qobject_cast<RPoint *>(value);
                                           createComponent();}

    inline void setPoint2(QObject *value){ thePoint2 = qobject_cast<RPoint *>(value);
                                           createComponent();}


    // Set the orientation of the component along the centroidal axis
    inline void setOrientation(QString value){ setStringParameter(value, &theOrientationDouble, &theOrientationParameter);
                                               createComponent();}

    // Create the standard steel component
    int createComponent();


private:

    double inchToMeterConversion = 0.0254;

    RPoint *thePoint1 = nullptr;
    RPoint *thePoint2 = nullptr;

    QString theCrossSectionType;

    QPointer<RParameter> theOrientationParameter;
    double theOrientationDouble;

    unique_ptr<RSteelSectionImporter> theSectionImporter = nullptr;

    inline QString getOrientation(){return getStringParameter(&theOrientationDouble, theOrientationParameter);}

};

#endif
