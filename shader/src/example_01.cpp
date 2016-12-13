#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

//include header file for glfw library so that we can use OpenGL
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265 // Should be used from mathlib
#define MAX(a,b) ((a>b) ? a:b)

using namespace std;

//****************************************************
// Global Variables
//****************************************************
GLfloat translation[3] = {0.0f, 0.0f, 0.0f};
bool auto_strech = false;
int Width_global = 400;
int Height_global = 400;

// BRDF coefficients
vector<float> ka = {0, 0, 0}; 
vector<float> kd = {0, 0, 0}; 
vector<float> ks = {0, 0, 0}; 

// specular power coefficient
float spu = -1.0f;
float spv = -1.0f;
float sp = 1.0f;

// For more than 5 lights:
int dl_count = 0;
vector<vector<float>> dir_lights_l(5, vector<float>(3));
vector<vector<float>> dir_lights_c(5, vector<float>(3));
int pl_count = 0;
vector<vector<float>> point_lights_l(5, vector<float>(3));
vector<vector<float>> point_lights_c(5, vector<float>(3));

inline float sqr(float x) { return x*x; }

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
// Keyboard inputs
//****************************************************
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
            
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        case GLFW_KEY_Q: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        case GLFW_KEY_LEFT :
            if (action) translation[0] -= 0.01f * Width_global; break;
        case GLFW_KEY_RIGHT:
            if (action) translation[0] += 0.01f * Width_global; break;
        case GLFW_KEY_UP   :
            if (action) translation[1] += 0.01f * Height_global; break;
        case GLFW_KEY_DOWN :
            if (action) translation[1] -= 0.01f * Height_global; break;
        case GLFW_KEY_F:
            if (action) auto_strech = !auto_strech; break;
        case GLFW_KEY_SPACE: break;
            
        default: break;
    }
    
}

//****************************************************
// Helper functions
//****************************************************
void printVec (vector<float> &v) {
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    } 
    cout << endl;
}

// Vector operation helper functions
float v_dot(vector<float> a, vector<float> b){
    // compute
    float product = 0;
    for (int i = 0; i <= a.size()-1; i++)
       product += (a[i])*(b[i]); // += means add to product
    return product;
}

vector<float> v_cross(vector<float> &v, vector<float> &u) {
    vector<float> f;
    f.push_back(v[1]*u[2] - v[2]*u[1]);
    f.push_back(v[2]*u[0] - v[0]*u[2]);
    f.push_back(v[0]*u[1] - v[1]*u[0]);
    return f;
}

vector<float> v_mul(vector<float> a, vector<float> b){
    vector<float> result(a.size());
    for (int i = 0; i <= a.size()-1; i++)
        result[i] = (a[i])*(b[i]);
    return result;
}

// Scaler multiplication
vector<float> v_s_mul(vector<float> a, float c){
    vector<float> result(a.size());
    for (int i = 0; i <= a.size()-1; i++)
        result[i] = (a[i])*c;
    return result;
}

float v_magnitude(vector<float> v) {
    return sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
}

vector<float> v_normalize(vector<float> v){
    float length = v_magnitude(v);

    for (int i = 0; i <= v.size()-1; i++)
        v[i] /= length;
    return v;
}

// Subtract vector a from b
vector<float> v_sub(vector<float> a, vector<float> b){
    vector<float> result(a.size());
    for (int i = 0; i <= a.size()-1; i++)
        result[i] = a[i] - b[i];
    return result;
}

vector<float> v_sum(vector<float> a, vector<float> b){
    vector<float> result(a.size());
    for (int i = 0; i <= a.size()-1; i++)
        result[i] = a[i] + b[i];
    return result;
}

vector<float> half_vector(vector<float> &light, vector<float> &view) {
    vector<float> temp;
    for (int i=0; i<3; i++) {
        temp.push_back(light[i] + view[i]);
    }

    float length = v_magnitude(temp);
    vector<float> v;
    for (int i=0;i<3; i++) {
        v.push_back((light[i] + view[i])/length);
    }
    return v;
}

// Anisotropic BRDF helper functions
// L, N, E(viewer), H(half)
float ANIexponent(vector<float> &v, vector<float> &u, vector<float> &half, vector<float> &normal, float spu, float spv) {
  float num = pow(v_dot(u, half), 2) * spu + pow(v_dot(v, half), 2) * spv;
  float denom = 1-pow(v_dot(half, normal), 2);

  return num/denom;
}

