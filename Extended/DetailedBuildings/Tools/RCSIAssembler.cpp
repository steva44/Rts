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

#include "RCSIAssembler.h"
#include "RContinuousRandomVariable.h"
#include "RRectangularRCColumnComponent.h"
#include "RRCShearWallComponent.h"
#include "RWSteelBeamColumnComponent.h"
#include "RRectangularGlulamBeamColumnComponent.h"
#include "RRCSlabComponent.h"
#include "RCLTPanelComponent.h"
#include "RCompositePanelComponent.h"
#include "RCrew.h"
#include "RJob.h"
#include "RBIM.h"

#include <tuple>

RCSIAssembler::RCSIAssembler(QObject *parent, QString name)
    : RMethod(parent, name)
{
    // Conversion factors to convert from metric to imperial and between volumes
    convertMeterToInch = 39.3701;
    convertMeterSqToFtSq = 10.7639;
    convertCubicMToCubicYd = 1.30795;
    convertCubicMToCubicFt = 35.3147;
    convertMeterToFoot = 3.28084;
    convertBfToCubicFt = 0.0833333;
    convertCubicFttoYrd = 0.037037;
    convertInchtoFt = 0.0833333;
    convertYrdToCubicFt = 27.0;
}


RCSIAssembler::~RCSIAssembler()
{

}


int RCSIAssembler::getRectRCColLabourConstructionCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                                       RJob* jobPhase,
                                                       CSIVector* labourQuantityCSICodes)
{

    // Four stages
    //      (1) Forming: Form material, form labor
    //      (2) Reinforcing: reinforcing steel material, reinforcing steel labor
    //      (3) Placing: labour, concrete
    //      (4) Form Stripping/finishing/curing: labour

    /***** (1) Formwork****/

    this->getRectRCColFormingCodes(theRectConcreteColumn,jobPhase,labourQuantityCSICodes);

    /***** (2) Reinforcement****/

    this->getRectRCColReinforcingCodes(theRectConcreteColumn,jobPhase,labourQuantityCSICodes);

    /***** (3) Placement****/

    this->getRectRCColPlacingLabourCodes(theRectConcreteColumn,jobPhase,labourQuantityCSICodes);

    /***** (4) Finishing & Curing ****/

    // No trowel finish required for columns since most area is covered by forms

    // Sq. ft. of plywood required - the form contact area
    auto formContactArea = theRectConcreteColumn->getSurfaceArea()*convertMeterSqToFtSq; //Need to convert m2 into sqft.

    // Curing of finished concrete
    this->getConcreteCuringCSICodes(formContactArea,theRectConcreteColumn,jobPhase,labourQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getRectRCColReinforcingCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                                RJob* jobPhase,
                                                CSIVector* CSICodes)
{
    // 1) Material Quantities

    // Get steel volume and convert into "ton"
    auto steelVolume = theRectConcreteColumn->getTotalVolumeOfReinforcement();

    // Convert into US ton
    auto tonReinforcementSteel = steelVolume*8050.0/907.185;

    // 2) Installation time in labour-hours - Figure 3-39 Installation Time in Labor-Hours for Concrete Flat Slabs with Drop Panels (RSMeans Costing Div. 3.,p.g. 86)

    // Installation crew
    RCrew* crew4Rodm = nullptr;

    if(jobPhase)
    {
        crew4Rodm = jobPhase->addCrew(RCrewCode::Rodm4);

        if(crew4Rodm == nullptr)
        {
            qCritical()<<"Error could not get a crew in "<<__FUNCTION__;
            return -1;
        }
    }

    // Get the diameter of longitudinal reinforcement from the component
    auto rebarDiameter = theRectConcreteColumn->getLongitudinalReinforcementDiameter();

    QString transverseRebarType = theRectConcreteColumn->getTypeTransverseReinforcement();

    auto tonsReinfNo3toNo7 = 0.0;
    auto tonsReinfNo8toNo18 = 0.0;

    // If spiral rebar is being used
    if(transverseRebarType == "spirals")
    {
        // CSI CODE ### for the labour for column reinforcement install - 8" - 15" spiral columns
        {
            // Line Number:	032111600300
            // Description:	Reinforcing steel, in place, columns, spirals, 8" to 15" diameter, A615, grade 60, incl labor for accessories, excl material for accessories
            // Unit: ton
            //Crew: 4Rodm
            QString CSINumber = "032111600300";
            QString Description = "Reinforcing steel, in place, columns, spirals, 8 in. to 15 in. diameter, A615, grade 60, incl labor for accessories, excl material for accessories";

            CSICodes->push_back(std::make_tuple(CSINumber,tonReinforcementSteel,Description));

            if(jobPhase != nullptr)
            {
                crew4Rodm->addTask(CSINumber,tonReinforcementSteel,Description,theRectConcreteColumn);
            }
        }

        tonsReinfNo3toNo7 += tonReinforcementSteel;
    }
    // Longitudinal w/ transverse hoop reinforcement
    else
    {
        if(rebarDiameter <= 20) //Equivalent to rebar that is between #3 & #7
        {
            // CSI CODE ### for the labour for column reinforcement install
            {
                // Line Number:	032111600200
                // Description:	Reinforcing steel, in place, columns, #3 to #7, A615, grade 60, incl labor for accessories, excl material for accessories
                // Unit: Ton
                QString CSINumber = "032111600200";
                QString Description = "Reinforcing steel, in place, columns, #3 to #7, A615, grade 60, incl labor for accessories, excl material for accessories";

                CSICodes->push_back(std::make_tuple(CSINumber,tonReinforcementSteel,Description));

                if(jobPhase != nullptr)
                {
                    crew4Rodm->addTask(CSINumber,tonReinforcementSteel,Description,theRectConcreteColumn);
                }
            }

            // Add this components contribution to the total reinforcement amount
            tonsReinfNo3toNo7 += tonReinforcementSteel;
        }
        // Rebar diameter is larger than 20M
        else
        {
            // CSI CODE ### for the labour for column reinforcement install
            {
                // Line Number:	032111600250
                // Description:	Reinforcing steel, in place, columns, #8 to #18, A615, grade 60, incl labor for accessories, excl material for accessories
                // Unit: Ton
                QString CSINumber = "032111600250";
                QString Description = "Reinforcing steel, in place, columns, #8 to #18, A615, grade 60, incl labor for accessories, excl material for accessories";

                CSICodes->push_back(std::make_tuple(CSINumber,tonReinforcementSteel,Description));

                if(jobPhase != nullptr)
                {
                    crew4Rodm->addTask(CSINumber,tonReinforcementSteel,Description,theRectConcreteColumn);
                }
            }

            // Add this components contribution to the total reinforcement amount
            tonsReinfNo8toNo18 += tonReinforcementSteel;
        }
    }


    if(jobPhase)
    {
        jobPhase->getTonsReinfNo3toNo7() += tonsReinfNo3toNo7;
        jobPhase->getTonsReinfNo8toNo18() += tonsReinfNo8toNo18;
    }

    return 0;
}


int RCSIAssembler::getRectRCColFormingCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                            RJob* jobPhase,
                                            CSIVector* CSICodes,
                                            double contactArea)
{

    // Get the column dimension; this is used later to calculate the cost
    auto maxColumnDimension = theRectConcreteColumn->getCrossSectionWidth()->getCurrentValue();

    auto columnDepth = theRectConcreteColumn->getCrossSectionDepth()->getCurrentValue();

    if(columnDepth > maxColumnDimension)
        maxColumnDimension = columnDepth;

    // Convert from meters into inches
    auto columnDimension = maxColumnDimension*convertMeterToInch;

    // Assume wooden forms

    // Four stages:
    //      (1) Forming: Form material, form labor
    //      (2) Reinforcing: reinforcing steel material, reinforcing steel labor
    //      (3) Placing: labour, concrete
    //      (4) Form Stripping/finishing/curing: labour

    // Each stage has:
    //      i) Material Quantities
    //      ii) Installation time in labour-hours

    /***** (1) Formwork****/

    // i) Material Quantities - assume that rectangular concrete columns are formed with wood

    // Sq. ft. of plywood required - the form contact area
    auto formContactArea = 0.0;

    if(contactArea != 0.0)
        formContactArea = contactArea;
    else
        formContactArea = theRectConcreteColumn->getSurfaceArea()*convertMeterSqToFtSq; //Need to convert m2 into sqft.

    // ii) Installation of formwork

    // CSI CODE  for installing plywood column forms, bracing, etc.
    // The labour cost depends on the size of the column and number of the times the forms are used
    // In this case assume that the forms are only used once

    RCrew* crewC1 = nullptr;

    if(jobPhase)
        crewC1 = jobPhase->addCrew(RCrewCode::C1);

    // The labour/equipment requirements for crew C1 are:
    // 3 Carpenters
    // 1 Laborer

    if(columnDimension <= 8.0)
    {
        // Line Number:	031113255000
        // Description:	C.I.P. concrete forms, column, square, plywood, 8" x 8", 1 use, includes erecting, bracing, stripping and cleaning
        // Unit: S.F.C.A.
        //Crew C1
        QString CSINumber = "031113255000";
        QString Description = "C.I.P. concrete forms, column, square, plywood, 8x8 in., 1 use, includes erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,formContactArea,Description));

        if(crewC1)
        {
            crewC1->addTask(CSINumber,formContactArea,Description,theRectConcreteColumn);
        }
    }
    else if(columnDimension <= 12.0)
    {
        // Line Number:	031113255500
        // Description:	C.I.P. concrete forms, column, square, plywood, 12" x 12", 1 use, includes erecting, bracing, stripping and cleaning
        // Unit: S.F.C.A.
        //Crew C1
        QString CSINumber = "031113255500";
        QString Description = "C.I.P. concrete forms, column, square, plywood, 12x12 in., 1 use, includes erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,formContactArea,Description));

        if(crewC1)
        {
            crewC1->addTask(CSINumber,formContactArea,Description,theRectConcreteColumn);
        }
    }
    else if(columnDimension <= 16.0)
    {
        // Line Number:	031113256000
        // Description:	C.I.P. concrete forms, column, square, plywood, 16" x 16", 1 use, includes erecting, bracing, stripping and cleaning
        // Unit: S.F.C.A.
        //Crew C1
        QString CSINumber = "031113256000";
        QString Description = "C.I.P. concrete forms, column, square, plywood, 16x16 in., 1 use, includes erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,formContactArea,Description));

        if(crewC1)
        {
            crewC1->addTask(CSINumber,formContactArea,Description,theRectConcreteColumn);
        }
    }
    else if(columnDimension <= 24.0)
    {
        // Line Number:	031113256500
        // Description:	C.I.P. concrete forms, column, square, plywood, 24" x 24", 1 use, includes erecting, bracing, stripping and cleaning
        // Unit: S.F.C.A.
        //Crew C1
        QString CSINumber = "031113256500";
        QString Description = "C.I.P. concrete forms, column, square, plywood, 24x24 in., 1 use, includes erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,formContactArea,Description));

        if(crewC1)
        {
            crewC1->addTask(CSINumber,formContactArea,Description,theRectConcreteColumn);
        }
    }
    else if (columnDimension <= 36.0)
    {
        // Line Number:	031113257000
        // Description:	C.I.P. concrete forms, column, square, plywood, 36" x 36", 1 use, includes erecting, bracing, stripping and cleaning
        // Unit: S.F.C.A.
        //Crew C1
        QString CSINumber = "031113257000";
        QString Description = "C.I.P. concrete forms, column, square, plywood, 36x36 in., 1 use, includes erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,formContactArea,Description));

        if(crewC1)
        {
            crewC1->addTask(CSINumber,formContactArea,Description,theRectConcreteColumn);
        }
    }
    else
    {
        qDebug()<<"Column dimension is over 36 in., costs are not available for columns this large";
    }


    // To calculate the board feet of lumber needed for the studs and bracing - Figure 3-2 Ratio of Board Feet to Square Feet of Net Contact Area (RSMeans Costing Div. 3.,p.g. 48)
    // the board feet of lumber required; for each square foot of form contact area
    auto boardFeetPerSqFtForColumn = 3.0;

    auto totalBoardFeetForms = boardFeetPerSqFtForColumn*formContactArea/10.0; //Has to be in unit "thousand board feet" of M.B.F.

    auto YrdWood = 0.0;

    // Get volume of wood in for studs and bracking yards
    auto yrdBdft = totalBoardFeetForms*convertBfToCubicFt*convertCubicFttoYrd;
    YrdWood += yrdBdft;

    // Calculate the wood used for forming
    // Assume 3/4" forms
    auto formThickness = 0.75;

    // Get the volume of formwork
    auto yrdForms = this->getVolYrdWoodForms(formThickness,formContactArea);
    YrdWood += yrdForms;

    if(jobPhase)
        jobPhase->getYrdWood() += YrdWood;

    return 0;
}


int RCSIAssembler::getRectRCColPlacingLabourCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                                  RJob* jobPhase,
                                                  CSIVector* labourQuantityCSICodes)
{
    // Get the storey number
    auto componentStoreyNumber = theRectConcreteColumn->getBuildingStoreyNumber();

    // Get the column dimension; this is used later to calculate the cost
    auto maxColumnDimension = theRectConcreteColumn->getCrossSectionWidth()->getCurrentValue();

    auto columnDepth = theRectConcreteColumn->getCrossSectionDepth()->getCurrentValue();

    if(columnDepth > maxColumnDimension)
        maxColumnDimension = columnDepth;

    // Convert from meters into inches
    auto columnDimension = maxColumnDimension*convertMeterToInch;

    // Method for delivering concrete Figure 3-12  RSMeans Costing Div. 3. Preferred and Acceptable Methods for Transporting Concrete to the Job Site and at the Job Site
    // 1) Pump / Pump Truck
    // 2) Crane and Bucket

    bool isPumped = false;

    // Use pump or pump truck if storey is less than 3, else crane is required
    if(componentStoreyNumber<3)
    {
        isPumped = true;
    }


    // i) Material Quantities

    // Volume of concrete in yards
    auto volumeOfConcrete = theRectConcreteColumn->getVolumeOfConcrete()*convertCubicMToCubicYd;

    // Placing rates - Figure 3-13 Data on Handling and Transporting Concrete (RSMeans Costing Div. 3.,p.g. 58)
    if(isPumped)
    {
        // Crew C20 if the concrete is pumped
        RCrew* crewC20 = jobPhase->addCrew(RCrewCode::C20);

        // Labour and Equipment for crew C20
        // 1 Labor Foreman (outside)
        // 5 laborers
        // 1 Cement Finisher
        // 1 Equip. Oper. (medium)
        // 2 Gas Engine Vibrators
        // 1 Concrete Pump (Small)

        if(columnDimension <= 12)
        {
            // Line Number:	033113700400
            // Description:	Structural concrete, placing, column, square or round, pumped, 12" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.
            QString CSINumber = "033113700400";
            QString Description = "Structural concrete, placing, column, square or round, pumped, 12 in. thick, includes leveling (strike off) & consolidation, excludes material";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC20->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
        }
        else if(columnDimension <= 18)
        {
            // Line Number: 033113700600
            // Description:	Structural concrete, placing, column, square or round, pumped, 18" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.
            QString CSINumber = "033113700600";
            QString Description = "Structural concrete, placing, column, square or round, pumped, 18 in. thick, includes leveling (strike off) & consolidation, excludes material";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC20->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
        }
        else if(columnDimension <= 24)
        {
            // Line Number:	033113700800
            // Description:	Structural concrete, placing, column, square or round, pumped, 24" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.
            QString CSINumber = "033113700800";
            QString Description = "Structural concrete, placing, column, square or round, pumped, 24 in. thick, includes leveling (strike off) & consolidation, excludes material";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC20->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
        }
        else if (columnDimension <= 36)
        {
            // Line Number:	033113701000
            // Description:	Structural concrete, placing, column, square or round, pumped, 36" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.
            QString CSINumber = "033113701000";
            QString Description = "Structural concrete, placing, column, square or round, pumped, 36 in. thick, includes leveling (strike off) & consolidation, excludes material";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC20->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
        }
        else
        {
            qDebug()<<"Column dimension is over 36 in., costs are not available for columns this large";
        }

    }
    else
    {
        // Using a crane and bucket - Labour and Equipment for crew C7
        RCrew* crewC7 = jobPhase->addCrew(RCrewCode::C7);
        // 1 Labor Foreman (outside)
        // 5 Laborers
        // 1 Cement Finisher
        // 1 Equip. Oper. (medium)
        // 1 Equip. Oper. (oiler)
        // 2 Gas Engine Vibrators
        // 1 Concrete Bucket, 1 C.Y.
        // 1 Hyd. Crane, 55 Ton

        //CSI CODE  for the crane/bucket
        if(columnDimension <= 12)
        {
            // Line Number: 033113700450
            // Description:	Structural concrete, placing, column, square or round, with crane and bucket, 12" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.
            QString CSINumber = "033113700450";
            QString Description = "Structural concrete, placing, column, square or round, with crane and bucket, 12 in. thick, includes leveling (strike off) & consolidation, excludes material";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC7->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
        }
        else if(columnDimension <= 18)
        {
            // Line Number: 033113700650
            // Description:	Structural concrete, placing, column, square or round, with crane and bucket, 18" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.
            QString CSINumber = "033113700650";
            QString Description = "Structural concrete, placing, column, square or round, with crane and bucket, 18 in. thick, includes leveling (strike off) & consolidation, excludes material";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC7->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
        }
        else if(columnDimension <= 24)
        {
            // Line Number: 033113700850
            // Description:	Structural concrete, placing, column, square or round, with crane and bucket, 24" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.
            QString CSINumber = "033113700850";
            QString Description = "Structural concrete, placing, column, square or round, with crane and bucket, 24 in. thick, includes leveling (strike off) & consolidation, excludes material";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC7->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
        }
        else if (columnDimension <= 36)
        {
            // Line Number: 033113701050
            // Description:	Structural concrete, placing, column, square or round, with crane and bucket, 36" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.
            QString CSINumber = "033113701050";
            QString Description = "Structural concrete, placing, column, square or round, with crane and bucket, 36 in. thick, includes leveling (strike off) & consolidation, excludes material";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC7->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
        }
        else
        {
            qDebug()<<"Column dimension is over 36 in., costs are not available for columns this large";
        }
    }

    return 0;
}


