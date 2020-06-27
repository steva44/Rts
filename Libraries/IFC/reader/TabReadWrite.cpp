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

#pragma warning( disable: 4996 )
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QFile>

#include <model/IfcPPBasicTypes.h>
#include <model/IfcPPModel.h>
#include <model/IfcPPException.h>
#include <model/IfcPPGuid.h>

#include "LoadIfcFileCommand.h"
#include "IfcPlusPlusSystem.h"
#include "TabReadWrite.h"

TabReadWrite::TabReadWrite( IfcPlusPlusSystem* sys) : m_system(sys)
{

}


TabReadWrite::~TabReadWrite()
{

}


void TabReadWrite::slotLoadIfcFile( QString& path_in )
{

    qDebug()<<"Loading file: "<<path_in;
    QApplication::processEvents();

    clock_t millisecs = clock();

    if( !QFile::exists(path_in) )
    {
        qDebug()<<"file "<<path_in<<" does not exist\n";
        return;
    }

    try
    {
        shared_ptr<LoadIfcFileCommand> cmd_load(new LoadIfcFileCommand(m_system));
        std::wstring path_str = path_in.toStdWString();
        cmd_load->setFilePath(path_str);
        cmd_load->doCmd();
    }
    catch(IfcPPOutOfMemoryException& e)
    {
        qDebug()<<e.what();
    }
    catch(IfcPPException& e)
    {
        qDebug()<<e.what();
    }
    catch(std::exception& e)
    {
        qDebug()<<e.what() ;
    }

    clock_t time_diff = clock() - millisecs;
    int num_entities = m_system->getIFCModel()->getMapIfcEntities().size();

    qDebug()<<"IFC file loaded. Created a total of "<<QString::number(num_entities)<<" entities in "<<QString::number(round(time_diff*0.1)*1e-6 )<<" sec.";

}


