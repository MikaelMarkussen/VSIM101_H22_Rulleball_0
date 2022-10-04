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
    void triangulateGrids(int t);
    void triangulate(int x,int y);


    float x,y,z;

    float xmin = 10000000.f,ymin=10000000.f,xmax = 0.f,ymax = 0.f, zmin = 100000.f,zmax = 0.f, xrange,yrange,xmidt,ymidt;
    int testY =0, testX =0,tempX = 0,tempY =0;
    std::vector<gsml::Vertex> tempVertices;
    //std::vector<gsml::Vertex> tempVertices2;

};



#endif // LASTERRAIN_H
