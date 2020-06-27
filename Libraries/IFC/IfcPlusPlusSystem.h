#ifndef IfcPlusPlusSystem_H
#define IfcPlusPlusSystem_H
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
************Based on code from: - IFC++ www.ifcquery.com *************
*                                                                    *
* MIT License                                                        *
*                                                                    *
* Copyright (c) 2017 Fabian Gerold                                   *
*                                                                    *
* Permission is hereby granted, free of charge, to any person        *
* obtaining a copy of this software and associated documentation     *
* files (the "Software"), to deal                                    *
* in the Software without restriction, including without limitation  *
* the rights to use, copy, modify, merge, publish, distribute,       *
* sublicense, and/or sell copies of the Software, and to permit      *
* persons to whom the Software is furnished to do so, subject to     *
* the following conditions:                                          *
*                                                                    *
* The above copyright notice and this permission notice shall be     *
* included in all copies or substantial portions of the Software.    *
*                                                                    *
*                                                                    *
* Contributors to this file:                                         *
*   - Stevan Gavrilovic                                              *
*                                                                    *
*********************************************************************/

#include <geometry/Carve/GeometryConverter.h>
#include <geometry/GeometrySettings.h>

#include <QObject>
#include "IfcPPBasicTypes.h"

class IfcPPModel;
class IfcPPEntity;
class IfcPPReaderSTEP;

class IfcPlusPlusSystem : public QObject
{
    Q_OBJECT

public:
    IfcPlusPlusSystem();
    ~IfcPlusPlusSystem();

    inline shared_ptr<GeometryConverter>	getGeometryConverter()	{ return m_geometry_converter; }
    inline shared_ptr<GeometrySettings>&    getGeomSettings ()      { return m_geometry_converter->getGeomSettings();}
    inline shared_ptr<IfcPPReaderSTEP>&     getIfcPPReader()		{ return m_step_reader; }
    inline shared_ptr<IfcPPModel>&          getIFCModel()           { return m_ifc_model; }

private:

    shared_ptr<GeometryConverter>		m_geometry_converter;
    shared_ptr<IfcPPReaderSTEP>			m_step_reader;
    shared_ptr<IfcPPModel>				m_ifc_model;

};
#endif
