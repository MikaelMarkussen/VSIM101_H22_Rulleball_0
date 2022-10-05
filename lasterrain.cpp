#include "lasterrain.h"
#include <sstream>
#include <thread>


LasTerrain::LasTerrain(std::string filePath)
{
    readLasFile(filePath);
    mPosition.translate(0,0,-2);
    mPosition.rotate(-90,0,0,1);
    mMatrix =mPosition;
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
    las.open(filePath.c_str(),std::ios::in);


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

            x = std::stof(word);//100000.f;
           if(x < xmin)
               xmin = x;

            if(x > xmax)
                xmax = x;
            word = "";
            temp >> word;

            y = std::stof(word);//100000.f;

            if(y > ymax)
                ymax=y;

            if(y < ymin)
                ymin = y;
            word = "";
            temp >> word;

            z = std::stof(word);//100.f;
            if(z > zmax)
                zmax = z;

            if(z < zmin)
                zmin = z;

            //skalerer



            v.set_xyz(x,y,z);
            v.set_rgb(1.f,0.f,0.f);
            //mVertices.push_back(v);
            tempVertices.push_back(v);
        }

        xrange = xmax-xmin;
        yrange = ymax-ymin;

        xmidt = xmin+xrange/2;
        ymidt = ymin+yrange/2;

        for(auto i = tempVertices.begin(); i != tempVertices.end();i++)
        {
            auto vert = *i;
            float x = 2*(vert.getX()-xmidt)/xrange;
            float y = 2*(vert.getY()-ymidt)/yrange;
            float z = vert.getZ()/(zmax/3);
            float tempZ = 1/z;
            mVertices.push_back(gsml::Vertex(x,y,z,1/z,1*z,0));
        }
        //mVertices.clear();
    }
    las.close();
//    qDebug() << xmin << xmax;
//    qDebug() << ymin << ymax;
//    qDebug() << zmin << zmax;
   // triangulateGrids(1);

   // gridX = gridX/100;
    //qDebug() << xrange<< yrange;



}

void LasTerrain::triangulateGrids(int t)
{
  gsml::Vector3d grid[116][66];

  for(auto i = 0;i < tempVertices.size(); i++)
  {
       tempX = tempVertices[i].getX();
       tempY = tempVertices[i].getY();


       grid[tempX/10][tempY/10].z= tempVertices[i].getZ();




//      for(int x = 0;x <= 116;x++)
//      {
//          for(int y = 0;y <= 66;y++)
//          {
//             testY = ymin+(10*y);
//             testX = xmin+(10*x);
//             if(tempY <= testY && tempX <= testX)
//             {
//                 grid[x][y].z=mVertices[i].getZ();
//             }
//          }

    //  }




//      for(int x = xmin; x < xmax; x+=10)
//      {
//          for(int y = ymin; y < ymax; y+=10)
//          {
//          if(tempX <= x && tempY <= y)
//          {
//              grid[y][]
//          }
//        }
//      }

      //qDebug() << tempX << tempY;
      //grid[tempX][tempY].z= tempVertices[i].getZ();

  }
  int m = 116;
        int n= 66;
          std::vector<gsml::Vertex> v;


      for(auto i = 0;i < m-1; i++)
      {
          for(auto j = 0 ; j < n-1; j++)
          {
              auto start = v.size();

               float x = grid[i][j].x;
               float y = grid[i][j].y;
               float z = grid[i][j].z;

              mVertices.push_back(gsml::Vertex(x,y,z,1,0,0));

               float x1 = grid[i+1][j].x;
               float y1 = grid[i+1][j].y;
               float z1 = grid[i+1][j].z;

              mVertices.push_back(gsml::Vertex(x1,y1,z1,0,1,0));


               float x2 = grid[i+1][j+1].x;
               float y2 = grid[i+1][j+1].y;
               float z2 = grid[i+1][j+1].z;

              mVertices.push_back(gsml::Vertex(x2,y2,z2,0,0,1));
          }
      }
}

