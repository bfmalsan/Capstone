//Implementation file of Fluid class

#include "Fluid.h"

Fluid::Fluid(Mouse inMouse, int initID) {
	mouse = &inMouse;
	this->initID = initID;
}

Fluid::~Fluid() {
	cleanUp();
}

void Fluid::cleanUp() {

	glDeleteProgram(renderProgramID);
	glDeleteProgram(computeProgramID[0]);
	glDeleteProgram(computeProgramID[1]);
	glDeleteProgram(computeProgramID[2]);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &particlesBuffer);
}

void Fluid::init() {
	//load the vertex and fragment shaders
	renderProgramID = loadShaders("../shaders/fVertex.vs", "../shaders/fFragment.ps");
	if (!renderProgramID) {
		cout << "ERROR: Cannot load fluid render shader!" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "Fluid render shader loaded!" << endl;
	}

	viewMatLoc = glGetUniformLocation(renderProgramID, "viewMat");
	projMatLoc = glGetUniformLocation(renderProgramID, "projMat");

	//load the compute shader
	computeProgramID[0] = loadComputeShader("../shaders/compute_density_pressure.comp");
	if (!computeProgramID[0]) {
		cout << "ERROR: Cannot load fluid compute shader!" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "Fluid compute shader loaded!" << endl;
	}


	computeProgramID[1] = loadComputeShader("../shaders/compute_force.comp");
	if (!computeProgramID[1]) {
		cout << "ERROR: Cannot load fluid compute shader!" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "Fluid compute shader loaded!" << endl;
	}

	mouseLoc = glGetUniformLocation(computeProgramID[1], "mouse_pos");

	computeProgramID[2] = loadComputeShader("../shaders/integrate.comp");
	if (!computeProgramID[2]) {
		cout << "ERROR: Cannot load fluid compute shader!" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "Fluid compute shader loaded!" << endl;
	}

	objectPosLoc = glGetUniformLocation(computeProgramID[2], "objectPos");

	constexpr ptrdiff_t position_ssbo_size = sizeof(glm::vec2) * NUM_PARTICLES;
	constexpr ptrdiff_t velocity_ssbo_size = sizeof(glm::vec2) * NUM_PARTICLES;
	constexpr ptrdiff_t force_ssbo_size = sizeof(glm::vec2) * NUM_PARTICLES;
	constexpr ptrdiff_t density_ssbo_size = sizeof(float) * NUM_PARTICLES;
	constexpr ptrdiff_t pressure_ssbo_size = sizeof(float) * NUM_PARTICLES;
	

	constexpr ptrdiff_t particle_buffer_size = position_ssbo_size + velocity_ssbo_size + force_ssbo_size + density_ssbo_size + pressure_ssbo_size;

	constexpr ptrdiff_t position_ssbo_offset = 0;
	constexpr ptrdiff_t velocity_ssbo_offset = position_ssbo_size;
	constexpr ptrdiff_t force_ssbo_offset = velocity_ssbo_offset + velocity_ssbo_size;
	constexpr ptrdiff_t density_ssbo_offset = force_ssbo_offset + force_ssbo_size;
	constexpr ptrdiff_t pressure_ssbo_offset = density_ssbo_offset + density_ssbo_size;
	
	
	vector<glm::vec2> initial_position(NUM_PARTICLES);

	int x = 0, y = 0;

	//initial position 1: particles in top center of screen
	if (initID == 0) {
		for (int i = 0; i < NUM_PARTICLES; i++)
		{
			initial_position[i].x = -.625 + PARTICLE_RADIUS * 2 * x;
			initial_position[i].y = 1 - PARTICLE_RADIUS * 2 * y;
			x++;
			if (x >= 125)
			{
				x = 0;
				y++;
			}
		}
	}
	//initial position 2: particles on the left side of screen
	else if (initID == 1) { 
		for (int i = 0; i < NUM_PARTICLES; i++)
		{
			initial_position[i].x = -1 + PARTICLE_RADIUS * 2 * x;
			initial_position[i].y = -1 + PARTICLE_RADIUS * 2 * y;
			x++;
			if (x >= 75)
			{
				x = 0;
				y++;
			}
		}
	}
	else if (initID == 2) {
		for (int i = 0; i < NUM_PARTICLES; i++)
		{
			initial_position[i].x = 1 - PARTICLE_RADIUS * 2 * x;
			initial_position[i].y = 1 - PARTICLE_RADIUS * 1 * y;
			x++;
			if (x >= 80)
			{
				x = 0;
				y++;
			}
		}
	}
	else {
		cout << "Need to insert a valid number(0-2) or nothing in the command line" << endl;
		//system("pause");
		exit(EXIT_FAILURE);
	}

	void* initial_data = malloc(particle_buffer_size);
	memset(initial_data, 0, particle_buffer_size);
	memcpy(initial_data, initial_position.data(), position_ssbo_size);

	glGenBuffers(1, &particlesBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particlesBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, particle_buffer_size, initial_data, GL_DYNAMIC_STORAGE_BIT);
	free(initial_data);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, particlesBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, particlesBuffer, position_ssbo_offset, position_ssbo_size);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, particlesBuffer, velocity_ssbo_offset, velocity_ssbo_size);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 2, particlesBuffer, force_ssbo_offset, force_ssbo_size);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 3, particlesBuffer, density_ssbo_offset, density_ssbo_size);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 4, particlesBuffer, pressure_ssbo_offset, pressure_ssbo_size);
	

	glBindVertexArray(VAO);
}

//send values to the compute shader
void Fluid::update(glm::vec4 objectDims) {

	double xpos = mouse->getCurPosX();
	double ypos = mouse->getCurPosY();
	//cout << xpos << " " << ypos << endl;
	//cout << objectDims.x << " " << objectDims.y << " " << objectDims.z << " " << objectDims.w << endl;


	glUseProgram(computeProgramID[0]);
	glDispatchCompute(NUM_WORK_GROUPS, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glUseProgram(computeProgramID[1]);
	glDispatchCompute(NUM_WORK_GROUPS, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glUniform3f(mouseLoc, mouse->getCurPosX(), mouse->getCurPosY(),mouse->getRadius());
	glUseProgram(computeProgramID[2]);
	glDispatchCompute(NUM_WORK_GROUPS, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glUniform4f(objectPosLoc, objectDims.x, objectDims.y, objectDims.z, objectDims.w);
	

}

void Fluid::draw(glm::mat4 viewMat, glm::mat4 projMat) {

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(renderProgramID);

	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));

	glBindVertexArray(VAO);
	
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

	glBindVertexArray(0);

	glUseProgram(0);
}

Mouse* Fluid::getMouse() {
	return mouse;
}
