//Implementation file of the mous class


#include "Mouse.h"

Mouse::Mouse() {
	oldPos = glm::vec2(0, 0);
	curPos = glm::vec2(0, 0);
	vel = glm::vec2(0, 0);
	force = glm::vec2(0, 0);
	radius = 0.05f;
}

Mouse::~Mouse() {

}


void Mouse::processInput(GLFWwindow* window) {
	//get left mouse button down
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		lbutton_down = true;
	}
	//get left mouse button up
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		lbutton_down = false;
	}

	double xpos, ypos;
	//if down get mouse position
	if (lbutton_down) {

		glfwGetCursorPos(window, &xpos, &ypos);
		
		//curPos.x = xpos;
		//curPos.y = ypos;
		setCurPos(glm::vec2(xpos, ypos));
		//drawCircle();

		//cout << "Pressed: " << curPos.x << " " << curPos.y << endl;
	}
	else {
		//glfwGetCursorPos(window, &xpos, &ypos);

		curPos.x = -4000;
		curPos.y = -4000;
		//cout << "Released: " << xpos << " " << ypos << endl;
	}
}

void Mouse::convertPos(int width, int height) {
	//need to convert the mouse position from resolution value to screen coords (-1,1)
	float halfWidth = width / 2.0;
	float halfHeight = height / 2.0;


	curPos.x = (curPos.x /(float) halfWidth) - 1;
	curPos.y = (curPos.y /(float) halfHeight) - 1;
}

//to show a cirlce where the mouse is pressed
void Mouse::drawCircle() {
	
}


glm::vec2 Mouse::getCurPos() {
	return curPos;
}
double Mouse::getCurPosX() {
	return (double)curPos.x;
}
double Mouse::getCurPosY() {
	return (double)curPos.y;
}
void Mouse::setCurPos(glm::vec2 ipos) {
	curPos = ipos;
}
glm::vec2 Mouse::getVel() {
	return vel;
}
void Mouse::setVel(glm::vec2 ivel) {
	vel = ivel;
}
glm::vec2 Mouse::getForce() {
	return force;
}
void Mouse::setForce(glm::vec2 iforce) {
	force = iforce;
}
float Mouse::getRadius() {
	return radius;
}