#include <osg/Matrix>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/StateSet>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/Texture2D>
#include <osg/TexMat>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <iostream>


using namespace osg;

// factor for speed of whole animation
const float ANIMATION_SPEED = 2.0;

PositionAttitudeTransform *starfield[6];
PositionAttitudeTransform *sun;
PositionAttitudeTransform *mercury;
PositionAttitudeTransform *venus;
PositionAttitudeTransform *earth;
PositionAttitudeTransform *emoon;
PositionAttitudeTransform *mars;
PositionAttitudeTransform *jupiter;
PositionAttitudeTransform *saturn;


int main()
{
    // create a sphere
    Geode *sphere = new Geode();
    sphere->addDrawable(new ShapeDrawable(new Sphere(Vec3(), 1)));

// create a simple material
    Material *material = new Material();
    material->setEmission(Material::FRONT, Vec4(0.8, 0.8, 0.8, 1.0));

// create a texture
// load image for texture
    Image *image = osgDB::readImageFile("sunmap.bmp");
    if (!image) {
        std::cout << "Couldn't load texture." << std::endl;
        return NULL;
    }
    osg::Texture2D *texture = new Texture2D;
    texture->setDataVariance(Object::DYNAMIC);
    texture->setFilter(Texture::MIN_FILTER, Texture::LINEAR_MIPMAP_LINEAR);
    texture->setFilter(Texture::MAG_FILTER, Texture::LINEAR);
    texture->setWrap(Texture::WRAP_S, Texture::CLAMP);
    texture->setWrap(Texture::WRAP_T, Texture::CLAMP);
    texture->setImage(image);

// assign the material and texture to the sphere
    StateSet *sphereStateSet = sphere->getOrCreateStateSet();
    sphereStateSet->ref();
    sphereStateSet->setAttribute(material);
    sphereStateSet->setTextureAttributeAndModes(0, texture, StateAttribute::ON);

    Group* scene = new Group;
    scene->addChild(sphere);

    osgViewer::Viewer viewer;
    viewer.run();
}
