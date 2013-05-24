// rodgl rev1.1 RB 03May13  (cube, sphere, OBJ loader)
// =====
// Bugs:
//   1. Something wrong with camaro normals (see through)
//   2. understand glortho
//   3. illumination
//   4. use normals?
//   5. Memory management: core dump when n>=21 (n=21 should only need vt[18585]
//
// int w=1024; int h=768; when this was 500,500 FPS were disastrous! WHY?
// why is FPS sometimes in the 500+, then sometimes almost exactly 60?
//-------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <math.h>
#include <signal.h>
#include "GLU3/glu3.h"
#include "GLES/gl.h"
#include "EGL/egl.h"
#include "FSL/fsl_egl.h"
#include "FSL/fslutil.h"
#include <sys/time.h>
//-------------------------------------------------------------------
#include "OBJloader.h"
//-------------------------------------------------------------------
EGLDisplay			egldisplay;
EGLConfig			eglconfig;
EGLSurface			eglsurface;
EGLContext			eglcontext;
EGLNativeWindowType eglNativeWindow;
EGLNativeDisplayType eglNativeDisplayType;
volatile sig_atomic_t quit = 0;
//-------------------------------------------------------------------
struct timeval now;
int mode,rot,oo,currentFrame;
float time0,time1,dt;
float angle;
const int usevrtcs=1;                                      // use Vertices (always)                                
int usetxtrs;                                              // use Textures
int usenrmls;                                              // use Normals
GLfloat vt[40000];                                         // key array containing vertices + textures (+ normals)
GLuint texture[2];
int state,mousex0,mousex1,mousey0,mousey1,mousemtn;
float newanglex,oldanglex,newangley,oldangley;
int nvt,stride;
//-------------------------------------------------------------------
void LoadCube(void)                                        // loads vt[] array with model
{
stride=5;
int i,j;
float x[8],y[8],z[8];
float a=0.00,b=0.33,c=0.67,d=1.00;
float w=0.9;
i=0; x[i]=-w; y[i]=-w; z[i]=+w;
i=1; x[i]=+w; y[i]=-w; z[i]=+w;
i=2; x[i]=+w; y[i]=+w; z[i]=+w;
i=3; x[i]=-w; y[i]=+w; z[i]=+w;
i=4; x[i]=+w; y[i]=-w; z[i]=-w;
i=5; x[i]=-w; y[i]=-w; z[i]=-w;
i=6; x[i]=-w; y[i]=+w; z[i]=-w;
i=7; x[i]=+w; y[i]=+w; z[i]=-w;
i=0;
// Front face (yellow or Twin Towers) *** CREATE TRIANGLES CCW ***
j=0; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=a; vt[i+0x04]=a; i+=stride;
j=1; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=a; i+=stride;
j=2; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=b; i+=stride;
j=0; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=a; vt[i+0x04]=a; i+=stride;
j=2; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=b; i+=stride;
j=3; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=a; vt[i+0x04]=b; i+=stride;
// Right face (blue or Pyramid)
j=1; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=c; i+=stride;
j=4; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=c; i+=stride;
j=7; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=d; i+=stride;
j=1; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=c; i+=stride;
j=7; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=d; i+=stride;
j=2; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=d; i+=stride;
// Back face (magenta or Big Ben)
j=4; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=a; i+=stride;
j=5; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=a; i+=stride;
j=6; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=b; i+=stride;
j=4; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=a; i+=stride;
j=6; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=b; i+=stride;
j=7; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=b; i+=stride;
// Left face (cyan or Rio)
j=5; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=c; i+=stride;
j=0; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=d; vt[i+0x04]=c; i+=stride;
j=3; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=d; vt[i+0x04]=d; i+=stride;
j=5; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=c; i+=stride;
j=3; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=d; vt[i+0x04]=d; i+=stride;
j=6; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=d; i+=stride;
// Top face (red or Taj Mahal)
j=3; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=a; vt[i+0x04]=c; i+=stride;
j=2; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=c; i+=stride;
j=7; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=d; i+=stride;
j=3; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=a; vt[i+0x04]=c; i+=stride;
j=7; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=d; i+=stride;
j=6; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=a; vt[i+0x04]=d; i+=stride;
// Bottom face (green or Pisa)
j=5; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=b; i+=stride;
j=4; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=b; i+=stride;
j=1; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=c; i+=stride;
j=5; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=b; i+=stride;
j=1; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=c; vt[i+0x04]=c; i+=stride;
j=0; vt[i+0x00]=x[j]; vt[i+0x01]=y[j]; vt[i+0x02]=z[j];    vt[i+0x03]=b; vt[i+0x04]=c; i+=stride;
nvt=i;
usetxtrs=1; usenrmls=0;
}
//-------------------------------------------------------------------
void AddVertex(int n,int jj,int kk)                        // auto increments increments nvt
{
const float pk=3.141592654f/180.0f;
float va,vb,vx,vy,vz;
va=jj*360.0f/n; vb=90.0f+kk*180.0f/n;
vx=+(float)(cos(va*pk)*cos(vb*pk)); 
vz=-(float)(sin(va*pk)*cos(vb*pk));	
vy=+(float)sin(vb*pk);
vt[nvt++]=vx; vt[nvt++]=vy; vt[nvt++]=vz;    vt[nvt++]=jj*1.0f/n; vt[nvt++]=1.0f-kk*1.0f/n;
}
//-------------------------------------------------------------------
void LoadSphere(int n)                               // loads vt[] array with model
{
// Sphere = n + n + n*(n-2)*2 triangles = 2n^2-2n triangles  ===> 3x(2n^2-2n) vertices
int j,k;
nvt=0;
stride=5;

// Top Layer (n-faced polygon = n triangles)
for (j=0;j<n;j++)
  {
  AddVertex(n,0,  0); // 1st vertex=north pole
  AddVertex(n,j+0,1); // 2nd vertex=bottom left
  AddVertex(n,j+1,1); // 3rd vertex=bottom right
  }

// Middle Section (2 triangles per trapezoid= n*(n-2)*2 triangles)
for (k=1;k<n-1;k++)
  for (j=0;j<n;j++)
    {
    //...1st triangle...
    AddVertex(n,j+0,k+0); // 1st vertex=top left (A)
    AddVertex(n,j+0,k+1); // 2nd vertex=bottom left (P)
    AddVertex(n,j+1,k+0); // 3rd vertex=bottom middle (B)
    //...2nd triangle...
    AddVertex(n,j+1,k+0); // 1st vertex=top left (B)
    AddVertex(n,j+0,k+1); // 2nd vertex=bottom left (P)
    AddVertex(n,j+1,k+1); // 3rd vertex=bottom middle (Q)
    }
// Bottom Layer (n-faced polygon = n triangles)
for (j=0;j<n;j++)
  {
  //x=0; z=0; y=-r;  vt[nvt++]=x; vt[nvt++]=y; vt[nvt++]=z;  vt[nvt++]=0.5f; vt[nvt++]=1.0f; 
  AddVertex(n,n,  n  ); // 1st vertex=south pole
  AddVertex(n,j+1,n-1); // 2nd vertex=reversed from Top Layer
  AddVertex(n,j+0,n-1); // 3rd vertex=reversed from Top Layer
  }
printf("*** CREATED %d TRIANGLES ***\n",nvt);
usetxtrs=1; usenrmls=0;
}
//-------------------------------------------------------------------
int LoadGLTextures(char *fname)
{
Image *image1;
image1 = (Image *) malloc(sizeof(Image));
if (image1 == NULL) 
  {
  printf("Error allocating space for image");
  return 0;
  }	
if (LoadBMP(fname,image1))
  {
  glGenTextures(1, &texture[0]);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image1->sizeX,image1->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,image1->data);
  }
