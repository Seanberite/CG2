
// Converted to MicroGlut and VectorUtils3 2013.
// MicroGLut currently exists for Linux and Mac OS X, and a beta for Windows.
// You will also need GLEW for Windows. You may sometimes need to work around
// differences, e.g. additions in MicroGlut that don't exist in FreeGlut.

// 2015:

// Linux: gcc lab0.c ../common/*.c ../common/Linux/MicroGlut.c -lGL -o lab0 -I../common -I../common/Linux

// Mac: gcc lab0.c ../common/*.c ../common/Mac/MicroGlut.m -o lab0 -framework OpenGL -framework Cocoa -I../common/Mac -I../common

#include <GL/gl.h>
#include "MicroGlut.h"

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
//#include "zpr.h"
#include "LoadTGA.h"
#include <stdio.h>
#include <sstream>


// Model-to-world matrix
// Modify this matrix.
// See below for how it is applied to your model.
// mat4 shear = {{ 1.0, 1.0, 0.0, 0.0,
//                 0.0, 1.0, 0.0, 0.0,
//                 0.0, 0.0, 1.0, 0.0,
//                 0.0, 0.0, 0.0, 1.0}};


// World-to-view matrix. Usually set by lookAt() or similar.
mat4 projectionMatrix, camMatrix, mdlMatrix;
// mdlMatrix should later be in each object
// Projection matrix, set by a call to perspective().

// Movenemt variables
vec3 p,l,v, lookingDir, rightDir;
float xMouseOrigin, yMouseOrigin;
float speed = 0.1;
bool is_w_pressed,is_s_pressed,is_a_pressed, is_d_pressed, isLeftMousePressed;

// Globals
// * Model(s)
Model *teapot, *skySphere;
// * Reference(s) to shader program(s)
GLuint program;
// * Texture(s)
GLuint maskrosTex;


void updatePlayerPosition(){
  if(isLeftMousePressed==true){
    speed = 0.5;
  }
  else{
    speed = 0.1;
  }
  if(is_w_pressed){
    lookingDir = ScalarMult(Normalize(VectorSub(l,p)),speed);
    p = VectorAdd(p,lookingDir);
    l = VectorAdd(l,lookingDir);
  }
  if(is_s_pressed){
    lookingDir = ScalarMult(Normalize(VectorSub(l,p)),speed);
    p = VectorSub(p,lookingDir);
    l = VectorSub(l,lookingDir);
  }
  if(is_a_pressed){
    lookingDir = Normalize(VectorSub(l,p));
    rightDir = ScalarMult(Normalize(CrossProduct(lookingDir, v)),speed);
    p = VectorSub(p,rightDir);
    l = VectorSub(l,rightDir);
  }
  if(is_d_pressed){
    lookingDir = Normalize(VectorSub(l,p));
    rightDir = ScalarMult(Normalize(CrossProduct(lookingDir, v)),speed);
    p = VectorAdd(p,rightDir);
    l = VectorAdd(l,rightDir);
  }
}
void keyboardUpFunc(unsigned char key, int x, int y){
  switch (key) {
    case 27:
      exit(-1);
      break;
    case 119: // w
      is_w_pressed = false;
      break;
    case 115: // s
      is_s_pressed = false;
      break;
    case 100: // d
      is_d_pressed = false;
      break;
    case 97: // a
      is_a_pressed = false;
      break;
  }

}
void keyboardFunc(unsigned char key, int x, int y){
// Add any keyboard control you want here
  switch (key) {
    case 27:
      exit(-1);
      break;
    case 119: // w
      is_w_pressed = true;
      break;
    case 115: // s
      is_s_pressed = true;
      break;
    case 100: // d
      is_d_pressed = true;
      break;
    case 97: // a
      is_a_pressed = true;
      break;
  }
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
}
void mouse(int button, int state, int x, int y){
  if (button == 2 && state == GLUT_DOWN){
    isLeftMousePressed = true;
  }
  if (button == 2 && state == GLUT_UP){
    isLeftMousePressed = false;
    printf("%s\n", "hej");
  }
}

void init(void)
{
  dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits");

  // Init player stuff
  p = SetVector(0.0, 0.0, 1.0);
  l = SetVector(0.0, 0.0, 0.0);
  v = SetVector(0.0, 1.0, 0.0);
  lookingDir = Normalize(VectorSub(l,p));
  rightDir = Normalize(CrossProduct(lookingDir, v));
  xMouseOrigin = 500;
  yMouseOrigin = 1000;
  mdlMatrix = T(0,0,-3);
	projectionMatrix = perspective(90, 1.0, 0.1, 1000);
	camMatrix = lookAtv(p, l, v);




	// Load and compile shader
	program = loadShaders("lab0.vert", "lab0.frag");
	printError("init shader");
  glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	// Upload geometry to the GPU:
	teapot = LoadModelPlus("objects/teapot.obj");
	printError("load models");

	// Load textures
	LoadTGATextureSimple("textures/maskros512.tga",&maskrosTex);
	printError("load textures");
  glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, maskrosTex);
  glUniform1i(glGetUniformLocation(program, "objectTexture"), 0);

}


void display(void)
{
  updatePlayerPosition();

  camMatrix = lookAtv(p,l,v);
  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000;
  //printf("%f\n",t);
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//activate the program, and set its variables
	glUseProgram(program);
  glUniform1f(glGetUniformLocation(program, "time"), t);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mdlMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);


	//draw the model
	DrawModel(teapot, program, "in_Position", "in_Normal", "in_TexCoord");

	printError("display");

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    GLfloat ratio = (GLfloat) w / (GLfloat) h;
    projectionMatrix = perspective(100, ratio, 0.1, 50);
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Virre");
	glutDisplayFunc(display);
  glutFullScreen();
  glutKeyboardFunc(keyboardFunc);
  glutKeyboardUpFunc(keyboardUpFunc);
  glutPassiveMotionFunc(passiveMouse);
  glutMouseFunc(mouse);
  glutReshapeFunc(reshape);
	glutRepeatingTimer(15);
	init();
  glutHideCursor();
	glutMainLoop();
	exit(0);
}
