#ifndef LASTERRAIN_H
#define LASTERRAIN_H
#include "visualobject.h"
#include <fstream>

class LasTerrain : public VisualObject
{
public:
    LasTerrain(std::string filePath);
    void init(GLint matrixUniform) override;
    void draw() override;
    bool drawPointCloud = false;

private:
    void readLasFile(std::string filePath);
    void test(std::string filePath);
    void triangulate(int t);

    float x,y,z;

    float xmin,ymin,xmax = 0,ymax = 0;
    std::vector<gsml::Vertex> tempVerticies;
    int t = 0;

};



#endif // LASTERRAIN_H
