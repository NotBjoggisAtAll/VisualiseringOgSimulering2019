#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "sphere.h"

class RollingBall : public Sphere
{
public:
    RollingBall();

    void move(VisualObject *plane);

    gsl::Vector3D CalculateBarycentricCoordinates(VisualObject *plane);
private:

    gsl::Vector3D Velocity{};
    gsl::Vector3D Acceleration{};

    gsl::Vector3D LastLocation{};

    gsl::Vector3D lastPlaneNormal{0};



    float radius = 1;
    float mass;
};

#endif // ROLLINGBALL_H
