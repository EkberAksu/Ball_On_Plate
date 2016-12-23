#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/TexMat>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>

#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <string>


using namespace osg;
using namespace osgGA;
using namespace std;

unsigned int rcvShadowMask = 0x1;
unsigned int castShadowMask = 0x2;

float jointAngle1;
float jointAngle2;
float EndEffector;

osg::MatrixTransform *joint1;
osg::MatrixTransform *joint2;
osg::MatrixTransform *joint3;

osg::MatrixTransform *buildJoint1(osg::MatrixTransform *previousJoint);

osg::MatrixTransform *buildJoint2(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint3(osg::MatrixTransform *previousJoint);

osg::MatrixTransform *buildEndEffector();

osg::TessellationHints *hints = new osg::TessellationHints;



class KeyboardEventHandler : public osgGA::GUIEventHandler {
public:

    KeyboardEventHandler() {}


    static void rotateY(float angle, osg::MatrixTransform *joint) {
        osg::Matrix yRot;
        yRot.makeRotate(angle, 0.0, 1.0, 0.0);
        joint->setMatrix(joint->getMatrix()*yRot );
    }

    static void rotateX(float angle, osg::MatrixTransform *joint) {
        osg::Matrix xRot;
        xRot.makeRotate(angle, 1.0, 0.0, 0.0);
        joint->setMatrix(joint->getMatrix()*xRot);
    }

    static void rotateXY(float angleX,float angleY, osg::MatrixTransform *joint) {
        osg::Matrix xRot;
        xRot.makeRotate(angleX, 1.0, 0.0, 0.0);
        osg::Matrix yRot;
        yRot.makeRotate(angleY, 0.0, 1.0, 0.0);
        joint->setMatrix(joint->getMatrix()*xRot*yRot);
    }

    static void rotateYX(float angle, osg::MatrixTransform *joint) {
        osg::Matrix xRot;
        xRot.makeRotate(angle, 1.0, 0.0, 0.0);
        joint->setMatrix(joint->getMatrix()*xRot);
    }

    static void translate(float x, float y, float z, osg::MatrixTransform *joint) {
        osg::Matrix trans;
        trans.makeTranslate(x, y, z);
        joint->setMatrix(trans * joint->getMatrix());
    }

    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &) {
        switch (ea.getEventType()) {
            case (osgGA::GUIEventAdapter::KEYDOWN): {
                switch (ea.getKey()) {
                    case 'd':
                        rotateY(-osg::PI/180, joint1);
                        translate(0.0, 0.0, 0.2,joint2);
                        return true;
                    case 'a':
                        rotateY(osg::PI/180, joint1);
                        translate(0.0, 0.0, -0.2,joint2);
                        return true;
                    case 's':
                        rotateX(-osg::PI/180, joint1);
                        translate(-0.2, 0.0, 0.0,joint2);
                        return true;
                    case 'w':
                        rotateX(osg::PI/180, joint1);
                        translate(0.2, 0.0, 0.0,joint2);
                        return true;
                    case 'e':
                        rotateXY(osg::PI/180,-osg::PI/180, joint1);
                        translate(0.25, 0.0, 0.25,joint2);
                        return true;
                    case 'z':
                        rotateXY(-osg::PI/180,osg::PI/180, joint1);
                        translate(-0.25, 0.0, -0.25,joint2);
                        return true;
                    case 'q':
                        rotateXY(osg::PI/180,osg::PI/180, joint1);
                        translate(0.25, 0.0,- 0.25,joint2);
                        return true;
                    case 'x':
                        rotateXY(-osg::PI/180,-osg::PI/180, joint1);
                        translate(-0.25, 0.0, 0.25,joint2);
                        return true;
                }
            }
            default:
                break;

        }
        //return false to allow mouse manipulation
        return false;
    }
};


osg::Group *createShapes() {
    osg::ref_ptr<osg::LightSource> source = new osg::LightSource;
    source->getLight()->setPosition( osg::Vec4(-5.0, 10.0, 5.0,
                                               0.0) );
    source->getLight()->setAmbient( osg::Vec4(0.2, 0.2, 0.2, 1.0)
    );
    source->getLight()->setDiffuse( osg::Vec4(0.8, 0.8, 0.8, 1.0)
    );

    osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
    sm->setLight( source.get() );
    sm->setTextureSize( osg::Vec2s(1024, 1024) );
    sm->setTextureUnit( 1 );

    osg::ref_ptr<osgShadow::ShadowedScene> root =
            new osgShadow::ShadowedScene;
    root->setShadowTechnique( sm.get() );
    root->setReceivesShadowTraversalMask( rcvShadowMask );
    root->setCastsShadowTraversalMask( castShadowMask );

    osg::Group *group = new osg::Group();
    osg::MatrixTransform *transform = new osg::MatrixTransform();
    group->addChild(transform);

    joint3 = buildJoint3(transform);
    joint1 = buildJoint1(joint3);
    joint2 = buildJoint2(joint1);
    joint2->addChild(buildEndEffector());

    root->addChild( transform );
    root->addChild( joint3 );
    root->addChild( joint1 );
    root->addChild( joint2 );
    root->addChild(buildEndEffector()  );
    //root->addChild( source.get() );

    //group->addChild(source.get());
    return group;
}

