#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"

class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dt) override;
    void setSurface(VisualObject* surface) { triangle_surface = surface; }
    void test();
    float masse = 0.500f;

protected:
    VisualObject* triangle_surface;
private:
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    float g = -1.81f;
    float angle;

    gsml::Vector3d NormalVec(gsml::Vector3d &v0, gsml::Vector3d &v1, gsml::Vector3d &v2);
    //gsml::Vector3d velocity{1,1,1};


   gsml::Vector3d pos;
};
#endif // ROLLINGBALL_H
