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
osg::Matrix initPosServo1;
osg::Matrix initPosServo2;
bool  setPointActive = true;

float setPointX = 0;
float setPointY = 0;

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
osg::MatrixTransform *joint7;
osg::MatrixTransform *joint8;
osg::MatrixTransform *joint9;
osg::MatrixTransform *joint10;
osg::MatrixTransform *joint11;
osg::MatrixTransform *joint12;
osg::MatrixTransform *joint13;
osg::MatrixTransform *joint14;
osg::MatrixTransform *joint15;
osg::MatrixTransform *joint16;
osg::MatrixTransform *joint17;
osg::MatrixTransform *joint18;
osg::MatrixTransform *joint19;
osg::MatrixTransform *joint20;

osg::MatrixTransform *buildjoint0(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint1(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint2(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint3(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint4(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint5(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint6(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint7();
osg::MatrixTransform *buildJoint8();
osg::MatrixTransform *buildJoint9(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint10(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint11(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint12(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint13();
osg::MatrixTransform *buildJoint14();
osg::MatrixTransform *buildJoint15();
osg::MatrixTransform *buildJoint16();
osg::MatrixTransform *buildJoint17();
osg::MatrixTransform *buildJoint18();
osg::MatrixTransform *buildJoint19();
osg::MatrixTransform *buildJoint20();
osg::MatrixTransform *buildEndEffector();

void makeMove(float x, float y);

osg::TessellationHints *hints = new osg::TessellationHints;

extern bool tabPressed( osgWidget::Event& ev );

void changeSetPoint(float x, float y){
    initPosBall.translate(x, 0, y);
    setPointX = x;
    setPointY = y;
}

int setPointsXfromPano(int xP, int yP){

    if(1000 + 40*0.8f<xP && xP<1000+(400*0.8f) - 40*0.8f
       &&50 + 30*0.8f<yP && yP<50+(300*0.8f) - 30*0.8f)
        return (xP-1000)/0.8f;
    else
        return  setPointX;
}

int setPointsYfromPano(int xP, int yP){

    if(1000 + 40*0.8f<xP && xP<1000+(400*0.8f) - 40*0.8f
       &&50 + 30*0.8f<yP && yP<50+(300*0.8f) - 30*0.8f)
        return  (yP-50)/0.8f;
    else
        return  setPointY;
}

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
    label->addSize( 0.0f, 25.0f );
    //label->setCanFill( true );
    return label.release();
}

osgWidget::Window* createSimpleTabs( float winX, float winY )
{
    osg::ref_ptr<osgWidget::Canvas> contents =
            new osgWidget::Canvas("contents");
    osg::ref_ptr<osgWidget::Box> tabs =
            new osgWidget::Box("tabs", osgWidget::Box::HORIZONTAL);

    string button1 = "Ball&Plate";
    string button2 = "FootballGame";
    //tabs->setScale(0.8f);

    osg::Vec4 color1(0.0f, (float)0 / 3.0f, 0.0f, 1.0f);
    std::stringstream ss, ss2;
    ss << "Button" ;
    ss2 << "Detected action:" <<" Button " << " is pressed" ;
    osgWidget::Label* content1 = createLabel(button1,
                                             ss2.str(), 11.0f, color1);
    content1->setLayer( osgWidget::Widget::LAYER_LOW, 0 );
    content1->setSize(480.0f,80.0f);
    contents->addWidget( content1, 0.0f, 0.0f );
    osgWidget::Label* tab1 = createLabel(button1,
                                         button1, 17.0f, color1);
    tab1->setEventMask( osgWidget::EVENT_MOUSE_PUSH );
    tab1->addCallback( new osgWidget::Callback(
            &tabPressed, osgWidget::EVENT_MOUSE_PUSH, content1) );
    tab1->setLayer( osgWidget::Widget::LAYER_TOP, 1 );
    tabs->addWidget( tab1);

    osg::Vec4 color(0.0f, (float)1 / 3.0f, 0.0f, 1.0f);

    osgWidget::Label* content2 = createLabel(button2,
                                             ss2.str(), 11.0f, color);
    content2->setLayer( osgWidget::Widget::LAYER_LOW, 0);
    content2->setSize(480.0f,78.0f);
    contents->addWidget( content2, 0.0f, 0.0f );
    osgWidget::Label* tab2 = createLabel(button2,
                                         button2, 17.0f, color);
    tab2->setEventMask( osgWidget::EVENT_MOUSE_PUSH );
    tab2->addCallback( new osgWidget::Callback(
            &tabPressed, osgWidget::EVENT_MOUSE_PUSH, content2) );
    tab2->setLayer( osgWidget::Widget::LAYER_TOP, 1 );
    tabs->addWidget( tab2 );
    tab1->setCanFill(true);
    tab2->setCanFill(true);

    osg::ref_ptr<osgWidget::Box> main =
            new osgWidget::Box("main", osgWidget::Box::VERTICAL);
    main->setOrigin( winX, winY );
    main->attachMoveCallback();
    main->addWidget( tabs->embed() );
    main->addWidget( createLabel("title", "Choose a Mode to Play >",
                                 17.0f, osg::Vec4(0.0f, 0.4f, 1.0f, 1.0f)) );
    main->setScale(0.7);
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
    cout<<content->getName();
    if(content->getName() == "FootballGame"){
        setPointActive = false;
        //changeGame1 = true;
        setPointX = 0;
        setPointY = 0;
    }
    if(content->getName() == "Ball&Plate"){
        setPointActive = true;
        //changeGame2 = true;
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

osgText::Text* createTextColored( const osg::Vec3& pos,osg::Vec4& color,
                           const std::string& content,
                           float size )
{
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    text->setFont( g_font.get() );
    text->setCharacterSize( size );
    text->setAxisAlignment( osgText::TextBase::XY_PLANE );
    text->setPosition( pos );
    text->setText( content );
    text->setColor(color);
    return text.release();
}

osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size )
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


class GeometryUpdateCallback : public osg::Geometry::UpdateCallback {
public:

    GeometryUpdateCallback(int _cornerX, int _cornerY, int _x, int _y, char axis){
        x = _x;
        y = _y;
        cornerX = _cornerX;
        cornerY = _cornerY;
        this->axis = axis;

        vertices = new osg::Vec3Array;
        for(int i=0; i<x; i++) {
            vertices->push_back(osg::Vec3(cornerX+1+i, cornerY, 0));
        }

    }

    virtual void update(osg::NodeVisitor *, osg::Drawable *drawable) {
        osg::Vec3 vecA;
        osg::Vec3 vecB;


        osg::Geometry *geometry = dynamic_cast<osg::Geometry *>(drawable);
        if (!geometry) return;

        vertices->erase(vertices->begin());
        for(int i=0; i<vertices->size()-1; i++) {
            vertices->at(i).set(cornerX+i, vertices->at(i+1).y(), 0);
        }
        if(axis == 'x')
            vertices->push_back(osg::Vec3(cornerX + x, cornerY + (rand() % y) , 0));
        else if(axis == 'y')
            vertices->push_back(osg::Vec3(cornerX + x, cornerY + (rand() % y) , 0));


        geometry->setVertexArray(vertices);
        geometry->removePrimitiveSet(0, 1);
        geometry->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP  , 0, vertices->size()));

    }

protected:
    int x ;
    int y ;
    int cornerX;
    int cornerY;
    char axis;
    osg::Vec3Array * vertices;

};

osg::Geometry *makeChart(int _cornerX, int _cornerY, int _x, int _y, char axis){

    osg::Geometry *linesGeom = new osg::Geometry();// is my geometry
    osg::DrawArrays *drawArrayLines = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);

    linesGeom->addPrimitiveSet(drawArrayLines);
    osg::Vec3Array *vertexData = new osg::Vec3Array;
    linesGeom->setVertexArray(vertexData);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(0.9f, 0.0f, 0.0f, 1.0f) );
    linesGeom->setColorArray( colors.get() );
    linesGeom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

    linesGeom->setUseDisplayList( false );
    linesGeom->setUseVertexBufferObjects( false );

    linesGeom->setUpdateCallback( new GeometryUpdateCallback(_cornerX, _cornerY, _x, _y, axis) );

    return linesGeom;

}

osg::Geometry *makeChartPano(int _cornerX, int _cornerY, int _x, int _y){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry

    int emptyEdge = 30;
    osg::Vec3Array *chartCorner = new osg::Vec3Array;
    chartCorner->push_back(osg::Vec3(_cornerX - emptyEdge, _cornerY- emptyEdge, 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x + emptyEdge, _cornerY - emptyEdge, 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x + emptyEdge, _cornerY + _y + emptyEdge, 0));
    chartCorner->push_back(osg::Vec3(_cornerX - emptyEdge,_cornerY + _y + emptyEdge, 0));

    chartGeom->setVertexArray(chartCorner);

    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(0.8f, 0.8f, 0.8f, 0.2f));
    chartGeom->setColorArray(colors);
    chartGeom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);


    osg::StateSet* stateset = chartGeom->getOrCreateStateSet();
    stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    return chartGeom;

}
osg::Geometry *makePano(int _cornerX, int _cornerY, int _x, int _y){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry

    int emptyEdge = 0;
    osg::Vec3Array *chartCorner = new osg::Vec3Array;
    chartCorner->push_back(osg::Vec3(_cornerX , _cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x , _cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x , _cornerY + _y, 0));
    chartCorner->push_back(osg::Vec3(_cornerX ,_cornerY + _y , 0));

    chartGeom->setVertexArray(chartCorner);

    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(0.8f, 0.8f, 0.8f, 0.6f));
    chartGeom->setColorArray(colors);
    chartGeom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);


    osg::StateSet* stateset = chartGeom->getOrCreateStateSet();
    stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    return chartGeom;

}

osg::Geometry *makePanoLines(int _cornerX, int _cornerY, int _x, int _y){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry


    osg::Vec3Array *chartCorner = new osg::Vec3Array;

    chartCorner->push_back(osg::Vec3(_cornerX+40*0.8f,_cornerY + _y - 30*0.8f, 0));
    chartCorner->push_back(osg::Vec3(_cornerX+40*0.8f, _cornerY +  30*0.8f, 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x - 40*0.8f, _cornerY + 30*0.8f, 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x - 40*0.8f, _cornerY + _y - 30*0.8f, 0));

    chartGeom->setVertexArray(chartCorner);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f) );
    chartGeom->setColorArray( colors.get() );
    chartGeom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

    chartGeom->setUseDisplayList( false );
    chartGeom->setUseVertexBufferObjects( false );

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    return chartGeom;

}

osg::Geometry *makeLines(int _cornerX, int _cornerY, int _x, int _y){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry


    osg::Vec3Array *chartCorner = new osg::Vec3Array;

    chartCorner->push_back(osg::Vec3(_cornerX ,_cornerY + _y , 0));
    chartCorner->push_back(osg::Vec3(_cornerX , _cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x , _cornerY , 0));

    chartGeom->setVertexArray(chartCorner);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f) );
    chartGeom->setColorArray( colors.get() );
    chartGeom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

    chartGeom->setUseDisplayList( false );
    chartGeom->setUseVertexBufferObjects( false );

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, 3));

    return chartGeom;

}

