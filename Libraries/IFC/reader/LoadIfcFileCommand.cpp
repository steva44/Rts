/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <IfcPPBasicTypes.h>
#include <IfcPPObject.h>
#include <IfcPPException.h>
#include <reader/IfcPPReaderSTEP.h>
#include "IfcPlusPlusSystem.h"
#include "LoadIfcFileCommand.h"

#include <QDebug>

LoadIfcFileCommand::LoadIfcFileCommand(IfcPlusPlusSystem* system)
    : m_system(system)
{

}

LoadIfcFileCommand::~LoadIfcFileCommand()
{

}


void LoadIfcFileCommand::setFilePath( std::wstring& path_in )
{
	m_file_path = path_in;
}


bool LoadIfcFileCommand::doCmd()
{
	if( m_file_path.length() == 0 )
	{
		return false;
	}

	// reset the IFC model
	shared_ptr<GeometryConverter> geometry_converter = m_system->getGeometryConverter();
	geometry_converter->resetModel();
	std::stringstream err;
	
	try
	{
		// load file to IFC model
		m_system->getIfcPPReader()->loadModelFromFile( m_file_path, geometry_converter->getIfcPPModel() );
		
        // convert IFC geometry to Carve
		geometry_converter->convertGeometry();

	}
    catch(IfcPPOutOfMemoryException& e)
	{
        qDebug()<<e.what();
	}
    catch(IfcPPException& e)
	{
        qDebug()<< e.what();
	}
    catch(std::exception& e)
	{
        qDebug()<<e.what();
	}
    catch(...)
	{
        qDebug()<<"loadModelFromFile, createGeometryOSG failed";
	}

	if( err.tellp() > 0 )
	{
        qDebug()<<err.str().c_str();
	}

	return true;
}

