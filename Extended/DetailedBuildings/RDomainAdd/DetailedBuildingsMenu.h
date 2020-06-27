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

//Add the components item in the menu...
if (className == QString("Objects").remove(' ')) {
    result << "COMPONENTS";
}



if (className == QString("MODELS").remove(' ')) {
    result << "Detailed Buildings"
           << "Structural Analysis Models"
           << "Visual Damage"
           << "Repair Impact"
           << "Building Pushover"
           << "Displacement Controlled Pushover"
           << "Time History Scaling Plot"
           << "Time History Evolution Plot";
}


if (className == QString("COMPONENTS").remove(' ')) {
    result << "Points"
           << "Nodes"
           << "Circular RC Columns"
           << "RC Slabs"
           << "Rectangular Timber Beam Columns"
           << "CLT Panels"
           << "Steel Beam Columns"
           << "Corrugated Roofs"
           << "Non Load Bearing Walls"
           << "Fixed Planes";
}