else return 0;
if (image1 != NULL) free(image1);
usetxtrs=1; glEnable(GL_TEXTURE_2D);
return 1;
}

//-------------------------------------------------------------------
int render()
{
/*
int i,j;
GLfloat baseverts[4][3];
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glLoadIdentity();
glTranslatef(0.0f, 0.0f, -4.0f);
glRotatef(-75.0f, 1.0f, 0.0f, 0.0f);
glRotatef(rotz, 0.0f, 0.0f, 1.0f);
glBindTexture(GL_TEXTURE_2D, texture[0]);
glVertexPointer(3, GL_FLOAT, 0, &mybezier.vertices[0][0]);
glTexCoordPointer(2, GL_FLOAT, 0, &mybezier.texcoords);
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY);
for (i=0; i<divs; i++) glDrawArrays(GL_TRIANGLE_STRIP, i*(divs+1)*2, (divs+1)*2);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, baseverts);
glEnableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_VERTEX_ARRAY);
glFinish();
return 1;
*/

/*
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glEnableClientState(GL_VERTEX_ARRAY);
glLoadIdentity();
glTranslatef(-1.5f, 0.0f, -6.0f);
glVertexPointer(3,GL_FLOAT,5*4,vt);
glColor4f(0,1,0,1);
glRotatef(currentFrame/10,0,0,1);
glDrawArrays(GL_LINE_LOOP,0,4*oo*oo);
glFlush();
glDisableClientState(GL_VERTEX_ARRAY);
glFinish();
*/

int i;
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//glEnableClientState(GL_VERTEX_ARRAY);
//glVertexPointer(3,GL_FLOAT,stride*sizeof(GL_FLOAT),vt);
glLoadIdentity();
//RODMOD glOrtho(-2.0,+2.0,-2.0,+2.0,2.0,-2.0);
glTranslatef(0.0f, 0.0f, -4.0f);
glRotatef(-75.0f, 1.0f, 0.0f, 0.0f);

switch (rot)
  {
  case 0: glRotatef(angle,0,0,1); break;
  case 1: glRotatef(angle,0,1,0); break;
  case 2: glRotatef(angle,1,0,0); break;
  case 3: glRotatef(angle,0,1,1); break;
  case 4: glRotatef(angle,1,0,1); break;
  case 5: glRotatef(angle,1,1,0); break;
  case 6: glRotatef(angle,1,1,1); break;
  }
angle+=0.5; if (angle>360.0) {angle=0.0; rot++; if (rot>6) rot=0;}

/*
if (mousemtn)
  {
  newangley=oldangley+(mousex1-mousex0)/2.0; glRotatef(newangley,0,1,0);
  newanglex=oldanglex+(mousey1-mousey0)/2.0; glRotatef(newanglex,1,0,0);
  }
else {glRotatef(oldangley,0,1,0); glRotatef(oldanglex,1,0,0);}
*/
if (usetxtrs) glDrawArrays(GL_TRIANGLES,0,nvt/stride*3); 
else for (i=0;i<nvt/stride;i++) glDrawArrays(GL_LINE_LOOP,i*3,3);

glFlush();
//glDisableClientState(GL_VERTEX_ARRAY);
glFinish();
//RODMOD glutPostRedisplay();
return 1;
}
//-------------------------------------------------------------------
void resize(int w,int h)
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glViewport(0,0,w,h);
GLUmat4 perspective;
gluPerspective4f(&perspective,45.0f,((GLfloat)w/(GLfloat)h),1.0f,100.0f);
glMultMatrixf(&perspective.col[0].values[0]);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
}
//-------------------------------------------------------------------
void InitVariables(void) 
{
mousex0=mousey0=mousex1=mousey1=mousemtn=0; oldanglex=oldangley=angle=0.0f; state=-1; rot=0; currentFrame=0;
}
//-------------------------------------------------------------------
int InitGL(void)
{
int w=640;
int h=480;
static const EGLint s_configAttribs[] =
  {
  EGL_RED_SIZE,	  5,
  EGL_GREEN_SIZE, 6,
  EGL_BLUE_SIZE,  5,
  EGL_ALPHA_SIZE, 0,
  EGL_SAMPLES, 	  0,
  EGL_NONE
  };
EGLint numconfigs;
// get egl display
eglNativeDisplayType=fsl_getNativeDisplay();
egldisplay=eglGetDisplay(eglNativeDisplayType);
// Initialize egl
eglInitialize(egldisplay,NULL,NULL);
assert(eglGetError() == EGL_SUCCESS);
// tell the driver we are using OpenGL ES
eglBindAPI(EGL_OPENGL_ES_API);
// pass our egl configuration to egl
eglChooseConfig(egldisplay,s_configAttribs,&eglconfig,1,&numconfigs);
printf("chooseconfig,\n");
assert(eglGetError()==EGL_SUCCESS);
assert(numconfigs==1);
eglNativeWindow=fsl_createwindow(egldisplay,eglNativeDisplayType);	
assert(eglNativeWindow);	
eglsurface=eglCreateWindowSurface(egldisplay,eglconfig,(EGLNativeWindowType)eglNativeWindow,NULL);
printf("createwindow,\n");
assert(eglGetError()==EGL_SUCCESS);
// create the egl graphics context
eglcontext=eglCreateContext(egldisplay,eglconfig,NULL,NULL);
printf("creatcontext,\n");
assert(eglGetError()==EGL_SUCCESS);
// make the context current
eglMakeCurrent(egldisplay,eglsurface,eglsurface,eglcontext);
printf("makecurrent,\n");
assert(eglGetError() == EGL_SUCCESS);
//...................................................................
printf("nvt=%d  stride=%d  usevrtcs=%d usetxtrs=%d usenrmls=%d\n",nvt,stride,usevrtcs,usetxtrs,usenrmls);
// -------------

glShadeModel(GL_SMOOTH);
glClearColor(0.0,0.0,0.0,0.0);                             // use Black as background color
glClearDepthf(1.0f);                                       // Depth Buffer Setup
glEnable(GL_DEPTH_TEST);                                   // Enables Depth Testing
glDepthFunc(GL_LEQUAL);                                    // The Type Of Depth Testing To Do
glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);          // Really Nice Perspective
glFrontFace(GL_CCW);		                           // Counter-clockwise winding.
glEnable(GL_CULL_FACE);	                                   // Enable face culling.
glCullFace(GL_BACK);		                           // What faces to remove with the face culling.