int RCSIAssembler::getRCSlabLabourConstructionCodes(RRCSlabComponent* theConcreteSlab,
                                                    RJob* jobPhase,
                                                    RBIM* theBIM,
                                                    CSIVector* labourQuantityCSICodes)
{

    // For a slab assume plywood pan forms on stringers with shoring or scaffolding

    // Four stages
    //      (1) Forming: Form material, form labor
    //      (2) Reinforcing: reinforcing steel material, reinforcing steel labor
    //      (3) Placing: labour, concrete
    //      (4) Form Stripping/finishing/curing: labour

    // Each stage has:
    //      i) Material Quantities
    //      ii) Installation time in labour-hours

    // Sq. ft. of pan forms
    auto panContactArea = theConcreteSlab->getPlanArea()*convertMeterSqToFtSq;

    /***** (1) Formwork****/

    this->getRCSlabFormingCodes(theConcreteSlab,jobPhase,theBIM,labourQuantityCSICodes);

    /***** (2) Reinforcement****/

    this->getRCSlabReinforcingCodes(theConcreteSlab,jobPhase,labourQuantityCSICodes);

    /***** (3) Placement****/

    this->getRCSlabPlacingLabourCodes(theConcreteSlab,jobPhase,theBIM,labourQuantityCSICodes);

    /***** (4) Finishing & Curing ****/

    // ii) Installation time in labour-hours

    RCrew* crewC10C = jobPhase->addCrew(RCrewCode::C10C);
    // Labour and equipment for crew C10C
    // 1 Laborer
    // 2 Cement Finishers
    // 1 Walk-Behind Trowel

    // Finishing - Floor trowel
    {
        // Line Number:	033513300250
        // Description:	Concrete finishing, specified Random Access Floors ACI Classes 1, 2, 3 & 4, for Composite Overall Floor Flatness & Levelness to FF35/FL25, bull float, machine float & steel trowel (walk-behind), excl placing, striking off & consolidating
        // Unit: S.F.

        QString CSINumber = "033513300250";
        QString Description = "Concrete finishing, specified Random Access Floors ACI Classes 1, 2, 3 & 4, for Composite Overall Floor Flatness & Levelness to FF35/FL25, bull float, machine float & steel trowel (walk-behind), excl placing, striking off & consolidating";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,panContactArea,Description));

        crewC10C->addTask(CSINumber,panContactArea,Description,theConcreteSlab);
    }

    // Curing
    this->getConcreteCuringCSICodes(panContactArea,theConcreteSlab,jobPhase,labourQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getRCSlabFormingCodes(RRCSlabComponent* theConcreteSlab,
                                         RJob* jobPhase,
                                         RBIM* theBIM,
                                         CSIVector* CSICodes)
{
    // For a slab assume plywood pan forms on stringers with shoring or scaffolding

    /***** (1) Formwork****/

    // 1. Shores and stringers are erected
    // 2. Steel Soffit forms or plywood forms are placed on stringers, shores are adjusted to support the forms
    // 3. Wood headers and blockouts are placed for electrical outlets etc.
    // 4. End forms are placed and nailed to soffit
    // 5. Pans oiled to facilitate removal
    // 6. All reinforcing is placed
    // 7. Concrete is placed
    // 8. Shores are removed after concrete gains proper strength
    // 9. Forms are removed and reconditioned for next use

    // Get the storey number
    auto componentStoreyNumber = theConcreteSlab->getBuildingStoreyNumber();

    // The ceiling height in feet (i.e., the height of the storey from floor to ceiling, NOT storey elevation)
    auto storeyHeight = theBIM->getBuildingStoreyHeight(componentStoreyNumber)*convertMeterToFoot;

    // The slab thickness in inches
    auto slabThicknessInches = theConcreteSlab->getThickness()->getCurrentValue()*convertMeterToInch;

    // Sq. ft. of pan forms required
    auto panContactArea = theConcreteSlab->getPlanArea()*convertMeterSqToFtSq;

    // Sq. ft. of edge forms required
    auto EdgeFormArea = theConcreteSlab->getPerimeter()*theConcreteSlab->getThickness()->getCurrentValue()*convertMeterSqToFtSq;

    // ii) Installation time in labour-hours

    // CSI codes for form labour installing flat plate forms including shoring, erecting, bracing, stripping and cleaning

    RCrew* crewC2 = nullptr;

    if(jobPhase)
        crewC2 = jobPhase->addCrew(RCrewCode::C2);

    // The labour/equipment requirements for crew C2 are:
    // 1 Carpenter Foreman (outside)
    // 4 Carpenters
    // 1 Laborer

    if(storeyHeight <= 15.0)
    {
        // Line Number:	031113351000
        // Description:	C.I.P. concrete forms, elevated slab, flat plate, plywood, to 15' high, 1 use, includes shoring, erecting, bracing, stripping and cleaning
        // Unit: S.F.

        QString CSINumber = "031113351000";
        QString Description = "C.I.P. concrete forms, elevated slab, flat plate, plywood, to 15' high, 1 use, includes shoring, erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,panContactArea,Description));

        if(crewC2)
        {
            crewC2->addTask(CSINumber,panContactArea,Description,theConcreteSlab);
        }
    }
    else if (storeyHeight <= 20.0)
    {
        // Line Number:	031113351500
        // Description:	C.I.P. concrete forms, elevated slab, flat plate, plywood, to 20' high, 1 use, includes shoring, erecting, bracing, stripping and cleaning
        // Unit: S.F.

        QString CSINumber = "031113351500";
        QString Description = "C.I.P. concrete forms, elevated slab, flat plate, plywood, 15' to 20' high ceilings, 4 use, includes shoring, erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,panContactArea,Description));

        if(crewC2)
        {
            crewC2->addTask(CSINumber,panContactArea,Description,theConcreteSlab);
        }
    }
    else if (storeyHeight <= 35.0)
    {
        // Line Number:	031113351600
        // Description:	C.I.P. concrete forms, elevated slab, flat plate, plywood, 21' to 35' high ceilings, 4 use, includes shoring, erecting, bracing, stripping and cleaning
        // Unit: S.F.

        QString CSINumber = "031113351600";
        QString Description = "C.I.P. concrete forms, elevated slab, flat plate, plywood, 21' to 35' high ceilings, 4 use, includes shoring, erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,panContactArea,Description));

        if(crewC2)
        {
            crewC2->addTask(CSINumber,panContactArea,Description,theConcreteSlab);
        }
    }
    else
    {
        qDebug()<<"There is no cost information available for formwork install, for storey height "<<storeyHeight;
    }

    auto edgeFormLength = theConcreteSlab->getPerimeter()*3.28084;

    // For installing the edging on the forms, per square foot contact area
    RCrew* crewC1 = nullptr;

    if(jobPhase)
        crewC1 = jobPhase->addCrew(RCrewCode::C1);


    // The labour/equipment requirements for crew C1 are:
    // 3 Carpenters
    // 1 Laborer

    // CSI codes for edge form labour
    if( slabThicknessInches <= 6.0)
    {
        // Line Number:	031113357000
        // Description:	C.I.P. concrete forms, elevated slab, edge forms, to 6" high, 4 use, includes shoring, erecting, bracing, stripping and cleaning
        // Unit: L.F.

        QString CSINumber = "031113357000";
        QString Description = "C.I.P. concrete forms, elevated slab, edge forms, to 6 in. high, 4 use, includes shoring, erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,edgeFormLength,Description));

        if(crewC1)
            crewC1->addTask(CSINumber,edgeFormLength,Description,theConcreteSlab);

    }
    else if (slabThicknessInches <= 12.0)
    {
        // Line Number: 031113357070
        // Description: C.I.P. concrete forms, elevated slab, edge forms, 7" to 12" high, 1 use, includes shoring, erecting, bracing, stripping and cleaning
        // Unit: S.F.C.A

        QString CSINumber = "031113357070";
        QString Description = "C.I.P. concrete forms, elevated slab, edge forms, 7 in. to 12 in. high, 1 use, includes shoring, erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,EdgeFormArea,Description));

        if(crewC1)
            crewC1->addTask(CSINumber,edgeFormLength,Description,theConcreteSlab);
    }


    // Box out for openings, per square foot contact area - Figure3-3 Formwork labor-Hours (RSMeans Costing Div. 3.,p.g. 49)
    // To do: Get the size and number of any openings in the slab, e.g., for stairs, mechanical, ventilation, etc.

    // Assume 20 box outs for now
    auto numBoxOuts = 20;

    // CSI codes for the labour installing box outs
    {
        // Line Number: 031113355500
        // Description: C.I.P. concrete forms, elevated slab, box-out for shallow slab openings, to 10 S.F., includes shoring, erecting, bracing, stripping and cleaning
        // Unit: Ea.

        QString CSINumber = "031113355500";
        QString Description = "C.I.P. concrete forms, elevated slab, box-out for shallow slab openings, to 10 S.F., includes shoring, erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,numBoxOuts,Description));
    }

    // Calculate the board feet of lumber needed for the studs and bracing - Figure 3-2 Ratio of Board Feet to Square Feet of Net Contact Area (RSMeans Costing Div. 3.,p.g. 48)
    // The board feet of lumber required per square foot of form contact area
    auto boardFeetForMetalPanSlab = 1.5;

    auto BoardFeetForms = boardFeetForMetalPanSlab*panContactArea;

    auto boardFeetSlabShoring = 1.8;

    auto BoardFeetShoring = boardFeetSlabShoring*panContactArea;

    auto totalBoardFeetForms = (BoardFeetForms+BoardFeetShoring)/10.0; // has to be in units of "thousand board feet"

    auto YrdWood = 0.0;

    // Volume of wood in yards for bracing
    auto yrdBrdFt = totalBoardFeetForms*convertBfToCubicFt*convertCubicFttoYrd;
    YrdWood += yrdBrdFt;

    // Wood used for forming, assume 3/4" forms
    auto formThickness = 0.75;

    auto yrdFormWood = this->getVolYrdWoodForms(formThickness,panContactArea);
    YrdWood += yrdFormWood;

    if(jobPhase)
        jobPhase->getYrdWood() += YrdWood;

    return 0;
}


int RCSIAssembler::getRCSlabReinforcingCodes(RRCSlabComponent* theConcreteSlab,
                                             RJob* jobPhase,
                                             CSIVector* CSICodes)
{
    // i) Material Quantities

    // Volume of steel reinforcement
    auto steelVolume = theConcreteSlab->getVolumeOfReinforcement();

    // Convert into US ton
    auto tonReinforcementSteel = steelVolume*8050.0/907.185;

    // Reinforcement installation crew
    RCrew* crew4Rodm = nullptr;

    if(jobPhase)
        crew4Rodm = jobPhase->addCrew(RCrewCode::Rodm4);

    auto rebarDiameter = theConcreteSlab->getLongitudinalReinforcementDiameter();

    // CSI code for the labour for slab reinforcement install
    if(rebarDiameter <= 20.0) //Equivalent to rebar that is between #3 & #7
    {
        // Line Number:	032111600400
        // Description:	Reinforcing steel, in place, elevated slabs, #4 to #7, A615, grade 60, incl labor for accessories, excl material for accessories
        // Unit: Ton
        QString CSINumber = "032111600400";
        QString Description = "Reinforcing steel, in place, elevated slabs, #4 to #7, A615, grade 60, incl labor for accessories, excl material for accessories";

        CSICodes->push_back(std::make_tuple(CSINumber,tonReinforcementSteel,Description));

        if(crew4Rodm)
            crew4Rodm->addTask(CSINumber,tonReinforcementSteel,Description,theConcreteSlab);

        // Add the mass of reinf. to the job
        if(jobPhase)
            jobPhase->getTonsReinfNo3toNo7() += tonReinforcementSteel;
    }
    else
    {
        qDebug()<<"No cost information available for a rebar size greater than #7 (20M) for elevated flat slabs";
    }

    return 0;
}


int RCSIAssembler::getRCSlabPlacingLabourCodes(RRCSlabComponent* theConcreteSlab,
                                               RJob* jobPhase,
                                               RBIM* theBIM,
                                               CSIVector* labourQuantityCSICodes)
{
    // Method for delivering concrete Figure 3-12  RSMeans Costing Div. 3. Preferred and Acceptable Methods for Transporting Concrete to the Job Site and at the Job Site
    //      1) Pump / Pump Truck
    //      2) Crane and Bucket

    // Slab thickness in inches
    auto slabThicknessInches = theConcreteSlab->getThickness()->getCurrentValue()*convertMeterToInch;

    // Number of building stories
    auto numBuildingStoreys = theBIM->getNumberOfStoreys();

    bool isPumped = false;

    // Use pump or pump truck if the number of building storeys is less than 4, else crane is required
    if(numBuildingStoreys < 4)
    {
        isPumped = true;

    }

    //Volume of concrete in yards
    auto volumeOfConcrete = theConcreteSlab->getVolumeOfConcrete()*convertCubicMToCubicYd;

    // Placing rates - Figure 3-13 Data on Handling and Transporting Concrete (RSMeans Costing Div. 3.,p.g. 58)
    if(isPumped)
    {
        // Crew C20 if the concrete is pumped
        RCrew* crewC20 = jobPhase->addCrew(RCrewCode::C20);

        //CSI CODE for the concrete pump
        // Labour and equipment for crew C20
        // 1 Labor Foreman (outside)
        // 5 Laborers
        // 1 Cement Finisher
        // 1 Equip. Oper. (medium)
        // 2 Gas Engine Vibrators
        // 1 Concrete Pump (Small)

        if( slabThicknessInches <= 6.0)
        {
            // Line Number: 033113701400
            // Description:	Elevated slabs, less than 6" thick, pumped
            // Unit: C.Y.

            QString CSINumber = "033113701400";
            QString Description = "Elevated slabs, less than 6 in. thick, pumped";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC20->addTask(CSINumber,volumeOfConcrete,Description,theConcreteSlab);
        }
        else if (slabThicknessInches <= 10.0)
        {
            // Line Number:	033113701500
            // Description:	Elevated slabs, 6" to 10" thick, pumped
            // Unit: C.Y.

            QString CSINumber = "033113701500";
            QString Description = "Elevated slabs, 6 in. to 10 in. thick, pumped";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC20->addTask(CSINumber,volumeOfConcrete,Description,theConcreteSlab);
        }
        else
        {
            // Line Number:	033113701600
            // Description:	Elevated slabs, over 10" thick, pumped
            // Unit: C.Y.

            QString CSINumber = "033113701600";
            QString Description = "Elevated slabs, over 10 in. thick, pumped";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC20->addTask(CSINumber,volumeOfConcrete,Description,theConcreteSlab);
        }
    }
    else
    {
        // Crane and Bucket - 1 operator + 2 labourers - cubic yards per hour

        // Using a crane and bucket - Labour and Equipment for crew C7
        RCrew* crewC7 = jobPhase->addCrew(RCrewCode::C7);
        // 1 Labor Foreman (outside)
        // 5 Laborers
        // 1 Cement Finisher
        // 1 Equip. Oper. (medium)
        // 1 Equip. Oper. (oiler)
        // 2 Gas Engine Vibrators
        // 1 Concrete Bucket, 1 C.Y.
        // 1 Hyd. Crane, 55 Ton


        if( slabThicknessInches <= 6.0)
        {
            // Line Number: 033113701450
            // Description:	Elevated slabs, less than 6" thick, with crane and bucket
            // Unit: C.Y.

            QString CSINumber = "033113701450";
            QString Description = "Elevated slabs, less than 6 in. thick, with crane and bucket";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC7->addTask(CSINumber,volumeOfConcrete,Description,theConcreteSlab);
        }
        else if (slabThicknessInches <= 10.0)
        {
            // Line Number: 033113701550
            // Description:	Elevated slabs, 6" to 10" thick, with crane and bucket
            // Unit: C.Y.

            QString CSINumber = "033113701550";
            QString Description = "Elevated slabs, 6 in. to 10 in. thick, with crane and bucket";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC7->addTask(CSINumber,volumeOfConcrete,Description,theConcreteSlab);
        }
        else
        {
            // Line Number:	033113701650
            // Description:	Elevated slabs, over 10" thick, with crane and bucket
            // Unit: C.Y.

            QString CSINumber = "033113701650";
            QString Description = "Elevated slabs, over 10 in. thick, with crane and bucket";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            crewC7->addTask(CSINumber,volumeOfConcrete,Description,theConcreteSlab);
        }

    }

    return 0;
}


int RCSIAssembler::getRCShearWallLabourConstructionCodes(RRCShearWallComponent* theConcreteShearwall,
                                                         RJob* jobPhase,
                                                         RBIM* theBIM,
                                                         CSIVector* labourQuantityCSICodes)
{
    // For a shear wall assume plywood forms
    // Four stages:
    //      (1) Forming: Form material, form labor
    //      (2) Reinforcing: reinforcing steel material, reinforcing steel labor
    //      (3) Placing: labour, concrete
    //      (4) Form Stripping/finishing/curing: labour

    // Each stage has:
    //      i) Material Quantities
    //      ii) Installation time in labour-hours

    /***** (1) Formwork****/

    this->getRCShearWallFormingCodes(theConcreteShearwall,jobPhase,theBIM,labourQuantityCSICodes);

    /***** (2) Reinforcement****/

    this->getRCShearWallReinforcingCodes(theConcreteShearwall,jobPhase,labourQuantityCSICodes);

    /***** (3) Placement****/

    this->getRCShearWallPlacingCodes(theConcreteShearwall,jobPhase,theBIM,labourQuantityCSICodes);

    /***** (4) Stripping & Finishing  ****/

    // Sq. ft. of forms required
    auto contactArea = theConcreteShearwall->getSurfaceArea()*convertMeterSqToFtSq;

    RCrew* crewCefi = jobPhase->addCrew(RCrewCode::Cefi1);

    // CSI code for finishing, break form ties and patch voids; burlap rub with grout
    // 1 Cefi
    {
        // Line Number: 033529600050
        // Description: Concrete finishing, walls, burlap rub with grout, includes breaking ties and patching voids
        // Unit: S.F.

        QString CSINumber = "033529600050";
        QString Description = "Concrete finishing, walls, burlap rub with grout, includes breaking ties and patching voids";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

        crewCefi->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
    }

    // Curing of concrete
    this->getConcreteCuringCSICodes(contactArea,theConcreteShearwall,jobPhase,labourQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getRCShearWallFormingCodes(RRCShearWallComponent* theConcreteShearwall,
                                              RJob* jobPhase,
                                              RBIM* theBIM,
                                              CSIVector* CSICodes)
{
    //  Forms types for a wall:
    //  1) Job-Built Plywood
    //  2) Modular Prefabricated Plywood
    //  3) Steel Framed Plywood

    // Number of building storeys
    auto numBuildingStoreys = theBIM->getNumberOfStoreys();

    // Component storey number
    auto componentStoreyNumber = theConcreteShearwall->getBuildingStoreyNumber();

    // To do: implement methods to get form type, assume form type of "0" which is Job-Built Plywood
    int formType = 0;

    // The wall height in feet
    auto theWallHeight = theConcreteShearwall->getHeightValue()*convertMeterToFoot;

    // i) Material Quantities

    // Sq. ft. of forms required
    auto contactArea = theConcreteShearwall->getSurfaceArea()*convertMeterSqToFtSq;

    // ii) Installation time in labour-hours

    // Figure 3-35 Installation Time in tabor-Hours for Foundation Walls (RSMeans Costing Div. 3.,p.g. 82)
    // Assume forms are used only once if buildings is 2 or less storeys in height
    bool useAgain = false;

    // Assume forms will be reused if the buildings is 3 or more storeys in height
    if(numBuildingStoreys>2)
        useAgain = true;

    RCrew* crewC2 = nullptr;

    if(jobPhase)
        crewC2 = jobPhase->addCrew(RCrewCode::C2);

    // For all types of forms below the equipment and labor requirements for crew C2
    // 1 Carpenter Foreman (outside)
    // 4 Carpenters
    // 1 Laborer

    // 1) Job-Built Plywood
    if(formType == 0)
    {

        // CSI code Labour Install Job-Built Plywood
        if(theWallHeight > 0.0 && theWallHeight <= 8.0)
        {
            if(useAgain)
            {
                // Line Number:	031113852150
                // Description:	C.I.P. concrete forms, wall, job built, plywood, to 8' high, 4 use, includes erecting, bracing, stripping and cleaning
                // Unit: S.F.C.A.
                QString CSINumber = "031113852150";
                QString Description = "C.I.P. concrete forms, wall, job built, plywood, to 8' high, 4 use, includes erecting, bracing, stripping and cleaning";

                CSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

                if(crewC2)
                {
                    crewC2->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
                }
            }
            else
            {

                // Line Number:	031113852000
                // Description:	C.I.P. concrete forms, wall, job built, plywood, to 8' high, 1 use, includes erecting, bracing, stripping and cleaning
                // Unit: S.F.C.A.
                QString CSINumber = "031113852000";
                QString Description = "C.I.P. concrete forms, wall, job built, plywood, to 8' high, 1 use, includes erecting, bracing, stripping and cleaning";

                CSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

                if(crewC2)
                {
                    crewC2->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
                }
            }

        }
        else if( theWallHeight > 8.0 && theWallHeight <= 16.0)
        {
            if(useAgain)
            {
                // Line Number:	031113852550
                // Description:	C.I.P. concrete forms, wall, job built, plywood, over 8' to 16' high, 4 use, includes erecting, bracing, stripping and cleaning
                // Unit: S.F.C.A.
                QString CSINumber = "031113852550";
                QString Description = "C.I.P. concrete forms, wall, job built, plywood, over 8' to 16' high, 4 use, includes erecting, bracing, stripping and cleaning";

                CSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

                if(crewC2)
                {
                    crewC2->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
                }
            }
            else
            {
                // Line Number:	031113852400
                // Description:	C.I.P. concrete forms, wall, job built, plywood, over 8' to 16' high, 1 use, includes erecting, bracing, stripping and cleaning
                // Unit: S.F.C.A.
                QString CSINumber = "031113852400";
                QString Description = "C.I.P. concrete forms, wall, job built, plywood, over 8' to 16' high, 1 use, includes erecting, bracing, stripping and cleaning";

                CSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

                if(crewC2)
                {
                    crewC2->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
                }
            }
        }
        else if(theWallHeight > 16.0)
        {
            if(useAgain)
            {
                // Line Number: 031113852850
                // Description:	C.I.P. concrete forms, wall, job built, plywood, over 16' high, 4 use, includes erecting, bracing, stripping and cleaning
                // Unit: S.F.C.A.
                QString CSINumber = "031113852850";
                QString Description = "C.I.P. concrete forms, wall, job built, plywood, over 16' high, 4 use, includes erecting, bracing, stripping and cleaning";

                CSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

                if(crewC2)
                {
                    crewC2->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
                }
            }
            else
            {
                // Line Number: 031113852700
                // Description:	C.I.P. concrete forms, wall, job built, plywood, over 16' high, 1 use, includes erecting, bracing, stripping and cleaning
                // Unit: S.F.C.A.
                QString CSINumber = "031113852700";
                QString Description = "C.I.P. concrete forms, wall, job built, plywood, over 16' high, 1 use, includes erecting, bracing, stripping and cleaning";

                CSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

                if(crewC2)
                {
                    crewC2->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
                }
            }
        }
        else
        {
            qDebug()<<"No costing information available for the wall height of "<<theWallHeight<<". Information is only available for walls up to 16.0ft in height."<<__FUNCTION__;
            return -1;
        }

    }
    // 2) Modular Prefabricated Plywood
    else if(formType == 1)
    {

        //CSI CODE Labour Install Modular Prefabricated Plywood
        if(theWallHeight > 0.0 && theWallHeight <= 8)
        {

            // Line Number:	031113857860
            // Description:	C.I.P. concrete forms, walls, modular prefabricated plywood, to 8' high, includes erecting, bracing, stripping and cleaning
            // Unit: S.F.C.A.
            QString CSINumber = "031113857860";
            QString Description = "C.I.P. concrete forms, walls, modular prefabricated plywood, to 8' high, includes erecting, bracing, stripping and cleaning";

            CSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

            if(crewC2)
            {
                crewC2->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
            }
        }
        else if( theWallHeight > 8 && theWallHeight <=16)
        {

            // Line Number:	031113858060
            // Description:	C.I.P. concrete forms, walls, modular prefabricated plywood, over 8' to 16' high, includes erecting, bracing, stripping and cleaning
            // Unit: S.F.C.A.
            QString CSINumber = "031113858060";
            QString Description = "C.I.P. concrete forms, walls, modular prefabricated plywood, over 8' to 16' high, includes erecting, bracing, stripping and cleaning";

            CSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

            if(crewC2)
            {
                crewC2->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
            }
        }
        else
        {
            qDebug()<<"A shear wall height of "<<theWallHeight<<" is not supported for modular plywood forms in RStevesConstructionImpactModel";
        }


    }
    // 3) Steel Framed Plywood
    else if(formType == 2)
    {

        //CSI CODE  Labour Install Steel Framed Plywood
        if(theWallHeight > 0.0 && theWallHeight <= 8)
        {
            // Line Number:	031113859060
            // Description:	C.I.P. concrete forms, walls, steel framed plywood, to 8' high, based on 50 uses of purchased forms, 4 uses of bracing lumber, includes erecting, bracing, stripping and cleaning
            // Unit: S.F.C.A.
            QString CSINumber = "031113859060";
            QString Description = "C.I.P. concrete forms, walls, steel framed plywood, to 8' high, based on 50 uses of purchased forms, 4 uses of bracing lumber, includes erecting, bracing, stripping and cleaning";

            CSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

            if(crewC2)
            {
                crewC2->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
            }
        }
        else if( theWallHeight > 8 && theWallHeight <=16)
        {
            // Line Number:	031113859260
            // Description:	C.I.P. concrete forms, walls, steel framed plywood, over 8' to 16' high, based on 50 uses of purchased forms, 4 uses of bracing lumber, includes erecting, bracing, stripping and cleaning
            // Unit: S.F.C.A.
            QString CSINumber = "031113859260";
            QString Description = "C.I.P. concrete forms, walls, steel framed plywood, over 8' to 16' high, based on 50 uses of purchased forms, 4 uses of bracing lumber, includes erecting, bracing, stripping and cleaning";

            CSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

            if(crewC2)
            {
                crewC2->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
            }
        }
        else if(theWallHeight > 16 && theWallHeight <= 20)
        {
            // Line Number: 031113859460
            // Description:	C.I.P. concrete forms, walls, steel framed plywood, over 16' to 20' high, based on 50 uses of purchased forms, 4 uses of bracing lumber, includes erecting, bracing, stripping and cleaning
            // Unit: S.F.C.A.
            QString CSINumber = "031113859460";
            QString Description = "C.I.P. concrete forms, walls, steel framed plywood, over 16' to 20' high, based on 50 uses of purchased forms, 4 uses of bracing lumber, includes erecting, bracing, stripping and cleaning";

            CSICodes->push_back(std::make_tuple(CSINumber,contactArea,Description));

            if(crewC2)
            {
                crewC2->addTask(CSINumber,contactArea,Description,theConcreteShearwall);
            }
        }
        else
        {
            qDebug()<<"A shear wall height of "<<theWallHeight<<" is not supported for steel framed plywood in RStevesConstructionImpactModel";
        }

    }
    else
    {
        qDebug()<<"The form type is not supported in RStevesConstructionImpactModel";
    }


    // To avoid doubling up on this cost, only add with labor
    if(jobPhase)
    {
        // Add an extra cost for elevated wall construction
        if(componentStoreyNumber >1)
        {
            // Line Number:	031113859475
            // Description: C.I.P. concrete forms, for elevated walls, add, includes erecting, bracing, stripping and cleaning
            // Unit: Ea.
            QString CSINumber = "031113859475";
            QString Description = "C.I.P. concrete forms, for elevated walls, add, includes erecting, bracing, stripping and cleaning";
            CSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));
        }
    }

    // Get the area of the voids for doors and windows, required for boxing out
    auto areaVoidsInWall = theConcreteShearwall->getAreaOfVoids()*10.7639;

    // CSI code for the labour hrs installing box outs
    // Crew C2 same as above

    // To do: Get the voids in the shearwall
    // Assume the following
    auto numVoids = 1;
    auto voidArea = 10.0;       // sq. ft.
    auto voidPerimeter = 100;   // ft.

    if(voidArea <= 10.0)
    {
        // Line Number:	031113850100
        // Description:	C.I.P. concrete forms, wall, box out for opening, to 16" thick, to 10 S.F., includes erecting, bracing, stripping and cleaning
        // Unit: Ea.
        QString CSINumber = "031113850100";
        QString Description = "C.I.P. concrete forms, wall, box out for opening, to 16 in. thick, to 10 S.F., includes erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,numVoids,Description));

        if(crewC2)
        {
            crewC2->addTask(CSINumber,numVoids,Description,theConcreteShearwall);
        }
    }
    else
    {
        // Line Number:	031113850150
        // Description:	C.I.P. concrete forms, wall, box out for opening, to 16" thick, over 10 S.F. (use perimeter), includes erecting, bracing, stripping and cleaning
        // Unit: L.F.
        QString CSINumber = "031113850150";
        QString Description = "C.I.P. concrete forms, wall, box out for opening, to 16 in. thick, over 10 S.F. (use perimeter), includes erecting, bracing, stripping and cleaning";

        CSICodes->push_back(std::make_tuple(CSINumber,voidPerimeter,Description));

        if(crewC2)
        {
            crewC2->addTask(CSINumber,areaVoidsInWall/10.0,Description,theConcreteShearwall);
        }
    }

    // Calculate the board feet of lumber needed for the studs and bracing - Figure 3-2 Ratio of Board Feet to Square Feet of Net Contact Area (RSMeans Costing Div. 3.,p.g. 48)
    // The board feet of lumber required per square foot of form contact area

    // Get the height of the wall
    auto heightOfWall = theConcreteShearwall->getHeightValue()*3.28084;

    auto boardFeetForShearwall = 0.0;

    if(heightOfWall>12.0)
    {
        boardFeetForShearwall=3.5;
    }
    else
    {
        boardFeetForShearwall=2.5;
    }

    auto totalBoardFeetStudsBracing = boardFeetForShearwall*contactArea/10.0;

    auto YrdWood = 0.0;

    // Yards of wood for bracing
    auto yrdBrdFt = totalBoardFeetStudsBracing*convertBfToCubicFt*convertCubicFttoYrd;
    YrdWood += yrdBrdFt;

    // Yards of wood for forming
    //Assume 3/4" forms
    auto formThickness = 0.75;

    auto yrdFormWood =  this->getVolYrdWoodForms(formThickness,contactArea);
    YrdWood += yrdFormWood;

    if(jobPhase)
        jobPhase->getYrdWood() += YrdWood;

    return 0;
}


int RCSIAssembler::getRCShearWallReinforcingCodes(RRCShearWallComponent* theConcreteShearwall,
                                                  RJob* jobPhase,
                                                  CSIVector* CSICodes)
{
    // Steel volume in m3
    auto steelVolume = theConcreteShearwall->getVolumeofReinforcement();

    // Convert into US ton
    auto tonReinforcementSteel = steelVolume*8050.0/907.185;

    // Figure 3-35 Installation Time in labor-Hours for Foundation Walls (RSMeans Costing Div. 3.,p.g. 82)

    // Diameter of reinforcing in m
    auto phiTrans = theConcreteShearwall->getTransverseReinforcementDiameter();
    auto phiLong = theConcreteShearwall->getLongitudinalReinforcementDiameter();

    // Get the SMALLEST bar diameter - conservative, it costs more for the smallest diameter
    auto diameterOfReinforcing = (phiTrans<phiLong) ? phiTrans : phiLong;

    // Crew 4 rodman
    RCrew* crew4Rodm = nullptr;

    if(jobPhase)
        crew4Rodm = jobPhase->addCrew(RCrewCode::Rodm4);

    auto tonsReinfNo3toNo7 = 0.0;
    auto tonsReinfNo8toNo18 = 0.0;

    // CSI code for the labour for wall reinforcement install
    // Reinforcing #3 to #7
    if (diameterOfReinforcing<0.023)
    {
        // Line Number:	032111600700
        // Description:	Reinforcing steel, in place, walls, #3 to #7, A615, grade 60, incl labor for accessories, excl material for accessories
        // Unit: ton
        QString CSINumber = "032111600700";
        QString Description = "Reinforcing steel, in place, walls, #3 to #7, A615, grade 60, incl labor for accessories, excl material for accessories";

        CSICodes->push_back(std::make_tuple(CSINumber,tonReinforcementSteel,Description));

        if(crew4Rodm)
            crew4Rodm->addTask(CSINumber,tonReinforcementSteel,Description,theConcreteShearwall);

        // Add this components contribution to the total reinforcement amount
        tonsReinfNo3toNo7 += tonReinforcementSteel;

    }
    //Reinforcing #8 to #14
    else
    {
        // Line Number:	032111600750
        // Description:	Reinforcing steel, in place, walls, #8 to #18, A615, grade 60, incl labor for accessories, excl material for accessories
        // Unit: ton
        QString CSINumber = "032111600750";
        QString Description = "Reinforcing steel, in place, walls, #8 to #18, A615, grade 60, incl labor for accessories, excl material for accessories";

        CSICodes->push_back(std::make_tuple(CSINumber,tonReinforcementSteel,Description));

        if(crew4Rodm)
            crew4Rodm->addTask(CSINumber,tonReinforcementSteel,Description,theConcreteShearwall);

        // Add this components contribution to the total reinforcement amount
        tonsReinfNo8toNo18 += tonReinforcementSteel;
    }


    if(jobPhase)
    {
        jobPhase->getTonsReinfNo3toNo7() += tonsReinfNo3toNo7;
        jobPhase->getTonsReinfNo8toNo18() += tonsReinfNo8toNo18;
    }

    return 0;
}


int RCSIAssembler::getRCShearWallPlacingCodes(RRCShearWallComponent* theConcreteShearwall,
                                              RJob* jobPhase,
                                              RBIM* theBIM,
                                              CSIVector* CSICodes)
{
    // Method for delivering concrete Figure 3-12  RSMeans Costing Div. 3. Preferred and Acceptable Methods for Transporting Concrete to the Job Site and at the Job Site
    //      1) Pump / Pump Truck
    //      2) Crane and Bucket

    // Wall thickness in inches
    auto theWallThickness = theConcreteShearwall->getThickness()->getCurrentValue()*convertMeterToInch;

    // Number of building storeys to be used to determine if concrete placed by pump or crane
    auto numBuildingStoreys = theBIM->getNumberOfStoreys();

    bool isPumped = false;

    // Use pump or pump truck if the number of building storeys is less than 4, else crane is required
    if(numBuildingStoreys < 4)
    {
        isPumped = true;
    }

    // Material Quantities - transform m^3 into yards
    auto volumeOfConcrete = theConcreteShearwall->getVolumeOfConcrete()*1.30795;

    // Placing rates - Figure 3-13 Data on Handling and Transporting Concrete (RSMeans Costing Div. 3.,p.g. 58)
    if(isPumped)
    {
        //Crew C20 if the concrete is pumped
        RCrew* crewC20 = nullptr;

        if(jobPhase)
            crewC20 = jobPhase->addCrew(RCrewCode::C20);

        // Equipment and labor requirements for Crew C20
        // 1 Labor Foreman (outside)
        // 5 Laborers
        // 1 Cement Finisher
        // 1 Equip. Oper. (medium)
        // 2 Gas Engine Vibrators
        // 1 Concrete Pump (Small)

        // Pumped- 1 operator + 1 labourer - pumpong rate cubic yards per hour

        // ii) Installation time in labour-hours -Figure 3-35 Installation Time in tabor-Hours for Foundation Walls (RSMeans Costing Div. 3.,p.g. 82)

        if(theWallThickness <= 8.0)
        {
            // Line Number: 033113704950
            // Description:	Structural concrete, placing, walls, pumped, 8" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.

            QString CSINumber = "033113704950";
            QString Description = "Structural concrete, placing, walls, pumped, 8 in. thick, includes leveling (strike off) & consolidation, excludes material";

            CSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            if(crewC20)
                crewC20->addTask(CSINumber,volumeOfConcrete,Description,theConcreteShearwall);
        }
        if(theWallThickness <= 12.0)
        {
            // Line Number: 033113705100
            // Description:	Structural concrete, placing, walls, pumped, 12" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.

            QString CSINumber = "033113705100";
            QString Description = "Structural concrete, placing, walls, pumped, 12 in. thick, includes leveling (strike off) & consolidation, excludes material";

            CSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            if(crewC20)
                crewC20->addTask(CSINumber,volumeOfConcrete,Description,theConcreteShearwall);
        }
        if(theWallThickness <= 15.0)
        {
            // Line Number: 033113705350
            // Description: Structural concrete, placing, walls, pumped, 15" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.

            QString CSINumber = "033113705350";
            QString Description = "Structural concrete, placing, walls, pumped, 15 in. thick, includes leveling (strike off) & consolidation, excludes material";

            CSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            if(crewC20)
                crewC20->addTask(CSINumber,volumeOfConcrete,Description,theConcreteShearwall);
        }
        else
        {
            qDebug()<<"No costing data available for shear walls over 15 in. thick in RStevesConstructionImpactModel";
        }

    }
    else
    {
        RCrew* crewC7 = nullptr;

        if(jobPhase)
            crewC7 = jobPhase->addCrew(RCrewCode::C7);

        // Equipment and labor requirements for Crew C7
        // 1 Labor Foreman (outside)
        // 5 Laborers
        // 1 Cement Finisher
        // 1 Equip. Oper. (medium)
        // 1 Equip. Oper. (oiler)
        // 2 Gas Engine Vibrators
        // 1 Concrete Bucket, 1 C.Y. 1 Hyd. Crane, 55 Ton

        if(theWallThickness <= 8.0)
        {
            // Line Number: 033113705000
            // Description:	Structural concrete, placing, walls, with crane and bucket, 8" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.

            QString CSINumber = "033113705000";
            QString Description = "Structural concrete, placing, walls, with crane and bucket, 8 in. thick, includes leveling (strike off) & consolidation, excludes material";

            CSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            if(crewC7)
                crewC7->addTask(CSINumber,volumeOfConcrete,Description,theConcreteShearwall);
        }
        if(theWallThickness <= 12.0)
        {
            // Line Number: 033113705200
            // Description:	Structural concrete, placing, walls, with crane and bucket, 12" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.

            QString CSINumber = "033113705200";
            QString Description = "Structural concrete, placing, walls, with crane and bucket, 12 in. thick, includes leveling (strike off) & consolidation, excludes material";

            CSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            if(crewC7)
                crewC7->addTask(CSINumber,volumeOfConcrete,Description,theConcreteShearwall);
        }
        if(theWallThickness <= 15.0)
        {
            // Line Number: 033113705400
            // Description: Structural concrete, placing, walls, with crane and bucket, 15" thick, includes leveling (strike off) & consolidation, excludes material
            // Unit: C.Y.

            QString CSINumber = "033113705400";
            QString Description = "Structural concrete, placing, walls, with crane and bucket, 15 in. thick, includes leveling (strike off) & consolidation, excludes material";

            CSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

            if(crewC7)
                crewC7->addTask(CSINumber,volumeOfConcrete,Description,theConcreteShearwall);
        }
        else
        {
            qDebug()<<"No costing data available for shear walls over 15 in. thick in RStevesConstructionImpactModel";
        }

    }

    return 0;
}


int RCSIAssembler::getRectRCColMaterialConstructionCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                                         RJob* jobPhase,
                                                         CSIVector* materialQuantityCSICodes)
{
    // Assume wooden forms

    // Four stages:
    //      (1) Forming: Form material, form labor
    //      (2) Reinforcing: reinforcing steel material, reinforcing steel labor
    //      (3) Placing: labour, concrete
    //      (4) Form Stripping/finishing/curing: labour

    /***** (1) Formwork****/

    // Assume that rectangular concrete columns are formed with wood
    this->getRectRCColFormingCodes(theRectConcreteColumn,jobPhase,materialQuantityCSICodes);

    /***** (2) Reinforcement****/

    this->getRectRCColReinforcingCodes(theRectConcreteColumn,jobPhase,materialQuantityCSICodes);

    /***** (3) Placement****/

    // Method for delivering concrete Figure 3-12  RSMeans Costing Div. 3. Preferred and Acceptable Methods for Transporting Concrete to the Job Site and at the Job Site
    //      1) Pump / Pump Truck
    //      2) Crane and Bucket

    // Volume of concrete in yards
    auto volumeOfConcrete = theRectConcreteColumn->getVolumeOfConcrete()*convertCubicMToCubicYd;

    // Get the type of concrete, which also signifies its strength i.e., C20, C25
    auto theConcreteType = theRectConcreteColumn->getConcreteStrength();

    // CSI code for the supply of concrete -- as concrete is not included in the same code as labour
    this->getConcreteCSICode(theConcreteType,volumeOfConcrete,materialQuantityCSICodes);

    // Add to the total volume of concrete of the job
    if(jobPhase)
        jobPhase->getYrdConcrete() += volumeOfConcrete;

    // No trowel finish required for columns since most area is covered by forms

    return 0;
}