class GeoSetLinesUpdateCallback : public osg::Geometry::UpdateCallback {
public:
    GeoSetLinesUpdateCallback(int _cornerX, int _cornerY, int _x, char _axis ){
        x = _x;
        cornerX = _cornerX;
        cornerY = _cornerY;
        axis = _axis;

    }

    virtual void update(osg::NodeVisitor *, osg::Drawable *drawable) {

        osg::Geometry *geometry = dynamic_cast<osg::Geometry *>(drawable);
        if (!geometry) return;

        osg::Vec3Array *chartCorner = new osg::Vec3Array;

        if(axis == 'x') {
            chartCorner->push_back(osg::Vec3(cornerX, cornerY + setPointX/4.0f, 0));
            chartCorner->push_back(osg::Vec3(cornerX + x, cornerY + setPointX/4.0f, 0));
        }
        else if(axis == 'y'){
            chartCorner->push_back(osg::Vec3(cornerX, cornerY + setPointY/4.0f, 0));
            chartCorner->push_back(osg::Vec3(cornerX + x, cornerY + setPointY/4.0f, 0));
        }

        geometry->setVertexArray(chartCorner);

        osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
        colors->push_back( osg::Vec4(0.0f, 0.0f, 0.5f, 0.0f) );
        geometry->setColorArray( colors.get() );
        geometry->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

        geometry->setUseDisplayList( false );
        geometry->setUseVertexBufferObjects( false );

        geometry->removePrimitiveSet(0, 1);
        geometry->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, 2));

    }

