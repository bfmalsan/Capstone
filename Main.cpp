/*Capstone Project: Fluid and Density sandbox simulator
Exploring the use of fluid dynamics in computer graphics
Using C++ and OpenGL
By: Brian Malsan
Instructed By: Professor Reale
Started on 1/11/18
Last edit: 6/11/18
*/

#include "gl_setup.h"
#include "Fluid.h"
#include "Object.h"


GLuint boxShaderID = 0;
GLuint fluidShaderID = 0;
GLuint fluidComputeShaderID = 0;
GLuint objectShaderID = 0;

GLuint modelMatLoc = -1;
glm::mat4 modelMat;
GLuint viewMatLoc = -1;
glm::mat4 viewMat;
GLuint projMatLoc = -1;
glm::mat4 projMat;

float dt = 0.1;

//vector to hold vertices for bounding box
vector<GLfloat> box_vertices = {
	2.0f,  1.0f,  1.0f, //0
	2.0f, -1.0f,  1.0f, //1
   -2.0f, -1.0f,  1.0f, //2
   -2.0f,  1.0f,  1.0f, //3
	2.0f,  1.0f, -1.0f, //4
	2.0f, -1.0f, -1.0f, //5
   -2.0f, -1.0f, -1.0f, //6
   -2.0f,  1.0f, -1.0f, //7
	
};
vector<GLfloat> box_vertices_small = {
	1.0f,  0.5f,  0.5f, //0
	1.0f, -0.0f,  0.5f, //1
	-1.0f, -0.0f,  0.5f, //2
	-1.0f,  0.5f,  0.5f, //3
	1.0f,  0.5f, -0.5f, //4
	1.0f, -0.0f, -0.5f, //5
	-1.0f, -0.0f, -0.5f, //6
	-1.0f,  0.5f, -0.5f, //7
};

//vector to hold indices of cube 
vector<GLuint> box_indices = {
	0, 1, 2, 0, 3, 2, // front
	0, 1, 5, 0, 4, 5, // right
	4, 5, 6, 4, 7, 6, // back
	2, 3, 6, 3, 6, 7, // left
	0, 3, 4, 3, 7, 4, // top
	1, 2, 5, 2, 6, 5, // bottom
};

//Three different object positions
vector<GLfloat> object_vertices0 = {
	-.1f, -1.f, 0.0f,
	-.1f, -.4f, 0.0f,
	.1f, -1.f,0.0f,
	.1f, -.4f, 0.0f,
};
vector<GLfloat> object_vertices1 = {
	.2f, -.8f, 0.0f,
	.2f, -.4f, 0.0f,
	.5f, -.8f,0.0f,
	.5f, -.4f, 0.0f,
};
vector<GLfloat> object_vertices2 = {
	1.f, -1.f, 0.0f,
	1.f, -.4f, 0.0f,
	1.1f, -1.f,0.0f,
	1.1f, -.4f, 0.0f,
};
vector<GLuint> object_indices = {
	0,1,2,
	1,2,3,
};


//class that creates and displays a bounding box of where the liquid will go 
class Box {
private:
	vector<GLfloat> pos;
	GLuint vertexPositionBuffer = 0;
	GLuint VAO = 0;
	vector<GLuint> elements;
	GLuint elementBuffer = 0;
	int vcnt = 0;
	//vector <GLfloat> uv;
	//GLuint textCoordBuffer = 0;

	// 0=xMax,1=xMin,2=yMax, 3=yMin, 4=zMax, 5=zMin;
	int maxCoords[6];

public:
	Box(vector<GLfloat> &positions, vector<GLuint> &indices) {
		pos.insert(pos.end(), positions.begin(), positions.end());
		
		elements.insert(elements.end(), indices.begin(), indices.end());

		maxCoords[0] = pos.at(0); //X Max
		maxCoords[1] = -pos.at(0); //X Min
		maxCoords[2] = pos.at(1); //Y Max
		maxCoords[3] = -pos.at(1); //Y Min
		maxCoords[4] = pos.at(2); //Z Max
		maxCoords[5] = -pos.at(2); //Z Min

		/* print coords for testing
		for (int i = 0; i < 6; i++) {
			cout << maxCoords[i] << " ";
		}
		cout << endl;
		*/

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
	~Box() {
		pos.clear();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &(vertexPositionBuffer));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &(elementBuffer));

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAO);
	}
	void draw() {
		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//glDrawArrays(GL_TRIANGLES, 0, vcnt);
		//glDisableVertexAttribArray(0);

		//delete next line for filled in box
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
	}

	int* getMaxCoords() {
		return maxCoords;
	}

	GLuint getVAO() {
		return VAO;
	}

};

