#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

//include header file for glfw library so that we can use OpenGL
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <stdio.h>
#include "Vectors.h"

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265 // Should be used from mathlib

using namespace std;

/*
For UC Berkeley's CS184 Fall 2016 course, assignment 3 (Bezier surfaces)
*/

//****************************************************
// Global Variables
//****************************************************
GLfloat translation[3] = {0.0f, 0.0f, 0.0f};
float x_rot_angle = 0.0f;
float y_rot_angle = 0.0f;
GLfloat zoom[4] = {-2, 2, -2, 2};

bool auto_strech = false;
int Width_global = 400;
int Height_global = 400;
int Z_buffer_bit_depth = 128;

bool isFilled = true;
bool isSmooth = true;

float subdivision_param;
string flag;
vector<vector<vector<Vector3>>> patch;

int numdiv = 0;

inline float sqr(float x) { return x*x; }

struct subDPatch {
  Vector3 point;
  Vector3 normal;

  subDPatch() : point(Vector3()), normal(Vector3()) {};
  subDPatch(Vector3 x, Vector3 y) : point(x), normal(y) {}; 
};

vector<subDPatch> unif;

struct curveInterp {
  Vector3 point;
  Vector3 derivative;

  curveInterp() : point(Vector3()), derivative(Vector3()) {};
  curveInterp(Vector3 x, Vector3 y) : point(x), derivative(y) {}; 
};

struct UV {
  float u;
  float v;

  UV(float _u, float _v) : u(_u), v(_v) {}; 
};

struct Triangle {
  subDPatch a;
  UV uva;
  subDPatch b;
  UV uvb;
  subDPatch c;
  UV uvc;

  Triangle(subDPatch _a, UV _uva, subDPatch _b, UV _uvb, subDPatch _c, UV _uvc) :
    a(_a), uva(_uva), b(_b), uvb(_uvb), c(_c), uvc(_uvc) {};
};


//****************************************************
// Simple init function
//****************************************************
void initializeRendering()
{
    glfwInit();
}

//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************
void setPixel(float x, float y, GLfloat r, GLfloat g, GLfloat b) {
    glColor3f(r, g, b);
    glVertex2f(x+0.5, y+0.5);  // The 0.5 is to target pixel centers
    // Note: Need to check for gap bug on inst machines.
}

//****************************************************
// Keyboard inputs. Add things to match the spec! 
//****************************************************
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        case GLFW_KEY_Q: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        // when arrow keys are pressed object will be rotated
        case GLFW_KEY_LEFT :
            if (action && mods == GLFW_MOD_SHIFT) {
              translation[0] -= 0.001f * Width_global;
            }
            else if (action) {
              y_rot_angle -= 30;
            }
            break;
        case GLFW_KEY_RIGHT:
            if (action && mods == GLFW_MOD_SHIFT) {
              translation[0] += 0.001f * Width_global;
            }
            else if (action) {
              y_rot_angle += 30;
            }
            break;
        case GLFW_KEY_UP   :
            if (action && mods == GLFW_MOD_SHIFT) {
              translation[1] += 0.001f * Height_global;
            }
            else if (action) {
              x_rot_angle += 30;
            }
            break;
        case GLFW_KEY_DOWN :
            if (action && mods == GLFW_MOD_SHIFT) {
              translation[1] -= 0.001f * Height_global;
            }
            else if (action) {
              x_rot_angle -= 30;
            }
            break;
        case GLFW_KEY_F:
            if (action && mods == GLFW_MOD_SHIFT) auto_strech = !auto_strech; break;
        case GLFW_KEY_S:
        // program will toggle between flat and smooth shading   
          if (action) {
            cout << "pressed s" << endl;
            if (isSmooth) {
              // glEnable(GL_FLAT);
              glShadeModel(GL_FLAT);
              isSmooth = false;
            } else {
              // glEnable(GL_SMOOTH);
              glShadeModel(GL_SMOOTH);
              isSmooth = true;
            }
          }
          break;
        case GLFW_KEY_W:
          // toggle between filled and wireframe mode
          if (action) {
            cout << "pressed w" << endl;
            if (isFilled) {
              glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
              isFilled = false;
            } else {
              glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
              isFilled = true;
            }
          }
          break;
        case GLFW_KEY_EQUAL:
          if (action && mods == GLFW_MOD_SHIFT) {
            cout << "pressed +" << endl;
            zoom[0] += 0.25;
            zoom[1] -= 0.25;
            zoom[2] += 0.25;
            zoom[3] -= 0.25;
          }
          break;
        case GLFW_KEY_MINUS:
          if (action) {
            cout << "pressed -" << endl;
            zoom[0] -= 0.25;
            zoom[1] += 0.25;
            zoom[2] -= 0.25;
            zoom[3] += 0.25;
          }
          break;
        case GLFW_KEY_SPACE: break;
            
        default: break;
    } 
}