protected:
    int x ;
    int cornerX;
    int cornerY;
    char axis;
    osg::Vec3Array * vertices;

};

osg::Geometry *makeSetPointLine(int _cornerX, int _cornerY, int _x, char axis){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry

    int line = 5;

    osg::Vec3Array *chartCorner = new osg::Vec3Array;

    chartCorner->push_back(osg::Vec3(_cornerX ,_cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX + _x,_cornerY , 0));

    chartGeom->setVertexArray(chartCorner);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(0.0f, 0.0f, 0.5f, 0.0f) );
    chartGeom->setColorArray( colors.get() );
    chartGeom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

    chartGeom->setUseDisplayList( false );
    chartGeom->setUseVertexBufferObjects( false );

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, 2));
    chartGeom->setUpdateCallback( new GeoSetLinesUpdateCallback(_cornerX, _cornerY, _x, axis) );

    return chartGeom;

}


osg::Geometry *makeGuideLines(int _cornerX, int _cornerY, int _x, int _y, char axes){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry
    int emptyBetween;

    if(axes == 'y')
        emptyBetween = _y/6;
    else if(axes == 'x')
        emptyBetween = _y/8;
    int line = 5;

    osg::Vec3Array *chartCorner = new osg::Vec3Array;

    for(int i = 0; i< 8; i++){
        chartCorner->push_back(osg::Vec3(_cornerX ,_cornerY + (i+1)*emptyBetween, 0));
        chartCorner->push_back(osg::Vec3(_cornerX + line + _x,_cornerY + (i+1)*emptyBetween, 0));
    }

    chartGeom->setVertexArray(chartCorner);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(0.7f, 0.7f, 0.7f, 0.1f) );
    chartGeom->setColorArray( colors.get() );
    chartGeom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

    chartGeom->setUseDisplayList( false );
    chartGeom->setUseVertexBufferObjects( false );

    if(axes == 'y')
        chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 12));
    else if(axes == 'x')
        chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 16));

    osg::StateSet* stateset = chartGeom->getOrCreateStateSet();
    stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);


    return chartGeom;

}
osg::Geometry *makeShortLines(int _cornerX, int _cornerY, int _x, int _y, char axis){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry
    float emptyBetween;

    if(axis == 'y')
        emptyBetween = _y/6.0f;
    else if(axis == 'x')
        emptyBetween = _y/8.0f;
    int line = 5;

    osg::Vec3Array *chartCorner = new osg::Vec3Array;

    for(int i = 0; i< 8; i++){
        chartCorner->push_back(osg::Vec3(_cornerX - line,_cornerY + (i+1)*emptyBetween-1, 0));
        chartCorner->push_back(osg::Vec3(_cornerX,_cornerY + (i+1)*emptyBetween-1, 0));
    }

    chartGeom->setVertexArray(chartCorner);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f) );
    chartGeom->setColorArray( colors.get() );
    chartGeom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

    chartGeom->setUseDisplayList( false );
    chartGeom->setUseVertexBufferObjects( false );

    if(axis == 'y')
        chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 12));
    else if(axis == 'x')
        chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 16));


    return chartGeom;

}


