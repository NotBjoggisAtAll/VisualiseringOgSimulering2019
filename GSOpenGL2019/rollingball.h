#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "sphere.h"

class RollingBall : public Sphere
{
public:
    RollingBall();

    void move(VisualObject *plane);

    void CalculateBarycentricCoordinates(VisualObject *plane);
private:

    gsl::Vector3D Velocity{};
    gsl::Vector3D Acceleration{};

    gsl::Vector3D LastLocation{};

    gsl::Vector3D CurrentTriangleNormal{};
};

#endif // ROLLINGBALL_H
