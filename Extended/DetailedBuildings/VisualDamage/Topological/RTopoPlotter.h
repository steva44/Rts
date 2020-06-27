#ifndef RTOPOPLOTTER_H
#define RTOPOPLOTTER_H
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

// This class controls the plotting of the visual damage on the topological mesh
// It uses the shader pipeline and graphics shaders

#include <osgDB/ReadFile>
#include <osg/Geode>

#include <QTemporaryDir>

class RTopoMesh;

class RTopoPlotter
{
public:
    RTopoPlotter();
     ~RTopoPlotter();

    // Draw the topological mesh object
    osg::ref_ptr<osg::Geode> drawTopo(RTopoMesh* topoMesh, const double scale);

    // Vertices from the topological mesh
    osg::ref_ptr<osg::Vec3Array> getTopoVertices( const RTopoMesh* topoMesh,const double scale);

private:

    // Tesselation level on the tesselation control shader, the higher this number the "smoother" the shape will look, but will require more computational effort to display
    osg::ref_ptr<osg::Uniform> tessInnerU;
    osg::ref_ptr<osg::Uniform> tessOuterU;

    // Create the topological geometry
    osg::ref_ptr<osg::Geometry> createTopoGeometry(osg::Vec3Array* vertices);

    // Options for the reader/writer for importing shader code
    osg::ref_ptr<osgDB::ReaderWriter::Options> opt;

    // Shader program that creates the shader "pipeline" within OpenGL
    osg::ref_ptr<osg::Program> program;
    osg::ref_ptr<osg::Program> createProgram(void);

    // State set functions - a state set is a set of attributes for displaying an OpenGL object
    osg::ref_ptr<osg::StateSet> concreteStateSet;
    osg::ref_ptr<osg::StateSet> getStateSet(RTopoMesh* topoMesh);
    void createConcreteStateSets(void);
    void setStateParameters(osg::StateSet* state);

    // Imports the concrete damage textures, i.e., cracking, spalling, cover loss, etc.
    int importTextures(void);
    osg::ref_ptr<osg::Image> concreteBaseTexture;
    osg::ref_ptr<osg::Image> concreteCrackedTexture;
    osg::ref_ptr<osg::Image> concreteSpalledTexture;
    osg::ref_ptr<osg::Image> concreteCoverLossTexture;
    osg::ref_ptr<osg::Image> concreteCrushedTexture;
    osg::ref_ptr<osg::Image> concreteRebarFracturedTexture;

    QTemporaryDir tempDir;
};

#endif // RTOPOPLOTTER_H
