#ifndef BALL_H
#define BALL_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QMesh>
#include <QPhongMaterial>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <QPhongMaterial>
#include <QSphereMesh>
#include <QPropertyAnimation>

class Ball : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(float theta READ theta WRITE setTheta NOTIFY thetaChanged)
    Q_PROPERTY(float phi READ phi WRITE setPhi NOTIFY phiChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)

public:
    explicit Ball(Qt3DCore::QNode *parent = 0);
    ~Ball();
    float theta() const;
    float phi() const;
    QVector3D position() const;
    float scale() const;

public slots:
    void setTheta(float theta);
    void setPhi(float phi);
    void setPosition(QVector3D position);
    void setScale(float scale);

signals:
    void thetaChanged(float theta);
    void phiChanged(float phi);
    void positionChanged(QVector3D position);
    void scaleChanged(float scale);

protected:
    void updateTransform();

private:
    Qt3DExtras::QSphereMesh *m_mesh;
    Qt3DCore::QTransform *m_transform;
    Qt3DExtras::QPhongMaterial *ballMaterial;
    float m_theta;
    float m_phi;
    QVector3D m_position;
    float m_scale;
    float x=0.0f, y=0.55f, z=0.0f;
    QPropertyAnimation *m_thetaAnimation;
    QPropertyAnimation *m_phiAnimation;
};

#endif // BALL_H
