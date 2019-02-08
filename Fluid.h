//Header file for fluid class. Will handle all things fluid
//Created for Capstone Project
//By: Brian Malsan
//Instructed By: Professor Reale
//Created: 2/1/2018
	
#ifndef FLUID_H
#define FLUID_H
/*
#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
*/
#include "gl_setup.h"


using namespace std;

#define NUM_PARTICLES 15000

#define PARTICLE_RADIUS 0.005f

#define WORK_GROUP_SIZE 1024
// work group count is the ceiling of particle count divided by work group size
#define NUM_WORK_GROUPS ((NUM_PARTICLES + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE)

class Fluid {
private:
	GLuint VAO = 0;
	GLuint renderProgramID = 0;
	GLuint computeProgramID[3] = { 0,0,0 };
	GLuint particlesBuffer = 0;

	GLuint viewMatLoc = -1;
	GLuint projMatLoc = -1;
	GLuint mouseLoc = -1;
	GLuint objectPosLoc = -1;

	Mouse* mouse;

	double lastTime = glfwGetTime();
	int frames = 0;

	int initID = 0; //USed to state different initial positions of particles

	void cleanUp();

public:
	Fluid(Mouse inMouse,int initID);
	~Fluid();

	void init();
	void update(glm::vec4 objectDims);
	void draw(glm::mat4 viewMat,glm::mat4 projMat);
	Mouse* getMouse();

};


#endif // !FLUID_H
