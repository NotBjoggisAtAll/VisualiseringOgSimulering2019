#include "rollingball.h"

RollingBall::RollingBall() : Sphere(3)
{

}

void RollingBall::move(VisualObject* plane)
{
    CalculateBarycentricCoordinates(plane);
    Velocity = {0.01,0,0};
    mMatrix.translate(Velocity);
}

void RollingBall::CalculateBarycentricCoordinates(VisualObject* plane)
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

            float playerTempPos = (pos1.y*bar.x + pos2.y*bar.y + pos3.y*bar.z) + 1; //1 = radius
            LastLocation = gsl::Vector3D(mMatrix.getPosition().x,playerTempPos,mMatrix.getPosition().z);
            mMatrix.setPosition(LastLocation);

        auto normal = gsl::Vector3D::cross(pos2 - pos1, pos3 - pos1);
        normal.normalize();
        CurrentTriangleNormal = normal;
        qDebug() << CurrentTriangleNormal;
        }
    }
    mMatrix.setPosition(LastLocation);
}