void makeAllTextYChart(osg::ref_ptr<osg::Geode> geode, int _cornerX, int _cornerY, int _y){

    int emptyEdge = 30;

    geode->addDrawable( createText(
            osg::Vec3(_cornerX - emptyEdge + 5, _cornerY + _y + 10, 0),
            "Y-Error Chart",
            15.0f)
    );

    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 12 , _cornerY-3, 0),
            "0",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 18 , _cornerY + _y/6-3, 0),
            "50",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 2*_y/6-3, 0),
            "100",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 3*_y/6-3, 0),
            "150",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 4*_y/6-3, 0),
            "200",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 5*_y/6-3, 0),
            "250",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 6*_y/6-3, 0),
            "300",
            10.0f)
    );
}
void makeAllTextXChart(osg::ref_ptr<osg::Geode> geode, int _cornerX, int _cornerY, int _y){

    int emptyEdge = 30;

    geode->addDrawable( createText(
            osg::Vec3(_cornerX - emptyEdge + 5, _cornerY + _y + 10, 0),
            "X-Error Chart",
            15.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 12 , _cornerY-3, 0),
            "0",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 18 , _cornerY + _y/8-3, 0),
            "50",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 2*_y/8-3, 0),
            "100",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 3*_y/8-3, 0),
            "150",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 4*_y/8-3, 0),
            "200",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 5*_y/8-3, 0),
            "250",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 6*_y/8-3, 0),
            "300",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 7*_y/8-3, 0),
            "350",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 8*_y/8-3, 0),
            "400",
            10.0f)
    );
}

void drawCharts(osg::ref_ptr<osg::Geode> geode,float x, float y){

    geode->addDrawable(makeChart(x, y+150, 250, 100, 'x'));
    geode->addDrawable(makeLines(x, y+150, 250, 100));
    geode->addDrawable(makeShortLines(x, y+150, 250, 100, 'x'));
    geode->addDrawable(makeGuideLines(x, y+150, 250, 100, 'x'));
    geode->addDrawable(makeSetPointLine(x, y+150, 250, 'x'));
    geode->addDrawable(makeChartPano(x, y+150, 250, 100));

    geode->addDrawable(makeChart(x, y, 250, 75, 'y'));
    geode->addDrawable(makeLines(x, y, 250, 75));
    geode->addDrawable(makeShortLines(x, y, 250, 75, 'y'));
    geode->addDrawable(makeGuideLines(x, y, 250, 75, 'y'));
    geode->addDrawable(makeSetPointLine(x, y, 250, 'y'));
    geode->addDrawable(makeChartPano(x, y, 250, 75));

    //sağ alttaki panoyu çiz
    geode->addDrawable(makePano( 1000.0, 50.0, 400*0.8f, 300*0.8f));
    geode->addDrawable(makePanoLines( 1000.0, 50.0, 400*0.8f, 300*0.8f));


}

