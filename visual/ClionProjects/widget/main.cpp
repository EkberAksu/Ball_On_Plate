/*#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgText/Font>
#include <osgText/Text>
#include <osgViewer/Viewer>

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
    return text.release();
}

int main(int, char **){

    osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
    textGeode->addDrawable(createText(
            osg::Vec3(150.0f, 500.0f, 0.0f),
            "Sahin geri zekalii",
            20.0f)
    );
    textGeode->addDrawable(createText(
            osg::Vec3(150.0f, 450.0f, 0.0f),
            "Yazilimdan kaldin!!!",
            15.0f)
    );

    osg::Camera *camera = createHUDCamera(0, 1024, 0, 768);
    camera->addChild(textGeode.get());
    camera->getOrCreateStateSet()->setMode(
            GL_LIGHTING, osg::StateAttribute::OFF);

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(osgDB::readNodeFile("cessna.osg"));
    root->addChild(camera);
    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());
    return viewer.run();
}*/

#include <osgWidget/Util>
#include <osgWidget/WindowManager>
#include <osgWidget/Canvas>

const unsigned int MASK_2D = 0xF0000000;

bool colorWidgetEnter(osgWidget::Event& event) {
    event.getWidget()->addColor(0.5f, 0.2f, 0.3f, 0.0f);

    // osgWidget::warn() << "WIDGET mouseEnter " << event.getWidget()->getName() << std::endl;

    return false;
}


bool colorWidgetLeave(osgWidget::Event& event) {
    event.getWidget()->addColor(-0.5f, -0.2f, -0.3f, 0.0f);

    // osgWidget::warn() << "WIDGET mouseLeave" << std::endl;

    return true;
}





osgWidget::Widget* createWidget(
        const std::string&       name,
        osgWidget::color_type    col,
        osgWidget::Widget::Layer layer
) {
    osgWidget::Widget* widget = new osgWidget::Widget(name, 200.0f, 200.0f);

    widget->setEventMask(osgWidget::EVENT_ALL);
    widget->addCallback(new osgWidget::Callback(&colorWidgetEnter, osgWidget::EVENT_MOUSE_PUSH));
    widget->addCallback(new osgWidget::Callback(&colorWidgetLeave, osgWidget::EVENT_MOUSE_RELEASE));
    widget->addCallback(new osgWidget::Callback(&colorWidgetEnter, osgWidget::EVENT_MOUSE_ENTER));
    widget->addCallback(new osgWidget::Callback(&colorWidgetLeave, osgWidget::EVENT_MOUSE_LEAVE));
//    widget->addCallback(new osgWidget::Callback(&widgetMouseOver, osgWidget::EVENT_MOUSE_OVER));
    widget->setColor(col, col, col, 0.5f);
    widget->setLayer(layer);

    return widget;
}

int main(int, char**)
{
    osgViewer::Viewer viewer;

    osgWidget::WindowManager* wm = new osgWidget::WindowManager(
            &viewer,
            1280.0f,
            1024.0f,
            MASK_2D,
            osgWidget::WindowManager::WM_PICK_DEBUG
    );

    osgWidget::Canvas* canvas = new osgWidget::Canvas("canvas");

    // canvas->addCallback(new osgWidget::Callback(&windowMouseOver, osgWidget::EVENT_MOUSE_OVER));
    canvas->attachMoveCallback();
    canvas->attachRotateCallback();
    canvas->attachScaleCallback();

    canvas->   addWidget    (
            createWidget("w1", 0.2f, osgWidget::Widget::LAYER_LOW),
            0.0f,
            0.0f
    );

    canvas->addWidget(
            createWidget("w2", 0.4f, osgWidget::Widget::LAYER_MIDDLE),
            200.0f,
            0.0f
    );

    /*  canvas->addWidget(
              createWidget("w3", 0.6f, osgWidget::Widget::LAYER_HIGH),
              400.0f,
              0.0f
      );
  */
    // Add a child and then resize it relatively to the size of the parent Window.
    osgWidget::Widget* relWidget = new osgWidget::Widget("relative");

    relWidget->setLayer(osgWidget::Widget::LAYER_LOW, 1);
    relWidget->setCoordinateMode(osgWidget::Widget::CM_RELATIVE);
    relWidget->setSize(0.2f, 0.2f);
    relWidget->setColor(0.5f, 0.5f, 0.1f, 0.9f);

    osgWidget::warn() << canvas->getWidth() << std::endl;

    canvas->addWidget(relWidget, 0.4f, 0.4f);

    relWidget->addOrigin(0.1f, 0.1f);
    relWidget->addSize(0.2f, 0.2f);

    canvas->resize();

    // Finally, add the whole thing to the WindowManager.
    wm->addChild(canvas);

    return osgWidget::createExample(viewer, wm);
}



