#include "ball.h"

Ball::Ball(Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent), m_mesh(new Qt3DExtras::QSphereMesh()),
      m_transform(new Qt3DCore::QTransform()), m_position(), m_scale(0.4f),
      m_thetaAnimation(new QPropertyAnimation(this)),
      m_phiAnimation(new QPropertyAnimation(this)),
      ballMaterial(new Qt3DExtras::QPhongMaterial()) {
  m_mesh->setRings(20);
  m_mesh->setSlices(20);
  m_mesh->setRadius(1);

  m_transform->setScale(m_scale);
  m_transform->setTranslation(QVector3D(x, y, z));

  ballMaterial->setDiffuse(QColor(QRgb(0xCCFFFF)));

  addComponent(m_mesh);
  addComponent(m_transform);
  addComponent(ballMaterial);

  /****ANİMASYONLAR******/
  m_thetaAnimation->setDuration(2000);
  m_thetaAnimation->setStartValue(0.0f);
  m_thetaAnimation->setEndValue(0.1f);
  m_thetaAnimation->setLoopCount(-1);
  m_thetaAnimation->setTargetObject(this);
  m_thetaAnimation->setPropertyName("phi");
  m_thetaAnimation->start();

  m_phiAnimation->setDuration(2000);
  m_phiAnimation->setStartValue(0.0f);
  m_phiAnimation->setEndValue(0.1f);
  m_phiAnimation->setLoopCount(-1);
  m_phiAnimation->setTargetObject(this);
  m_phiAnimation->setPropertyName("theta");
  m_phiAnimation->start();
}

Ball::~Ball() {}

void Ball::updateTransform() {

  /******SAYI UYDURUYORUM BURADA ŞİMDİLİK*****/
  if (x >= -3 && x <= 3) {
    x = (x + 0.005);
  } else
    x = -3;

  QMatrix4x4 m;
  m.translate(QVector3D(x, y, z));
  m.rotate(m_phi, QVector3D(x, 50.0f, 100.0f));
  m.rotate(m_phi, QVector3D(x, 20.0f, 10.0f));
  m.scale(m_scale);
  m_transform->setMatrix(m);
}

float Ball::theta() const { return m_theta; }

float Ball::phi() const { return m_phi; }

QVector3D Ball::position() const { return m_position; }

float Ball::scale() const { return m_scale; }

void Ball::setTheta(float theta) {
  if (m_theta == theta)
    return;

  m_theta = theta;
  emit thetaChanged(theta);
  updateTransform();
}

void Ball::setPhi(float phi) {
  if (m_phi == phi)
    return;

  m_phi = phi;
  emit phiChanged(phi);
  updateTransform();
}

void Ball::setPosition(QVector3D position) {
  if (m_position == position)
    return;

  m_position = position;
  emit positionChanged(position);
  updateTransform();
}

void Ball::setScale(float scale) {
  if (m_scale == scale)
    return;

  m_scale = scale;
  emit scaleChanged(scale);
  updateTransform();
}
