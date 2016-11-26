#ifndef ROTATINGTREFOILKNOT_H
#define ROTATINGTREFOILKNOT_H

#include "ball.h"
#include <QPropertyAnimation>

class RotatingTrefoilKnot : public Ball
{
public:
    explicit RotatingTrefoilKnot(Qt3DCore::QNode *parent = 0);
    ~RotatingTrefoilKnot();

private:
    QPropertyAnimation *m_thetaAnimation;
    QPropertyAnimation *m_phiAnimation;

};

#endif // ROTATINGTREFOILKNOT_H