int main(int argc, char **argv) {
	int windowWidth = 0;
	int windowHeight = 0;
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	int initID = 0;
	int objectID = 0;

	if (argc >= 2) {
		initID = stoi(argv[1]);//convert string to int, from string class
		objectID = stoi(argv[2]);
		cout << "Position ID = " << initID << endl;
		cout << "Object ID = " << objectID << endl;
	} 
	else {
		initID = 0;
		objectID = 0;
		cout << "Position ID = " << initID << endl;
		cout << "Object ID = " << objectID << endl;
	}


	//initialize OpenGL
	if (!init_gl(windowWidth,windowHeight,framebufferWidth,framebufferHeight)) {
		exit(EXIT_FAILURE);
	}
	
	Mouse mouse;
	//glfwSetMouseButtonCallback(window, Mouse::mouse_button_callback);

	//create a box
	//Box *box = new Box(box_vertices,box_indices);
	Object *object;
	if (objectID == 0) {
		object = new Object(object_vertices0, object_indices);
	}
	else if (objectID == 1) {
		object = new Object(object_vertices1, object_indices);
	}
	else if (objectID == 2) {
		object = new Object(object_vertices2, object_indices);
	}
	glm::vec4 objectDims = object->getDims();
	
	//load shader code
	//vertex and fragment shader program for bounding box
	boxShaderID = loadShaders("../shaders/Box.vs", "../shaders/Box.ps");
	objectShaderID = loadShaders("../shaders/ObjectV.vs", "../shaders/ObjectF.ps");
	//compute shader program for fluid
	//fluidComputeShaderID = loadComputeShader("../shaders/fCompute.comp");
	//vertex and fragment shader program for fluid
	fluidShaderID = loadShaders("../shaders/fVertex.vs", "../shaders/fFragment.ps");

	if (!boxShaderID) {
		cout << "ERROR: Cannot load box shader!" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "Box shader loaded!" << endl;
	}
	

	modelMatLoc = glGetUniformLocation(boxShaderID, "modelMat");
	viewMatLoc = glGetUniformLocation(boxShaderID, "viewMat");
	projMatLoc = glGetUniformLocation(boxShaderID, "projMat");
	

	int tex_w = 512, tex_h = 512;
	//Create Fluid class. Particle system
	Fluid* fluidSystem = new Fluid(mouse,initID);

	//Fluid* systemTwo = new Fluid(numParticles, 1.21e-5, tex_h, tex_w);

	//cout << fluidComputeShaderID << endl;


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //set background color
	glPointSize(10.0);					  //size of particles

	 
	// texture handle and dimensions
	//GLuint tex_output = 0;
	
	//tex_output = allocateTexture(tex_w, tex_h);
	//cout << tex_output << endl;

	
	//after getting the work groups init fluidSystem
	fluidSystem->init();


	//GLuint quad_vao = create_quad_vao();
	//GLuint quad_vao = box->getVAO();

	//Variables used in perspective for the window.
	float fov = glm::radians(45.0f);
	float ar = ((float)framebufferWidth) / ((float)framebufferHeight);
	float near = 0.01;
	float far = 1000.0;

	
	double lastTime = glfwGetTime();
	int frames = 0;
	

	//drawing loop
	do {
		
		/*
		Uncomment to display frames per second
		double currentTime = glfwGetTime();
		frames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
											 // printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(frames));
			frames = 0;
			lastTime += 1.0;
		}
		*/
		
		
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		//cout << framebufferWidth << " " << framebufferHeight << endl;
		glViewport(0, 0, framebufferWidth, framebufferHeight);
		glGetError();

		//Dispatch the work groups for the compute shader
		//glUseProgram(fluidComputeShaderID);
		//glDispatchCompute((GLuint)tex_w, (GLuint)tex_h, 1);

		// prevent sampling before all writes to image are done
		//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(boxShaderID);

		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

		viewMat = glm::lookAt(glm::vec3(0, 0, 6.0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));

		projMat = glm::perspective(fov, ar, near, far);
		glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));
		

		//draw stuff here
		//box->draw();
	
		fluidSystem->getMouse()->processInput(window);
		fluidSystem->getMouse()->convertPos(framebufferWidth, framebufferHeight);

		glUseProgram(0);
		fluidSystem->update(objectDims);
		
		
		//glUseProgram(fluidShaderID);
	
		//glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
		//glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));
		
		
		fluidSystem->draw(viewMat, projMat);

		glUseProgram(objectShaderID);
		object->draw();
			
		
		
		glUseProgram(0);
		

		

		//double xpos, ypos;
		//xpos = fluidSystem->getMouse().getCurPosX();
		//cout << xpos << endl;
		//ypos = fluidSystem->getMouse().getCurPosY();
		//cout << ypos << endl;
		//fluidSystem->getMouse().setCurPos(glm::vec2(xpos / (float)framebufferWidth, ypos / (float)framebufferHeight));
		//cout << fluidSystem->getMouse().getCurPos().x << " " << fluidSystem->getMouse().getCurPos().y << endl;


		glfwSwapBuffers(window);
		glfwPollEvents();
		//std::this_thread::sleep_for(std::chrono::milliseconds(90));

	} while (!glfwWindowShouldClose(window));


	//delete box;
	delete fluidSystem;
	

	//cleanup
	glUseProgram(0);
	glDeleteProgram(boxShaderID);
	

	glfwDestroyWindow(window);
	stop_gl();

	return 0;
}