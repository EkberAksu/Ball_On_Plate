/*
 * Create by Şahin Eğilmez
 */

#ifndef PLATE_H
#define PLATE_H

#include <QtCore/QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

class Plate : public QObject
{
    Q_OBJECT

public:
    Plate(Qt3DCore::QEntity *rootEntity);
    ~Plate();

public slots:
    void enableCuboid(bool enabled);

private:
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DCore::QEntity *m_cuboidEntity;
};

#endif // PLATE_H

