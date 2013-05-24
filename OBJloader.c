// OBJloader.c  RB 31Dec12
// ===========
// Bugs:
// ------------------------------------------------------------------
#include "OBJloader.h"
#include <stdio.h>
// ------------------------------------------------------------------
extern GLfloat vt[50000];                                  // key array containing vertices + textures (+ normals)
extern int nvt,stride,usetxtrs,usenrmls;
// ------------------------------------------------------------------
int vertices,textures,normals,refvertices,reftextures,refnormals,faces,vtns,minvtns,NI;
int M[50000][3];
float V[15000][3],T[15000][2],N[15000][3];
//...................................................................
GLfloat tt[50000];
//-------------------------------------------------------------------
int digit(char c)  {if ((c>='0')&&(c<='9')) return 1; else return 0;}
//-------------------------------------------------------------------
int fdigit(char c) {if (((c>='0')&&(c<='9'))||(c=='.')||(c=='+')||(c=='-')) return 1; else return 0;}
//-------------------------------------------------------------------
void OBJ_vertices(char* l)
  {
  int p;
  float xx,yy,zz,sign,mant;
  p=0;
  while (l[p]>0x0D)
    {
    //vertex xx
    while ((l[p]>0x0D)&&(!fdigit(l[p]))) p++;
    xx=0.0; sign=1.0; mant=1.0;
    while ((l[p]>0x0D)&&(fdigit(l[p])))
      {
           if (l[p]=='-') sign=-1.0;
      else if (l[p]=='+') sign=+1.0;
      else if (l[p]=='.') mant=0.1;
      else if (mant>0.9) {xx*=10.0; xx+=l[p]-'0';} else {xx+=(l[p]-'0')*mant; mant/=10.0;}
      p++;
      }
    xx=xx*sign;
    //vertex yy
    while ((l[p]>0x0D)&&(!fdigit(l[p]))) p++;
    yy=0.0; sign=1.0; mant=1.0;
    while ((l[p]>0x0D)&&(fdigit(l[p])))
      {
           if (l[p]=='-') sign=-1.0;
      else if (l[p]=='+') sign=+1.0;
      else if (l[p]=='.') mant=0.1;
      else if (mant>0.9) {yy*=10.0; yy+=l[p]-'0';} else {yy+=(l[p]-'0')*mant; mant/=10.0;}
      p++;
      }
    yy=yy*sign;
    //vertex zz
    while ((l[p]>0x0D)&&(!fdigit(l[p]))) p++;
    zz=0.0; sign=1.0; mant=1.0;
    while ((l[p]>0x0D)&&(fdigit(l[p])))
      {
           if (l[p]=='-') sign=-1.0;
      else if (l[p]=='+') sign=+1.0;
      else if (l[p]=='.') mant=0.1;
      else if (mant>0.9) {zz*=10.0; zz+=l[p]-'0';} else {zz+=(l[p]-'0')*mant; mant/=10.0;}
      p++;
      }
    zz=zz*sign;
    //if (vertices==0) printf("%3d: [%9.6f] [%9.6f] [%9.6f]\n",vertices,xx,yy,zz);
    V[vertices][0]=xx; V[vertices][1]=yy; V[vertices][2]=zz;
    vertices++;
    }
  }
//-------------------------------------------------------------------
void OBJ_textures(char* l)
  {
  int p;
  float xx,yy,zz,sign,mant;
  p=0;
  while (l[p]>0x0D)
    {
    //texture xx
    while ((l[p]>0x0D)&&(!fdigit(l[p]))) p++;
    xx=0.0; sign=1.0; mant=1.0;
    while ((l[p]>0x0D)&&(fdigit(l[p])))
      {
           if (l[p]=='-') sign=-1.0;
      else if (l[p]=='+') sign=+1.0;
      else if (l[p]=='.') mant=0.1;
      else if (mant>0.9) {xx*=10.0; xx+=l[p]-'0';} else {xx+=(l[p]-'0')*mant; mant/=10.0;}
      p++;
      }
    xx=xx*sign;
    //texture yy
    while ((l[p]>0x0D)&&(!fdigit(l[p]))) p++;
    yy=0.0; sign=1.0; mant=1.0;
    while ((l[p]>0x0D)&&(fdigit(l[p])))
      {
           if (l[p]=='-') sign=-1.0;
      else if (l[p]=='+') sign=+1.0;
      else if (l[p]=='.') mant=0.1;
      else if (mant>0.9) {yy*=10.0; yy+=l[p]-'0';} else {yy+=(l[p]-'0')*mant; mant/=10.0;}
      p++;
      }
    yy=yy*sign;
    //texture zz
    while ((l[p]>0x0D)&&(!fdigit(l[p]))) p++;
    zz=0.0; sign=1.0; mant=1.0;
    while ((l[p]>0x0D)&&(fdigit(l[p])))
      {
           if (l[p]=='-') sign=-1.0;
      else if (l[p]=='+') sign=+1.0;
      else if (l[p]=='.') mant=0.1;
      else if (mant>0.9) {zz*=10.0; zz+=l[p]-'0';} else {zz+=(l[p]-'0')*mant; mant/=10.0;}
      p++;
      }
    zz=zz*sign;
    //if (textures==0) printf("%3d: [%9.6f] [%9.6f] [%9.6f]\n",textures,xx,yy,zz);
    T[textures][0]=xx; T[textures][1]=yy;
    textures++;
    }
  }