// Isotropic BRDF helper functions
// L, N, V, R
vector<float> findISOR (vector<float> ka, vector<float> kd, vector<float> ks,  
                    vector<float> l_vec, vector<float> n_vec, vector<float> v_vec, vector<float> r_vec,
                    vector<float> color, float sp){
    
    vector<float> ambient = v_mul(ka, color);
    vector<float> diffuse = v_mul(kd, v_s_mul(color, MAX(v_dot(l_vec,n_vec), 0.f)));
    vector<float> specular = v_mul(ks, v_s_mul(color, pow( MAX(v_dot(r_vec,v_vec), 0.f), sp) ));

    vector<float> R = v_sum(v_sum(ambient, diffuse), specular);
    return R;
}



// NOT USED:  Ashikhmin Shirley Equation
vector<float> Fresnel(vector<float> &ks, vector<float> &view, vector<float> &half) {
    vector<float> f;
    for (int i=0; i<3; i++) {
        f.push_back(ks[i] + (1-ks[i]) * pow(1 - half[i] * view[i], 5));
    }
    return f;
}

vector<float> ANI_specular(vector<float> ks, float spu, float spv, 
    vector<float> u_param, vector<float> v_param, 
    vector<float> l_vec, vector<float> n_vec, vector<float> e_vec, vector<float> h_vec) {
    
    // first Term
    float firstTerm = sqrt((spu + 1) * (spv + 1))/(8 * PI);

    // second Term
    float exp = (spu * pow(v_dot(h_vec, u_param), 2) + spv * pow(v_dot(h_vec, v_param), 2)) / (1 - pow(v_dot(h_vec, n_vec), 2));
    float num = pow(v_dot(n_vec, h_vec), exp);
    float denom = v_dot(h_vec, e_vec) * MAX(v_dot(n_vec, e_vec), v_dot(n_vec, l_vec));

    // third Term
    vector<float> thirdTerm = Fresnel(ks, e_vec, h_vec);

    vector<float> diffuse_agg = v_s_mul(thirdTerm, (firstTerm * (num / denom)));
    return diffuse_agg;
}

vector<float> ANI_diffuse(vector<float> kd, vector<float> ks, vector<float> &normal, vector<float> &view, vector<float> &light) {
    
    // first Term
    float firstTerm = 28 / (23 * PI);

    // second Term implemented implicitly 
    
    // third Term
    float thirdTerm = (1 - pow((1 - v_dot(normal, view)/2), 5));

    // last Term 
    float lastTerm = (1 - pow((1 - v_dot(normal, light)/2), 5));

    vector<float> diffuse_agg;
    for (int i =0; i<3; i++) {
        diffuse_agg.push_back(firstTerm * kd[i] * (1 - ks[i]) * thirdTerm * lastTerm);
    }

    return diffuse_agg;
}