// class to handle events with a pick
class PickHandler : public osgGA::GUIEventHandler
{
    float x, y;
public:

    PickHandler():
            _mx(0.0),_my(0.0){}

    ~PickHandler() {}

    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if (!viewer) return false;

        switch(ea.getEventType())
        {
            case(osgGA::GUIEventAdapter::PUSH):
            {
                if(setPointActive){
                    _mx = ea.getX();
                    _my = ea.getY();
                    x = setPointsXfromPano(_mx, _my);
                    y = setPointsYfromPano(_mx, _my);
                    changeSetPoint(x,y);
                    osg::notify(osg::NOTICE)<<"setPointX = "<<setPointX<<", "<<_mx<<std::endl;
                    osg::notify(osg::NOTICE)<<"setPointY = "<<setPointY<<", "<<_my<<std::endl;
                }

                return false;
            }
            default:
                return false;
        }
    }
protected:
    float _mx,_my;
};

class KeyboardEventHandler : public osgGA::GUIEventHandler {
public:

    KeyboardEventHandler() {}


    static void rotateY(float angle, osg::MatrixTransform *joint) {
        osg::Matrix yRot;
        yRot.makeRotate(angle, 0.0, 1.0, 0.0);
        //joint->setMatrix(joint->getMatrix()*yRot );
        joint->setMatrix(initPosPlate*yRot );
        osg::Matrix rot;
        rot.makeRotate(-angle/3, 1.0, 0.0, 0.0);
        joint10->setMatrix(initPosServo2*rot);
    }