//-------------------------------------------------------------------
void OBJ_normals(char* l)
  {
  int p;
  float xx,yy,zz,sign,mant;
  p=0;
  while (l[p]>0x0D)
    {
    //normal xx
    while ((l[p]>0x0D)&&(!fdigit(l[p]))) p++;
    xx=0.0; sign=1.0; mant=1.0;
    while ((l[p]>0x0D)&&(fdigit(l[p])))
      {
           if (l[p]=='-') sign=-1.0;
      else if (l[p]=='+') sign=+1.0;
      else if (l[p]=='.') mant=0.1;
      else if (mant>0.9) {xx*=10.0; xx+=l[p]-'0';} else {xx+=(l[p]-'0')*mant; mant/=10.0;}
      p++;
      }
    xx=xx*sign;
    //normal yy
    while ((l[p]>0x0D)&&(!fdigit(l[p]))) p++;
    yy=0.0; sign=1.0; mant=1.0;
    while ((l[p]>0x0D)&&(fdigit(l[p])))
      {
           if (l[p]=='-') sign=-1.0;
      else if (l[p]=='+') sign=+1.0;
      else if (l[p]=='.') mant=0.1;
      else if (mant>0.9) {yy*=10.0; yy+=l[p]-'0';} else {yy+=(l[p]-'0')*mant; mant/=10.0;}
      p++;
      }
    yy=yy*sign;
    //normal zz
    while ((l[p]>0x0D)&&(!fdigit(l[p]))) p++;
    zz=0.0; sign=1.0; mant=1.0;
    while ((l[p]>0x0D)&&(fdigit(l[p])))
      {
           if (l[p]=='-') sign=-1.0;
      else if (l[p]=='+') sign=+1.0;
      else if (l[p]=='.') mant=0.1;
      else if (mant>0.9) {zz*=10.0; zz+=l[p]-'0';} else {zz+=(l[p]-'0')*mant; mant/=10.0;}
      p++;
      }
    zz=zz*sign;
    //if (normals==0) printf("%3d: [%9.6f] [%9.6f] [%9.6f]\n",normals,xx,yy,zz);
    N[normals][0]=xx; N[normals][1]=yy; N[normals][2]=zz;
    normals++;
    }
  }
//-------------------------------------------------------------------
void OBJ_faces(char* l)
  {
  int p,vrtx[5],text[5],nrml[5];
  int fv;
  //printf("%s\n",l);
  p=0;
  fv=0;
  while (l[p]>0x0D)
    {
    //vertex
    while ((l[p]>0x0D)&&(!digit(l[p]))) p++;
    vrtx[fv]=text[fv]=nrml[fv]=0;
    while ((l[p]>0x0D)&&(digit(l[p]))) {vrtx[fv]*=10; vrtx[fv]+=l[p]-'0'; p++;}
    if (l[p]=='/')
      {
      //texture
      while ((l[p]>0x0D)&&(!digit(l[p]))) p++;
      text[fv]=0;
      while ((l[p]>0x0D)&&(digit(l[p]))) {text[fv]*=10; text[fv]+=l[p]-'0'; p++;}
      }
    if (l[p]=='/')
      {
      //normals
      while ((l[p]>0x0D)&&(!digit(l[p]))) p++;
      nrml[fv]=0;
      while ((l[p]>0x0D)&&(digit(l[p]))) {nrml[fv]*=10; nrml[fv]+=l[p]-'0'; p++;}
      }
    fv++;
    }
  //printf("fv=%d  [%s]\n",fv,l);
  //printf("%d:%d:%d  ",vrtx,text,nrml);
  if (fv==3)
    {
    // Faces are Triangles
    M[vtns][0]=vrtx[0]; M[vtns][1]=text[0]; M[vtns++][2]=nrml[0];
    M[vtns][0]=vrtx[1]; M[vtns][1]=text[1]; M[vtns++][2]=nrml[1];
    M[vtns][0]=vrtx[2]; M[vtns][1]=text[2]; M[vtns++][2]=nrml[2];
    }
  else if (fv==4)
    {
    // Faces are Quads ===> replace with 2 Triangles
    M[vtns][0]=vrtx[0]; M[vtns][1]=text[0]; M[vtns++][2]=nrml[0];
    M[vtns][0]=vrtx[1]; M[vtns][1]=text[1]; M[vtns++][2]=nrml[1];
    M[vtns][0]=vrtx[2]; M[vtns][1]=text[2]; M[vtns++][2]=nrml[2];
    M[vtns][0]=vrtx[0]; M[vtns][1]=text[0]; M[vtns++][2]=nrml[0];
    M[vtns][0]=vrtx[2]; M[vtns][1]=text[2]; M[vtns++][2]=nrml[2];
    M[vtns][0]=vrtx[3]; M[vtns][1]=text[3]; M[vtns++][2]=nrml[3];
    }
  else printf("*** ERROR ***: allowable vertices per face are 3 or 4 ==> fv=%d  [%s]\n",fv,l);

  faces++;
  }
