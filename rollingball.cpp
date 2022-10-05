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
          //qDebug() << h.x() << h.y() << h.z();
          gsml::Vector3d postmp = gsml::Vector3d(h.x(),h.y(),h.z());


          gsml::Vector3d temp = postmp.barycentricCoordinates(v1,v2,v3);

         //qDebug()  << temp.x << temp.y << temp.z;
         if(temp.x > 0 && temp.x < 1 && temp.y > 0 && temp.y < 1){
             //beregne Normalen til flaten
             gsml::Vector3d N = NormalVec(v1,v2,v3);
             N.normalize();
             gsml::Vector3d acceleration = gsml::Vector3d(N.x*N.z,N.y*N.z,(pow(N.z,2)-1))*g;

             qDebug() << N.x << N.y << N.z;
             mVelocity = gsml::Vector3d(acceleration.x,acceleration.y,-acceleration.z);
             mPosition.translate(mVelocity.x/dt,mVelocity.y/dt,mVelocity.z/dt);
             old_index = i;
         }
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

     norm = QR.cross(QS);

    //    float t  = QR*QS;
//    float temp = sqrt(pow(QR.x,2)+pow(QR.y,2)+pow(QR.z,2));
//    float temp2 = sqrt(pow(QS.x,2)+pow(QS.y,2)+pow(QS.z,2));
//     angle = cos(t/(temp*temp2));

    return norm; //gsml::Vector3d(normalX,normalY,normalZ);

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
