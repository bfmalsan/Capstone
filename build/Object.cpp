//implementation file of object class

#include "Object.h"

Object::Object(vector<GLfloat> &positions, vector<GLuint> &indices) {
	pos.insert(pos.end(), positions.begin(), positions.end());

	elements.insert(elements.end(), indices.begin(), indices.end());

	glGenBuffers(1, &vertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*pos.size(), pos.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	vcnt = pos.size() / 3;
}

Object::~Object() {
	pos.clear();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &(vertexPositionBuffer));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &(elementBuffer));

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
}

void Object::draw() {

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

GLuint Object::getVAO() {
	return VAO;
}

glm::mat4 Object::convertToMat() {
	glm::mat4 uniformPos;
	
	uniformPos = glm::mat4(glm::vec4(pos.at(0), pos.at(1), pos.at(2), 1.0),
				glm::vec4(pos.at(3), pos.at(4), pos.at(5), 1.0),
				glm::vec4(pos.at(6), pos.at(7), pos.at(8), 1.0),
				glm::vec4(pos.at(9), pos.at(10), pos.at(11), 1.0));

	return uniformPos;


}

//get the diminsions of box to use in collision detection on GPU
glm::vec4 Object::getDims() {
	glm::vec4 dims;
	//(left,right,bottom,top)
	//(minx,maxx,miny,maxy)

	dims = glm::vec4(pos.at(0), pos.at(6), pos.at(1), pos.at(4));
	//cout << dims.x << " " << dims.y << " " << dims.z << " " << dims.w << endl;

	return dims;

}