int RCSIAssembler::getRectRCColReplacementCodes(RRectangularRCColumnComponent* theRectConcreteColumn,
                                                RJob* jobPhase,
                                                CSIVector* materialQuantityCSICodes,
                                                CSIVector* labourQuantityCSICodes)
{
    // For replacement of section
    //    1. Shoring of structure
    //    2. Demolition of old concrete
    //    3. Removal/Disposal/Cleaning debris
    //    4. Drill in reinforcement dowels and/or apply external reinforcement
    //    5. Concrete Formwork
    //    6. Concrete Placement
    //    7. Finishing/Form removal
    //    8. Curing

    //*************  1.	Shoring of structure  *************

    // Assume eight frames
    auto numberOfFrames = 8.0;

    this->getShoringCSICodes(numberOfFrames,materialQuantityCSICodes);

    //*************  2.&3. Demolition of component removal/disposal/cleaning debris *************

    // The column reinforcement ratio
    auto longReinfRatio = theRectConcreteColumn->getTheLongitudinalReinforcementRatioParameter()->getCurrentValue();
    auto transReinfRatio = theRectConcreteColumn->getTheTransverseReinforcementRatioParameter()->getCurrentValue();

    auto reinfRatio = longReinfRatio > transReinfRatio ? longReinfRatio : transReinfRatio;

    // Demolition volume in cubic yards
    auto demoVol = theRectConcreteColumn->getVolume()*1.30795;

    this->getSelectiveConcreteDemolitionCodes(reinfRatio,demoVol,jobPhase,labourQuantityCSICodes,theRectConcreteColumn);

    // Concrete Disposal in cubic yards
    this->getConcreteRemovalDisposalCodes(demoVol,jobPhase,labourQuantityCSICodes,theRectConcreteColumn);

    //*************  4. Drill in reinforcement dowels and/or apply external reinforcement *************

    // Assume 16 dowels, 4 per end
    auto numDowels = 16.0;

    this->getReinforcementDowelCodes(numDowels,theRectConcreteColumn,jobPhase,labourQuantityCSICodes);

    // Assume that from here on-wards a new column is installed same procedure as construction
    //*************  5.-8.	Concrete Formwork, Concrete Placement, Finishing/Form removal, Curing *************

    // Material
    this->getRectRCColMaterialConstructionCodes(theRectConcreteColumn,jobPhase,materialQuantityCSICodes);

    // Labour (forming, placing, curing)
    this->getRectRCColLabourConstructionCodes(theRectConcreteColumn,jobPhase,labourQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getRectRCColCoverReplacementCodes(double volumeRepair,
                                                     double areaRepair,
                                                     RRectangularRCColumnComponent* theRectConcreteColumn,
                                                     RJob* jobPhase,
                                                     CSIVector* materialQuantityCSICodes,
                                                     CSIVector* labourQuantityCSICodes)
{
    //For replacement of cover
    //    1. Removal of old concrete (150mm min.) with light jack hammer or sufficient depth to reach at least 7mm, plus the dimension of the maximum size aggregate behind any reinforcement
    //    2. Clean with water blasting or sand blasting
    //    3. Drill in reinforcement dowels and/or apply external reinforcement
    //    4. Set up forms and shoring (if vertical or on ceiling)
    //    5. Placement
    //    6. Form removal
    //    7. Curing


    // Removal of old concrete - unlike the slab and wall that have special cutout items, there is nothing for columns

    // For the generic selective demo codes, we need the reinforcement ratio - pick the largest one
    auto longReinfRatio = theRectConcreteColumn->getTheLongitudinalReinforcementRatioParameter()->getCurrentValue();
    auto transReinfRatio = theRectConcreteColumn->getTheTransverseReinforcementRatioParameter()->getCurrentValue();

    auto reinfRatio = longReinfRatio > transReinfRatio ? longReinfRatio : transReinfRatio;

    // Demolition volume in yards
    auto demoVol = volumeRepair;

    // CSI codes for selective demolition
    this->getSelectiveConcreteDemolitionCodes(reinfRatio,demoVol,jobPhase,labourQuantityCSICodes,theRectConcreteColumn);

    // Concrete disposal in yards
    this->getConcreteRemovalDisposalCodes(demoVol,jobPhase,labourQuantityCSICodes,theRectConcreteColumn);

    // Column surface are in sq. ft.
    auto colSurfaceArea = theRectConcreteColumn->getSurfaceArea()*10.7639; //Need to convert m2 into sqft.

    // Ensure that very small areas of cover repair are not repaired with forming
    auto formArea = 0.0;

    if(areaRepair*2.0 < 0.25*colSurfaceArea)
        formArea = 0.25*colSurfaceArea;
    else if(areaRepair*2.0 < 0.5*colSurfaceArea)
        formArea = 0.5*colSurfaceArea;
    else
        formArea = colSurfaceArea;

    // Get the column forming codes
    this->getRectRCColFormingCodes(theRectConcreteColumn,jobPhase,labourQuantityCSICodes,formArea);

    // Placement - assume that only pumped is possible
    // Get the column dimension; this is used later to calculate the cost
    auto maxColumnDimension = theRectConcreteColumn->getCrossSectionWidth()->getCurrentValue();

    // Calculate the column depth
    auto columnDepth = theRectConcreteColumn->getCrossSectionDepth()->getCurrentValue();

    if(columnDepth > maxColumnDimension)
        maxColumnDimension = columnDepth;

    // Maximum column dimension in inches
    auto columnDimension = maxColumnDimension*39.3701;

    // Volume of concrete in yards, add 20% for waste
    auto volumeOfConcrete = volumeRepair*1.20;

    // Crew C20 if the concrete is pumped
    RCrew* crewC20 = jobPhase->addCrew(RCrewCode::C20);

    // Labour and Equipment for crew C20
    // 1 Labor Foreman (outside)
    // 5 laborers
    // 1 Cement Finisher
    // 1 Equip. Oper. (medium)
    // 2 Gas Engine Vibrators
    // 1 Concrete Pump (Small)

    if(columnDimension <= 12)
    {
        // Line Number:	033113700400
        // Description:	Structural concrete, placing, column, square or round, pumped, 12" thick, includes leveling (strike off) & consolidation, excludes material
        // Unit: C.Y.
        QString CSINumber = "033113700400";
        QString Description = "Structural concrete, placing, column, square or round, pumped, 12 in. thick, includes leveling (strike off) & consolidation, excludes material";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

        crewC20->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
    }
    else if(columnDimension <= 18)
    {
        // Line Number: 033113700600
        // Description:	Structural concrete, placing, column, square or round, pumped, 18" thick, includes leveling (strike off) & consolidation, excludes material
        // Unit: C.Y.
        QString CSINumber = "033113700600";
        QString Description = "Structural concrete, placing, column, square or round, pumped, 18 in. thick, includes leveling (strike off) & consolidation, excludes material";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

        crewC20->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
    }
    else if(columnDimension <= 24)
    {
        // Line Number:	033113700800
        // Description:	Structural concrete, placing, column, square or round, pumped, 24" thick, includes leveling (strike off) & consolidation, excludes material
        // Unit: C.Y.
        QString CSINumber = "033113700800";
        QString Description = "Structural concrete, placing, column, square or round, pumped, 24 in. thick, includes leveling (strike off) & consolidation, excludes material";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

        crewC20->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
    }
    else if (columnDimension <= 36)
    {
        // Line Number:	033113701000
        // Description:	Structural concrete, placing, column, square or round, pumped, 36" thick, includes leveling (strike off) & consolidation, excludes material
        // Unit: C.Y.
        QString CSINumber = "033113701000";
        QString Description = "Structural concrete, placing, column, square or round, pumped, 36 in. thick, includes leveling (strike off) & consolidation, excludes material";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfConcrete,Description));

        crewC20->addTask(CSINumber,volumeOfConcrete,Description,theRectConcreteColumn);
    }
    else
    {
        qDebug()<<"Column dimension is over 36 in., costs are not available for columns this large";
    }

    // Add repair concrete to the total volume of concrete for the job
    jobPhase->getYrdConcrete() += volumeOfConcrete;

    // Get the type of concrete
    auto theConcreteType = theRectConcreteColumn->getConcreteStrength();

    // CSI code for the supply of concrete - assume same type of concrete used for repair as in construction
    this->getConcreteCSICode(theConcreteType,volumeOfConcrete,materialQuantityCSICodes);

    return  0;
}


