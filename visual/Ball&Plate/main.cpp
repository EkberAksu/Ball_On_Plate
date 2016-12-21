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
#include <osgWidget/Util>
#include <osgWidget/WindowManager>
#include <osgWidget/Table>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgWidget/WindowManager>
#include <osgWidget/Box>
#include <osgWidget/Canvas>
#include <osgWidget/Label>
#include <osgWidget/ViewerEventHandlers>
#include <iostream>
#include <sstream>
#include <windows.h>


using namespace osg;
using namespace osgGA;
using namespace std;

unsigned int rcvShadowMask = 0x1;
unsigned int castShadowMask = 0x2;
osg::Matrix initPosPlate;
osg::Matrix initPosBall;

float jointAngle1;
float jointAngle2;
float EndEffector;

osg::MatrixTransform *joint0; //alt tahta
osg::MatrixTransform *joint1;  //silindir
osg::MatrixTransform *joint2; //tahta
osg::MatrixTransform *joint3;
osg::MatrixTransform *joint4;
osg::MatrixTransform *joint5;
osg::MatrixTransform *joint6;

osg::MatrixTransform *buildjoint0(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint1(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint2(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint3();
osg::MatrixTransform *buildJoint4(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint5(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildEndEffector();

void makeMove(float x, float y);

osg::TessellationHints *hints = new osg::TessellationHints;

extern bool tabPressed( osgWidget::Event& ev );

osgWidget::Label* createLabel( const std::string& name,
                               const std::string& text, float size,
                               const osg::Vec4& color )
{
    osg::ref_ptr<osgWidget::Label> label =
            new osgWidget::Label(name);
    label->setLabel( text );
    label->setFont( "fonts/arial.ttf" );
    label->setFontSize( size );
    label->setFontColor( 1.0f, 1.0f, 1.0f, 1.0f );
    label->setColor( color );
    label->addSize( 10.0f, 10.0f );
    //label->setCanFill( true );
    return label.release();
}

osgWidget::Window* createSimpleTabs( float winX, float winY )
{
    osg::ref_ptr<osgWidget::Canvas> contents =
            new osgWidget::Canvas("contents");
    osg::ref_ptr<osgWidget::Box> tabs =
            new osgWidget::Box("tabs", osgWidget::Box::HORIZONTAL);
    //tabs->setScale(0.8f);

    for ( unsigned int i=0; i<3; ++i )
    {
        osg::Vec4 color(0.0f, (float)i / 3.0f, 0.0f, 1.0f);
        std::stringstream ss, ss2;
        ss << "Button" << i;
        ss2 << "Detected action:" <<" Button "<< i << " is pressed" ;
        osgWidget::Label* content = createLabel(ss.str(),
                                                ss2.str(), 13.0f, color);
        content->setLayer( osgWidget::Widget::LAYER_MIDDLE, i );
        content->setSize(480.0f,30.0f);
        contents->addWidget( content, 0.0f, 0.0f );
        osgWidget::Label* tab = createLabel(ss.str(),
                                            ss.str(), 10.0f, color);
        tab->setEventMask( osgWidget::EVENT_MOUSE_PUSH );
        tab->addCallback( new osgWidget::Callback(
                &tabPressed, osgWidget::EVENT_MOUSE_PUSH, content) );
        tabs->addWidget( tab );
    }
    osg::ref_ptr<osgWidget::Box> main =
            new osgWidget::Box("main", osgWidget::Box::VERTICAL);
    main->setOrigin( winX, winY );
    main->attachMoveCallback();
    main->addWidget( contents->embed() );
    main->addWidget( tabs->embed() );
    main->addWidget( createLabel("title", "This is only an example",
                                 15.0f, osg::Vec4(0.0f, 0.4f, 1.0f, 1.0f)) );
    main->setScale(0.8);
    return main.release();
}

bool tabPressed( osgWidget::Event& ev )
{
    osgWidget::Label* content = static_cast<
            osgWidget::Label*>( ev.getData() );
    if ( !content ) return false;
    osgWidget::Canvas* canvas = dynamic_cast<
            osgWidget::Canvas*>( content->getParent() );
    if ( canvas )
    {
        osgWidget::Canvas::Vector& objs = canvas->getObjects();
        for( unsigned int i=0; i<objs.size(); ++i )
            objs[i]->setLayer( osgWidget::Widget::LAYER_MIDDLE, i );
        content->setLayer( osgWidget::Widget::LAYER_TOP, 0 );
        canvas->resize();
    }
    return true;
}

osg::ref_ptr<osgText::Font> g_font =
        osgText::readFontFile("fonts/arial.ttf");

osg::Camera* createHUDCamera( double left, double right,
                              double bottom, double top )
{
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    camera->setClearMask( GL_DEPTH_BUFFER_BIT );
    camera->setRenderOrder( osg::Camera::POST_RENDER );
    camera->setAllowEventFocus( false );
    camera->setProjectionMatrix(
            osg::Matrix::ortho2D(left, right, bottom, top) );
    return camera.release();
}
osgText::Text* createText( const osg::Vec3& pos,
                           const std::string& content,
                           float size )
{
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    text->setFont( g_font.get() );
    text->setCharacterSize( size );
    text->setAxisAlignment( osgText::TextBase::XY_PLANE );
    text->setPosition( pos );
    text->setText( content );
    text->setColor(osg::Vec4(0.5f, 0.9f, 0.1f, 1.0f));
    return text.release();
}

class KeyboardEventHandler : public osgGA::GUIEventHandler {
public:

    KeyboardEventHandler() {}


    static void rotateY(float angle, osg::MatrixTransform *joint) {
        osg::Matrix yRot;
        yRot.makeRotate(angle, 0.0, 1.0, 0.0);
        //joint->setMatrix(joint->getMatrix()*yRot );
        joint->setMatrix(initPosPlate*yRot );
    }

    static void rotateX(float angle, osg::MatrixTransform *joint) {
        osg::Matrix xRot;
        xRot.makeRotate(angle, 1.0, 0.0, 0.0);
        //joint->setMatrix(joint->getMatrix()*xRot);
        joint->setMatrix(initPosPlate*xRot );
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
        //joint->setMatrix(trans * joint->getMatrix());
        joint->setMatrix(initPosBall*trans );
    }

    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &) {
        switch (ea.getEventType()) {
            case (osgGA::GUIEventAdapter::FRAME): {
                makeMove(5,7);
                Sleep(100);
                makeMove(-5,7);
                Sleep(100);
                makeMove(-5,-7);
                /*
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
                }*/
            }
            default:
                break;

        }
        //return false to allow mouse manipulation
        return false;
    }
};

void makeMove(float x, float y){
    KeyboardEventHandler::rotateX(-x*osg::PI/180, joint1);
    KeyboardEventHandler::rotateY(-y*osg::PI/180, joint1);
    KeyboardEventHandler::translate(x, 0.0, y,joint2);
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

osg::Group *createShapes() {
    /*osg::ref_ptr<osg::LightSource> source = new osg::LightSource;
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
    root->setCastsShadowTraversalMask( castShadowMask );*/

    osg::Group *group = new osg::Group();
    osg::MatrixTransform *transform = new osg::MatrixTransform();
    group->addChild(transform);

    joint0 = buildjoint0(transform);
    joint0->addChild(buildJoint3());
    joint1 = buildJoint1(joint0);
    joint2 = buildJoint2(joint1);
    joint2->addChild(buildEndEffector());

    initPosPlate = joint1->getMatrix();
    initPosBall = joint2->getMatrix();

   /* root->addChild( transform );
    root->addChild( joint0 );
    root->addChild( joint1 );
    root->addChild( joint2 );
    root->addChild(buildEndEffector()  );
    //root->addChild( source.get() );

    //group->addChild(source.get());*/
    return group;
}


osg::MatrixTransform *buildjoint0(osg::MatrixTransform *previousJoint)  //The Cylinder at the button
{
    double length = 18.75;
    double width = 25;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 1.0, 0.0, 0.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, -10.0f, 0.0f), width, 1.0f, length), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    addTexture(plate, "altTahta.bmp");

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
    double width = 20.0;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 1.0, 0.0, 0.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate= new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, -10.0f, 0.0f), width, 0.6, length), hints);
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

osg::MatrixTransform *buildJoint3() {
    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix m;

    double length = 2.0;
    double width = 2.0;
    m.makeTranslate(0, 0, 0);    //coordinate of the box
    mt->setMatrix(m);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Box(osg::Vec3(-0.5f, -9.0f, 7.0f), width, 0.5, length), hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.9f, 1.0f));


    geode_3->addDrawable(shape1);

    addTexture(shape1, "sunmap.bmp");

    mt->addChild(geode_3);

    //mt->setNodeMask( castShadowMask );
    return mt;
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

    //mt->setNodeMask( castShadowMask );
    return mt;
}