curveInterp bezCurveInterp(vector<Vector3> curve, float u) {
  Vector3 A = (1.0-u) * curve[0] + u * curve[1];
  Vector3 B = (1.0-u) * curve[1] + u * curve[2];
  Vector3 C = (1.0-u) * curve[2] + u * curve[3];

  Vector3 D = (1.0-u) * A + u * B;
  Vector3 E = (1.0-u) * B + u * C;

  Vector3 P = (1.0-u) * D + u * E;
  Vector3 dPdu = 3*(E-D);

  curveInterp c(P, dPdu);

  return c;
}

subDPatch bezPatchInterp(vector<vector<Vector3>> patch, float u, float v) {
  vector<Vector3> ucurve(4), vcurve(4), tmp(4);

  for (int i=0; i<4; i++) {
      ucurve[i] = bezCurveInterp(patch[i], v).point;
  }
  
  for (int i=0; i<4; i++) {
      tmp[0] = patch[0][i];
      tmp[1] = patch[1][i];
      tmp[2] = patch[2][i];
      tmp[3] = patch[3][i];
      vcurve[i] = bezCurveInterp(tmp, u).point;
  }
  
  curveInterp pv = bezCurveInterp(vcurve, v);
  curveInterp pu = bezCurveInterp(ucurve, u);

  Vector3 norm = pv.derivative.cross(pu.derivative);
  // Vector3 norm = pu.derivative.cross(pv.derivative);
  if (norm == Vector3()) {
    float x, y;
    if (u == 1) { x = 0.009;}
    if (v == 1) { y = 0.009;}
    if (u == 0) { x = 0.001;}
    if (v == 0) { y = 0.001;}
    return bezPatchInterp(patch, x, y);
  }

  norm.normalize();
  
  subDPatch sub(pu.point, norm);

  return sub;
}

void uniformSubdividePatch(vector<vector<Vector3>> patch) {
  float epsilon = 0.00001;
  float u, v = 0.0f;

  numdiv = (int) ((1 + epsilon) / subdivision_param);

  for (int iu = 0; iu<=numdiv; iu++) {
      u = iu * subdivision_param;
      for (int iv = 0; iv<=numdiv; iv++) {
          v = iv * subdivision_param;

          subDPatch dpatch = bezPatchInterp(patch, u, v);
          unif.push_back(dpatch);
      }
  }
}