    static void rotateX(float angle, osg::MatrixTransform *joint) {
        osg::Matrix xRot;
        xRot.makeRotate(angle, 1.0, 0.0, 0.0);
        //joint->setMatrix(joint->getMatrix()*xRot);
        joint->setMatrix(initPosPlate*xRot );
        osg::Matrix rot;
        rot.makeRotate(angle/3, 1.0, 0.0, 0.0);
        joint4->setMatrix(rot*initPosServo1);
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
            case (osgGA::GUIEventAdapter::KEYDOWN): {
                switch (ea.getKey()) {
                    case 'd':
                        rotateY(-10*osg::PI/180, joint1);
                        translate(0.2, 0.0, 0.0,joint2);
                        return true;
                    case 'a':
                        rotateY(10*osg::PI/180, joint1);
                        translate(-0.2, 0.0, 0.0,joint2);
                        return true;
                    case 's':
                        rotateX(10*-osg::PI/180, joint1);
                        translate(0.0, 0.0, -0.2,joint2);
                        return true;
                    case 'w':
                        rotateX(10 *osg::PI/180, joint1);
                        translate(0.0, 0.0, 0.2,joint2);
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

void makeMove(float x, float y){
    KeyboardEventHandler::rotateX(-(setPointX-x)*osg::PI/180, joint1);
    KeyboardEventHandler::rotateY(-(setPointY-y)*osg::PI/180, joint1);
    KeyboardEventHandler::translate((setPointX-x), 0.0, (setPointY-y),joint2);
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

void addTexture(osg::Geode* shape, string file){
    osg::Texture2D* KLN89FaceTexture = new osg::Texture2D;

    // protect from being optimized away as static state:
    KLN89FaceTexture->setDataVariance(osg::Object::DYNAMIC);

    // load an image by reading a file:
    osg::Image* klnFace = osgDB::readImageFile("KLN89FaceB.tga");
    if (!klnFace)
    {
        std::cout << " couldn't find texture, quiting." << std::endl;
        return ;
    }

    // Assign the texture to the image we read from file:
    KLN89FaceTexture->setImage(klnFace);

    // Create a new StateSet with default settings:
    osg::StateSet* stateOne = new osg::StateSet();

    // Assign texture unit 0 of our new StateSet to the texture
    // we just created and enable the texture.
    stateOne->setTextureAttributeAndModes
            (0,KLN89FaceTexture,osg::StateAttribute::ON);
    // Associate this state set with the Geode that contains
    // the pyramid:
    shape->setStateSet(stateOne);
   /* // create a simple material
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
    //texture->setWrap(Texture::WRAP_S, Texture::CLAMP);
    //texture->setWrap(Texture::WRAP_T, Texture::CLAMP);
    texture->setImage(image);

    StateSet *sphereStateSet = shape->getOrCreateStateSet();
    sphereStateSet->ref();
    sphereStateSet->setAttribute(material);
    sphereStateSet->setTextureAttributeAndModes(0, texture, StateAttribute::ON);*/
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
    joint1 = buildJoint1(joint0);
    joint2 = buildJoint2(joint1);
    joint2->addChild(buildEndEffector());

    //joint2->setNodeMask(0x0);

    joint3 = buildJoint3(joint0);
    joint4 = buildJoint4(joint3);
    joint5 = buildJoint5(joint4);
    joint6 = buildJoint6(joint5);
    joint7 = buildJoint7();
    joint6->addChild(joint7);
    joint8 = buildJoint8();
    joint2->addChild(joint8);

    joint9 = buildJoint9(joint0);
    joint10 = buildJoint10(joint9);
    joint11 = buildJoint11(joint10);
    joint12 = buildJoint12(joint11);
    joint13 = buildJoint13();
    joint12->addChild(joint13);
    joint14 = buildJoint14();
    joint2->addChild(joint14);

    joint1->addChild(buildJoint15());
    joint1->addChild(buildJoint16());
    joint1->addChild(buildJoint17());
    joint1->addChild(buildJoint18());
    joint1->addChild(buildJoint19());
    joint1->addChild(buildJoint20());

    //joint14->setCullingActive(false);

    initPosPlate = joint1->getMatrix();
    initPosBall = joint2->getMatrix();
    initPosServo1 = joint4->getMatrix();
    initPosServo2 = joint10->getMatrix();

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

osg::MatrixTransform *buildJoint3(osg::MatrixTransform *previousJoint) {
    double length = 2.0;
    double width = 2.5;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.6f, -9.2f, 4.0f), width, 0.8, length), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();

    xTransform->addChild(zTransform);

    addTexture(plate, "cubuk.bmp");

    zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint4(osg::MatrixTransform *previousJoint) {
    double length = 2.0;
    double width = 2.0;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.6f, -7.8f, 4.0f), 0.8, width, length), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();

    xTransform->addChild(zTransform);

    addTexture(plate, "siyah.bmp");

    zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint5(osg::MatrixTransform *previousJoint) {
    double height = 1.0;
    double radius = 0.1;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.6f, -7.5f, 5.0f), radius, height),hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();

    xTransform->addChild(zTransform);

    addTexture(plate, "servo.bmp");

    zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint6(osg::MatrixTransform *previousJoint) {
    double length = 2.0;
    double width = 2.0;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.6f, -7.5f, 5.5f), width , 0.3 ,0.1 ), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();

    xTransform->addChild(zTransform);

    addTexture(plate, "servo.bmp");

    zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint7() {
    double height = 7.0;
    double radius = 0.1;

    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix xRot = osg::Matrix::rotate(-osg::PI_2, 1.0, 0.0, 0.0);
    //xTransform->setMatrix(xRot);
    osg::Matrix m;

    m.makeTranslate(0, 0, 0);    //coordinate of the box
    mt->setMatrix(xRot);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f, -5.5f, -3.9f), radius, height),hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));

    geode_3->addDrawable(shape1);

    addTexture(shape1, "servo.bmp");

    mt->addChild(geode_3);

    //mt->setNodeMask( castShadowMask );
    return mt;
}

osg::MatrixTransform *buildJoint8() {
    double length = 1.5;
    double width = 1.5;

    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix xRot = osg::Matrix::rotate(-osg::PI_2, 1.0, 0.0, 0.0);
    //xTransform->setMatrix(xRot);
    osg::Matrix m;

    m.makeTranslate(0, 0, 0);    //coordinate of the box
    mt->setMatrix(xRot);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Box(osg::Vec3(-5.5f, 0.0f, -10.5f), width, 1.5, length),hints);
    //shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.9f, 1.0f));

    geode_3->addDrawable(shape1);

    addTexture(shape1, "altTahta.bmp");

    mt->addChild(geode_3);

    //mt->setNodeMask( castShadowMask );
    return mt;
}

