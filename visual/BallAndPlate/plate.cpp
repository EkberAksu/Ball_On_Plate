/*
 * Create by Şahin Eğilmez
 */

#include "plate.h"

#include <QtCore/QDebug>

Plate::Plate(Qt3DCore::QEntity *rootEntity)
    : m_rootEntity(rootEntity)
{
    //*******EKRAN*******

    // Cuboid shape data
    Qt3DExtras::QCuboidMesh *cuboid = new Qt3DExtras::QCuboidMesh();
    cuboid->setYExtent(0.05f);

    // CuboidMesh Transform
    Qt3DCore::QTransform *cuboidTransform = new Qt3DCore::QTransform();
    cuboidTransform->setScale(6.0f);
    cuboidTransform->setTranslation(QVector3D(0.0f, -0.0f, 0.0f));

    Qt3DExtras::QPhongMaterial *cuboidMaterial = new Qt3DExtras::QPhongMaterial();
    cuboidMaterial->setDiffuse(QColor(QRgb(0xE0E0E0)));

    //Cuboid
    m_cuboidEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_cuboidEntity->addComponent(cuboid);
    m_cuboidEntity->addComponent(cuboidMaterial);
    m_cuboidEntity->addComponent(cuboidTransform);

}

Plate::~Plate()
{
}
void Plate::enableCuboid(bool enabled)
{
    m_cuboidEntity->setParent(enabled ? m_rootEntity : nullptr);
}