int RCSIAssembler::getRCSlabMaterialConstructionCodes(RRCSlabComponent* theConcreteSlab,
                                                      RJob* jobPhase,
                                                      RBIM* theBIM,
                                                      CSIVector* materialQuantityCSICodes)
{
    // For a slab assume plywood pan forms on stringers with shoring or scaffolding

    // Four stages
    //      (1) Forming: Form material, form labor
    //      (2) Reinforcing: reinforcing steel material, reinforcing steel labor
    //      (3) Placing: labour, concrete
    //      (4) Form Stripping/finishing/curing: labour


    /***** (1) Formwork****/

    // 1. Shores and stringers are erected
    // 2. Steel Soffit forms or plywood forms are placed on stringers, shores are adjusted to support the forms
    // 3. Wood headers and blockouts are placed for electrical outlets etc.
    // 4. End forms are placed and nailed to soffit
    // 5. Pans oiled to facilitate removal
    // 6. All reinforcing is placed
    // 7. Concrete is placed
    // 8. Shores are removed after concrete gains proper strength
    // 9. Forms are removed and reconditioned for next use

    // Material Quantities - assume metal open deck pan forms

    // Formwork installation - material costs are bundled with the labour codes
    this->getRCSlabFormingCodes(theConcreteSlab,nullptr,theBIM,materialQuantityCSICodes);

    /***** (2) Reinforcement****/

    // Reinforcement installation - material costs are bundled with the labour codes
    this->getRCSlabReinforcingCodes(theConcreteSlab,nullptr,materialQuantityCSICodes);

    /***** (3) Placement****/

    // Method for delivering concrete Figure 3-12  RSMeans Costing Div. 3. Preferred and Acceptable Methods for Transporting Concrete to the Job Site and at the Job Site

    // Volume of concrete in yards
    auto volumeOfConcrete = theConcreteSlab->getVolumeOfConcrete()*1.30795;

    // Add to the total volume of concrete for the job
    if(jobPhase)
        jobPhase->getYrdConcrete() += volumeOfConcrete;

    auto theConcreteType = theConcreteSlab->getConcreteStrength();

    // CSI code for the supply of concrete - assume same type of concrete used for repair as in construction
    this->getConcreteCSICode(theConcreteType,volumeOfConcrete,materialQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getRCShearWallMaterialConstructionCodes(RRCShearWallComponent* theConcreteShearwall,
                                                           RJob* jobPhase,
                                                           RBIM* theBIM,
                                                           CSIVector* materialQuantityCSICodes)
{
    // For a shear wall assume plywood forms

    // Four stages:
    //      (1) Forming: Form material, form labor
    //      (2) Reinforcing: reinforcing steel material, reinforcing steel labor
    //      (3) Placing: labour, concrete
    //      (4) Form Stripping/finishing/curing: labour

    /***** (1) Formwork****/

    this->getRCShearWallFormingCodes(theConcreteShearwall,nullptr,theBIM,materialQuantityCSICodes);

    /***** (2) Reinforcement****/

    this->getRCShearWallReinforcingCodes(theConcreteShearwall,nullptr,materialQuantityCSICodes);

    /***** (3) Placement****/

    // Method for delivering concrete Figure 3-12  RSMeans Costing Div. 3. Preferred and Acceptable Methods for Transporting Concrete to the Job Site and at the Job Site
    //      1) Pump / Pump Truck
    //      2) Crane and Bucket

    // Number of building storeys to be used to determine if concrete placed by pump or crane
    auto numBuildingStoreys = theBIM->getNumberOfStoreys();

    bool isPumped = false;

    // Use pump or pump truck if the number of building storeys is less than 4, else crane is required
    if(numBuildingStoreys < 4)
    {
        //Use pump or pump truck
        isPumped = true;
    }

    // Volume of concrete in yards
    auto volumeOfConcrete = theConcreteShearwall->getVolumeOfConcrete()*1.30795;

    // Add to the total volume of concrete for the job
    if(jobPhase)
        jobPhase->getYrdConcrete() += volumeOfConcrete;

    // Get the type of concrete
    auto theConcreteType = theConcreteShearwall->getConcreteStrength();

    // CSI code for the supply of concrete - assume same type of concrete used for repair as in construction
    this->getConcreteCSICode(theConcreteType,volumeOfConcrete,materialQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getRCShearWallReplacementCodes(RRCShearWallComponent* theConcreteShearwall,
                                                  RBIM* theBIM,
                                                  RJob* jobPhase,
                                                  CSIVector* materialQuantityCSICodes,
                                                  CSIVector* labourQuantityCSICodes)
{

    // The reinforcment ratio, greater or the long. and trans. ratios
    auto longReinfRatio = theConcreteShearwall->getTheLongitudinalReinforcementRatioParameter()->getCurrentValue();
    auto transReinfRatio = theConcreteShearwall->getTheTransverseReinforcementRatioParameter()->getCurrentValue();

    auto reinfRatio = longReinfRatio > transReinfRatio ? longReinfRatio : transReinfRatio;

    // Wall length in feet
    auto wallLength = theConcreteShearwall->getLengthValue()*3.28084;

    // For replacement of section
    //    1.	Shoring of structure
    //    2.	Demolition of old concrete
    //    3.	Removal/Disposal/Cleaning debris
    //    4.	Drill in reinforcement dowels and/or apply external reinforcement
    //    5.	Concrete Formwork
    //    6.	Concrete Placement
    //    7.	Finishing/Form removal
    //    8.    Curing

    //*************  1.	Shoring of structure  *************

    //Assume eight frames
    auto numberOfFrames = 8.0;

    this->getShoringCSICodes(numberOfFrames,materialQuantityCSICodes);

    //*************  2.&3. Demolition of Component/Removal/Disposal/Cleaning debris *************

    // Floor sawing
    // Line Number: 038113000000
    // Description: Flat concrete sawing

    // Depth of saw cut in inches
    auto depthOfCut = theConcreteShearwall->getThickness()->getCurrentValue()*39.3701;

    // Parent: Wall sawing (038116000000 - 038116500750)
    // Description: Concrete sawing, track mounted wall, includes blade cost, layout and set up time
    RCrew* crewB89B = jobPhase->addCrew(RCrewCode::B89B);
    {
        auto lengthOfCut = 2.0*wallLength*depthOfCut;

        // Per inch of cut depth
        // With rod reinforcement
        // Line Number: 038116500820
        // Description: Concrete sawing, concrete walls, rod reinforcing, per inch of depth
        // Unit: L.F.
        // Crew:  Crew B89B
        QString CSINumber = "038116500820";
        QString Description = "Concrete sawing, concrete walls, rod reinforcing, per inch of depth";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,lengthOfCut,Description));

        crewB89B->addTask(CSINumber,lengthOfCut,Description,theConcreteShearwall);
    }

    RCrew* crewB9 = jobPhase->addCrew(RCrewCode::B9);
    {
        auto lengthOfCut = 2.0*wallLength;

        // Line Number: 024119271040
        // Description: Selective demolition, torch cutting, steel, reinforced concrete walls, 12"-16" thick, oxygen lance cutting
        // Unit: L.F.
        // Crew: Crew B9
        QString CSINumber = "024119271040";
        QString Description = "Selective demolition, torch cutting, steel, reinforced concrete walls, 12-16 in thick, oxygen lance cutting";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,lengthOfCut,Description));

        crewB9->addTask(CSINumber,lengthOfCut,Description,theConcreteShearwall);
    }

    // In addition to sawing, there will be some selective demolition
    // Demolition volume in yards
    auto demoVol = theConcreteShearwall->getVolume()*1.30795;

    this->getSelectiveConcreteDemolitionCodes(reinfRatio,demoVol,jobPhase,labourQuantityCSICodes,theConcreteShearwall);

    // Quantity of concrete for disposal in yards
    this->getConcreteRemovalDisposalCodes(demoVol,jobPhase,labourQuantityCSICodes,theConcreteShearwall);

    //*************  4. Drill in reinforcement dowels and/or apply external reinforcement *************

    // Calculate the number of dowels, based on the wall length
    auto numDowels = 2.0*theConcreteShearwall->getLength12()/0.4064;

    this->getReinforcementDowelCodes(numDowels,theConcreteShearwall,jobPhase,labourQuantityCSICodes);

    //*************  5.-8.	Concrete Formwork, Concrete Placement, Finishing/Form removal, Curing *************

    // CSI codes for the supply of concrete - assume same type of concrete used for repair as in construction
    this->getRCShearWallMaterialConstructionCodes(theConcreteShearwall,jobPhase,theBIM,materialQuantityCSICodes);

    // CSI codes for labour (forming, placing, curing)
    this->getRCShearWallLabourConstructionCodes(theConcreteShearwall,jobPhase,theBIM,labourQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getRCWallCoverReplacementCodes(double volumeRepair,
                                                  double depthRepair,
                                                  double areaRepair,
                                                  RJob* jobPhase,
                                                  CSIVector* labourQuantityCSICodes,
                                                  CSIVector* materialQuantityCSICodes,
                                                  RRCShearWallComponent* theConcreteShearwall)
{
    //For replacement of cover
    //    1.	Removal of old concrete (150mm min.) with light jack hammer or sufficient depth to reach at least 7mm, plus the dimension of the maximum size aggregate behind any reinforcement
    //    2.	Clean with water blasting or sand blasting
    //    3.	Drill in reinforcement dowels and/or apply external reinforcement
    //    4.	Set up forms and shoring (if vertical or on ceiling)
    //    5.	Placement
    //    6.	Form removal
    //    7.    Curing

    auto longReinfRatio = theConcreteShearwall->getTheLongitudinalReinforcementRatioParameter()->getCurrentValue();
    auto transReinfRatio = theConcreteShearwall->getTheTransverseReinforcementRatioParameter()->getCurrentValue();

    auto reinfRatio = longReinfRatio > transReinfRatio ? longReinfRatio : transReinfRatio;

    auto wallHeight = theConcreteShearwall->getHeightValue()*convertMeterToFoot;

    // Volume in cubic yards
    this->getSelectiveConcreteDemolitionCodes(reinfRatio,volumeRepair,jobPhase,labourQuantityCSICodes,theConcreteShearwall);

    // Larger repair using forms
    // Area in sq. ft., volume in cubic feet, wall height in feet
    this->getWallCoverRepairFormsCodes(areaRepair,volumeRepair*convertYrdToCubicFt,wallHeight,theConcreteShearwall,jobPhase,materialQuantityCSICodes,labourQuantityCSICodes);

    // Volume of demolititon concrete in yards
    auto demoVol = volumeRepair;

    // Quantity of concrete for disposal in yards
    this->getConcreteRemovalDisposalCodes(demoVol,jobPhase,labourQuantityCSICodes,theConcreteShearwall);

    return 0;
}


int RCSIAssembler::getSteelWColumnMaterialConstructionCodes(RWSteelBeamColumnComponent* theWBeamCol,
                                                            CSIVector* materialQuantityCSICodes)
{
    // Length of the column in m
    auto lengthCol = theWBeamCol->getLength();

    // Length of the column in feet
    auto linearFeet = lengthCol*convertMeterToFoot;

    // Get the steel section type
    QString shape = theWBeamCol->getWSectionType();

    // CSI codes for construction material
    this->getSteelFramingCSICodes(shape,linearFeet,nullptr,nullptr,materialQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getSteelWColumnLabourConstructionCodes(RWSteelBeamColumnComponent* theWBeamCol,
                                                          RJob* jobPhase,
                                                          CSIVector* labourQuantityCSICodes)
{
    // Length of the column in m
    auto lengthCol = theWBeamCol->getLength();

    // Length of the column in feet
    auto linearFeet = lengthCol*convertMeterToFoot;

    // Get the steel section type
    QString shape = theWBeamCol->getWSectionType();

    // CSI codes for construction labour
    this->getSteelFramingCSICodes(shape,linearFeet,theWBeamCol,jobPhase,labourQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getGlulamColumnMaterialConstructionCodes(RRectangularGlulamBeamColumnComponent* theGlulamCol,
                                                            CSIVector* materialQuantityCSICodes)
{
    // Volume of column in cubic meters
    auto volWood = theGlulamCol->getVolumeOfWood();

    // Volume in thousand board-foot, 1MBF = 2.36 cubic meters
    auto MBFcol = volWood/2.36;

    // Line Number:	061813204400
    // Description:	Laminated framing, alternate pricing method: (use nominal footage of components), columns, includes hardware
    // Unit: M.B.F.
    QString CSINumber = "061813204400";
    QString Description = "Laminated framing, alternate pricing method: (use nominal footage of components), columns, includes hardware";

    // CSI codes for construction material
    materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,MBFcol,Description));

    return 0;
}


int RCSIAssembler::getGlulamColumnLabourConstructionCodes(RRectangularGlulamBeamColumnComponent* theGlulamCol,
                                                          RJob* jobPhase,
                                                          CSIVector* labourQuantityCSICodes)
{
    // Volume of column in cubic meters
    auto volWood = theGlulamCol->getVolumeOfWood();

    // Volume in thousand board-foot, 1MBF = 2.36 cubic meters
    auto MBFcol = volWood/2.36;

    // Line Number:	061813204400
    // Description:	Laminated framing, alternate pricing method: (use nominal footage of components), columns, includes hardware
    // Unit: M.B.F.
    QString CSINumber = "061813204400";
    QString Description = "Laminated framing, alternate pricing method: (use nominal footage of components), columns, includes hardware";

    // CSI codes for construction labour
    labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,MBFcol,Description));

    RCrew* crewF3 = nullptr;

    if(jobPhase)
        crewF3 = jobPhase->addCrew(RCrewCode::F3);

    if(crewF3)
        crewF3->addTask(CSINumber,MBFcol,Description,theGlulamCol);

    return 0;
}


int RCSIAssembler::getCLTPanelConstructionCodes(RCLTPanelComponent* theCLTPanel,
                                                RJob* jobPhase,
                                                CSIVector* CSICodes)
{
    // Area of CLT panel in sq. ft.
    auto areaPanel = theCLTPanel->getPlanArea()*convertMeterSqToFtSq;

    // Line Number:	061523000000
    // Description:	Laminated wood decking, CLT floor/roof, includes fire proofing and acoustical treatment
    // Unit: S.F.
    QString CSINumber = "061523000000";
    QString Description = "Laminated wood decking, CLT floor/roof, includes fire proofing and acoustical treatment";

    // CSI codes for construction labour
    CSICodes->push_back(std::make_tuple(CSINumber,areaPanel,Description));

    RCrew* crewF3 = nullptr;

    if(jobPhase)
    {
        crewF3 = jobPhase->addCrew(RCrewCode::F3);

        if(crewF3)
            crewF3->addTask(CSINumber,areaPanel,Description,theCLTPanel);
    }

    return 0;
}


int RCSIAssembler::getSteelSkinSlabConstructionCodes(RCompositePanelComponent* theSteelPanel,
                                                     RJob* jobPhase,
                                                     CSIVector* CSICodes)
{
    // Get the gauge of the steel decking
    auto guage = theSteelPanel->getGuage();

    // Depth of decking in inches - note that this is different than the component thickness
    auto depth = theSteelPanel->getDeckingDepth()*convertMeterToInch;

    // Area of steel panel in sq. ft.
    auto area = theSteelPanel->getPlanArea()*convertMeterSqToFtSq;

    QString CSINumber = "NULL";
    QString Description = "NULL";

    if(guage == 16)
    {
        if(fabs(depth - 1.5) < 1.0e-4)
        {
            CSINumber = "053113505100";
            Description = "Metal floor decking, steel, non-cellular, composite, galvanized, 1-1/2\" D, 16 gauge";
        }
        else if(fabs(depth - 2.0) < 1.0e-4)
        {
            CSINumber = "053113505500";
            Description = "Metal floor decking, steel, non-cellular, composite, galvanized, 2\" D, 16 gauge";
        }
        else if(fabs(depth - 3.0) < 1.0e-4)
        {
            CSINumber = "053113506000";
            Description = "Metal floor decking, steel, non-cellular, composite, galvanized, 3\" D, 16 gauge";
        }
        else
        {
            qDebug()<<"The depth value of "<<depth<<" is not supported in "<<__FUNCTION__;
            return -1;
        }
    }
    else if(guage == 18)
    {
        if(fabs(depth - 1.5) < 1.0e-4)
        {
            CSINumber = "053113505120";
            Description = "Metal floor decking, steel, non-cellular, composite, galvanized, 1-1/2\" D, 18 gauge";
        }
        else if(fabs(depth - 2.0) < 1.0e-4)
        {
            CSINumber = "053113505400";
            Description = "Metal floor decking, steel, non-cellular, composite, galvanized, 2\" D, 18 gauge";
        }
        else if(fabs(depth - 3.0) < 1.0e-4)
        {
            CSINumber = "053113505900";
            Description = "Metal floor decking, steel, non-cellular, composite, galvanized, 3\" D, 18 gauge";
        }
        else
        {
            qDebug()<<"The depth value of "<<depth<<" is not supported in "<<__FUNCTION__;
            return -1;
        }
    }
    else if(guage == 20)
    {
        if(fabs(depth - 1.5) < 1.0e-4)
        {
            CSINumber = "053113505140";
            Description = "Metal floor decking, steel, non-cellular, composite, galvanized, 1-1/2\" D, 20 gauge";
        }
        else if(fabs(depth - 2.0) < 1.0e-4)
        {
            CSINumber = "053113505300";
            Description = "Metal floor decking, steel, non-cellular, composite, galvanized, 2\" D, 20 gauge";
        }
        else if(fabs(depth - 3.0) < 1.0e-4)
        {
            CSINumber = "053113505800";
            Description = "Metal floor decking, steel, non-cellular, composite, galvanized, 3\" D, 20 gauge";
        }
        else
        {
            qDebug()<<"The depth value of "<<depth<<" is not supported in "<<__FUNCTION__;
            return -1;
        }
    }
    else if(guage == 22)
    {
        if(fabs(depth - 2.0) < 1.0e-4)
        {
            CSINumber = "053113505200";
            Description = "Metal floor decking, steel, non-cellular, composite, galvanized, 2\" D, 22 gauge";
        }
        else if(fabs(depth - 3.0) < 1.0e-4)
        {
            CSINumber = "053113505700";
            Description = "Metal floor decking, steel, non-cellular, composite, galvanized, 3\" D, 22 gauge";
        }
        else
        {
            qDebug()<<"The depth value of "<<depth<<" is not supported in "<<__FUNCTION__;
            return -1;
        }

    }
    else
    {
        qDebug()<<"The guage value of "<<guage<<" is not supported in "<<__FUNCTION__;
        return -1;
    }

    // CSI codes for the steel decking
    CSICodes->push_back(std::make_tuple(CSINumber,area,Description));

    if(jobPhase)
    {
        RCrew* crewE4 = jobPhase->addCrew(RCrewCode::E4);

        crewE4->addTask(CSINumber,area,Description,theSteelPanel);
    }

    // CSI codes for lightweight concrete topping reinforcement - assume welded wire fabric reinforcement
    {
        // Line Number: 032211100500
        // Description: 4 x 4 - W1.4 x W1.4 (10 x 10) 31 lb./C.S.F.
        // Unit: C.S.F.
        QString CSINumber = "032211100500";
        QString Description = "4 x 4 - W1.4 x W1.4 (10 x 10) 31 lb./C.S.F.";

        CSICodes->push_back(std::make_tuple(CSINumber,area/100.0,Description));

        if(jobPhase)
        {
            RCrew* crewRod2 = jobPhase->addCrew(RCrewCode::Rodm2);

            crewRod2->addTask(CSINumber,area/100.0,Description,theSteelPanel);
        }
    }

    // Now the concrete topping
    {
        // Line Number: 033053403300
        // Description: Structural concrete, in place, elevated slab, floor fill, lightweight, 110 lb./C.F., 2-1/2\" thick, concrete (Portland cement Type I), placing and finishing, excl forms, reinforcing
        // Unit: S.F.
        QString CSINumber = "033053403300";
        QString Description = "Structural concrete, in place, elevated slab, floor fill, lightweight, 110 lb./C.F., 2-1/2\" thick, concrete (Portland cement Type I), placing and finishing, excl forms, reinforcing";

        CSICodes->push_back(std::make_tuple(CSINumber,area,Description));

        if(jobPhase)
        {
            RCrew* crewC8 = jobPhase->addCrew(RCrewCode::C8);

            crewC8->addTask(CSINumber,area,Description,theSteelPanel);
        }
    }

    return 0;
}


int RCSIAssembler::getConcreteRemovalDisposalCodes(double volumeOfDemoConcrete,
                                                   RJob* jobPhase,
                                                   CSIVector* labourQuantityCSICodes,

                                                   RComponent* theComponent)
{

    RCrew* crewCLAB2 = jobPhase->addCrew(RCrewCode::CLAB2);
    {
        // Line Number: 024119192000
        // Description: Selective demolition, rubbish handling, 0'-50' haul, load, haul, dump and return, hand carried, cost to be added to demolition cost
        // Crew: 2 Clab
        // Unit: C.Y.
        QString CSINumber = "024119192000";
        QString Description = "Selective demolition, rubbish handling, 0'-50' haul, load, haul, dump and return, hand carried, cost to be added to demolition cost";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfDemoConcrete,Description));

        crewCLAB2->addTask(CSINumber,volumeOfDemoConcrete,Description,theComponent);
    }

    RCrew* crewB3 = jobPhase->addCrew(RCrewCode::B3);
    {
        // Line Number: 024119180300
        // Description: Selective demolition, disposal only, urban buildings with salvage value allowed, concrete frame, includes loading and 5 mile haul to dump
        // Crew: Crew B3
        // Unit: C.Y.
        QString CSINumber = "024119180300";
        QString Description = "Selective demolition, disposal only, urban buildings with salvage value allowed, concrete frame, includes loading and 5 mile haul to dump";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfDemoConcrete,Description));

        crewB3->addTask(CSINumber,volumeOfDemoConcrete,Description,theComponent);
    }

    return 0;
}


