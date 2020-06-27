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
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include "RRampLoadModel.h"
#include "RScaledGroundMotionModel.h"
#include "RSineLoadModel.h"
#include "RSyntheticGroundMotionModel.h"
#include "RIntensityModel.h"
#include "RAtkinson1997IntensityModel.h"
#include "RAtkinsonBoore2003IntensityModel.h"
#include "RBooreAtkinson2008IntensityModel.h"
#include "RGenericAttenuationModel.h"
#include "RJoynerBoore1981IntensityModel.h"
#include "RBoundedExponentialMagnitudeModel.h"
#include "RMomentMagnitudeModel.h"
#include "RUserDefinedMagnitudeModel.h"
#include "RGivenMagnitudeModel.h"
#include "RCircularAreaSourceModel.h"
#include "RMultipointLineSourceModel.h"
#include "RPolygonalAreaSourceModel.h"
#include "RQuadrilateralAreaSourceModel.h"
#include "RRectangularAreaSourceModel.h"
#include "RSingleLineSourceModel.h"
#include "RFixedSourceModel.h"
#include "RPoissonPointProcessOccurrenceModel.h"
#include "RUniformOccurrenceModel.h"
#include "RLognormalOccurrenceModel.h"
#include "RFixedTimeOccurrenceModel.h"
#include "RPGDIntensityModel.h"
#include "RLoadPatternModel.h"
#include "RNodalLoad.h"
#include "RSimpleSnowLoadModel.h"
#include "RSimpleWindPressureModel.h"