void adaptiveSubdividePatch(vector<vector<Vector3>> patch, Triangle tri, vector<Triangle> &newTriangle, int recursionDepth) {

  bool e1 = false;
  bool e2 = false;
  bool e3 = false;

  if (recursionDepth > 10) {
    newTriangle.push_back(tri);
    return;
  }

  Vector3 e1_mid = (tri.a.point + tri.b.point)/2;
  float e1u = (tri.uva.u + tri.uvb.u)/2;
  float e1v = (tri.uva.v + tri.uvb.v)/2;

  subDPatch e1_edge = bezPatchInterp(patch, e1u, e1v);
  float error = e1_edge.point.distance(e1_mid);
  if (error > subdivision_param) {
    e1 = true;
  }

  Vector3 e2_mid = (tri.a.point + tri.c.point)/2;
  float e2u = (tri.uva.u + tri.uvc.u)/2;
  float e2v = (tri.uva.v + tri.uvc.v)/2;

  subDPatch e2_edge = bezPatchInterp(patch, e2u, e2v);
  error = e2_edge.point.distance(e2_mid);
  if (error > subdivision_param) {
    e2 = true;
  }


  Vector3 e3_mid = (tri.b.point + tri.c.point)/2;
  float e3u = (tri.uvb.u + tri.uvc.u)/2;
  float e3v = (tri.uvb.v + tri.uvc.v)/2;

  subDPatch e3_edge = bezPatchInterp(patch, e3u, e3v);
  error = e3_edge.point.distance(e3_mid);
  if (error > subdivision_param) {
    e3 = true;
  }

  UV e1uv = UV(e1u, e1v);
  UV e2uv = UV(e2u, e2v);
  UV e3uv = UV(e3u, e3v);

  // success
  if (!e1 && !e2 && !e3) {
    newTriangle.push_back(tri);
    return;
  }
  // divide from a
  else if (!e1 && !e2 && e3) {
    // not CC
    Triangle t1 = Triangle(tri.a, tri.uva, e3_edge, e3uv, tri.c, tri.uvc);
    Triangle t2 = Triangle(tri.a, tri.uva, tri.b, tri.uvb, e3_edge, e3uv);
    adaptiveSubdividePatch(patch, t2, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t1, newTriangle, recursionDepth+1);
  }
  // divide from b
  else if (!e1 && e2 && !e3) {
    Triangle t1 = Triangle(tri.a, tri.uva, tri.b, tri.uvb, e2_edge, e2uv);
    Triangle t2 = Triangle(e2_edge, e2uv, tri.b, tri.uvb, tri.c, tri.uvc);
    adaptiveSubdividePatch(patch, t2, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t1, newTriangle, recursionDepth+1);
  }
  // divide from c
  else if (e1 && !e2 && !e3) {
    Triangle t1 = Triangle(tri.a, tri.uva, e1_edge, e1uv, tri.c, tri.uvc);
    Triangle t2 = Triangle(e1_edge, e1uv, tri.b, tri.uvb, tri.c, tri.uvc);
    adaptiveSubdividePatch(patch, t1, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t2, newTriangle, recursionDepth+1);
  }
  // divide from a and then to b
  else if (!e1 && e2 && e3) {
    Triangle t1 = Triangle(tri.a, tri.uva, tri.b, tri.uvb, e3_edge, e3uv);
    Triangle t2 = Triangle(tri.a, tri.uva, e3_edge, e3uv, e2_edge, e2uv);
    Triangle t3 = Triangle(e2_edge, e2uv, e3_edge, e3uv, tri.c, tri.uvc);
    adaptiveSubdividePatch(patch, t2, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t1, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t3, newTriangle, recursionDepth+1);
  }
  // divide from b and then to c
  else if (e1 && e2 && !e3) {
    Triangle t1 = Triangle(tri.a, tri.uva, e1_edge, e1uv, e2_edge, e2uv);
    Triangle t2 = Triangle(e1_edge, e1uv, tri.b, tri.uvb, e2_edge, e2uv);
    Triangle t3 = Triangle(e2_edge, e2uv, tri.b, tri.uvb, tri.c, tri.uvc);
    adaptiveSubdividePatch(patch, t3, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t1, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t2, newTriangle, recursionDepth+1);
  }
  // divide from c and then to a
  else if (e1 && !e2 && e3) {
    Triangle t1 = Triangle(tri.a, tri.uva, e1_edge, e1uv, tri.c, tri.uvc);
    Triangle t2 = Triangle(e1_edge, e1uv, tri.b, tri.uvb, e3_edge, e3uv);
    Triangle t3 = Triangle(e1_edge, e1uv, e3_edge, e3uv, tri.c, tri.uvc);
    adaptiveSubdividePatch(patch, t1, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t3, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t2, newTriangle, recursionDepth+1);
  }
  // 4 triangles
  else if (e1 && e2 && e3) {
    Triangle t1 = Triangle(tri.a, tri.uva, e1_edge, e1uv, e2_edge, e2uv);
    Triangle t2 = Triangle(e1_edge, e1uv, tri.b, tri.uvb, e3_edge, e3uv);
    Triangle t3 = Triangle(e1_edge, e1uv, e2_edge, e2uv, e3_edge, e3uv);
    Triangle t4 = Triangle(e2_edge, e2uv, e3_edge, e3uv, tri.c, tri.uvc);
    adaptiveSubdividePatch(patch, t2, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t1, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t4, newTriangle, recursionDepth+1);
    adaptiveSubdividePatch(patch, t3, newTriangle, recursionDepth+1);
  } else {
    cout << "err" << endl;
  }
}


