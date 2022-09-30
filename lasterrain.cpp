#include "lasterrain.h"
#include <sstream>
#include <thread>


LasTerrain::LasTerrain(std::string filePath)
{
    readLasFile(filePath);
    //test(filePath.c_str());
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
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}


/*
 *
 * denne funksjonen vill lese en txt fil som er convertert fra laz.
 * den gjør også nødvendige utreininger for terrenget
 *
 */
void LasTerrain::readLasFile(std::string filePath)
{
//qDebug() << mVertices.size();
    std::ifstream las;
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
           if(x < xmin)
               xmin = x;

            if(x > xmax)
                xmax = x;
            word = "";
            temp >> word;

            y = std::stof(word);

            if(y > ymax)
                ymax=y;

            if(y < ymin)
                ymin = y;
            word = "";
            temp >> word;

            z = std::stof(word);


            v.set_xyz(x,y,z);
            v.set_rgb(1.f,0.f,0.f);
            mVertices.push_back(v);
            //tempVerticies.push_back(v);
        }
    }
    las.close();
    //qDebug() << mVertices.size();
    qDebug() << xmin << xmax;
    int th = t/4;
//    std::thread t1(triangulate,th);
//    std::thread t2(triangulate,std::ref(th));


}

void LasTerrain::test(std::string filePath)
{
    std::ifstream las;
    las.open(filePath.c_str(),std::ios::in);
    std::string te;


    if(las.is_open()){
        while(!las.eof()){
            las >> te;
            t++;
        }
    }
     t=t/3;
    qDebug() << t;


    las.close();
}

void LasTerrain::triangulate(int t)
{

}
