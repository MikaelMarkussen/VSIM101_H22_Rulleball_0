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
    void triangulate();

    float x,y,z;

    float xmin = 0,ymin = 0,xmax = 0,ymax = 0;
    std::vector<gsml::Vertex> verticies;

};



#endif // LASTERRAIN_H
