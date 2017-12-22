#include "controllers.h"

#include <GL/gl.h>
#include "MicroGlut.h"

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
//#include "zpr.h"
#include "LoadTGA.h"
#include <stdio.h>


void keyboardFunc( unsigned char key, int x, int y)
{
// Add any keyboard control you want here
  switch (key) {
    case 27:
      exit(-1);
    case 119: // w
      lookingDir = ScalarMult(Normalize(VectorSub(l,p)),speed);
      p = VectorAdd(p,lookingDir);
      l = VectorAdd(l,lookingDir);
      break;
    case 115: // s
      lookingDir = ScalarMult(Normalize(VectorSub(l,p)),speed);
      p = VectorSub(p,lookingDir);
      l = VectorSub(l,lookingDir);
      break;
    case 100: // d
      lookingDir = ScalarMult(Normalize(VectorSub(l,p)),speed);
      rightDir = Normalize(CrossProduct(lookingDir, v));
      p = VectorAdd(p,rightDir);
      l = VectorAdd(l,rightDir);
      break;
    case 97: // a
      lookingDir = Normalize(VectorSub(l,p));
      rightDir = Normalize(CrossProduct(lookingDir, v));
      p = VectorSub(p,rightDir);
      l = VectorSub(l,rightDir);
      break;
  }
  camMatrix = lookAtv(p, l, v);
}

void passiveMouse(int x, int y){
	if (x != xMouseOrigin || y != xMouseOrigin){
		float deltaX = (xMouseOrigin - (float)x)/2000.0;
		float deltaY = (yMouseOrigin - (float)y)/2000.0;
		glutWarpPointer(xMouseOrigin, yMouseOrigin);

		mat4 trans = T(-p.x, -p.y, -p.z);
		mat4 trans2 = T(p.x, p.y, p.z);
		mat4 rot = Ry(deltaX);

		mat4 rotateUpDown = ArbRotate(rightDir, deltaY);

		l = MultVec3(trans2, MultVec3(rot, MultVec3(trans, l)));
		l = MultVec3(trans2, MultVec3(rotateUpDown, MultVec3(trans, l)));
	}
  camMatrix = lookAtv(p, l, v);

}