//****************************************************
// Draw a filled circle.
//****************************************************
void drawCircle(float centerX, float centerY, float radius) {
    // Draw inner circle
    glBegin(GL_POINTS);

    int minI = max(0,(int)floor(centerX-radius));
    int maxI = min(Width_global-1,(int)ceil(centerX+radius));

    int minJ = max(0,(int)floor(centerY-radius));
    int maxJ = min(Height_global-1,(int)ceil(centerY+radius));

    vector<float> origin = {0, 0, 0};
    for (int i = 0; i < Width_global; i++) {
        for (int j = 0; j < Height_global; j++) {

            // Location of the center of pixel relative to center of sphere
            float x = (i+0.5-centerX);
            float y = (j+0.5-centerY);

            float dist = sqrt(sqr(x) + sqr(y));

            if (dist <= radius) {
                // This is the front-facing Z coordinate
                float z = sqrt(radius*radius-dist*dist);

                // current pixel(p) point(pt)
                vector<float> ppt = {x, y, z};
                // calculate N: normal from point
                vector<float> n_vec = v_normalize(ppt);
                // calculate V: viewer direction
                vector<float> v_vec = v_normalize({0, 0, z});
                vector<float> e_vec = v_vec; //for anisotropic, avoid confusion with v
                
                // result
                vector<float> R = {0, 0, 0};
                vector<float> this_R;

                // Accumulate directional lights
                for (int d_c = 0; d_c < dl_count; d_c++) {
                    vector<float> dl_l = dir_lights_l[d_c];
                    vector<float> dl_c = dir_lights_c[d_c];

                    // Calculate L 
                    vector<float> dl_l_vec = v_normalize(dl_l);
                    // Calculate R
                    vector<float> dl_right = v_s_mul(v_s_mul(n_vec,(v_dot(dl_l_vec,n_vec))) , 2);
                    vector<float> dl_r_vec = v_normalize(v_sum(v_s_mul(dl_l_vec, -1), dl_right));
    
                    // ----- Anisotropic ----- 
                    if (spu != -1.0 && spv != -1.0 && spu != spv) {
                        if (spu != -1 && spu == spv) {  // take case of one sp case
                            sp = spv;
                        } else {
                            vector<float> y_vec = v_normalize({0.0, y, 0.0});
                            vector<float> v_param = v_normalize(v_sub(y_vec, v_s_mul(n_vec, v_dot(n_vec, y_vec))));
                            vector<float> u_param = v_normalize(v_cross(v_param, n_vec));

                            vector<float> half = half_vector(dl_l_vec, v_vec);

                            float exp = ANIexponent(v_param, u_param, half, n_vec, spu, spv);
                            this_R = findISOR(ka, kd, ks, dl_l_vec, n_vec, v_vec, dl_r_vec, dl_c, exp); 
                        }
                    } else {
                        this_R = findISOR(ka, kd, ks, dl_l_vec, n_vec, v_vec, dl_r_vec, dl_c, sp); 
                    }

                    R = v_sum(R, this_R);
                }

                // Accumulate point lights
                for (int p_c = 0; p_c < pl_count; p_c++) {
                    vector<float> pl_l = point_lights_l[p_c];
                    vector<float> pl_c = point_lights_c[p_c];
                    
                    // Calculate L 
                    vector<float> pl_l_vec = v_normalize(v_sub(pl_l, n_vec));
                    // Calculate R 
                    vector<float> pl_right = v_s_mul(v_s_mul(n_vec,(v_dot(pl_l_vec,n_vec))) , 2);
                    vector<float> pl_r_vec = v_normalize(v_sum(v_s_mul(pl_l_vec, -1), pl_right));

                    // ----- Anisotropic ----- 
                    if (spu != -1.0 && spv != -1.0 && spu != spv) {
                        if (spu != -1 && spu == spv) {  // take case of one sp case
                            sp = spv;
                        } else {
                            vector<float> y_vec = v_normalize({0.0, y, 0.0});
                            vector<float> v_param = v_normalize(v_sub(y_vec, v_s_mul(n_vec, v_dot(n_vec, y_vec))));
                            vector<float> u_param = v_normalize(v_cross(v_param, n_vec));

                            vector<float> half = half_vector(pl_l_vec, v_vec);

                            float exp = ANIexponent(v_param, u_param, half, n_vec, spu, spv);
                            this_R = findISOR(ka, kd, ks, pl_l_vec, n_vec, v_vec, pl_r_vec, pl_c, exp); 
                        }
                    } else {
                        this_R = findISOR(ka, kd, ks, pl_l_vec, n_vec, v_vec, pl_r_vec, pl_c, sp); 
                    }

                    R = v_sum(R, this_R);
                }

                    setPixel(i,j, R[0], R[1], R[2]);    
                
            }
        }
    }

    glEnd();
}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void display( GLFWwindow* window )
{
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black
    
    glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)
    
    glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
    glLoadIdentity();                            // make sure transformation is "zero'd"
    
    //----------------------- code to draw objects --------------------------
    glPushMatrix();
    glTranslatef (translation[0], translation[1], translation[2]);
    drawCircle(Width_global / 2.0 , Height_global / 2.0 , min(Width_global, Height_global) * 0.8 / 2.0);
    glPopMatrix();
    
    glfwSwapBuffers(window);
    
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

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
    int i = 1;
    while (i < argc) {
        string curr_arg = argv[i];
        if (curr_arg.compare("-ka") == 0) {
            ka = {atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3])};
            i += 4;
        } else if (curr_arg.compare("-kd") == 0){
            kd = {atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3])};
            i += 4;
        } else if (curr_arg.compare("-ks") == 0) {
            ks = {atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3])};
            i += 4;
        } else if (curr_arg.compare("-spu") == 0) {
            spu = atof(argv[i+1]);
            i += 2;
        } else if (curr_arg.compare("-spv") == 0) {
            spv = atof(argv[i+1]);
            i += 2;
        } else if (curr_arg.compare("-sp") == 0) {
            sp = atof(argv[i+1]);
            i += 2;
        } else if (curr_arg.compare("-pl") == 0) {
            vector<float> pl_l = {atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3])};
            vector<float> pl_c = {atof(argv[i+4]), atof(argv[i+5]), atof(argv[i+6])};

            point_lights_l[pl_count] = pl_l;
            point_lights_c[pl_count] = pl_c;
            pl_count += 1;
           
            i += 7;
        } else if (curr_arg.compare("-dl") == 0) {
            vector<float> dl_l = {atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3])};
            vector<float> dl_c = {atof(argv[i+4]), atof(argv[i+5]), atof(argv[i+6])};
            
            dir_lights_l[dl_count] = v_s_mul(dl_l, -1.0);
            dir_lights_c[dl_count] = dl_c;
            dl_count += 1;

            i += 7;
        } else {
            i++;
        }
    }


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
    glOrtho(0, Width_global, 0, Height_global, 1, -1);
    
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







