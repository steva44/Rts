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

#include "RTopoPlotter.h"
#include "RTopoMesh.h"
#include "RTopoFace.h"
#include "RTopoUtils.h"
#include "RNode.h"
#include "RDamageRegion.h"

#include <osg/Material>
#include <osg/BlendFunc>
#include <osg/Notify>
#include <osg/ref_ptr>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Program>
#include <osg/Shader>
#include <osg/Uniform>
#include <osg/Drawable>
#include <osg/Group>
#include <osg/Node>
#include <osg/Object>
#include <osg/PrimitiveSet>
#include <osg/StateSet>
#include <osg/Transform>
#include <osg/PatchParameter>
#include <osgDB/ReadFile>
#include <osg/Texture2D>

#include <QFile>

RTopoPlotter::RTopoPlotter()
{
    program = this->createProgram();

    concreteBaseTexture = nullptr;
    concreteCrackedTexture = nullptr;
    concreteSpalledTexture = nullptr;
    concreteCoverLossTexture = nullptr;
    concreteStateSet = nullptr;

    this->importTextures();

    tessInnerU = new osg::Uniform("TessLevelInner", 3.0f);
    tessOuterU = new osg::Uniform("TessLevelOuter", 3.0f);

    // Cache the texture images
    opt = new osgDB::ReaderWriter::Options;
    opt->setObjectCacheHint( osgDB::ReaderWriter::Options::CACHE_IMAGES );
}


RTopoPlotter::~RTopoPlotter()
{

}


osg::ref_ptr<osg::StateSet> RTopoPlotter::getStateSet(RTopoMesh* topoMesh)
{
    osg::StateSet* osgState = nullptr;

    auto texType = topoMesh->getTexture();

    // For now only concrete textures are supported
    switch(texType)
    {
    case RTopoMesh::CONCRETE:
        if(concreteStateSet == nullptr)
            this->createConcreteStateSets();
        osgState = concreteStateSet;
        break;
        // case RTopoMesh::CONCRETECRACKED:
        //     textureImage = concreteCrackedTexture;
        //     break;
        // case RTopoMesh::CONCRETESPALLED:
        //     textureImage = concreteSpalledTexture;
        //     break;
        // case RTopoMesh::CONCRETECOVERLOSS:
        //     textureImage = concreteCoverLossTexture;
        //     break;
    }

    if(!osgState)
        qDebug()<<"Error creating the state set";

    return osgState;
}


int RTopoPlotter::importTextures(void)
{
    if (tempDir.isValid())
    {
        {
            QString tempFile = tempDir.path() + "/Concrete.jpg";

            // if (QFile::exists(tempFile))
            // {
            //     QFile::remove(tempFile);
            // }

            if (!QFile::copy(":Textures/Concrete.jpg", tempFile))
            {
                qDebug()<<"Error copying the file to the temp directory";
            }

            concreteBaseTexture = osgDB::readImageFile(tempFile.toStdString(),opt);
        }

        {
            QString tempFile = tempDir.path() + "/ConcreteCrack.png";

            if (!QFile::copy(":Textures/ConcreteCrack.png", tempFile))
            {
                qDebug()<<"Error copying the file to the temp directory";
            }

            concreteCrackedTexture = osgDB::readImageFile(tempFile.toStdString(),opt);
        }

        {
            QString tempFile = tempDir.path() + "/ConcreteSpalled.jpg";

            if (!QFile::copy(":Textures/ConcreteSpalled.jpg", tempFile))
            {
                qDebug()<<"Error copying the file to the temp directory";
            }

            concreteSpalledTexture = osgDB::readImageFile(tempFile.toStdString(),opt);
        }

        {
            QString tempFile = tempDir.path() + "/ConcreteCoverLoss.jpg";

            if (!QFile::copy(":Textures/ConcreteCoverLoss.jpg", tempFile))
            {
                qDebug()<<"Error copying the file to the temp directory";
            }

            concreteCoverLossTexture = osgDB::readImageFile(tempFile.toStdString(),opt);
        }

        {
            QString tempFile = tempDir.path() + "/ConcreteCrushing.jpg";

            if (!QFile::copy(":Textures/ConcreteCrushing.jpg", tempFile))
            {
                qDebug()<<"Error copying the file to the temp directory";
            }

            concreteCrushedTexture = osgDB::readImageFile(tempFile.toStdString(),opt);
        }

        {
            QString tempFile = tempDir.path() + "/ConcreteFracturedRebar.jpg";

            if (!QFile::copy(":Textures/ConcreteFracturedRebar.jpg", tempFile))
            {
                qDebug()<<"Error copying the file to the temp directory";
            }

            concreteRebarFracturedTexture = osgDB::readImageFile(tempFile.toStdString(),opt);
        }
    }


    return 0;
}