int RCSIAssembler::getWoodRemovalDisposalCodes(double volumeOfDemoWood,
                                               RJob* jobPhase,
                                               CSIVector* labourQuantityCSICodes,
                                               RComponent* theComponent)
{

    RCrew* crewB3 = jobPhase->addCrew(RCrewCode::B3);
    {
        // Line Number: 024119180500
        // Description: Selective demolition, disposal only, urban buildings with salvage value allowed, wood frame, includes loading and 5 mile haul to dump
        // Unit: C.Y.
        QString CSINumber = "024119180500";
        QString Description = "Selective demolition, disposal only, urban buildings with salvage value allowed, wood frame, includes loading and 5 mile haul to dump";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfDemoWood,Description));

        crewB3->addTask(CSINumber,volumeOfDemoWood,Description,theComponent);
    }

    return 0;
}


int RCSIAssembler::getSteelRemovalDisposalCodes(double volumeOfDemoSteel,
                                                RJob* jobPhase,
                                                CSIVector* labourQuantityCSICodes,
                                                RComponent* theComponent)
{

    RCrew* crewB3 = jobPhase->addCrew(RCrewCode::B3);
    {
        // Line Number: 024119180200
        // Description: Selective demolition, disposal only, urban buildings with salvage value allowed, steel frame, includes loading and 5 mile haul to dump
        // Unit: C.Y.
        QString CSINumber = "024119180200";
        QString Description = "Selective demolition, disposal only, urban buildings with salvage value allowed, steel frame, includes loading and 5 mile haul to dump";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfDemoSteel,Description));

        crewB3->addTask(CSINumber,volumeOfDemoSteel,Description,theComponent);
    }

    return 0;
}