//-------------------------------------------------------------------
void OBJ_simplify(void)
  {
  int i,j,found;
  minvtns=0;
  refvertices=reftextures=refnormals=0;
  for (i=1;i<vtns;i++)
    {
    found=0;
    j=0; while ((found==0)&&(j<i)) {if ((M[i][0]==M[j][0])&&(M[i][1]==M[j][1])&&(M[i][2]==M[j][2])) found=1; j++;}
    if (found==0) minvtns++;
    }
  for (i=0;i<vtns;i++)
    {
    if (M[i][0]>refvertices) refvertices=M[i][0];
    if (M[i][1]>reftextures) reftextures=M[i][1];
    if (M[i][2]>refnormals)  refnormals =M[i][2];
    }
  }
//-------------------------------------------------------------------
void OBJ_Load(char* objname)
  {
  FILE *fin;
  char line[100];
  int i,j;
  printf("Loading object '%s'...\n",objname);
  fin=fopen(objname,"r");
  if (fin==0) {printf("Could not read file '%s'.\n",objname); return;}
  vertices=textures=faces=normals=vtns=0;
  while (!feof(fin))
    {
    fgets(line,100,fin);
    if ((line[0]=='v')&&(line[1]==' ')) OBJ_vertices(line);
    else if ((line[0]=='v')&&(line[1]=='t')) OBJ_textures(line);
    else if ((line[0]=='v')&&(line[1]=='n')) OBJ_normals(line);
    else if ((line[0]=='f')&&(line[1]==' ')) OBJ_faces(line);
    //else printf("%s",line);
    }
  fclose(fin);
  OBJ_simplify();
  printf("File has %d/%d vertices, %d/%d textures, %d/%d normals, and %d faces referencing %d vtns (3 x faces = %d).  Min vtns=%d\n",
         vertices,refvertices,textures,reftextures,normals,refnormals,faces,vtns,faces*3,minvtns);
  // now load vt[] array
  // 3D Vertex List + 2D Textures + 3D Normals

  j=0;
  for (i=0;i<vtns;i++)
    {
    vt[j++]=V[M[i][0]-1][0]; vt[j++]=V[M[i][0]-1][1]; vt[j++]=V[M[i][0]-1][2];
    if (textures) {vt[j++]=T[M[i][1]-1][0]; vt[j++]=T[M[i][1]-1][1];}
    if (normals)  {vt[j++]=N[M[i][2]-1][0]; vt[j++]=N[M[i][2]-1][1]; vt[j++]=N[M[i][2]-1][2];}
    }
  printf("vertices=%d textures=%d normals=%d faces=%d\n",vertices,textures,normals,faces);
  nvt=j;
  stride=3;
  if (textures) {stride+=2; usetxtrs=1;} else usetxtrs=0;
  if (normals)  {stride+=3; usenrmls=1;} else usenrmls=0;
  // Triangle Index List
  j=0;
  NI=vtns;
  //Create Test File
  /*
  FILE *fout;
  fout=fopen("rodres","w");
  fprintf(fout,"-------------- Vertices ----------------\n");
  for (i=0;i<vertices;i++) fprintf(fout,"%3d:  %9.6f  %9.6f  %9.6f\n",i,V[i][0],V[i][1],V[i][2]);
  fprintf(fout,"-------------- Textures ----------------\n");
  for (i=0;i<textures;i++) fprintf(fout,"%3d:  %9.6f  %9.6f\n",i,T[i][0],T[i][1]);
  fprintf(fout,"-------------- Normals  ----------------\n");
  for (i=0;i<normals;i++)  fprintf(fout,"%3d:  %9.6f  %9.6f  %9.6f\n",i,N[i][0],N[i][1],N[i][2]);
  fprintf(fout,"************** Indexes  ****************\n");
  for (i=0;i<vtns;i++)     fprintf(fout,"%3d:  %3d %3d %3d\n",i,M[i][0],M[i][1],M[i][2]);
  fprintf(fout,"\n\n========================================\n\n");
  i=0; while (i<nvt) {if (i%3==0) fprintf(fout,"\n%3d  ",i); fprintf(fout,"%9.6f  ",vt[i]); i++;}
  fclose(fout);
  printf("stride=%d  nvt=%d  NI=%d\n",stride,nvt,NI);
  */
  }
//-------------------------------------------------------------------