void  RTopoPlotter::createConcreteStateSets(void)
{
    concreteStateSet = new osg::StateSet;

    this->setStateParameters(concreteStateSet.get());

    // Now set the textures
    // Set the texture 1 - Undamaged concrete texture
    osg::ref_ptr<osg::Texture2D> texture1 = new osg::Texture2D;

    auto textureImage1 = concreteBaseTexture;

    texture1->setImage(textureImage1);

    texture1->setWrap(osg::Texture::WRAP_R, osg::Texture::REPEAT);
    texture1->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    texture1->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    // texture1->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    // texture1->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    concreteStateSet->addUniform(new osg::Uniform("tex1", 0));
    concreteStateSet->setTextureAttributeAndModes(0, texture1.get());

    // Set the texture 2 - Cracked concrete texture
    osg::ref_ptr<osg::Texture2D> texture2 = new osg::Texture2D;

    auto textureImage2 = concreteCrackedTexture;

    texture2->setImage(textureImage2);

    texture2->setWrap(osg::Texture::WRAP_R, osg::Texture::REPEAT);
    texture2->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    texture2->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    // texture2->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    // texture2->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    concreteStateSet->addUniform(new osg::Uniform("tex2", 1));
    concreteStateSet->setTextureAttributeAndModes(1, texture2.get());

    // Set the texture 3 - Spalled concrete texture
    osg::ref_ptr<osg::Texture2D> texture3 = new osg::Texture2D;

    auto textureImage3 = concreteSpalledTexture;

    texture3->setImage(textureImage3);

    texture3->setWrap(osg::Texture::WRAP_R, osg::Texture::REPEAT);
    texture3->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    texture3->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    // texture2->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    // texture2->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    concreteStateSet->addUniform(new osg::Uniform("tex3", 2));
    concreteStateSet->setTextureAttributeAndModes(2, texture3.get());

    // Set the texture 4 - Cover loss texture
    osg::ref_ptr<osg::Texture2D> texture4 = new osg::Texture2D;

    auto textureImage4 = concreteCoverLossTexture;

    texture4->setImage(textureImage4);

    texture4->setWrap(osg::Texture::WRAP_R, osg::Texture::REPEAT);
    texture4->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    texture4->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    // texture2->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    // texture2->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    concreteStateSet->addUniform(new osg::Uniform("tex4", 3));
    concreteStateSet->setTextureAttributeAndModes(3, texture4.get());


    // Set the texture 5 - Crushed concrete texture
    osg::ref_ptr<osg::Texture2D> texture5 = new osg::Texture2D;

    auto textureImage5 = concreteCrushedTexture;

    texture5->setImage(textureImage5);

    texture5->setWrap(osg::Texture::WRAP_R, osg::Texture::REPEAT);
    texture5->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    texture5->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    // texture2->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    // texture2->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    concreteStateSet->addUniform(new osg::Uniform("tex5", 4));
    concreteStateSet->setTextureAttributeAndModes(4, texture5.get());

    // Set the texture 6 - Fractured rebar texture
    osg::ref_ptr<osg::Texture2D> texture6 = new osg::Texture2D;

    auto textureImage6 = concreteRebarFracturedTexture;

    texture6->setImage(textureImage6);

    texture6->setWrap(osg::Texture::WRAP_R, osg::Texture::REPEAT);
    texture6->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    texture6->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    // texture2->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    // texture2->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    concreteStateSet->addUniform(new osg::Uniform("tex6", 5));
    concreteStateSet->setTextureAttributeAndModes(5, texture6.get());

}


