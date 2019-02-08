//Header file for object class
//These  objects act as obstacles that the particles cannot move

#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

using namespace std;

class Object {
private:
	vector<GLfloat> pos;
	GLuint vertexPositionBuffer = 0;
	GLuint VAO = 0;
	vector<GLuint> elements;
	GLuint elementBuffer = 0;
	int vcnt = 0;
	//vector <GLfloat> uv;
	//GLuint textCoordBuffer = 0;

	
	

public:
	Object(vector<GLfloat> &positions, vector<GLuint> &indices);
	~Object();
	void draw();
	GLuint getVAO();
	glm::mat4 convertToMat();
	glm::vec4 getDims();



};


#endif