void drawPatches() {
  vector<vector<Triangle>> outputTriangle;

  GLfloat cyan[] = {0.f, .8f, .8f, 0.f};
  GLfloat lighter[] = {0.0f, .1f, .1f, 0.f};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cyan);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lighter);
  
  if (flag != "-a") {

    for (int i=0; i < patch.size(); i++) {
      uniformSubdividePatch(patch[i]);
    }

    for (int j=0; j<unif.size(); j+=sqr(numdiv+1)) { 
        for (int k = j; k<j+sqr(numdiv); k+=numdiv+1) {
            for (int i=k; i<k+numdiv; i++) { 

                glBegin(GL_QUADS);
                
                glNormal3f(unif[i].normal[0], unif[i].normal[1], unif[i].normal[2]);
                glVertex3f(unif[i].point[0], unif[i].point[1], unif[i].point[2]);
                
                glNormal3f(unif[i+numdiv+1].normal[0], unif[i+numdiv+1].normal[1], unif[i+numdiv+1].normal[2]);
                glVertex3f(unif[i+numdiv+1].point[0], unif[i+numdiv+1].point[1], unif[i+numdiv+1].point[2]);
                
                glNormal3f(unif[i+numdiv+2].normal[0], unif[i+numdiv+2].normal[1], unif[i+numdiv+2].normal[2]);
                glVertex3f(unif[i+numdiv+2].point[0], unif[i+numdiv+2].point[1], unif[i+numdiv+2].point[2]);
                
                glNormal3f(unif[i+1].normal[0], unif[i+1].normal[1], unif[i+1].normal[2]);
                glVertex3f(unif[i+1].point[0], unif[i+1].point[1], unif[i+1].point[2]);
                
                glEnd();
                
            }
        }
    }
  }
  else {
    for (int p=0; p<patch.size(); p++) {

      subDPatch topLeft = bezPatchInterp(patch[p], 0, 0); //bl
      subDPatch bottomLeft = bezPatchInterp(patch[p], 1, 0); //br
      subDPatch topRight = bezPatchInterp(patch[p], 0, 1); //tr
      subDPatch bottomRight = bezPatchInterp(patch[p], 1, 1); // tr

      UV a(0, 0);
      UV b(1, 0);
      UV c(0, 1);
      UV d(1, 1);
      Triangle first(topLeft, a, bottomLeft, b, topRight, c);
      Triangle second(bottomLeft, b, topRight, c, bottomRight, d);

      vector<Triangle> newTriangle;

      adaptiveSubdividePatch(patch[p], first, newTriangle, 0);
      adaptiveSubdividePatch(patch[p], second, newTriangle, 0);

      outputTriangle.push_back(newTriangle);
    }

    // for (int j=0; j<unif.size(); j+=sqr(numdiv+1)) { //j = start of new patch
    //     for (int k = j; k<j+sqr(numdiv); k+=numdiv+1) 

    for (int i=0; i<outputTriangle.size(); i++) {
      for (int j=0; j<outputTriangle[i].size(); j++) {
        Triangle tri = outputTriangle[i][j];

        glBegin(GL_TRIANGLES);
        glNormal3f(tri.a.normal[0], tri.a.normal[1], tri.a.normal[2]);
        glVertex3f(tri.a.point[0], tri.a.point[1], tri.a.point[2]);

        glNormal3f(tri.b.normal[0], tri.b.normal[1], tri.b.normal[2]);
        glVertex3f(tri.b.point[0], tri.b.point[1], tri.b.point[2]);

        glNormal3f(tri.c.normal[0], tri.c.normal[1], tri.c.normal[2]);
        glVertex3f(tri.c.point[0], tri.c.point[1], tri.c.point[2]);
        glEnd();
      }
    }
  }
}



