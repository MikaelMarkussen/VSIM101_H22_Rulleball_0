#include "rollingball.h"
#include <math.h>

RollingBall::RollingBall(int n) : OctahedronBall (n)
{

    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(-1.5,-1.5,1.5);
    mScale.scale(0.25,0.25,0.25);
    mMatrix = mPosition*mScale;


}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{   
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    for(size_t i = 0; i<vertices.size();i+=3)
      {
          gsml::Vertex t1 = vertices[i];
          gsml::Vector3d v1 = t1.getXYZ();
          gsml::Vertex t2 = vertices[i+1];
          gsml::Vector3d v2 = t2.getXYZ();
          gsml::Vertex t3 = vertices[i+2];
          gsml::Vector3d v3 = t3.getXYZ();


          gsml::Vector4d h = mMatrix.getColumn(3);

          gsml::Vector2d postmp = gsml::Vector2d(h.x(),h.y());


          gsml::Vector3d temp = postmp.barycentricCoordinates(v1,v2,v3);
          gsml::Vector4d a = gsml::Vector4d(temp.x,temp.y,temp.z,1);



             //beregne Normalen til ballen
             gsml::Vector3d N = NormalVec(v1,v2,v3);
             //beregne kreftene for kulen
             gsml::Vector3d Gn = N*(masse*g*cos(angle));


             gsml::Vector3d acceleration=gsml::Vector3d(N.x*g*N.z,N.y*g*N.z,pow(N.z,2)*g-1);
             mVelocity = gsml::Vector3d(mVelocity.x * acceleration.x,mVelocity.y * acceleration.y,mVelocity.z * acceleration.z);
//           mPosition.translate(mVelocity.x,mVelocity.y,mVelocity.z);
             mPosition.translate(0,0,(9.81/2)/dt);
             old_index = i;
            if(i != old_index)
            {

            }


         old_index = i;

      }

    mMatrix = mPosition * mScale;

}

void RollingBall::test()
{

    //qDebug() << pos.x << pos.y << pos.z;
    //mMatrix.print();
}

gsml::Vector3d RollingBall::NormalVec(gsml::Vector3d &v0, gsml::Vector3d &v1, gsml::Vector3d &v2)
{

    gsml::Vector3d norm;

    gsml::Vector3d QR = gsml::Vector3d(v1.x-v0.x,v1.y-v0.y,v1.z-v0.z);
    gsml::Vector3d QS = gsml::Vector3d(v2.x-v0.x,v2.y-v0.y,v2.z-v0.z);

    //dette er for å beregne vinkelen til planet.
    float t  = QR*QS;
    float temp = sqrt(pow(QR.x,2)+pow(QR.y,2)+pow(QR.z,2));
    float temp2 = sqrt(pow(QS.x,2)+pow(QS.y,2)+pow(QS.z,2));
     angle = cos(t/(temp*temp2));

    return  gsml::Vector3d(QR.x*QS.x,QR.y*QS.y,QR.z*QS.z);

}



void RollingBall::init(GLint matrixUniform)
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

void RollingBall::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
}