void addTexture(osg::ShapeDrawable* shape, string file){
    // create a simple material
    Material *material = new Material();
    material->setEmission(Material::FRONT, Vec4(0.8, 0.8, 0.8, 1.0));

    Image *image = osgDB::readImageFile(file);
    if (!image) {
        std::cout << "Couldn't load texture." << std::endl;
        return ;
    }
    osg::Texture2D *texture = new Texture2D;
    texture->setDataVariance(Object::DYNAMIC);
    texture->setFilter(Texture::MIN_FILTER, Texture::LINEAR_MIPMAP_LINEAR);
    texture->setFilter(Texture::MAG_FILTER, Texture::LINEAR);
    texture->setWrap(Texture::WRAP_S, Texture::CLAMP);
    texture->setWrap(Texture::WRAP_T, Texture::CLAMP);
    texture->setImage(image);

    StateSet *sphereStateSet = shape->getOrCreateStateSet();
    sphereStateSet->ref();
    sphereStateSet->setAttribute(material);
    sphereStateSet->setTextureAttributeAndModes(0, texture, StateAttribute::ON);
}

osg::MatrixTransform *buildJoint3(osg::MatrixTransform *previousJoint)  //The Cylinder at the button
{
    double length = 20.0;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 1.0, 0.0, 0.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, -10.0f, 0.0f), length, 1.0f, length), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    addTexture(plate, "camTahta.bmp");

    zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint1(osg::MatrixTransform *previousJoint)  //The Cylinder at the button
{
    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(-osg::PI_2, 1.0, 0.0, 0.0);
    xTransform->setMatrix(xRot);
    float radius = 0.6f;
    float height = 10.0f;
    osg::Geode *joint = new osg::Geode();
    xTransform->addChild(joint);
    joint->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f, 0.0f, - height/2), radius, height),hints));//cylinder's coordinate and size

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    xTransform->addChild(zTransform);
    osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, height);
    osg::Matrix zRot = osg::Matrix::rotate(jointAngle1, 0.0, 0.0, 1.0);
    zTransform->setMatrix(zTrans * zRot);

    return zTransform;
}

osg::MatrixTransform *buildJoint2(osg::MatrixTransform *previousJoint) {

    double length = 15.0;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 1.0, 0.0, 0.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate= new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, -10.0f, 0.0f), length, 0.6, length), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0);
    osg::Matrix zRot = osg::Matrix::rotate(osg::PI_2 + jointAngle2, 0.0, 1.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);
    xTransform->addChild(zTransform);

    addTexture(plate, "tahta.bmp");

    zTransform->setNodeMask( rcvShadowMask|castShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildEndEffector() {
    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix m;

    double radius = 1.0;
    m.makeTranslate(0, 0, 0);    //coordinate of the box
    mt->setMatrix(m);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f, -8.7f, 0.0f), radius), hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.9f, 1.0f));


    geode_3->addDrawable(shape1);

    addTexture(shape1, "top.bmp");

    mt->addChild(geode_3);

    mt->setNodeMask( castShadowMask );
    return mt;
}

void setCameraPosision(osgViewer::Viewer & viewer){
    // Create a new Trackball manipulator
    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
    // Connect the trackball manipulator to the viewer
    viewer.setCameraManipulator(manipulator);

    // Set the desired home coordinates for the manipulator
    osg::Vec3d eye(1.0, -70.0, 20.0);
    osg::Vec3d center(1.0, 1.0, 1.0);
    osg::Vec3d up(0.0, 1.0, 0.0);

    // Make sure that OSG is not overriding our home position
    manipulator->setAutoComputeHomePosition(false);
    // Set the desired home position of the Trackball Manipulator
    manipulator->setHomePosition(eye, center, up);
    // Force the camera to move to the home position
    manipulator->home(0.0);
}

int main(int, char **) {
    hints->setDetailRatio(0.5f);


    jointAngle1 = 0;
    jointAngle2 = 0.0;
    EndEffector = 1.0;
    //Vec3d eye, center, up;
    osgViewer::Viewer viewer;
    viewer.addEventHandler(new KeyboardEventHandler());
    viewer.createDistortionTexture(20,20);
    // add model to viewer.
    viewer.setSceneData(createShapes());

   /* viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    viewer.getCameraManipulator()->getHomePosition(eye,center,up);
    viewer.getCameraManipulator()->setHomePosition(-Vec3d(eye.x(),eye.y()-70,eye.z()-60),
                                                   Vec3d(center.x(),center.y(),center.z()),
                                                   -Vec3d(up.x(),up.y(),up.z()));
    Vec3d eye( 1000.0, 1000.0, 0.0 );
    Vec3d center( 0.0, 0.0, 0.0 );
    Vec3d up( 0.0, 0.0, 1.0 );

    viewer.getCamera()->setViewMatrixAsLookAt( eye, center, up );*/

    setCameraPosision(viewer);

    viewer.setUpViewInWindow(25, 25, 1280, 680);

    return viewer.run();
}