int RCSIAssembler::getEpoxyCrackRepairCodes(double volumeOfCrack,
                                            CSIVector* materialQuantityCSICodes,
                                            RJob* jobPhase,
                                            CSIVector* labourQuantityCSICodes,
                                            RComponent* theComponent)
{

    // Return quickly if volume of crack is zero
    if(volumeOfCrack == 0.0)
    {
        qDebug()<<"Warning in "<<__FUNCTION__<<" the volume of the crack is zero";
        return -1;
    }

    RCrew* crewEPXY = jobPhase->addCrew(RCrewCode::EPXY);

    // NOTE: RSMeans does not have costs for epoxy grouting, it just lists the MasterFormat code
    // This is specialized work done by subcontractors
    // As a result, just add it as a material, i.e., at a unit cost
    {
        // Line Number: 036423000000
        // Description: Concrete resurfacing, epoxy injection grouting
        // Unit: C.F.
        QString CSINumber = "036423000000";
        QString Description = "Concrete resurfacing, epoxy injection grouting";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfCrack,Description));

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfCrack,Description));

        crewEPXY->addTask(CSINumber,volumeOfCrack,Description,theComponent);

        jobPhase->getCfEpoxy() += volumeOfCrack;
    }

    return 0;
}


int RCSIAssembler::getWallPatchCoverCodes(double areaOfPatch,
                                          double volumeOfPatch,
                                          RComponent* theComponent,
                                          RJob* repairPhase,
                                          CSIVector* materialQuantityCSICodes,
                                          CSIVector* labourQuantityCSICodes)
{

    RCrew* crewCefi = repairPhase->addCrew(RCrewCode::Cefi1);

    // Labour
    {
        // Line Number: 033529600600
        // Description: Concrete finishing, walls, float finish, 1/16" thick
        // Units: S.F.
        // Crew: 1 Cefi
        QString CSINumber = "033529600600";
        QString Description = "Concrete finishing, walls, float finish, 1/16in. thick";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,areaOfPatch,Description));

        crewCefi->addTask(CSINumber,areaOfPatch,Description,theComponent);
    }

    // Material - assume hand mix repair mortar - also ignore the extra labour on the mixer, assume the finisher mixes own concrete
    {
        // Line Number: 033113250340
        // Description:  Concrete, hand mix, for small quantities or remote areas, 4000 psi, using wheelbarrow, includes bagged pre-mixed dry ingredients (80-Lb bag = 0.6 C.F.) and water, excludes, forms, reinforcing, placing & finishing
        // Unit: C.F.
        // Crew: 1 Clab
        QString CSINumber = "033113250340";
        QString Description = "Concrete, hand mix, for small quantities or remote areas, 4000 psi, using wheelbarrow, includes bagged pre-mixed dry ingredients (80-Lb bag = 0.6 C.F.) and water, excludes, forms, reinforcing, placing & finishing";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfPatch,Description));

        repairPhase->getYrdConcrete() += volumeOfPatch*convertCubicFttoYrd;
    }

    // Curing
    this->getConcreteCuringCSICodes(areaOfPatch,theComponent,repairPhase,labourQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getSlabSpallPatchCodes(double areaOfPatch,
                                          double volumeOfPatch,
                                          RComponent* theComponent,
                                          RJob* repairPhase,
                                          CSIVector* materialQuantityCSICodes,
                                          CSIVector* labourQuantityCSICodes)
{

    // Convert volume into yards
    auto volumeInYrds = volumeOfPatch*convertCubicFttoYrd;

    // Add volume of concrete to the total volume of the job
    repairPhase->getYrdConcrete() += volumeInYrds;

    // Placing labour - assume that if the volume of the spalling repair is over two yards than will use redi-mix concrete and a cart, otherwise hand-mix and place
    if(volumeInYrds > 2.0)
    {
        // Assume C40 for repair
        auto theConcreteType = "C40";

        // CSI codes for the supply of concrete
        this->getConcreteCSICode(theConcreteType,volumeInYrds,materialQuantityCSICodes);

        RCrew* crewC18 = repairPhase->addCrew(RCrewCode::C18);

        // Line Number: 033113705620
        // Description: Structural concrete, placing, by walking cart, 150' haul, excludes material, add to placing costs above
        // Unit: C.Y.
        // Crew: Crew C18
        QString CSINumber = "033113705620";
        QString Description = "Structural concrete, placing, by walking cart, 150' haul, excludes material, add to placing costs above";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeInYrds,Description));

        crewC18->addTask(CSINumber,volumeInYrds,Description,theComponent);
    }
    // Material - assume hand mix repair mortar - also ignore the extra labour on the mixer, assume the finisher mixes own concrete
    else
    {
        // Line Number: 033113250340
        // Description:  Concrete, hand mix, for small quantities or remote areas, 4000 psi, using wheelbarrow, includes bagged pre-mixed dry ingredients (80-Lb bag = 0.6 C.F.) and water, excludes, forms, reinforcing, placing & finishing
        // Unit: C.F.
        // Crew: 1 Clab
        QString CSINumber = "033113250340";
        QString Description = "Concrete, hand mix, for small quantities or remote areas, 4000 psi, using wheelbarrow, includes bagged pre-mixed dry ingredients (80-Lb bag = 0.6 C.F.) and water, excludes, forms, reinforcing, placing & finishing";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfPatch,Description));

    }

    // Finishing
    RCrew* crewC10 = repairPhase->addCrew(RCrewCode::C10);
    {
        // Line Number: 033513300200
        // Description: Concrete finishing, fresh concrete flatwork, floors, basic finishing for unspecified flatwork, bull float, manual float & manual steel trowel, excl placing, striking off & consolidating
        // Unit: S.F.
        // Crew: Crew C10
        QString CSINumber = "033513300200";
        QString Description = "Concrete finishing, fresh concrete flatwork, floors, basic finishing for unspecified flatwork, bull float, manual float & manual steel trowel, excl placing, striking off & consolidating";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,areaOfPatch,Description));

        crewC10->addTask(CSINumber,areaOfPatch,Description,theComponent);
    }

    // Curing
    this->getConcreteCuringCSICodes(areaOfPatch,theComponent,repairPhase,labourQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getSlabCoverRepairCodes(double areaOfPatch,
                                           double volumeOfPatch,
                                           RComponent* theComponent,
                                           RJob* repairPhase,
                                           CSIVector* materialQuantityCSICodes,
                                           CSIVector* labourQuantityCSICodes)
{

    // Repair volume in yards
    auto volumeInYrds = volumeOfPatch*convertCubicFttoYrd;

    // Material - assume hand mix concrete if less than two yards and redi-mix if more
    if(volumeInYrds < 2.0)
    {
        // Line Number: 033113250340
        // Description:  Concrete, hand mix, for small quantities or remote areas, 4000 psi, using wheelbarrow, includes bagged pre-mixed dry ingredients (80-Lb bag = 0.6 C.F.) and water, excludes, forms, reinforcing, placing & finishing
        // Unit: C.F.
        // Crew: 1 Clab
        QString CSINumber = "033113250340";
        QString Description = "Concrete, hand mix, for small quantities or remote areas, 4000 psi, using wheelbarrow, includes bagged pre-mixed dry ingredients (80-Lb bag = 0.6 C.F.) and water, excludes, forms, reinforcing, placing & finishing";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfPatch,Description));
    }
    else
    {
        // Assume C40 for repair
        auto theConcreteType = "C40";

        // CSI code for the supply of concrete
        this->getConcreteCSICode(theConcreteType,volumeInYrds,materialQuantityCSICodes);
    }

    repairPhase->getYrdConcrete() += volumeInYrds;

    // Placing labour
    RCrew* crewC20 = repairPhase->addCrew(RCrewCode::C20);
    {
        // Line Number: 033113701400
        // Description: Structural concrete, placing, elevated slab, pumped, less than 6" thick, includes leveling (strike off) & consolidation, excludes material
        // Unit: C.Y.
        // Crew: Crew C20
        QString CSINumber = "033113701400";
        QString Description = " Structural concrete, placing, elevated slab, pumped, less than 6in. thick, includes leveling (strike off) & consolidation, excludes material";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeInYrds,Description));

        crewC20->addTask(CSINumber,volumeInYrds,Description,theComponent);
    }

    // Finishing
    RCrew* crewC10 = repairPhase->addCrew(RCrewCode::C10);
    {
        // Line Number: 033513300200
        // Description: Concrete finishing, fresh concrete flatwork, floors, basic finishing for unspecified flatwork, bull float, manual float & manual steel trowel, excl placing, striking off & consolidating
        // Unit: S.F.
        // Crew: Crew C10
        QString CSINumber = "033513300200";
        QString Description = "Concrete finishing, fresh concrete flatwork, floors, basic finishing for unspecified flatwork, bull float, manual float & manual steel trowel, excl placing, striking off & consolidating";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,areaOfPatch,Description));

        crewC10->addTask(CSINumber,areaOfPatch,Description,theComponent);
    }

    // Curing
    this->getConcreteCuringCSICodes(areaOfPatch,theComponent,repairPhase,labourQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getRCSlabReplacementCodes(RRCSlabComponent* theConcreteSlab,
                                             RBIM* theBIM,
                                             RJob* jobPhase,
                                             CSIVector* materialQuantityCSICodes,
                                             CSIVector* labourQuantityCSICodes)
{

    // For replacement of section
    //    1.	Shoring of structure
    //    2.	Demolition of old concrete
    //    3.	Removal/Disposal/Cleaning debris
    //    4.	Drill in reinforcement dowels and/or apply external reinforcement
    //    5.	Concrete Formwork
    //    6.	Concrete Placement
    //    7.	Finishing/Form removal
    //    8.    Curing

    //*************  1.	Shoring of structure  *************

    // Get the area of the slab
    auto slabArea = theConcreteSlab->getPlanArea()*convertMeterSqToFtSq;

    // Calculate the number of shoring jacks
    // Assume one jack for every 36 sq. ft., i.e., 6x6 area
    auto numberOfFrames = slabArea/36.0;

    this->getShoringCSICodes(numberOfFrames,materialQuantityCSICodes);

    //*************  2.&3. Demolition of Component/Removal/Disposal/Cleaning debris *************

    // Floor sawing
    // Line Number: 038113000000
    // Description: Flat concrete sawing

    // Depth of saw cut in inches
    auto depthOfCut = theConcreteSlab->getThickness()->getCurrentValue()*39.3701;

    // The perimeter of the slab in feet - Assume 4*times the perimeter because need to make the repair in sections, cannot replace all at once.
    auto slabPerimeter = theConcreteSlab->getPerimeter()*3.28084*4.0;

    RCrew* crewB89 = jobPhase->addCrew(RCrewCode::B89);

    if(depthOfCut <=3.0)
    {
        // Up to 3" deep
        // Line Number:  038113500500
        // Description:  Concrete sawing, concrete slabs, rod reinforced, up to 3" deep, includes blade cost, layout and set up time
        // Unit: L.F.
        // Crew: Crew B89

        QString CSINumber = "038113500500";
        QString Description = "Concrete sawing, concrete slabs, rod reinforced, up to 3in. deep, includes blade cost, layout and set up time";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,slabPerimeter,Description));

        crewB89->addTask(CSINumber,slabPerimeter,Description,theConcreteSlab);
    }
    else
    {
        // Each additional inch
        // Line Number: 038113500520
        // Description: Concrete sawing, concrete, existing slab, rod reinforced, for each additional inch of depth over 3", includes blade cost, layout and set up time
        // Unit: L.F.
        // Crew: Crew B89

        auto lengthOfCut = slabPerimeter*(depthOfCut-3.0);

        QString CSINumber = "038113500520";
        QString Description = "Concrete sawing, concrete slabs, rod reinforced, up to 3in. deep, includes blade cost, layout and set up time";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,lengthOfCut,Description));

        crewB89->addTask(CSINumber,lengthOfCut,Description,theConcreteSlab);
    }

    // In addition to sawing, there will be some selective demolition
    auto longReinfRatio = theConcreteSlab->getTheLongitudinalReinforcementRatioParameter()->getCurrentValue();
    auto transReinfRatio = theConcreteSlab->getTheTransverseReinforcementRatioParameter()->getCurrentValue();

    auto reinfRatio = longReinfRatio > transReinfRatio ? longReinfRatio : transReinfRatio;

    // Demolition volume in yards
    auto demoVol = theConcreteSlab->getVolume()*1.30795;

    this->getSelectiveConcreteDemolitionCodes(reinfRatio,demoVol,jobPhase,labourQuantityCSICodes,theConcreteSlab);


    // Concrete Disposal
    this->getConcreteRemovalDisposalCodes(demoVol,jobPhase,labourQuantityCSICodes,theConcreteSlab);


    //*************  4. Drill in reinforcement dowels and/or apply external reinforcement *************

    // Assume one dowel every ft of perimeter
    auto numDowels = slabPerimeter;

    this->getReinforcementDowelCodes(numDowels,theConcreteSlab,jobPhase,labourQuantityCSICodes);


    // Assume that from here on-wards a new slab is installed with the same procedure as construction

    // Material
    this->getRCSlabMaterialConstructionCodes(theConcreteSlab,jobPhase,theBIM,materialQuantityCSICodes);

    // Labour (forming, placing, curing)
    this->getRCSlabLabourConstructionCodes(theConcreteSlab,jobPhase,theBIM,labourQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getRCSlabTopCoverReplacementCodes(double volumeRepair,
                                                     double depthRepair,
                                                     double areaRepair,
                                                     RJob* jobPhase,
                                                     CSIVector* labourQuantityCSICodes,
                                                     CSIVector* materialQuantityCSICodes,
                                                     RRCSlabComponent* theConcreteSlab)
{

    // Get information about the reinforcement ratio
    auto longReinfRatio = theConcreteSlab->getTheLongitudinalReinforcementRatioParameter()->getCurrentValue();
    auto transReinfRatio = theConcreteSlab->getTheTransverseReinforcementRatioParameter()->getCurrentValue();

    // Take the largest of the two of the reinforcement ratio
    auto reinfRatio = longReinfRatio > transReinfRatio ? longReinfRatio : transReinfRatio;

    // For replacement of cover:
    //    1.	Removal of old concrete with light jack hammer or sufficient depth to reach at least 7mm, plus the dimension of the maximum size aggregate behind any reinforcement
    //    2.	Clean with water blasting or sand blasting
    //    3.	Placement
    //    4.    Curing

    // Repair volume needs to be in cubic feet
    this->getConcreteSlabCutOutDemolitionCodes(reinfRatio,volumeRepair,jobPhase,labourQuantityCSICodes,theConcreteSlab);

    // Placement/finishing/curing of floor - Area  needs to be in square foot
    this->getSlabCoverRepairCodes(areaRepair,volumeRepair,theConcreteSlab,jobPhase,materialQuantityCSICodes,labourQuantityCSICodes);

    // Disposal of concrete
    this->getConcreteRemovalDisposalCodes(volumeRepair,jobPhase,labourQuantityCSICodes,theConcreteSlab);

    return 0;
}


int RCSIAssembler::getRCSlabBottomCoverReplacementCodes(double volumeRepair,
                                                        double depthRepair,
                                                        double areaRepair,
                                                        RJob* jobPhase,
                                                        CSIVector* labourQuantityCSICodes,
                                                        CSIVector* materialQuantityCSICodes,
                                                        RRCSlabComponent* theConcreteSlab)
{
    // Get information about the reinforcement ratio
    auto longReinfRatio = theConcreteSlab->getTheLongitudinalReinforcementRatioParameter()->getCurrentValue();
    auto transReinfRatio = theConcreteSlab->getTheTransverseReinforcementRatioParameter()->getCurrentValue();

    // Take the largest of the two of the reinforcement ratio
    auto reinfRatio = longReinfRatio > transReinfRatio ? longReinfRatio : transReinfRatio;

    // For replacement of cover:
    //    1.	Removal of old concrete with light jack hammer or sufficient depth to reach at least 7mm, plus the dimension of the maximum size aggregate behind any reinforcement
    //    2.	Clean with water blasting or sand blasting
    //    3.	Placement
    //    4.    Curing


    this->getConcreteSlabCutOutDemolitionCodes(reinfRatio,volumeRepair,jobPhase,labourQuantityCSICodes,theConcreteSlab);

    // For fixing the underside of slabs -- can use spray concrete only no forms
    this->getConcreteRepairShotcreteCodes(areaRepair,depthRepair,theConcreteSlab,jobPhase,labourQuantityCSICodes);

    // Disposal of concrete
    this->getConcreteRemovalDisposalCodes(volumeRepair,jobPhase,labourQuantityCSICodes,theConcreteSlab);

    return 0;
}


int RCSIAssembler::getWallCoverRepairFormsCodes(double areaOfPatch,
                                                double volumeOfPatch,
                                                double theWallHeight,
                                                RComponent* theComponent,
                                                RJob* repairPhase,
                                                CSIVector* materialQuantityCSICodes,
                                                CSIVector* labourQuantityCSICodes)
{


    //************Labour************
    // For all types of forms below the equipment and labor requirements for crew C2
    // 1 Carpenter Foreman (outside)
    // 4 Carpenters
    // 1 Laborer
    RCrew* crewC2 = repairPhase->addCrew(RCrewCode::C2);

    auto contactAreaForms = areaOfPatch*2.0;

    auto formThickness = 0.75;

    // CSI codes for formwork labour with job-built plywood
    if(theWallHeight > 0.0 && theWallHeight <= 8.0)
    {
        // Line Number:	031113852000
        // Description:	C.I.P. concrete forms, wall, job built, plywood, to 8' high, 1 use, includes erecting, bracing, stripping and cleaning
        // Unit: S.F.C.A.
        QString CSINumber = "031113852000";
        QString Description = "C.I.P. concrete forms, wall, job built, plywood, to 8' high, 1 use, includes erecting, bracing, stripping and cleaning";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,contactAreaForms,Description));
        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,contactAreaForms,Description));

        crewC2->addTask(CSINumber,contactAreaForms,Description,theComponent);
    }
    else if( theWallHeight > 8.0 && theWallHeight <=16.0)
    {
        // Line Number:	031113852400
        // Description:	C.I.P. concrete forms, wall, job built, plywood, over 8' to 16' high, 1 use, includes erecting, bracing, stripping and cleaning
        // Unit: S.F.C.A.
        QString CSINumber = "031113852400";
        QString Description = "C.I.P. concrete forms, wall, job built, plywood, over 8' to 16' high, 1 use, includes erecting, bracing, stripping and cleaning";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,contactAreaForms,Description));
        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,contactAreaForms,Description));

        crewC2->addTask(CSINumber,contactAreaForms,Description,theComponent);
    }
    else if(theWallHeight > 16.0)
    {
        // Line Number: 031113852700
        // Description:	C.I.P. concrete forms, wall, job built, plywood, over 16' high, 1 use, includes erecting, bracing, stripping and cleaning
        // Unit: S.F.C.A.
        QString CSINumber = "031113852700";
        QString Description = "C.I.P. concrete forms, wall, job built, plywood, over 16' high, 1 use, includes erecting, bracing, stripping and cleaning";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,contactAreaForms,Description));
        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,contactAreaForms,Description));

        crewC2->addTask(CSINumber,contactAreaForms,Description,theComponent);
    }
    else
    {
        qDebug()<<"No costing information available for the wall height of "<<theWallHeight<<". Information is only available for walls up to 16.0ft in height."<<__FUNCTION__;
        return -1;
    }

    //Calculate the wood used for forming
    auto yrdWoodFrame = this->getVolYrdWoodForms(formThickness,contactAreaForms);

    repairPhase->getYrdWood() += yrdWoodFrame;

    //***Placing***

    RCrew* crewC6 = repairPhase->addCrew(RCrewCode::C6);

    {
        // Line Number: 033113704900
        // Description: Structural concrete, placing, walls, direct chute, 8" thick, includes leveling (strike off) & consolidation, excludes material
        // Unit: C.Y.
        // Crew: Crew C6

        QString CSINumber = "033113704900";
        QString Description = "Structural concrete, placing, walls, direct chute, 8in. thick, includes leveling (strike off) & consolidation, excludes material";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfPatch*convertCubicFttoYrd,Description));

        crewC6->addTask(CSINumber,volumeOfPatch*convertCubicFttoYrd,Description,theComponent);
    }

    // No finishing since using forms

    //************Material************// assume hand mix repair mortar - also ignore the extra labour on the mixer, assume the finisher mixes own concrete


    // Concrete
    {
        // Line Number: 033113250340
        // Description:  Concrete, hand mix, for small quantities or remote areas, 4000 psi, using wheelbarrow, includes bagged pre-mixed dry ingredients (80-Lb bag = 0.6 C.F.) and water, excludes, forms, reinforcing, placing & finishing
        // Unit: C.F.
        // Crew: 1 Clab
        QString CSINumber = "033113250340";
        QString Description = "Concrete, hand mix, for small quantities or remote areas, 4000 psi, using wheelbarrow, includes bagged pre-mixed dry ingredients (80-Lb bag = 0.6 C.F.) and water, excludes, forms, reinforcing, placing & finishing";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeOfPatch,Description));
    }

    // Add repair material
    repairPhase->getYrdConcrete() += volumeOfPatch*convertCubicFttoYrd;

    // Curing
    this->getConcreteCuringCSICodes(areaOfPatch,theComponent,repairPhase,labourQuantityCSICodes);

    return 0;
}


