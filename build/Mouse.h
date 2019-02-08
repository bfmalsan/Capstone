//Header file for mouse class
//The mouse class handles mouse input to the screen


#ifndef MOUSE_H
#define MOUSE_H

#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define PI_FLOAT 3.1415927410125732421875f

using namespace std;

class Mouse {
private: 
	glm::vec2 oldPos;
	glm::vec2 curPos;
	glm::vec2 vel; //=curPos - oldPos, velocity not implemented
	glm::vec2 force;
	float radius; //for the force circle
    bool lbutton_down = false;
	

public:
	Mouse();

	~Mouse();
		
	void processInput(GLFWwindow *window);
	void convertPos(int width, int height);

	void update();
	void drawCircle();


	glm::vec2 getCurPos();
	double getCurPosX();
	double getCurPosY();
	void setCurPos(glm::vec2 ipos);
	glm::vec2 getVel();
	void setVel(glm::vec2 ivel);
	glm::vec2 getForce();
	void setForce(glm::vec2 iforce);
	float getRadius();

};





#endif