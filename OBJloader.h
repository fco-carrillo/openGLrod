// OBJloader.h  RB 31Dec12
// ===========
// Bugs:
//-------------------------------------------------------------------
#include "GL/gl.h"                                         // to access OpenGL
// ------------------------------------------------------------------
int digit(char c);
int fdigit(char c);
void OBJ_vertices(char* l);
void OBJ_textures(char* l);
void OBJ_normals(char* l);
void OBJ_faces(char* l);
void OBJ_simplify(void);
void OBJ_Load(char* objname);