int RCSIAssembler::getConcreteRepairShotcreteCodes(double areaDamage,
                                                   double depthDamage,
                                                   RComponent* theComponent,
                                                   RJob* repairPhase,
                                                   CSIVector* labourQuantityCSICodes)
{

    //****Labour AND Material****

    RCrew* crewC10 = repairPhase->addCrew(RCrewCode::C10);
    {
        // Line Number: 033713300900
        // Description: Gunite, preparation of old walls, excludes scaffolding, good condition
        // Unit: S.F.
        // Crew: Crew C10

        QString CSINumber = "033713300900";
        QString Description = "Description: Gunite, preparation of old walls, excludes scaffolding, good condition";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,areaDamage,Description));

        crewC10->addTask(CSINumber,areaDamage,Description,theComponent);
    }

    RCrew* crewC16 = repairPhase->addCrew(RCrewCode::C16);
    {
        // Line Number:  033713300020
        // Description:  Gunite, dry mix, applied in layers, 1" thick, excludes reinforcing mesh
        // Unit: S.F.
        // Crew:  Crew C16

        QString CSINumber = "033713300020";
        QString Description = "Gunite, dry mix, applied in layers, 1in. thick, excludes reinforcing mesh";

        //in 1 inch layers
        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,areaDamage*depthDamage,Description));

        crewC16->addTask(CSINumber,areaDamage*depthDamage,Description,theComponent);
    }

    {
        // Line Number: 033713301100
        // Description: Gunite, high finish requirement or close tolerance, add
        // Unit: S.F.

        QString CSINumber = "033713301100";
        QString Description = "Gunite, high finish requirement or close tolerance, add";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,areaDamage,Description));
    }

    repairPhase->getYrdConcrete() += areaDamage*depthDamage*convertInchtoFt*convertCubicFttoYrd;

    return 0;
}


int RCSIAssembler::getSelectiveConcreteDemolitionCodes(double reinfRatio,
                                                       double volumeDemoConcrete,
                                                       RJob* repairPhase,
                                                       CSIVector* labourQuantityCSICodes,
                                                       RComponent* parentComponent)
{

    if(reinfRatio <= 0.0 || volumeDemoConcrete <= 0.0)
    {
        qDebug()<<"Warning: in "<<__FUNCTION__<<" the reinforcement ratio and/or volume of demoilition concrete is zero or less";
        return -1;
    }

    RCrew* crewB9 = repairPhase->addCrew(RCrewCode::B9);

    if(reinfRatio < 0.01)
    {
        // Line Number: 030505100050
        // Description: Selective concrete demolition, reinforcing less than 1% of cross-sectional area, break up into small pieces, excludes shoring, bracing, saw or torch cutting, loading, hauling, dumping
        // Unit: C.Y.
        // Crew: Crew B9
        QString CSINumber = "030505100050";
        QString Description = "Selective concrete demolition, reinforcing less than 1% of cross-sectional area, break up into small pieces, excludes shoring, bracing, saw or torch cutting, loading, hauling, dumping";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeDemoConcrete,Description));

        crewB9->addTask(CSINumber,volumeDemoConcrete,Description,parentComponent);
    }
    else if(reinfRatio >= 0.01 && reinfRatio <= 0.02)
    {
        // Line Number: 030505100060
        // Description: Selective concrete demolition, reinforcing 1% - 2% of cross-sectional area, break up into small pieces, excludes shoring, bracing, saw or torch cutting, loading, hauling, dumping
        // Unit: C.Y.
        // Crew: Crew B9
        QString CSINumber = "030505100060";
        QString Description = "Selective concrete demolition, reinforcing 1% - 2% of cross-sectional area, break up into small pieces, excludes shoring, bracing, saw or torch cutting, loading, hauling, dumping";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeDemoConcrete,Description));

        crewB9->addTask(CSINumber,volumeDemoConcrete,Description,parentComponent);

    }
    else // reinfRatio > 0.02
    {
        // Line Number: 030505100070
        // Description: Selective concrete demolition, reinforcing more than 2% cross-sectional area, break up into small pieces, excludes shoring, bracing, saw or torch cutting, loading, hauling, dumping
        // Unit: C.Y.
        // Crew: Crew B9

        QString CSINumber = "030505100070";
        QString Description = "Selective concrete demolition, reinforcing more than 2% cross-sectional area, break up into small pieces, excludes shoring, bracing, saw or torch cutting, loading, hauling, dumping";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeDemoConcrete,Description));

        crewB9->addTask(CSINumber,volumeDemoConcrete,Description,parentComponent);
    }


    return 0;
}


int RCSIAssembler::getSelectiveSteelDemolitionCodes(double massSteel,
                                                    RJob* repairPhase,
                                                    CSIVector* labourQuantityCSICodes,
                                                    RComponent* theComponent)

{

    if (massSteel <= 0.25)
    {
        // Line Number:  050505100230
        // Description:  Structural members, remove whole or cut up, up to 500 lb.
        // Unit: Ea.
        // Crew:  Crew E19

        RCrew* crewE19 = repairPhase->addCrew(RCrewCode::E19);

        QString CSINumber = "050505100230";
        QString Description = "Structural members, remove whole or cut up, up to 500 lb";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));

        crewE19->addTask(CSINumber,1.0,Description,theComponent);
    }
    else if (massSteel > 0.25 && massSteel <= 2.0)
    {
        // Line Number:  050505100240
        // Description:  Structural members, remove whole or cut up, up to 1/4-2 tons.
        // Unit: Ea.
        // Crew:  Crew E18

        RCrew* crewE18 = repairPhase->addCrew(RCrewCode::E18);

        QString CSINumber = "050505100240";
        QString Description = "Structural members, remove whole or cut up, up to 1/4-2 tons";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));

        crewE18->addTask(CSINumber,1.0,Description,theComponent);
    }
    else if (massSteel > 2.0 && massSteel <= 5.0)
    {
        // Line Number:  050505100250
        // Description:  Structural members, remove whole or cut up, up to 2-5 tons.
        // Unit: Ea.
        // Crew:  Crew E24

        RCrew* crewE24 = repairPhase->addCrew(RCrewCode::E24);

        QString CSINumber = "050505100250";
        QString Description = "Structural members, remove whole or cut up, up to 2-5 tons";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));

        crewE24->addTask(CSINumber,1.0,Description,theComponent);
    }
    else if (massSteel > 5.0 && massSteel <= 10.0)
    {
        // Line Number:  050505100260
        // Description:  Structural members, remove whole or cut up, up to 5-10 tons.
        // Unit: Ea.
        // Crew:  Crew E20

        RCrew* crewE20 = repairPhase->addCrew(RCrewCode::E20);

        QString CSINumber = "050505100260";
        QString Description = "Structural members, remove whole or cut up, up to 5-10 tons";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));

        crewE20->addTask(CSINumber,1.0,Description,theComponent);
    }
    else if (massSteel > 10.0 && massSteel <= 15.0)
    {
        // Line Number:  050505100270
        // Description:  Structural members, remove whole or cut up, up to 10-15 tons.
        // Unit: Ea.
        // Crew:  Crew E2

        RCrew* crewE2 = repairPhase->addCrew(RCrewCode::E2);

        QString CSINumber = "050505100270";
        QString Description = "Structural members, remove whole or cut up, up to 10-15 tons";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));

        crewE2->addTask(CSINumber,1.0,Description,theComponent);
    }
    else
    {
        qDebug()<<"No costing information available for this amount of steel "<<massSteel<<" max 15 tons in "<<__FUNCTION__;
    }


    return 0;
}


int RCSIAssembler::getSteelDeckingDemolitionCodes(double areaDecking,
                                                  RJob* repairPhase,
                                                  CSIVector* labourQuantityCSICodes,
                                                  RComponent* theComponent)
{
    // Line Number:  050505100500
    // Description:  Steel roof decking, uncovered, bare
    // Unit: S.F.
    // Crew:  Crew B2

    RCrew* crewB2 = repairPhase->addCrew(RCrewCode::B2);

    QString CSINumber = "050505100500";
    QString Description = "Steel roof decking, uncovered, bare";

    labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,areaDecking,Description));

    crewB2->addTask(CSINumber,areaDecking,Description,theComponent);

    return 0;
}


int RCSIAssembler::getConcreteSlabCutOutDemolitionCodes(double reinfRatio,
                                                        double volumeDemoConcrete,
                                                        RJob* repairPhase,
                                                        CSIVector* labourQuantityCSICodes,
                                                        RComponent* parentComponent)
{

    RCrew* crewB9 = repairPhase->addCrew(RCrewCode::B9);

    // For slabs with bar reinforcing
    if(volumeDemoConcrete < 6.0)
    {
        // Line Number: 024119161000
        // Description: Selective demolition, cutout, concrete, elevated slab, bar reinforced, under 6 C.F., excludes loading and disposal
        // Unit: C.F.
        // Crew: Crew B9

        QString CSINumber = "024119161000";
        QString Description = "Selective demolition, cutout, concrete, elevated slab, bar reinforced, under 6 C.F., excludes loading and disposal";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeDemoConcrete,Description));

        crewB9->addTask(CSINumber,volumeDemoConcrete,Description,parentComponent);
    }
    else
    {
        // Line Number: 024119161050
        // Description: Selective demolition, cutout, concrete, elevated slab, bar reinforced, over 6 C.F., excludes loading and disposal
        // Unit: C.F.
        // Crew:  Crew B9

        QString CSINumber = "024119161050";
        QString Description = "Selective demolition, cutout, concrete, elevated slab, bar reinforced, over 6 C.F., excludes loading and disposal";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeDemoConcrete,Description));

        crewB9->addTask(CSINumber,volumeDemoConcrete,Description,parentComponent);
    }

    return 0;
}


int RCSIAssembler::getConcreteSlabCutOutWireMeshDemolitionCodes(double volumeDemoConcrete,
                                                                RJob* repairPhase,
                                                                CSIVector* labourQuantityCSICodes,
                                                                RComponent* parentComponent)
{

    RCrew* crewB9 = repairPhase->addCrew(RCrewCode::B9);

    // For slabs with light reinforcing i.e., wire mesh
    if(volumeDemoConcrete < 6.0)
    {
        // Line Number: 024119160020
        // Description: Selective demolition, cutout, concrete elevated slab, light reinforcing, under 6 C.F., excludes loading and disposal
        // Unit: C.F.
        // Crew: Crew B9

        QString CSINumber = "024119160020";
        QString Description = "Selective demolition, cutout, concrete elevated slab, light reinforcing, under 6 C.F., excludes loading and disposal";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeDemoConcrete,Description));

        crewB9->addTask(CSINumber,volumeDemoConcrete,Description,parentComponent);
    }
    else
    {
        // Line Number: 024119160050
        // Description: Selective demolition, cutout, concrete elevated slab, light reinforcing, over 6 C.F., excludes loading and disposal
        // Unit: C.F.
        // Crew: Crew B9

        QString CSINumber = "024119160050";
        QString Description = "Selective demolition, cutout, concrete elevated slab, light reinforcing, over 6 C.F., excludes loading and disposal";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeDemoConcrete,Description));

        crewB9->addTask(CSINumber,volumeDemoConcrete,Description,parentComponent);
    }

    return 0;
}


int RCSIAssembler::getConcreteWallCutOutDemolitionCodes(double volumeDemoConcrete,
                                                        double wallLength,
                                                        RJob* repairPhase,
                                                        CSIVector* labourQuantityCSICodes,
                                                        RComponent* parentComponent)
{


    if(volumeDemoConcrete <= 0.0 || volumeDemoConcrete > 12.0)
    {
        qDebug()<<"The cut-out volume for the wall of "<<volumeDemoConcrete<<", should be less than 12.0 and greater than 0.0 in "<<__FUNCTION__;
        return -1;
    }

    RCrew* crewB9 = repairPhase->addCrew(RCrewCode::B9);

    // For walls with bar reinforcing
    if(volumeDemoConcrete < 6.0)
    {
        // Line Number: 024119161400
        // Description: Selective demolition, cutout, concrete, walls, bar reinforced, under 6 C.F., excludes loading and disposal
        // Unit: C.F.
        // Crew: Crew B9

        QString CSINumber = "024119161400";
        QString Description = "Selective demolition, cutout, concrete, walls, bar reinforced, under 6 C.F., excludes loading and disposal";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeDemoConcrete,Description));

        crewB9->addTask(CSINumber,volumeDemoConcrete,Description,parentComponent);
    }
    else if(volumeDemoConcrete >= 6.0 && volumeDemoConcrete <= 12.0 )
    {
        // Line Number: 024119161450
        // Description: Selective demolition, cutout, concrete, walls, bar reinforced, 6-12 C.F., excludes loading and disposal
        // Unit: C.F.
        // Crew: Crew B9

        QString CSINumber = "024119161450";
        QString Description = "Selective demolition, cutout, concrete, walls, bar reinforced, 6-12 C.F., excludes loading and disposal";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,volumeDemoConcrete,Description));

        crewB9->addTask(CSINumber,volumeDemoConcrete,Description,parentComponent);
    }

    RCrew* crewCLAB = repairPhase->addCrew(RCrewCode::CLAB1);
    {
        // Line Number: 024119271040
        // Description: Selective demolition, torch cutting, steel, reinforced concrete walls, 12"-16" thick, oxygen lance cutting
        // Unit: L.F.
        // Crew: 1 Clab

        QString CSINumber = "024119271040";
        QString Description = "Selective demolition, torch cutting, steel, reinforced concrete walls, 12in.-16in. thick, oxygen lance cutting";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,wallLength,Description));

        crewCLAB->addTask(CSINumber,volumeDemoConcrete,Description,parentComponent);
    }

    return 0;

}


