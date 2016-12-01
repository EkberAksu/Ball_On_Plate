#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>



float jointAngle1;
float jointAngle2;
float EndEffector;

osg::MatrixTransform* joint1;
osg::MatrixTransform* joint2;

osg::MatrixTransform* buildJoint1(osg::MatrixTransform* previousJoint);
osg::MatrixTransform* buildJoint2(osg::MatrixTransform* previousJoint);
osg::MatrixTransform* buildEndEffector();
osg::TessellationHints* hints = new osg::TessellationHints;


class KeyboardEventHandler : public osgGA::GUIEventHandler
{
public:

    KeyboardEventHandler()
    {}


    static void rotateX(float angle,osg::MatrixTransform *joint)
    {
        osg::Matrix zRot;
        zRot.makeRotate(angle, 0.0,1.0,0.0);
        joint->setMatrix(zRot*joint->getMatrix());
    }

    static void rotateZ(float angle,osg::MatrixTransform *joint)
    {
        osg::Matrix zRot;
        zRot.makeRotate(angle, 1.0,0.0,0.0);
        joint->setMatrix(zRot*joint->getMatrix());
    }

    static void translate(float x,float y,float z, osg::MatrixTransform *joint){
        osg::Matrix trans;
        trans.makeTranslate(x,y,z);
        joint->setMatrix(trans*joint->getMatrix());
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
    {
        switch (ea.getEventType())
        {
            case(osgGA::GUIEventAdapter::KEYDOWN):
            {
                switch (ea.getKey())
                {
                    case 'q':
                        translate(0.0, 0.0, 0.2,joint2);
                        rotateX(osg::PI/180, joint1);
                        return true;
                    case 'a':
                        translate(0.0, 0.0, -0.2,joint2);
                        rotateX(-osg::PI/180, joint1);
                        return true;
                    case 'w':
                        translate(-0.2, 0.0, 0.0,joint2);
                        rotateZ(osg::PI/180, joint1);
                        return true;
                    case 's':
                        translate(0.2, 0.0, 0.0,joint2);
                        rotateZ(-osg::PI/180, joint1);
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


osg::Group* createShapes()
{
    osg::Group* group = new osg::Group();
    osg::MatrixTransform* transform =new osg::MatrixTransform();
    group->addChild(transform);

    joint1 = buildJoint1(transform);
    joint2 = buildJoint2(joint1);
    joint2->addChild( buildEndEffector());
    return group;
}

osg::MatrixTransform* buildJoint1(osg::MatrixTransform* previousJoint)  //The Cylinder at the button
{
    osg::MatrixTransform* xTransform =new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    float radius = 1.0f;
    float height = 15.0f;
    osg::Geode* joint = new osg::Geode();
    xTransform->addChild(joint);
    joint->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f,0.0f,height/2),radius,height),hints));//cylinder's coordinate and size

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    xTransform->addChild(zTransform);
    osg::Matrix zTrans=osg::Matrix::translate(0.0, 0.0, height);
    osg::Matrix zRot= osg::Matrix::rotate(jointAngle1, 0.0, 0.0, 1.0);
    zTransform->setMatrix(zTrans*zRot);
    return zTransform;
}

osg::MatrixTransform* buildJoint2(osg::MatrixTransform* previousJoint)
{

    double length = 17.0;

    osg::MatrixTransform* xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 1.0, 0.0, 0.0);
    xTransform->setMatrix(xRot);
    osg::Geode* joint = new osg::Geode();//
    joint->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), length, 1.0f, length), hints));
    xTransform->addChild(joint);

    osg::MatrixTransform* zTransform = new osg::MatrixTransform();
    osg::Matrix zTrans = osg::Matrix::translate( 0.0, 0.0, 0);
    osg::Matrix zRot = osg::Matrix::rotate(osg::PI_2+jointAngle2, 0.0,1.0,0.0);
    zTransform->setMatrix(zTrans*zRot);
    xTransform->addChild(zTransform);
    return zTransform;
}

osg::MatrixTransform* buildEndEffector()
{
    osg::MatrixTransform* mt = new osg::MatrixTransform();
    osg::Matrix m;

    double radius = 1.0;
    m.makeTranslate(0,0,0);    //coordinate of the box
    mt->setMatrix(m);
    osg::Geode *geode_3 = new osg::Geode;
    osg::ShapeDrawable *shape1 = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,1.5f,0.0f),radius),hints);
    shape1->setColor(osg::Vec4(0.5f, 0.5f, 0.9f, 1.0f));


    geode_3->addDrawable(shape1);
    mt->addChild(geode_3);
    return mt;
}

int main(int, char **)
{
    hints->setDetailRatio(0.5f);

    jointAngle1=0.0;
    jointAngle2=0.0;
    EndEffector=1.0;
    osgViewer::Viewer viewer;
    viewer.addEventHandler(new KeyboardEventHandler());

    // add model to viewer.
    viewer.setSceneData( createShapes() );
    return viewer.run();
}

