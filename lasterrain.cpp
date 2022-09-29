#include "lasterrain.h"
#include <sstream>
#include <thread>


LasTerrain::LasTerrain(std::string filePath)
{
    std::thread t1;
    std::thread t2;
    int hardwear;

    hardwear = t2.hardware_concurrency();


    if(hardwear < 4)
    {

    }


    readLasFile(filePath);
}

void LasTerrain::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(gsml::Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void LasTerrain::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

 if(drawPointCloud)
 {
    glDrawArrays(GL_POINT, 0, mVertices.size());
 }else{
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
 }
}


/*
 *
 * denne funksjonen vill lese en txt fil som er convertert fra laz.
 * den gjør også nødvendige utreininger for terrenget
 *
 * */
void LasTerrain::readLasFile(std::string filePath)
{
    std::ifstream las;
    auto it = mVertices.begin();
    las.open(filePath,std::ios::in);


    gsml::Vertex v;
    std::string tempWord;

    std::string line;
    std::string word;
    if(las.is_open())
    {
        while(std::getline(las,line))
        {

            std::stringstream temp;

            temp << line;

            word = "";

            temp >> word;
            float x,y,z;

            x = std::stof(word);
            if(x > xmax)
                xmax = x;

            temp >> word;

            y = std::stof(word);

            if(y > ymax)
                ymax=y;

            temp >> word;

            z = std::stof(word);


            v.set_xyz(x,y,z);
            mVertices.push_back(v);




        }
    }
}

void LasTerrain::triangulate()
{

}