//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void display( GLFWwindow* window )
{
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black)
    
    glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
    glLoadIdentity();                            // make sure transformation is "zero'd"
    
    glOrtho(zoom[0], zoom[1], zoom[2], zoom[3], 5, -5);


    GLfloat ambientColor[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color(0.2, 0.2, 0.2)
    GLfloat light0_position[] = { -1, 0.5, 3.5, 0 };
    GLfloat light0_brightness[] = { 0.5, 0.2, 0.2, 0 }; // use direction light
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_brightness);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_brightness);

    GLfloat light1_position[] = { 2, -2, -2, 0 };
    GLfloat light1_brightness[] = { 0.3, 0.2, 0.5, 0};
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_brightness);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_brightness);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    glEnable(GL_LIGHTING);                    // enables lighting
    glEnable(GL_LIGHT0);                    // enables light0
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);                // enables opengl to use glColor3f to define material color
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


    //----------------------- code to draw objects --------------------------
    glPushMatrix();
    glTranslatef (translation[0], translation[1], translation[2]);
    glRotatef(y_rot_angle, 0, 1, 0);
    glRotatef(x_rot_angle, 1, 0, 0);

    drawPatches();

    glPopMatrix();
    
    glfwSwapBuffers(window);

    // note: check out glPolygonMode and glShadeModel 
    // for wireframe and shading commands
    
}

//****************************************************
// function that is called when window is resized
//***************************************************
void size_callback(GLFWwindow* window, int width, int height)
{
    // Get the pixel coordinate of the window
    // it returns the size, in pixels, of the framebuffer of the specified window
    glfwGetFramebufferSize(window, &Width_global, &Height_global);
    
    glViewport(0, 0, Width_global, Height_global);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, Width_global, 0, Height_global, 1, -1);
    
    display(window);
}


void parse(string filename) {
  string line;
  ifstream infile;
  infile.open(filename, ios::in);

  cout << "filename " << filename << endl;
  int patch_num = 0;

  if (infile.is_open()) {
    if (getline(infile, line)) {
      string num;
      stringstream ss(line);
      ss >> num;
      for (int i=0; i<stoi(num); i++) {
        vector<vector<Vector3>> ctr_points;
        patch.push_back(ctr_points);
      }
    }

    while (getline(infile, line)) {
      stringstream ss(line);
      vector<string> v;
      int pos = 0;
      string token;

      while (ss >> token) {
        v.push_back(token);
      }

      if (v.size() == 0) {
        patch_num++;
      }
      else {
        vector<Vector3> row;
        for(int i=0; i<v.size(); i+=3) {
          Vector3 point(stof(v[i]), stof(v[i+1]), stof(v[i+2]));
          row.push_back(point);
        }
        patch[patch_num].push_back(row);
      }

    }
    infile.close();
  } else {
    cout << "unable to open file" << endl;
  }
  
  cout << "file size " << patch.size() << endl;
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

    if (argc < 3 || argc > 4) {
      return -1;
    }

    string filename = argv[1];
    subdivision_param = atof(argv[2]);
    flag = "-u";
    if (argc == 4) {
      string temp = argv[3];
      if (temp.compare("-a") == 0) {
        flag = "-a";
      }
    }

    parse(filename);

    // if (flag == "-a") {
    //   for (int i=0; i < patch.size(); i++) {
    //     uniformSubdividePatch(patch[i]);
    //   }
    // } else {
    //   for (int i=0; i < patch.size(); i++) {
    //     uniformSubdividePatch(patch[i]);
    //   }
    // }

    //This initializes glfw
    initializeRendering();
    
    GLFWwindow* window = glfwCreateWindow( Width_global, Height_global, "CS184", NULL, NULL );
    if ( !window )
    {
        cerr << "Error on window creating" << endl;
        glfwTerminate();
        return -1;
    }

    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if ( !mode )
    {
        cerr << "Error on getting monitor" << endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent( window );
    
    // Get the pixel coordinate of the window
    // it returns the size, in pixels, of the framebuffer of the specified window
    glfwGetFramebufferSize(window, &Width_global, &Height_global);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(-3.5, 3.5, -3.5, 3.5, 5, -5);
  	glOrtho(zoom[0], zoom[1], zoom[2], zoom[3], 5, -5);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();

    glEnable(GL_DEPTH_TEST);	// enable z-buffering
    glDepthFunc(GL_LESS);

    glfwSetWindowTitle(window, "CS184");
    glfwSetWindowSizeCallback(window, size_callback);
    glfwSetKeyCallback(window, key_callback);

    while( !glfwWindowShouldClose( window ) ) // infinite loop to draw object again and again
    {   // because once object is draw then window is terminated
        display( window );
        
        if (auto_strech){
            glfwSetWindowSize(window, mode->width, mode->height);
            glfwSetWindowPos(window, 0, 0);
        }
        
        glfwPollEvents();
        
    }

    return 0;
}