void RTopoPlotter::setStateParameters(osg::StateSet* state)
{
    // Set how light will reflect off of the surface material, i.e., matte, glossy, etc.
    state->addUniform(new osg::Uniform("AmbientMaterial",osg::Vec3(0.05f, 0.05f, 0.05f)));
    state->addUniform(new osg::Uniform("DiffuseMaterial",osg::Vec3(0.0f, 0.75f, 0.75f)));
    state->addUniform(new osg::Uniform("LightPosition",osg::Vec3(0.25f, 0.25f, 1.0f)));
    state->addUniform(new osg::Uniform("SpecularMaterial",osg::Vec3(1.0f, 1.0f, 1.0f)));
    state->addUniform(new osg::Uniform("Shininess", 0.01f));
    state->addUniform(new osg::Uniform("Transparency", 1.0f));

    state->addUniform(tessInnerU.get());
    state->addUniform(tessOuterU.get());
    state->setAttribute(new osg::PatchParameter(13));
    state->setAttributeAndModes(program.get(), osg::StateAttribute::ON);

    // Use a blend function to enable alpha blending for transparancy - the alpha value is set in the fragment shader and given as a uniform
    auto blendFunc = new osg::BlendFunc();
    blendFunc->setFunction(osg::BlendFunc::SRC_ALPHA,osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
    state->setMode(GL_BLEND, osg::StateAttribute::ON);
    state->setAttributeAndModes(blendFunc, osg::StateAttribute::ON);
}


osg::ref_ptr<osg::Program> RTopoPlotter::createProgram(void)
{
    // Returning as ref_ptr
    osg::Program *program = new osg::Program();

    // Shaders are stored in the binaries as .qrc files; have to do it this was due to their large size
    // The vertex shader
    QFile vertFile(":Shaders/RTopo.vert");
    if (!vertFile.open(QIODevice::ReadOnly)) {
        qDebug() << vertFile.errorString();
        return nullptr;
    }

    QString vertData = vertFile.readAll();

    auto vertShader = new osg::Shader(osg::Shader::VERTEX, vertData.toStdString());

    if(!vertShader)
        qCritical()<<"Error could not read VERTEX shader from file";
    else
        program->addShader(vertShader);

    // Tessellation control shader
    QFile tesscontrolFile(":Shaders/RTopo.tesscon");
    if (!tesscontrolFile.open(QIODevice::ReadOnly)) {
        qDebug() << tesscontrolFile.errorString();
        return nullptr;
    }

    QString tessControlData = tesscontrolFile.readAll();

    auto tessControlShader = new osg::Shader(osg::Shader::TESSCONTROL, tessControlData.toStdString());

    if(!tessControlShader)
        qCritical()<<"Error could not read VERTEX shader from file";
    else
        program->addShader(tessControlShader);

    // Tessellation eval shader
    QFile tessevalFile(":Shaders/RTopo.tesseval");
    if (!tessevalFile.open(QIODevice::ReadOnly)) {
        qDebug() << tessevalFile.errorString();
        return nullptr;
    }

    QString tessEvalData = tessevalFile.readAll();

    auto tessEvalShader = new osg::Shader(osg::Shader::TESSEVALUATION, tessEvalData.toStdString());

    if(!tessEvalShader)
        qCritical()<<"Error could not read VERTEX shader from file";
    else
        program->addShader(tessEvalShader);

    // Geometry shader
    QFile geomFile(":Shaders/RTopo.geom");
    if (!geomFile.open(QIODevice::ReadOnly)) {
        qDebug() << geomFile.errorString();
        return nullptr;
    }

    QString geomData = geomFile.readAll();

    auto geometryShader = new osg::Shader(osg::Shader::GEOMETRY, geomData.toStdString());

    if(!geometryShader)
        qCritical()<<"Error could not read GEOMETRY shader from file";
    else
        program->addShader(geometryShader);

    // Fragment shader
    QFile fragmentFile(":Shaders/RTopo.frag");
    if (!fragmentFile.open(QIODevice::ReadOnly)) {
        qDebug() << fragmentFile.errorString();
        return nullptr;
    }

    QString fragmentData = fragmentFile.readAll();

    auto fragmentShader = new osg::Shader(osg::Shader::FRAGMENT, fragmentData.toStdString());

    if(!fragmentShader)
        qCritical()<<"Error could not read FRAGMENT shader from file";
    else
        program->addShader(fragmentShader);

    program->setParameter(GL_GEOMETRY_VERTICES_OUT_EXT, 3);
    program->setParameter(GL_GEOMETRY_INPUT_TYPE_EXT, GL_PATCHES);
    program->setParameter(GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);

    return program;
}


osg::ref_ptr<osg::Geometry> RTopoPlotter::createTopoGeometry(osg::Vec3Array* vertices)
{
    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    geometry->setUseVertexBufferObjects(true);
    geometry->setUseVertexArrayObject(true);
    geometry->setUseDisplayList(false);
    geometry->setDataVariance(osg::Object::DYNAMIC);

    // For testing
    /*
    float Verts[][3] = {
    { 0 , 0 , 3.2 },
    { 0 , 2 , 3.2 },
    { -0.051665 , 0.0931976 , 0 },
    { -1.0755e-15 , -0.0235796 , 0 },
    { 2 , 0 , 3.2 },
    { 2 , 2 , 3.2 },
    { -0.000155767 , -0.0220392 , 0 },
    { 6.72892e-16 , 0.0156604 , 0 },
    { -0.400001 , -4.47481e-06 , -0.0128395 },
    { -0.399836 , -6.15449e-14 , -0.0865074 },
    { -0.399854 , 2.17638e-05 , -0.0858827 },
    { -0.399854 , -5.7639e-14 , -0.0837103 },
    { 0 , 0 , 2.8 },
    { 0 , 2 , 2.8 },
    { -0.051665 , 0.0931976 , 0 },
    { -1.0755e-15 , -0.0235796 , 0 },
    { 2 , 0 , 2.8 },
    { 2 , 2 , 2.8 },
    { -0.000155767 , -0.0220392 , 0 },
    { 6.72892e-16 , 0.0156604 , 0 },
    { -0.400001 , -4.47481e-06 , -0.0128395 },
    { -0.399836 , -6.15449e-14 , -0.0865074 },
    { -0.399854 , 2.17638e-05 , -0.0858827 },
    { -0.399854 , -5.7639e-14 , -0.0837103 },
    { 0 , 2 , 3.2 },
    { 0 , 4 , 3.2 },
    { -1.0755e-15 , -0.0235796 , 0 },
    { 0.051665 , 0.0931976 , 0 },
    { 2 , 2 , 3.2 },
    { 2 , 4 , 3.2 },
    { 6.72892e-16 , 0.0156604 , 0 },
    { 0.000155767 , -0.0220392 , 0 },
    { -0.399836 , -6.15449e-14 , -0.0865074 },
    { -0.400001 , 4.47481e-06 , -0.0128395 },
    { -0.399854 , -5.7639e-14 , -0.0837103 },
    { -0.399854 , -2.17638e-05 , -0.0858827 },
    { 0 , 2 , 2.8 },
    { 0 , 4 , 2.8 },
    { -1.0755e-15 , -0.0235796 , 0 },
    { 0.051665 , 0.0931976 , 0 },
    { 2 , 2 , 2.8 },
    { 2 , 4 , 2.8 },
    { 6.72892e-16 , 0.0156604 , 0 },
    { 0.000155767 , -0.0220392 , 0 },
    { -0.399836 , -6.15449e-14 , -0.0865074 },
    { -0.400001 , 4.47481e-06 , -0.0128395 },
    { -0.399854 , -5.7639e-14 , -0.0837103 },
    { -0.399854 , -2.17638e-05 , -0.0858827 },
    { 2 , 0 , 3.2 },
    { 2 , 2 , 3.2 },
    { -0.000155767 , -0.0220392 , 0 },
    { 6.72892e-16 , 0.0156604 , 0 },
    { 4 , 0 , 3.2 },
    { 4 , 2 , 3.2 },
    { -0.0519707 , 0.0137564 , 0 },
    { -9.0758e-16 , -0.0204693 , 0 },
    { -0.399854 , 2.17638e-05 , -0.0858827 },
    { -0.399854 , -5.7639e-14 , -0.0837103 },
    { -0.399708 , -4.11799e-06 , -0.0250436 },
    { -0.399872 , -5.37618e-14 , -0.0990662 },
    { 2 , 0 , 2.8 },
    { 2 , 2 , 2.8 },
    { -0.000155767 , -0.0220392 , 0 },
    { 6.72892e-16 , 0.0156604 , 0 },
    { 4 , 0 , 2.8 },
    { 4 , 2 , 2.8 },
    { -0.0519707 , 0.0137564 , 0 },
    { -9.0758e-16 , -0.0204693 , 0 },
    { -0.399854 , 2.17638e-05 , -0.0858827 },
    { -0.399854 , -5.7639e-14 , -0.0837103 },
    { -0.399708 , -4.11799e-06 , -0.0250436 },
    { -0.399872 , -5.37618e-14 , -0.0990662 },
    { 2 , 2 , 3.2 },
    { 2 , 4 , 3.2 },
    { 6.72892e-16 , 0.0156604 , 0 },
    { 0.000155767 , -0.0220392 , 0 },
    { 4 , 2 , 3.2 },
    { 4 , 4 , 3.2 },
    { -9.0758e-16 , -0.0204693 , 0 },
    { 0.0519707 , 0.0137564 , 0 },
    { -0.399854 , -5.7639e-14 , -0.0837103 },
    { -0.399854 , -2.17638e-05 , -0.0858827 },
    { -0.399872 , -5.37618e-14 , -0.0990662 },
    { -0.399708 , 4.11799e-06 , -0.0250436 },
    { 2 , 2 , 2.8 },
    { 2 , 4 , 2.8 },
    { 6.72892e-16 , 0.0156604 , 0 },
    { 0.000155767 , -0.0220392 , 0 },
    { 4 , 2 , 2.8 },
    { 4 , 4 , 2.8 },
    { -9.0758e-16 , -0.0204693 , 0 },
    { 0.0519707 , 0.0137564 , 0 },
    { -0.399854 , -5.7639e-14 , -0.0837103 },
    { -0.399854 , -2.17638e-05 , -0.0858827 },
    { -0.399872 , -5.37618e-14 , -0.0990662 },
    { -0.399708 , 4.11799e-06 , -0.0250436 },
    { 0 , 0 , 2.8 },
    { 0 , 0 , 3.2 },
    { -0.051665 , 0.0931976 , 0 },
    { -0.051665 , 0.0931976 , 0 },
    { 2 , 0 , 2.8 },
    { 2 , 0 , 3.2 },
    { -0.000155767 , -0.0220392 , 0 },
    { -0.000155767 , -0.0220392 , 0 },
    { -0.400001 , -4.47481e-06 , -0.0128395 },
    { -0.400001 , -4.47481e-06 , -0.0128395 },
    { -0.399854 , 2.17638e-05 , -0.0858827 },
    { -0.399854 , 2.17638e-05 , -0.0858827 },
    { 0 , 4 , 2.8 },
    { 0 , 4 , 3.2 },
    { 0.051665 , 0.0931976 , 0 },
    { 0.051665 , 0.0931976 , 0 },
    { 2 , 4 , 2.8 },
    { 2 , 4 , 3.2 },
    { 0.000155767 , -0.0220392 , 0 },
    { 0.000155767 , -0.0220392 , 0 },
    { -0.400001 , 4.47481e-06 , -0.0128395 },
    { -0.400001 , 4.47481e-06 , -0.0128395 },
    { -0.399854 , -2.17638e-05 , -0.0858827 },
    { -0.399854 , -2.17638e-05 , -0.0858827 },
    { 2 , 0 , 2.8 },
    { 2 , 0 , 3.2 },
    { -0.000155767 , -0.0220392 , 0 },
    { -0.000155767 , -0.0220392 , 0 },
    { 4 , 0 , 2.8 },
    { 4 , 0 , 3.2 },
    { -0.0519707 , 0.0137564 , 0 },
    { -0.0519707 , 0.0137564 , 0 },
    { -0.399854 , 2.17638e-05 , -0.0858827 },
    { -0.399854 , 2.17638e-05 , -0.0858827 },
    { -0.399708 , -4.11799e-06 , -0.0250436 },
    { -0.399708 , -4.11799e-06 , -0.0250436 },
    { 2 , 4 , 2.8 },
    { 2 , 4 , 3.2 },
    { 0.000155767 , -0.0220392 , 0 },
    { 0.000155767 , -0.0220392 , 0 },
    { 4 , 4 , 2.8 },
    { 4 , 4 , 3.2 },
    { 0.0519707 , 0.0137564 , 0 },
    { 0.0519707 , 0.0137564 , 0 },
    { -0.399854 , -2.17638e-05 , -0.0858827 },
    { -0.399854 , -2.17638e-05 , -0.0858827 },
    { -0.399708 , 4.11799e-06 , -0.0250436 },
    { -0.399708 , 4.11799e-06 , -0.0250436 },
    { 0 , 0 , 2.8 },
    { 0 , 0 , 3.2 },
    { -0.051665 , 0.0931976 , 0 },
    { -0.051665 , 0.0931976 , 0 },
    { 0 , 2 , 2.8 },
    { 0 , 2 , 3.2 },
    { -1.0755e-15 , -0.0235796 , 0 },
    { -1.0755e-15 , -0.0235796 , 0 },
    { -0.400001 , -4.47481e-06 , -0.0128395 },
    { -0.400001 , -4.47481e-06 , -0.0128395 },
    { -0.399836 , -6.15449e-14 , -0.0865074 },
    { -0.399836 , -6.15449e-14 , -0.0865074 },
    { 4 , 0 , 2.8 },
    { 4 , 0 , 3.2 },
    { -0.0519707 , 0.0137564 , 0 },
    { -0.0519707 , 0.0137564 , 0 },
    { 4 , 2 , 2.8 },
    { 4 , 2 , 3.2 },
    { -9.0758e-16 , -0.0204693 , 0 },
    { -9.0758e-16 , -0.0204693 , 0 },
    { -0.399708 , -4.11799e-06 , -0.0250436 },
    { -0.399708 , -4.11799e-06 , -0.0250436 },
    { -0.399872 , -5.37618e-14 , -0.0990662 },
    { -0.399872 , -5.37618e-14 , -0.0990662 },
    { 0 , 2 , 2.8 },
    { 0 , 2 , 3.2 },
    { -1.0755e-15 , -0.0235796 , 0 },
    { -1.0755e-15 , -0.0235796 , 0 },
    { 0 , 4 , 2.8 },
    { 0 , 4 , 3.2 },
    { 0.051665 , 0.0931976 , 0 },
    { 0.051665 , 0.0931976 , 0 },
    { -0.399836 , -6.15449e-14 , -0.0865074 },
    { -0.399836 , -6.15449e-14 , -0.0865074 },
    { -0.400001 , 4.47481e-06 , -0.0128395 },
    { -0.400001 , 4.47481e-06 , -0.0128395 },
    { 4 , 2 , 2.8 },
    { 4 , 2 , 3.2 },
    { -9.0758e-16 , -0.0204693 , 0 },
    { -9.0758e-16 , -0.0204693 , 0 },
    { 4 , 4 , 2.8 },
    { 4 , 4 , 3.2 },
    { 0.0519707 , 0.0137564 , 0 },
    { 0.0519707 , 0.0137564 , 0 },
    { -0.399872 , -5.37618e-14 , -0.0990662 },
    { -0.399872 , -5.37618e-14 , -0.0990662 },
    { -0.399708 , 4.11799e-06 , -0.0250436 },
    { -0.399708 , 4.11799e-06 , -0.0250436 },
        { 3.5 , 3.75 , 0 },
        { 3.5 , 3.75 , 3 },
        { 0 , 0 , 0 },
        { 0.0519707 , 0.0137564 , 0 },
        { 4.5 , 3.75 , 0 },
        { 4.5 , 3.75 , 3 },
        { 0 , 0 , 0 },
        { 0.0519707 , 0.0137564 , 0 },
        { 0 , 0 , 0 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { 0 , 0 , 0 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { 3.5 , 4.25 , 0 },
        { 3.5 , 4.25 , 3 },
        { 0 , 0 , 0 },
        { 0.0519707 , 0.0137564 , 0 },
        { 4.5 , 4.25 , 0 },
        { 4.5 , 4.25 , 3 },
        { 0 , 0 , 0 },
        { 0.0519707 , 0.0137564 , 0 },
        { 0 , 0 , 0 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { 0 , 0 , 0 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { 3.5 , 3.75 , 0 },
        { 3.5 , 3.75 , 3 },
        { 0 , 0 , 0 },
        { 0.0519707 , 0.0137564 , 0 },
        { 3.5 , 4.25 , 0 },
        { 3.5 , 4.25 , 3 },
        { 0 , 0 , 0 },
        { 0.0519707 , 0.0137564 , 0 },
        { 0 , 0 , 0 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { 0 , 0 , 0 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { 4.5 , 3.75 , 0 },
        { 4.5 , 3.75 , 3 },
        { 0 , 0 , 0 },
        { 0.0519707 , 0.0137564 , 0 },
        { 4.5 , 4.25 , 0 },
        { 4.5 , 4.25 , 3 },
        { 0 , 0 , 0 },
        { 0.0519707 , 0.0137564 , 0 },
        { 0 , 0 , 0 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { 0 , 0 , 0 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { 3.5 , 3.75 , 0 },
        { 3.5 , 4.25 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 4.5 , 3.75 , 0 },
        { 4.5 , 4.25 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 3.5 , 3.75 , 3 },
        { 3.5 , 4.25 , 3 },
        { 0.0519707 , 0.0137564 , 0 },
        { 0.0519707 , 0.0137564 , 0 },
        { 4.5 , 3.75 , 3 },
        { 4.5 , 4.25 , 3 },
        { 0.0519707 , 0.0137564 , 0 },
        { 0.0519707 , 0.0137564 , 0 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { -0.399708 , 4.11799e-06 , -0.0250436 },
        { -0.5 , 3.75 , 0 },
        { -0.5 , 3.75 , 3 },
        { 0 , 0 , 0 },
        { 0.051665 , 0.0931976 , 0 },
        { 0.5 , 3.75 , 0 },
        { 0.5 , 3.75 , 3 },
        { 0 , 0 , 0 },
        { 0.051665 , 0.0931976 , 0 },
        { 0 , 0 , 0 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { 0 , 0 , 0 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { -0.5 , 4.25 , 0 },
        { -0.5 , 4.25 , 3 },
        { 0 , 0 , 0 },
        { 0.051665 , 0.0931976 , 0 },
        { 0.5 , 4.25 , 0 },
        { 0.5 , 4.25 , 3 },
        { 0 , 0 , 0 },
        { 0.051665 , 0.0931976 , 0 },
        { 0 , 0 , 0 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { 0 , 0 , 0 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { -0.5 , 3.75 , 0 },
        { -0.5 , 3.75 , 3 },
        { 0 , 0 , 0 },
        { 0.051665 , 0.0931976 , 0 },
        { -0.5 , 4.25 , 0 },
        { -0.5 , 4.25 , 3 },
        { 0 , 0 , 0 },
        { 0.051665 , 0.0931976 , 0 },
        { 0 , 0 , 0 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { 0 , 0 , 0 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { 0.5 , 3.75 , 0 },
        { 0.5 , 3.75 , 3 },
        { 0 , 0 , 0 },
        { 0.051665 , 0.0931976 , 0 },
        { 0.5 , 4.25 , 0 },
        { 0.5 , 4.25 , 3 },
        { 0 , 0 , 0 },
        { 0.051665 , 0.0931976 , 0 },
        { 0 , 0 , 0 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { 0 , 0 , 0 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { -0.5 , 3.75 , 0 },
        { -0.5 , 4.25 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0.5 , 3.75 , 0 },
        { 0.5 , 4.25 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { -0.5 , 3.75 , 3 },
        { -0.5 , 4.25 , 3 },
        { 0.051665 , 0.0931976 , 0 },
        { 0.051665 , 0.0931976 , 0 },
        { 0.5 , 3.75 , 3 },
        { 0.5 , 4.25 , 3 },
        { 0.051665 , 0.0931976 , 0 },
        { 0.051665 , 0.0931976 , 0 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { -0.400001 , 4.47481e-06 , -0.0128395 },
        { 3.5 , -0.25 , 0 },
        { 3.5 , -0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.0519707 , 0.0137564 , 0 },
        { 4.5 , -0.25 , 0 },
        { 4.5 , -0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.0519707 , 0.0137564 , 0 },
        { 0 , 0 , 0 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { 0 , 0 , 0 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { 3.5 , 0.25 , 0 },
        { 3.5 , 0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.0519707 , 0.0137564 , 0 },
        { 4.5 , 0.25 , 0 },
        { 4.5 , 0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.0519707 , 0.0137564 , 0 },
        { 0 , 0 , 0 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { 0 , 0 , 0 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { 3.5 , -0.25 , 0 },
        { 3.5 , -0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.0519707 , 0.0137564 , 0 },
        { 3.5 , 0.25 , 0 },
        { 3.5 , 0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.0519707 , 0.0137564 , 0 },
        { 0 , 0 , 0 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { 0 , 0 , 0 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { 4.5 , -0.25 , 0 },
        { 4.5 , -0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.0519707 , 0.0137564 , 0 },
        { 4.5 , 0.25 , 0 },
        { 4.5 , 0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.0519707 , 0.0137564 , 0 },
        { 0 , 0 , 0 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { 0 , 0 , 0 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { 3.5 , -0.25 , 0 },
        { 3.5 , 0.25 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 4.5 , -0.25 , 0 },
        { 4.5 , 0.25 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 3.5 , -0.25 , 3 },
        { 3.5 , 0.25 , 3 },
        { -0.0519707 , 0.0137564 , 0 },
        { -0.0519707 , 0.0137564 , 0 },
        { 4.5 , -0.25 , 3 },
        { 4.5 , 0.25 , 3 },
        { -0.0519707 , 0.0137564 , 0 },
        { -0.0519707 , 0.0137564 , 0 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { -0.399708 , -4.11799e-06 , -0.0250436 },
        { -0.5 , -0.25 , 0 },
        { -0.5 , -0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.051665 , 0.0931976 , 0 },
        { 0.5 , -0.25 , 0 },
        { 0.5 , -0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.051665 , 0.0931976 , 0 },
        { 0 , 0 , 0 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
        { 0 , 0 , 0 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
        { -0.5 , 0.25 , 0 },
        { -0.5 , 0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.051665 , 0.0931976 , 0 },
        { 0.5 , 0.25 , 0 },
        { 0.5 , 0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.051665 , 0.0931976 , 0 },
        { 0 , 0 , 0 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
        { 0 , 0 , 0 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
        { -0.5 , -0.25 , 0 },
        { -0.5 , -0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.051665 , 0.0931976 , 0 },
        { -0.5 , 0.25 , 0 },
        { -0.5 , 0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.051665 , 0.0931976 , 0 },
        { 0 , 0 , 0 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
        { 0 , 0 , 0 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
        { 0.5 , -0.25 , 0 },
        { 0.5 , -0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.051665 , 0.0931976 , 0 },
        { 0.5 , 0.25 , 0 },
        { 0.5 , 0.25 , 3 },
        { 0 , 0 , 0 },
        { -0.051665 , 0.0931976 , 0 },
        { 0 , 0 , 0 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
        { 0 , 0 , 0 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
        { -0.5 , -0.25 , 0 },
        { -0.5 , 0.25 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0.5 , -0.25 , 0 },
        { 0.5 , 0.25 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { 0 , 0 , 0 },
        { -0.5 , -0.25 , 3 },
        { -0.5 , 0.25 , 3 },
        { -0.051665 , 0.0931976 , 0 },
        { -0.051665 , 0.0931976 , 0 },
        { 0.5 , -0.25 , 3 },
        { 0.5 , 0.25 , 3 },
        { -0.051665 , 0.0931976 , 0 },
        { -0.051665 , 0.0931976 , 0 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
        { -0.400001 , -4.47481e-06 , -0.0128395 },
    };

    vertices = new osg::Vec3Array();

    int VertexCount = sizeof(Verts) / (sizeof(float) * 3);

    for (int vert = 0; vert < VertexCount; vert++) {
        float x = Verts[vert][0];
        float y = Verts[vert][1];
        float z = Verts[vert][2];
        vertices->push_back(osg::Vec3(x,y,z));
    }
    */

    int VertexCount = vertices->size();

    // Set the vertices to the vertex array
    geometry->setVertexArray(vertices);

    // Create and add the primitive set to the geometry -- in this case the primitive set are patches
    osg::ref_ptr<osg::PrimitiveSet> thePrimSet = new osg::DrawArrays(GL_PATCHES, 0, VertexCount);

    geometry->addPrimitiveSet(thePrimSet);

    // Set the OpenGL attributes to the vertices
    geometry->setVertexAttribArray(0, vertices, osg::Array::BIND_PER_VERTEX);

    return geometry;
}


osg::ref_ptr<osg::Geode> RTopoPlotter::drawTopo(RTopoMesh* topoMesh,const double scale)
{

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();

    osg::ref_ptr<osg::Vec3Array> vertices = this->getTopoVertices(topoMesh,scale);

    if(vertices.get()->empty())
        return nullptr;

    //Set the vertex buffer array in the topo mesh
    topoMesh->setVertexBufferArray(vertices);

    //Create the geode
    osg::ref_ptr<osg::Geometry> geom = createTopoGeometry(vertices);

    geode->addDrawable(geom);

    //Test geode
    // osg::ref_ptr<osg::Geode> geode = createTopoGeode(nullptr);

    auto state = this->getStateSet(topoMesh);

    geode->setStateSet(state.get());

    return geode;
}


osg::ref_ptr<osg::Vec3Array> RTopoPlotter::getTopoVertices(const RTopoMesh* topoMesh,const double scale)
{

    // First get the control vertices from the topological mesh
    auto faceVector = topoMesh->getFaceVector();

    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();

    bool deformed = false;

    if(scale != 0.0)
        deformed = true;

    auto count = 0;
    for(auto face : faceVector)
    {
        auto vertexVec2 = face->getVerticesVector();

        // Create the patch array
        for(auto& it : vertexVec2)
        {
            it->commitState(deformed);
        }

        auto theOSGPoint00 = vertexVec2[0]->getOSGPoint();
        auto theOSGPoint10 = vertexVec2[1]->getOSGPoint();
        auto theOSGPoint11 = vertexVec2[2]->getOSGPoint();
        auto theOSGPoint01 = vertexVec2[3]->getOSGPoint();

        // 12 control points per patch array - but the 13th is the texture selector
        // Get the undeformed location of the nodes
        osg::Vec3d p00(theOSGPoint00.x(),theOSGPoint00.y(),theOSGPoint00.z());
        osg::Vec3d p03(theOSGPoint01.x(),theOSGPoint01.y(),theOSGPoint01.z());

        osg::Vec3d p30(theOSGPoint10.x(),theOSGPoint10.y(),theOSGPoint10.z());
        osg::Vec3d p33(theOSGPoint11.x(),theOSGPoint11.y(),theOSGPoint11.z());

        auto rotScale = scale;

        osg::Vec3d rot00(vertexVec2[0]->getXRotation()*rotScale, vertexVec2[0]->getYRotation()*rotScale,vertexVec2[0]->getZRotation()*rotScale);
        osg::Vec3d rot03(vertexVec2[3]->getXRotation()*rotScale, vertexVec2[3]->getYRotation()*rotScale,vertexVec2[3]->getZRotation()*rotScale);
        osg::Vec3d rot30(vertexVec2[1]->getXRotation()*rotScale, vertexVec2[1]->getYRotation()*rotScale,vertexVec2[1]->getZRotation()*rotScale);
        osg::Vec3d rot33(vertexVec2[2]->getXRotation()*rotScale, vertexVec2[2]->getYRotation()*rotScale,vertexVec2[2]->getZRotation()*rotScale);

        vertices->push_back(p00);    // 0
        vertices->push_back(p03);    // 1
        vertices->push_back(rot00);  // 2
        vertices->push_back(rot03);  // 3

        vertices->push_back(p30);    // 4
        vertices->push_back(p33);    // 5
        vertices->push_back(rot30);  // 6
        vertices->push_back(rot33);  // 7

        auto dispScale = scale;

        osg::Vec3d delta00(vertexVec2[0]->getXDisplacement()*dispScale, vertexVec2[0]->getYDisplacement()*dispScale,vertexVec2[0]->getZDisplacement()*dispScale);
        osg::Vec3d delta03(vertexVec2[3]->getXDisplacement()*dispScale, vertexVec2[3]->getYDisplacement()*dispScale,vertexVec2[3]->getZDisplacement()*dispScale);
        osg::Vec3d delta30(vertexVec2[1]->getXDisplacement()*dispScale, vertexVec2[1]->getYDisplacement()*dispScale,vertexVec2[1]->getZDisplacement()*dispScale);
        osg::Vec3d delta33(vertexVec2[2]->getXDisplacement()*dispScale, vertexVec2[2]->getYDisplacement()*dispScale,vertexVec2[2]->getZDisplacement()*dispScale);

        vertices->push_back(delta00); // 8
        vertices->push_back(delta03); // 9
        vertices->push_back(delta30); // 10
        vertices->push_back(delta33); // 11

        // Get the damage type from the face
        double damageMode = static_cast<double>(face->getDamageType());

        if(damageMode == -1.0)
        {
            damageMode = 0.0;
        }

        // damageMode = 1.0; //testing purposes

        osg::Vec3d damageVec(damageMode,0.0,0.0);

        if(damageMode == 1.0)
        {
            auto theDamageMesh = face->getTheDamageRegion();

            if(theDamageMesh != nullptr)
            {
                auto faceIndex = face->getIndex();

                auto theta = theDamageMesh->getCrackOrientation(faceIndex);

                //Should be in radians
                auto crackOrientation = theta;

                damageVec[1]= crackOrientation;
            }
        }

        vertices->push_back(damageVec); // 12

        count++;
    }


    return vertices;
}