if (usevrtcs) 
  {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3,GL_FLOAT,stride*sizeof(GL_FLOAT),vt);
  }
if (usetxtrs) 
  {
  //(void)LoadGLTextures();
  //glGenTextures(2,&texture[0]);
  glTexCoordPointer(2,GL_FLOAT,stride*sizeof(GL_FLOAT),(const GLvoid*)&vt[3]);
  glEnable(GL_TEXTURE_2D);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }
if (usenrmls) 
  {
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT,stride*sizeof(GL_FLOAT),(const GLvoid*)&vt[5]);
  }
/*
RODBUG DOES THIS NEED TO BE DONE SOMEWHERE?
if (usenrmls) glDisableClientState(GL_NORMAL_ARRAY);
if (usetxtrs) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
if (usevrtcs) glDisableClientState(GL_VERTEX_ARRAY);
glDisable(GL_CULL_FACE);
*/


//...................................................................
/*
glEnable(GL_TEXTURE_2D);
glShadeModel(GL_SMOOTH);
glClearColor(0.25f,0.25f,0.25f,0.5f);
glClearDepthf(1.0f);
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LEQUAL);
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);
glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
initBezier();
if (!LoadGLTextures()) return 0;
genBezier(&mybezier, divs);
*/
eglQuerySurface(egldisplay,eglsurface,EGL_WIDTH,&w);
eglQuerySurface(egldisplay,eglsurface,EGL_HEIGHT,&h);
resize(w,h);
return 1;
}
//-------------------------------------------------------------------
void deinit(void)
{
printf("Cleaning up...\n");
eglMakeCurrent(egldisplay,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
assert(eglGetError()==EGL_SUCCESS);
eglDestroyContext(egldisplay,eglcontext);
eglDestroySurface(egldisplay,eglsurface);	
fsl_destroywindow(eglNativeWindow,eglNativeDisplayType);
eglTerminate(egldisplay);
assert(eglGetError()==EGL_SUCCESS);
eglReleaseThread();	
}
//-------------------------------------------------------------------
void sighandler(int signal) {printf("Quit because of signal %d\n", signal); quit=1;}
//-------------------------------------------------------------------
void changestate()
{
switch (state)
  {
  case 0:   state=1; break;
  case 1:  usetxtrs=1; glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D,texture[1]); state=2; break;
  case 2:                                           state=0; break;
  default: usetxtrs=0; glDisable(GL_TEXTURE_2D);                                         state=0; break;
  }
}
//===================================================================
int main(int argc,char** argv)
{
char *fname=(char*)malloc(100);
mode=0;
printf("argc=%d\n",argc);
if ((argc==2)||(argc==3)||(argc==4)) mode=argv[1][0]-'0';
if ((mode<1)||(mode>3)) 
  {
  printf("\nrodgl rev 1.1\nUsage: ./rodgl n [param1] [param2]  where n: 1=cube, 2=sphere, 3=OBJ_loader\n\n");
  printf("./rodgl 1 [texture]              draws a cube;   textures: 0=none, 1=colors, 2=landmarks\n");
  printf("./rodgl 2 [triangles] [texture]  draws a sphere; textures: 0=none, 1=earth, 2=moon\n");
  printf("./rodgl 3 OBJNAME                loads object='OBJmodels/OBJNAME.obj' and texture='textures/OBJNAME.bmp';\n\n");
  return 1;
  }
printf("Rod 01 e\n");
switch (mode)
  {
  case 1: LoadCube();                break; // Cube
  case 2: LoadSphere(atoi(argv[2])); break; // Sphere
  case 3: strcpy(fname,"OBJmodels/"); strcat(fname,argv[2]); strcat(fname,".obj"); OBJ_Load(fname); break; // Model
  }
//...................................................................
signal(SIGINT,sighandler); signal(SIGTERM,sighandler);
InitVariables();
assert(InitGL());
// Textures
switch (mode)
  {
  case 1: // Cube
          if ((argc==3)&&(argv[2][0]=='1')) {strcpy(fname,"textures/grid9.bmp");     LoadGLTextures(fname);}
    else  if ((argc==3)&&(argv[2][0]=='2')) {strcpy(fname,"textures/landmarks.bmp"); LoadGLTextures(fname);}
    else  {usetxtrs=0; glDisable(GL_TEXTURE_2D);}
    break;
  case 2: // Sphere
          if ((argc==4)&&(argv[3][0]=='1')) {strcpy(fname,"textures/earth.bmp");     LoadGLTextures(fname);}
    else  if ((argc==4)&&(argv[3][0]=='2')) {strcpy(fname,"textures/moon.bmp");      LoadGLTextures(fname);}
    else  {usetxtrs=0; glDisable(GL_TEXTURE_2D);}
    break;
  case 3: // Blender OBJ
    strcpy(fname,"textures/"); strcat(fname,argv[2]); strcat(fname,".bmp"); 
    if (!LoadGLTextures(fname)) {usetxtrs=0; glDisable(GL_TEXTURE_2D);}
    break;
  }
while (!quit)
  {
  render();
  eglSwapBuffers(egldisplay,eglsurface);
  currentFrame++;
  if ((currentFrame%500)==0) 
    {
    gettimeofday(&now,NULL); time1=now.tv_sec+now.tv_usec/1e6; dt=time1-time0; time0=time1;
    printf("nvt=%d  Frame=%d   FPS=%6.3f\n",nvt,currentFrame,500/dt);
    }
  }
deinit();
return 1;
}
//===================================================================


