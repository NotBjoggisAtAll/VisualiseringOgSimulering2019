#include "rollingball.h"

RollingBall::RollingBall() : Sphere(3), mass(1.f)
{

}

void RollingBall::move(VisualObject* plane)
{
    auto normal = CalculateBarycentricCoordinates(plane);

    //Gravitasjon vektoren

    gsl::Vector3D N = normal * gsl::Vector3D::dot(normal, -gravityVector);

    Acceleration = (N + gravityVector);
    Velocity += Acceleration * 0.005f * (1.f/60.f);
    mMatrix.translate(Velocity);
}

gsl::Vector3D RollingBall::CalculateBarycentricCoordinates(VisualObject* plane)
{
    for (unsigned int i=0; i<plane->mIndices.size(); i+=3)
    {
        gsl::Vector3D pos1;
        gsl::Vector3D pos2;
        gsl::Vector3D pos3;
        pos1 = plane->mVertices[plane->mIndices[i+0]].mXYZ;
        pos2 = plane->mVertices[plane->mIndices[i+1]].mXYZ;
        pos3 = plane->mVertices[plane->mIndices[i+2]].mXYZ;

        gsl::Vector2D temp = gsl::Vector2D(mMatrix.getPosition().x, mMatrix.getPosition().z);
        gsl::Vector3D bar = temp.barycentricCoordinates(gsl::Vector2D(pos1.x,pos1.z),gsl::Vector2D(pos2.x, pos2.z), gsl::Vector2D(pos3.x,pos3.z));

        if(bar.x>=0 && bar.x<=1 && bar.y>=0 && bar.y<=1 && bar.z>=0 && bar.z <=1)
        {
            gsl::Vector3D playerTempPos = (pos1 * bar.x + pos2*bar.y + pos3*bar.z);

            gsl::Vector3D pointToBall = mMatrix.getPosition() - playerTempPos;


            gsl::Vector3D normal = gsl::Vector3D::cross(pos3 - pos1, pos2 - pos1);
            normal.normalize();

            float closestPoint = gsl::Vector3D::dot(pointToBall, normal);

            if(closestPoint > radius) { //Radius
                normal = {0};
            }
            else
            {
               float distance = radius - closestPoint;
               if(distance > 0.002f)
               {
                   mMatrix.translate(normal * (radius - closestPoint));
               }
            }
            if(lastPlaneNormal == gsl::Vector3D(0))  //Kommer fra lufta
            {
                gsl::Vector3D N = normal * gsl::Vector3D::dot(normal, -gravityVector);
                Velocity = (gravityVector+N).normalized() * gsl::Vector3D::dot(Velocity, (gravityVector+N).normalized());

            }
            else if(normal != lastPlaneNormal)
            {
                gsl::Vector3D tangentNormal = normal + lastPlaneNormal;
                tangentNormal.normalize();
                auto newVelocity = Velocity - tangentNormal * (gsl::Vector3D::dot(Velocity, tangentNormal)*2);
                Velocity = newVelocity;
            }
            lastPlaneNormal = normal;
            return normal;
        }
    }
    return {0};
}
