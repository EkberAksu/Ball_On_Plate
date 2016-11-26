/*
 * Create by Şahin Eğilmez
 */

#include "plate.h"

#include <QGuiApplication>

#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qcameralens.h>

#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

#include <Qt3DInput/QInputAspect>

#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qtexture.h>

#include <Qt3DCore/qaspectengine.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DRender/qrenderaspect.h>

#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DExtras/qt3dwindow.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "rotatingtrefoilknot.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
  view->defaultFramegraph()->setClearColor(QColor(QRgb(0xFFFFFF)));
  QWidget *container = QWidget::createWindowContainer(view);
  QSize screenSize = view->screen()->size();
  container->setMinimumSize(QSize(200, 100));
  container->setMaximumSize(screenSize);

  QWidget *widget = new QWidget;
  QHBoxLayout *hLayout = new QHBoxLayout(widget);
  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignTop);
  hLayout->addWidget(container, 1);
  hLayout->addLayout(vLayout);

  widget->setWindowTitle(QStringLiteral("Ball And PLate"));

  Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
  view->registerAspect(input);

  // Root entity
  Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

  // Camera
  Qt3DRender::QCamera *cameraEntity = view->camera();

  cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f,
                                                 1000.0f);
  cameraEntity->setPosition(QVector3D(0, 15, 15.0f));
  cameraEntity->setUpVector(QVector3D(0, 1, 0));
  cameraEntity->setViewCenter(QVector3D(0, 0, 0));

  // For camera controls
  Qt3DExtras::QFirstPersonCameraController *camController =
      new Qt3DExtras::QFirstPersonCameraController(rootEntity);
  camController->setCamera(cameraEntity);

  // Plate
  Plate *plate = new Plate(rootEntity);
  // Set root object of the scene
  view->setRootEntity(rootEntity);

  // Create control widgets
  QCommandLinkButton *info = new QCommandLinkButton();
  info->setText(QStringLiteral("Qt3D Ball And PLate Visualization"));
  info->setDescription(QString::fromLatin1(
      "Burada simdilik top x koordinatinda artan hareket yapmakta. Gelistirilecektir! "));
  info->setIconSize(QSize(0, 0));
  vLayout->addWidget(info);

  // Show window
  widget->show();
  widget->resize(1200, 800);

  // Ball
  Ball *ball = new Ball(rootEntity);
  view->setRootEntity(rootEntity);
  view->show();

  app.exec();

  return 1;
}
