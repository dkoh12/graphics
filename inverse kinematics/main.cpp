#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <sys/time.h>

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "Arm.h"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

using namespace std;

const float PI = 3.1415926;

#define degrees_to_radians(angle_degrees) (angle_degrees * PI / 180.0)
#define radians_to_degrees(angle_radians) (angle_radians * 180.0 / PI)

float goal_theta = 0.0f;
float goal_phi = 0.0f;
float goal_radius = 0.5f;

float cam_dist = 20.0;
glm::vec3 goal_origin = glm::vec3(0.0f, 0.0f, 0.0f);
float goal_speed = 25.0;


// ARM. Comment one out and uncomment the other out

Link startLink{0.5, {1},
    make_shared<Link>(Link{1.0, {0.5}, 
    make_shared<Link>(Link{0.75, {0.5}, 
    make_shared<Link>(Link{0.6, {1}, })
})})};

// Link startLink{0.5, {0},
//     make_shared<Link>(Link{1.0, {0}, 
//     make_shared<Link>(Link{0.75, {0}, 
//     make_shared<Link>(Link{0.6, {0}, })
// })})};


float Colors[8][4] ={
	{0.557f, 0.83f, 0.78f, 1.0f}, //Teal
	{1.0f, 1.0f, 0.0f, 1.0f}, //Yellow
	{0.988f, 0.804f, 0.898f, 1.0f}, //Bubblegum Pink
	{0.75f, 0.73f, 0.85f, 1.0f}, //Purple
	{0.5f, 0.694f, 0.827f, 1.0f}, //Powder Blue
	{0.992f, 0.706f, 0.384f, 1.0f},  //Orange
	{0.984f, 0.5f, 0.447f, 1.0f}, //Salmon Red
	{0.702f, 0.871f, 0.412f, 1.0f}}; //Lime Green

void updateGoal()
{
	// cout << "speed: " << goal_speed << " radius: " << goal_radius << " theta: " << goal_theta << " phi: " << goal_phi << endl;
	
    goal_theta += 0.1f * goal_speed;
	goal_phi += 0.2f * goal_speed;
	
	goal_radius += 0.0001 * goal_speed; //having 3 zeros after decimal is important

    if (goal_theta >= 360.0f) {
    	goal_theta -= 360;
    }
    else if (goal_theta < 0){
    	goal_theta += 360;
    }

	if (goal_phi >= 360.0f){
		goal_phi -= 360;
	}
	else if (goal_phi < 0){
		goal_phi += 360;
	}

	if (goal_radius <= 0.1 || goal_radius >= 3.3){
    	goal_speed *= -1; // change direction
    }
}

// get Goal position
Vector getGoal()
{
	glm::vec4 goal = glm::vec4(0.0, 0.0, 0.0, 1.0); //point
	
	glm::mat4 transform = glm::translate(goal_origin); //transform
	float theta = degrees_to_radians(goal_theta);
	float phi = degrees_to_radians(goal_phi);
	
	// rotate about z axis
	transform = transform * glm::rotate(theta, glm::vec3(0.0f, 0.0f, 1.0f));
	// transform = transform * glm::rotate(theta, glm::vec3(0.0f, 1.0f, 2.0f));
	// transform = transform * glm::rotate(theta, glm::vec3(1.0f, 1.0f, 1.0f));

	// rotate about x axis
	transform = transform * glm::rotate(phi, glm::vec3(1.0f, 0.0f, 0.0f));
	// transform = transform * glm::rotate(phi, glm::vec3(1.0f, 0.0f, 0.0f));

	transform = transform * glm::translate(glm::vec3(goal_radius, 0.0, 0.0));

	goal = transform * goal;

	Vector goal_pos = {goal.x, goal.y, goal.z};
	
	return goal_pos;
}

// yellow circle
void drawGoal()
{
    Vector goal_position = getGoal();
	glTranslatef(goal_position.x, goal_position.y, goal_position.z);	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Colors[2]);
	glutSolidSphere(0.1, 100, 100);
	glTranslatef(-goal_position.x, -goal_position.y, -goal_position.z);	
}

// used in Arm.h
void drawLink(Vector origin, Vector end) {

	float cone_height = (end - origin).mag();
	Vector direction = (end - origin).norm();
	Vector forward = {0, 0, 1};
	
	Vector axis_of_rotation = direction.cross(forward).norm();
	float angle_of_rotation = radians_to_degrees(acos(direction.dot(forward)));
	
	int color_index = int(floor(cone_height * 7)) % 8;
	
	glTranslatef(origin.x, origin.y, origin.z);
	glRotatef(-angle_of_rotation, axis_of_rotation.x, axis_of_rotation.y, axis_of_rotation.z);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Colors[color_index]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Colors[color_index]);
	glutSolidCone(0.1, cone_height, 50, 50);

	glRotatef(angle_of_rotation, axis_of_rotation.x, axis_of_rotation.y, axis_of_rotation.z);
	glTranslatef(-origin.x, -origin.y, -origin.z);
}

void asciiInput(unsigned char key, int x, int y) {
    switch(key) {
	case '+':
	case '=':
		cam_dist -= 1;
		break;
	case '-':
		cam_dist += 1;
		break;
	default:
        exit(0);
		break;
    }
}

void reshapeWindow(int w, int h) {
    if(h == 0)
        h = 1;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport (0, 0, w, h);

    gluPerspective(18, (1.0 * w / h), 1, 1000); 
    glMatrixMode(GL_MODELVIEW);
}

void updateScene()
{
    updateGoal();
	Vector goal = getGoal();
    startLink.updateParams({goal.x, goal.y, goal.z, 1});
	glutPostRedisplay();
}

void renderScene() {
	GLfloat shinyMat[] = { 50 };
	GLfloat lightPos[] = {2, 2, 2, 0};
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

	glMaterialfv(GL_FRONT, GL_SHININESS, shinyMat);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(0.0, 0.0, cam_dist, 0.0, 0.0, 0.0, 0.0, 1.0f, 0.0);

    drawGoal();
    startLink.getVector();
    
    glFlush();
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Inverse Kinematics");

    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshapeWindow);
    glutKeyboardFunc(asciiInput);

    glutIdleFunc(updateScene);
    glutMainLoop();

    return 0;
}