osg::MatrixTransform *buildJoint9(osg::MatrixTransform *previousJoint) {
    double length = 2.0;
    double width = 2.5;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(-osg::PI_2, 0.0, 1.0, 0.0);
    xTransform->setMatrix(xRot);


    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.6f, -9.2f, 4.0f), width, 0.8, length), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();

    xTransform->addChild(zTransform);

    addTexture(plate, "cubuk.bmp");

    zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint10(osg::MatrixTransform *previousJoint) {
    double length = 2.0;
    double width = 2.0;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    /*osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 1.0, 0.0);
    xTransform->setMatrix(xRot);*/

    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.6f, -7.8f, 4.0f), 0.8, width, length), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();

    xTransform->addChild(zTransform);

    addTexture(plate, "siyah.bmp");

    zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint11(osg::MatrixTransform *previousJoint) {
    double height = 1.0;
    double radius = 0.1;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    /*osg::Matrix xRot = osg::Matrix::rotate(-osg::PI_2, 1.0, 0.0, 0.0);
    xTransform->setMatrix(xRot);*/

    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.6f, -7.5f, 5.0f), radius, height),hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();

    xTransform->addChild(zTransform);

    addTexture(plate, "servo.bmp");

    zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint12(osg::MatrixTransform *previousJoint) {
    double length = 2.0;
    double width = 2.0;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    /*osg::Matrix xRot = osg::Matrix::rotate(-osg::PI_2, 0.0, 1.0, 0.0);
    xTransform->setMatrix(xRot);*/

    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.6f, -7.5f, 5.5f), width , 0.3 ,0.1 ), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();

    xTransform->addChild(zTransform);

    addTexture(plate, "servo.bmp");

    zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint13() {
    double height = 7.0;
    double radius = 0.1;

    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix xRot = osg::Matrix::rotate(-osg::PI_2, 1.0, 0.0, 0.0);
    mt->setMatrix(xRot);

    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3( 0.0f, -5.5f, -3.9f), radius, height),hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));

    geode_3->addDrawable(shape1);

    addTexture(shape1, "servo.bmp");

    mt->addChild(geode_3);

    //mt->setNodeMask( castShadowMask );
    return mt;
}

osg::MatrixTransform *buildJoint14() {
    double length = 1.5;
    double width = 1.5;

    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix xRot = osg::Matrix::rotate(-osg::PI_2, 1.0, 0.0, 0.0);
    //xTransform->setMatrix(xRot);
    osg::Matrix m;

    m.makeTranslate(0, 0, 0);    //coordinate of the box
    mt->setMatrix(xRot);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 5.5f, -10.5f), 1.5, width, length ),hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.9f, 1.0f));

    geode_3->addDrawable(shape1);

    addTexture(shape1, "altTahta.bmp");

    mt->addChild(geode_3);

    //mt->setNodeMask( castShadowMask );
    return mt;
}

osg::MatrixTransform *buildJoint15()  //The Cylinder at the button
{
    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix m;
    float radius = 6.5f;
    float height = 0.2f;
    //double radius = 1.0;
    m.makeTranslate(0, 0, 0);    //coordinate of the box
    mt->setMatrix(m);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Box(osg::Vec3(9.0f, 0.0f, -5.35f), height, radius, 1.0f), hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.9f, 1.0f));


    geode_3->addDrawable(shape1);

    addTexture(shape1, "cubuk.bmp");

    mt->addChild(geode_3);

    mt->setNodeMask( castShadowMask );
    return mt;
}

osg::MatrixTransform *buildJoint16()  //The Cylinder at the button
{
    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix m;
    float radius = 0.1f;
    float height = 4.0f;
    //double radius = 1.0;
    m.makeTranslate(0, 0, 0);    //coordinate of the box
    mt->setMatrix(m);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(9.0f,3.0f, -7.5f), radius, height),hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.9f, 1.0f));


    geode_3->addDrawable(shape1);

    addTexture(shape1, "cubuk.bmp");

    mt->addChild(geode_3);

    mt->setNodeMask( castShadowMask );
    return mt;
}

osg::MatrixTransform *buildJoint17()  //The Cylinder at the button
{
    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix m;
    float radius = 0.1f;
    float height = 4.0f;
    //double radius = 1.0;
    m.makeTranslate(0, 0, 0);    //coordinate of the box
    mt->setMatrix(m);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(9.0f,-3.0f, -7.5f), radius, height),hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.9f, 1.0f));


    geode_3->addDrawable(shape1);

    addTexture(shape1, "cubuk.bmp");

    mt->addChild(geode_3);

    mt->setNodeMask( castShadowMask );
    return mt;
}

//gENİŞ kENAR