void setCameraPosision(osgViewer::Viewer & viewer){

    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;

    viewer.setCameraManipulator(manipulator);

    osg::Vec3d eye(1.0, -70.0, 20.0);
    osg::Vec3d center(1.0, 1.0, 1.0);
    osg::Vec3d up(0.0, 1.0, 0.0);

    manipulator->setAutoComputeHomePosition(false);
    manipulator->setHomePosition(eye, center, up);
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
    //viewer.setSceneData(createShapes());

    setCameraPosision(viewer);

    viewer.setUpViewInWindow(10, 35, 1350, 680);
//--------------------------------------------------------------------------------------------
    osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
    textGeode->addDrawable(createText(
            osg::Vec3(450.0f, 650.0f, 0.0f),
            "Project: Ball&Plate",
            30.0f)
    );
    textGeode->addDrawable(createText(
            osg::Vec3(600.0f, 600.0f, 0.0f),
            "- by Group 1",
            15.0f)
    );

//----------------------------------------------------------------------------------------------

    osg::ref_ptr<osgWidget::WindowManager> wm =
            new osgWidget::WindowManager(&viewer, 1350.0f,
                                         680.0f, 0xf0000000);
    osg::Camera* camera = wm->createParentOrthoCamera();
    wm->addChild( createSimpleTabs(920.0f, 450.0f) );
    wm->resizeAllWindows();
    viewer.addEventHandler(
            new osgWidget::MouseHandler(wm.get()) );
    viewer.addEventHandler(
            new osgWidget::KeyboardHandler(wm.get()) );
    viewer.addEventHandler( new osgWidget::ResizeHandler(wm.get(),
                                                         camera) );
    viewer.addEventHandler(
            new osgWidget::CameraSwitchHandler(wm.get(), camera) );

    //osg::Camera *camera = createHUDCamera(0, 1024, 0, 768);
    camera->addChild(textGeode.get());
    //camera->addChild( buttonGeode.get() );
    camera->getOrCreateStateSet()->setMode(
            GL_LIGHTING, osg::StateAttribute::OFF);
    osg::ref_ptr<osg::Group> root = createShapes();
    root->addChild( camera );
    viewer.setSceneData( root.get() );
    viewer.realize();

    //makeMove(5,-7,joint1);

    return viewer.run();
}