int RCSIAssembler::getShoringCSICodes(const double& numberOfFrames, CSIVector* materialQuantityCSICodes)
{
    // The shoring system consist of several two-legged frames
    // Each frame will have a cross-brace
    // Also, each frame will have two each of screw jacks, base plates, and U-heads (one per leg)

    // Frame shoring system, 12,000# per leg, steel, buy

    {
        // Line Number: 031505705040
        // Description: Frame shoring system, frame, 12000# per leg, 2' wide x 6' high, steel, buy
        // Unit: Ea.
        // Crew: NA
        QString CSINumber = "031505705040";
        QString Description = "Frame shoring system, frame, 12000# per leg, 2' wide x 6' high, steel, buy";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,numberOfFrames,Description));
    }


    {
        // Line Number: 031505705250
        // Description: Frame shoring system, X-brace, 12000# per leg, steel, buy
        // Unit: Ea.
        // Crew: NA
        QString CSINumber = "031505705250";
        QString Description = "Frame shoring system, X-brace, 12000# per leg, steel, buy";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,numberOfFrames*2.0,Description));
    }

    {
        // Line Number: 031505705600
        // Description:
        // Unit: Ea.
        // Crew: NA
        QString CSINumber = "031505705600";
        QString Description = "Frame shoring system, screw jack, 12000# per leg, steel, buy";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,numberOfFrames*2.0,Description));
    }

    {
        // Line Number: 031505705550
        // Description: Frame shoring system, base plate, 12000# per leg, steel, buy
        // Unit: Ea.
        // Crew: NA
        QString CSINumber = "031505705550";
        QString Description = "Frame shoring system, base plate, 12000# per leg, steel, buy";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,numberOfFrames*2.0,Description));
    }

    {
        // Line Number: 031505705650
        // Description: Frame shoring system, U-head, 12000# per leg, 8" x 8", steel, buy
        // Unit: Ea.
        // Crew: NA
        QString CSINumber = "031505705650";
        QString Description = "Frame shoring system, U-head, 12000# per leg, 8in. x 8in., steel, buy";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,numberOfFrames*2.0,Description));
    }


    return 0;
}


int RCSIAssembler::getReinforcementDowelCodes(double numberDowels,
                                              RComponent* theComponent,
                                              RJob* repairPhase,
                                              CSIVector* labourQuantityCSICodes)
{

    RCrew* crewSkwk = repairPhase->addCrew(RCrewCode::Skwk2);

    {
        // Line Number: 036305101535
        // Description: Chemical anchoring, for fastener 1" diam x 8" embedment, incl epoxy cartridge, excl layout, drilling & fastener
        // Units: Ea.
        // Crew: 2 Skwk (skwk == skilled worker)
        QString CSINumber = "036305101535";
        QString Description = "Chemical anchoring, for fastener 1in. diam x 8in. embedment, incl epoxy cartridge, excl layout, drilling & fastener";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,numberDowels,Description));

        crewSkwk->addTask(CSINumber,numberDowels,Description,theComponent);

    }

    RCrew* crewCarp1 = repairPhase->addCrew(RCrewCode::Carp1);

    {
        // Line Number:   038216100700
        // Description:  Concrete impact drilling, for anchors, up to 4" D, 1" dia, in concrete or brick walls and floors, includes bit cost, layout and set up time, excl anchor
        // Units: Ea.
        // Crew: 1 Carp
        QString CSINumber = "038216100700";
        QString Description = "Concrete impact drilling, for anchors, up to 4in. D, 1in dia, in concrete or brick walls and floors, includes bit cost, layout and set up time, excl anchor";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,numberDowels,Description));

        crewCarp1->addTask(CSINumber,numberDowels,Description,theComponent);
    }

    return 0;
}


void RCSIAssembler::getConcreteCSICode(QString theConcreteType, double yardsOfConcrete,QVector<std::tuple<QString,double,QString>>* materialQuantityCSICodes)
{
    // 2000 psi
    if (theConcreteType == "C15") {

        // Line Number:	033113350020
        // Description:	Structural concrete, ready mix, heavyweight, 2000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments
        // Unit: C.Y.

        QString CSINumber = "033113350020";
        QString Description = "Structural concrete, ready mix, heavyweight, 2000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,yardsOfConcrete,Description));
    }
    // 2500 psi
    else if (theConcreteType == "C18") {

        // Line Number:	033113350100
        // Description:	Structural concrete, ready mix, heavyweight, 2500 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments
        // Unit: C.Y.

        QString CSINumber = "033113350100";
        QString Description = "Structural concrete, ready mix, heavyweight, 2500 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,yardsOfConcrete,Description));

    }
    // 3000 psi
    else if (theConcreteType == "C20") {

        // Line Number:	033113350150
        // Description:	Structural concrete, ready mix, heavyweight, 3000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments
        // Unit: C.Y.

        QString CSINumber = "033113350150";
        QString Description = "Structural concrete, ready mix, heavyweight, 3000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,yardsOfConcrete,Description));

    }
    // 3500 psi
    else if (theConcreteType == "C25") {

        // Line Number:	033113350200
        // Description:	Structural concrete, ready mix, heavyweight, 3500 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments
        // Unit: C.Y.

        QString CSINumber = "033113350200";
        QString Description = "Structural concrete, ready mix, heavyweight, 3500 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,yardsOfConcrete,Description));

    }
    // 4000 psi
    else if (theConcreteType == "C30") {

        // Line Number:	033113350300
        // Description:	Structural concrete, ready mix, heavyweight, 4000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments
        // Unit: C.Y.

        QString CSINumber = "033113350300";
        QString Description = "Structural concrete, ready mix, heavyweight, 4000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,yardsOfConcrete,Description));

    }
    // 5000 psi
    else if (theConcreteType == "C35") {

        // Line Number:	033113350400
        // Description:	Structural concrete, ready mix, heavyweight, 5000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments
        // Unit: C.Y.

        QString CSINumber = "033113350400";
        QString Description = "Structural concrete, ready mix, heavyweight, 5000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,yardsOfConcrete,Description));

    }
    // 6000 psi
    else if (theConcreteType == "C40") {

        // Line Number:	033113350411
        // Description:	Structural concrete, ready mix, heavyweight, 6000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments
        // Unit: C.Y.

        QString CSINumber = "033113350411";
        QString Description = "Structural concrete, ready mix, heavyweight, 6000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,yardsOfConcrete,Description));


    }
    // 8000 psi
    else if (theConcreteType == "C55") {

        // Line Number:	033113350412
        // Description:	Structural concrete, ready mix, heavyweight, 8000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments
        // Unit: C.Y.

        QString CSINumber = "033113350412";
        QString Description = "Structural concrete, ready mix, heavyweight, 8000 psi, includes local aggregate, sand, Portland cement (Type I) and water, delivered, excludes all additives and treatments";

        materialQuantityCSICodes->push_back(std::make_tuple(CSINumber,yardsOfConcrete,Description));

    }
    else
    {
        qDebug()<<"The type of concrete in could not be determined in  "<<__FUNCTION__;
    }

}


void RCSIAssembler::getSteelFramingCSICodes(QString theSteelShape, double linearFeet, RComponent* theComponent, RJob* phase, CSIVector* CSICodes)
{
    QString CSINumber = "NULL";

    if (theSteelShape == "W8X31"  )       {CSINumber ="051223176850";}
    else if (theSteelShape == "W8X48"  )  {CSINumber ="051223176900";}
    else if (theSteelShape == "W8X67"  )  {CSINumber ="051223176950";}
    else if (theSteelShape == "W10X45" )  {CSINumber ="051223177000";}
    else if (theSteelShape == "W10X68" )  {CSINumber ="051223177050";}
    else if (theSteelShape == "W10X112")  {CSINumber ="051223177100";}
    else if (theSteelShape == "W12X50" )  {CSINumber ="051223177150";}
    else if (theSteelShape == "W12X87" )  {CSINumber ="051223177200";}
    else if (theSteelShape == "W12X120")  {CSINumber ="051223177250";}
    else if (theSteelShape == "W12X190")  {CSINumber ="051223177300";}
    else if (theSteelShape == "W14X74" )  {CSINumber ="051223177350";}
    else if (theSteelShape == "W14X120")  {CSINumber ="051223177400";}
    else if (theSteelShape == "W14X176")  {CSINumber ="051223177450";}
    else {
        qDebug()<<"Error getting the CSI code for shape "<<theSteelShape<<" in "<<__FUNCTION__;
    }

    // Unit: L.F.
    QString Description = theSteelShape;

    CSICodes->push_back(std::make_tuple(CSINumber,linearFeet,Description));

    if(phase)
    {
        RCrew* steelCrew = phase->addCrew(RCrewCode::E2);

        steelCrew->addTask(CSINumber,linearFeet,Description,theComponent);
    }

}


void RCSIAssembler::getConcreteCuringCSICodes(const double& formContactArea, RComponent* theComponent, RJob* phase, CSIVector* labourQuantityCSICodes)
{
    // Crew used by all concrete components
    RCrew* crewCLAB2 = phase->addCrew(RCrewCode::CLAB2);

    // Curing per square foot - Figure 3-37 Installation Time in Labor-Hours for Slab-on-Grade  (RSMeans Costing Div. 3.,p.g. 84)
    // CSI CODE Water Curing
    // 2 Laborers (CLAB)
    {
        // Line Number: 033913500100
        // Description:	Curing, burlap, 10 oz., 4 uses assumed
        // Unit: C.S.F. (hundred square feet)
        // Crew: 2 Clab
        QString CSINumber = "033913500100";
        QString Description = "Curing, burlap, 10 oz., 4 uses assumed";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,formContactArea/100.0,Description));

        crewCLAB2->addTask(CSINumber,formContactArea,Description,theComponent);
    }
}


int RCSIAssembler::getAdditionalLabourCosts(RJob* jobPhase, RBIM* theBIM, CSIVector* labourQuantityCSICodes)
{

    auto totalTonsReinforcementNo3toNo7 = jobPhase->getTonsReinfNo3toNo7();

    auto totalTonsReinforcementNo8toNo18 = jobPhase->getTonsReinfNo8toNo18();

    // To do
    /*
    //Extra costs to take into account the costs associated with smaller or larger amounts of reinforcement (economy of scale)
    //CSI codes for the added cost of reinforcment labour for smaller or larger amounts of reinforcement than the expected 50-60 tons that the costing above is based on
    //For reinforcement #3 to #7
    if(totalTonsReinforcementNo3toNo7 > 0.0 && totalTonsReinforcementNo3toNo7 <= 10.0)
    {
        // Line Number:032111601000
        // Description: Reinforcing steel, in place, under 10 ton job, #3 to #7, add
        // Unit: Each
        QString CSINumber = "032111601000";
        QString Description = "Reinforcing steel, in place, under 10 ton job, #3 to #7, add";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));
    }
    else if(totalTonsReinforcementNo3toNo7 > 10.0 && totalTonsReinforcementNo3toNo7 <= 50.0)
    {
        // Line Number: 032111601050
        // Description: Reinforcing steel, in place, 10 to 50 ton job, #3 to #7, add
        // Unit: Each
        QString CSINumber = "032111601050";
        QString Description = "Reinforcing steel, in place, 10 to 50 ton job, #3 to #7, add";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));
    }
    else if(totalTonsReinforcementNo3toNo7 > 60.0 && totalTonsReinforcementNo3toNo7 <= 100.0)
    {
        // Line Number:032111601100
        // Description:Reinforcing steel, in place, 60 to 100 ton job, #3 to #7, deduct
        // Unit: Each
        QString CSINumber = "032111601100";
        QString Description = "Reinforcing steel, in place, 60 to 100 ton job, #3 to #7, deduct";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));
    }
    else if(totalTonsReinforcementNo3toNo7 > 100.0 )//Over 100 tons
    {
        // Line Number:032111601150
        // Description:Reinforcing steel, in place, over 100 ton job, #3 to #7, deduct
        // Unit: Each
        QString CSINumber = "032111601150";
        QString Description = "Reinforcing steel, in place, over 100 ton job, #3 to #7, deduct";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));
    }

    //For reinforcement #8 to #18
    if(totalTonsReinforcementNo8toNo18 > 0.0 && totalTonsReinforcementNo8toNo18 <= 10.0)
    {
        // Line Number:032111601010
        // Description:Reinforcing steel, in place, under 10 ton job, #8 to #18, add
        // Unit: Each
        QString CSINumber = "032111601010";
        QString Description = "Reinforcing steel, in place, under 10 ton job, #8 to #18, add";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));
    }
    else if(totalTonsReinforcementNo8toNo18 > 10.0 && totalTonsReinforcementNo8toNo18 <= 50.0)
    {
        // Line Number:032111601060
        // Description: Reinforcing steel, in place, 10 to 50 ton job, #8 to #18, add
        // Unit: Each
        QString CSINumber = "032111601060";
        QString Description = "Reinforcing steel, in place, 10 to 50 ton job, #8 to #18, add";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));
    }
    else if(totalTonsReinforcementNo8toNo18 > 60.0 && totalTonsReinforcementNo8toNo18 <= 100.0)
    {
        // Line Number:032111601110
        // Description:Reinforcing steel, in place, 60 to 100 ton job, #8 to #18, deduct
        // Unit: Each
        QString CSINumber = "032111601110";
        QString Description = "Reinforcing steel, in place, 60 to 100 ton job, #8 to #18, deduct";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));
    }
    else if(totalTonsReinforcementNo8toNo18 > 100.0 )//Over 100 tons
    {
        // Line Number:032111601160
        // Description:Reinforcing steel, in place, over 100 ton job, #8 to #18, deduct
        // Unit: Each
        QString CSINumber = "032111601160";
        QString Description = "Reinforcing steel, in place, over 100 ton job, #8 to #18, deduct";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,1.0,Description));
    }
    */


    // Add labour for unloading and sorting for all reinforcement
    RCrew* crewC5 = jobPhase->addCrew(RCrewCode::C5);
    // Crew C5
    // 1 Rodman Foreman (outside) 4 Rodmen (reinf.)
    // 1 Equip. Oper. (crane)
    // 1 Equip. Oper. (oiler)
    // 1 Hyd. Crane, 25 Ton
    if(totalTonsReinforcementNo3toNo7+totalTonsReinforcementNo8toNo18 > 0.0)
    {
        // Line Number:032111602000
        // Description: Reinforcing steel, unload and sort, add to base
        // Unit: Ton
        //Crew: C5

        QString CSINumber = "032111602000";
        QString Description = "Reinforcing steel, unload and sort, add to base";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,totalTonsReinforcementNo3toNo7+totalTonsReinforcementNo8toNo18,Description));

        crewC5->addTask(CSINumber,totalTonsReinforcementNo3toNo7+totalTonsReinforcementNo8toNo18,Description,nullptr);
    }

    auto numBuildingStoreys = theBIM->getNumberOfStoreys();

    // Labour for hoisting of reinforcement
    // Hoisting reinforcement - number of labour hours per ton of reinforcement
    if(numBuildingStoreys>1)
    {
        RCrew* crewA3N = jobPhase->addCrew(RCrewCode::A3N);

        double labourMonthsHoisting = (totalTonsReinforcementNo3toNo7+totalTonsReinforcementNo8toNo18)/730.0;

        //CSI CODE for the labour for column reinforcement install
        if(labourMonthsHoisting > 0.0)
        {
            // Line Number:	015419600100
            // Description:	Crane crew, tower crane, static, 130' high, 106' jib, 6200 lb. capacity, monthly use, excludes concrete footing
            // Unit: Month
            //Crew: A3N

            QString CSINumber = "015419600100";
            QString Description = "Crane crew, tower crane, static, 130' high, 106' jib, 6200 lb. capacity, monthly use, excludes concrete footing";

            labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,labourMonthsHoisting,Description));

            crewA3N->addTask(CSINumber,labourMonthsHoisting,Description,nullptr);
        }
    }

    auto YrdConcrete = jobPhase->getYrdConcrete();

    // Added cost for concrete placing highrise
    if(numBuildingStoreys > 5)
    {
        // Line Number:	033113703510
        // Description:	Structural concrete, placing, high rise, with crane and bucket, more than 5 stories, includes leveling (strike off) & consolidation, excludes material, add per story
        // Unit: C.Y.
        QString CSINumber = "033113703510";
        QString Description = "Structural concrete, placing, high rise, with crane and bucket, more than 5 stories, includes leveling (strike off) & consolidation, excludes material, add per story";

        labourQuantityCSICodes->push_back(std::make_tuple(CSINumber,YrdConcrete,Description));
    }


    return 0;
}


double RCSIAssembler::getVolYrdWoodForms(const double& thickness,const double& area)
{
    auto thickFt = thickness*convertInchtoFt;

    auto volSqFt = thickFt*area;

    return volSqFt*convertCubicFttoYrd;
}