osg::MatrixTransform *buildJoint18()  //The Cylinder at the button
{
    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix m;
    float radius = 6.5f;
    float height = 0.2f;
    //double radius = 1.0;
    m.makeTranslate(0, 0, 0);    //coordinate of the box
    mt->setMatrix(m);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Box(osg::Vec3(-9.0f, 0.0f, -5.35f), height, radius, 1.0f), hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.9f, 1.0f));

    geode_3->addDrawable(shape1);

    addTexture(shape1, "cubuk.bmp");

    mt->addChild(geode_3);

    mt->setNodeMask( castShadowMask );
    return mt;
}

osg::MatrixTransform *buildJoint19()  //The Cylinder at the button
{
    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix m;
    float radius = 0.1f;
    float height = 4.0f;
    //double radius = 1.0;
    m.makeTranslate(0, 0, 0);    //coordinate of the box
    mt->setMatrix(m);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(-9.0f,3.0f, -7.5f), radius, height),hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.9f, 1.0f));


    geode_3->addDrawable(shape1);

    addTexture(shape1, "cubuk.bmp");

    mt->addChild(geode_3);

    mt->setNodeMask( castShadowMask );
    return mt;
}

osg::MatrixTransform *buildJoint20()  //The Cylinder at the button
{
    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix m;
    float radius = 0.1f;
    float height = 4.0f;
    //double radius = 1.0;
    m.makeTranslate(0, 0, 0);    //coordinate of the box
    mt->setMatrix(m);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(-9.0f,-3.0f, -7.5f), radius, height),hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.9f, 1.0f));


    geode_3->addDrawable(shape1);

    addTexture(shape1, "cubuk.bmp");

    mt->addChild(geode_3);

    //mt->setNodeMask( castShadowMask );
    return mt;
}

osg::MatrixTransform *buildEndEffector() {
    double radius = 1.0;

    osg::MatrixTransform *mt = new osg::MatrixTransform();
    osg::Matrix m;

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
    viewer.createDistortionTexture(50,50);
    // add model to viewer.
    //viewer.setSceneData(createShapes());

    setCameraPosision(viewer);

    viewer.setUpViewInWindow(10, 35, 1350, 680);
//--------------------------------------------Text------------------------------------------------

    osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;

    osg::Vec4 color = osg::Vec4(0.5f, 0.9f, 0.1f, 1.0f);

    textGeode->addDrawable(createTextColored(osg::Vec3(450.0f, 650.0f, 0.0f), color,
            "Project: Ball&Plate", 30.0f));

    textGeode->addDrawable(createTextColored( osg::Vec3(600.0f, 600.0f, 0.0f), color,
            "- by Group 1", 15.0f));

    color = osg::Vec4(0.9f, 0.9f, 0.9f, 1.0f);
    textGeode->addDrawable(createTextColored( osg::Vec3(1020.0f, 315.0f, 0.0f), color,
            "click to select a setPoint :", 15.0f));

//-----------------------------------------------Grafics------------------------------------------
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    drawCharts(geode, 50.0, 300.0);
    makeAllTextXChart(geode,50, 450, 100);
    makeAllTextYChart(geode,50, 300, 75);


    // geode'de derinlik kavramını kaldırıdm
    osg::StateSet* stateset = geode->getOrCreateStateSet();
    stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    stateset->setRenderBinDetails(11,"RenderBin");

    osg::ref_ptr<osg::Geode> screenGeode = new osg::Geode;
    osg::Geometry * screen = makePano( 1000.0, 50.0, 400*0.8, 300*0.8);
    screenGeode->addDrawable(screen);

    addTexture(screenGeode, "tahta.bmp");

//-----------------------------------------------layer/buttons----------------------------------------
    osg::ref_ptr<osgWidget::WindowManager> wm =
            new osgWidget::WindowManager(&viewer, 1350.0f,
                                         680.0f, 0xf0000000);
    osg::Camera* camera = wm->createParentOrthoCamera();
    wm->addChild( createSimpleTabs(985.0f, 485.0f) );
    wm->resizeAllWindows();
    viewer.addEventHandler(
            new osgWidget::MouseHandler(wm.get()) );
    viewer.addEventHandler(
            new osgWidget::KeyboardHandler(wm.get()) );
    viewer.addEventHandler( new osgWidget::ResizeHandler(wm.get(),
                                                         camera) );
    viewer.addEventHandler(
            new osgWidget::CameraSwitchHandler(wm.get(), camera) );
    viewer.addEventHandler(new PickHandler());

    //osg::Camera *camera = createHUDCamera(0, 1024, 0, 768);
    camera->addChild(textGeode.get());
    camera->addChild(geode);
    camera->addChild(screenGeode);
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