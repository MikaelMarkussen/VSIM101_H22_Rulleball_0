// VisualObject.cpp
#include "visualobject.h"

VisualObject::VisualObject()
{
    mMatrix.setToIdentity();
    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
}

VisualObject::~VisualObject()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

gsml::Vector3d VisualObject::getPos()
{
  gsml::Vector4d t = mMatrix.getColumn(3);

  return gsml::Vector3d(t.x(),t.y(),t.z());
}



