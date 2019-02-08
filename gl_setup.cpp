//Implementation file of gl_setup.h
//Brian Malsan

#include "gl_setup.h"

GLFWwindow* window;

//error callback function
static void error_callback(int error, const char* description) {
	cerr << "ERROR " << error << ": " << description << endl;
}

//key_callback for now just to close out of the window easily
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}

//static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
//	cout << xpos << " " << ypos << endl;
//}



bool init_gl(int &windowWidth, int &windowHeight, int &framebufferWidth, int &framebufferHeight) {
	cout << "START!!!" << endl;
	glfwSetErrorCallback(error_callback);

	//test if glfw is initialized
	if (!glfwInit()) {
		return false;
	}
	else {
		cout << "glfw initialized" << endl;
	}

	//set up window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //force use of version 4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	//Windowed Full Screen Window, I think it looks nicer than being completely fullscreen, might change later.
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	//sets window to be maximized
	glfwWindowHint(GLFW_MAXIMIZED, 1);

	windowWidth = mode->width;
	windowHeight = mode->height;

	//cout << windowWidth << endl;
	//cout << windowHeight << endl;

	window = glfwCreateWindow(windowWidth, windowHeight, "Fluid Dynamics", NULL, NULL);

	framebufferWidth = windowWidth;
	framebufferHeight = windowHeight;

	//Uncomment next line to make window fullscreen;
	//glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

	//test if window was created successfully
	if (!window) {
		glfwTerminate();
		return false;
	}
	else {
		cout << "Window created" << endl;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//set the key callback
	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, cursor_pos_callback);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);

	//set up GLEW
	glewExperimental = true;
	GLenum err = glewInit();
	glGetError();
	if (GLEW_OK != err) {
		cout << "ERROR: GLEW could not start: " << glewGetErrorString(err) << endl;
		glfwTerminate();
		return false;
	}
	else {
		cout << "GLEW initialized; version: " << glewGetString(GLEW_VERSION) << endl;
	}

	//getting shader version
	GLint glMajor, glMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
	glGetIntegerv(GL_MINOR_VERSION, &glMinor);
	cout << "OpenGL context version: ";
	cout << glMajor << "." << glMinor << endl;
	cout << "Supported GLSL version is ";
	cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	//Allow for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	return true;
}

void stop_gl() {
	glfwTerminate();

}

GLuint loadComputeShader(string cFilename) {
	ifstream computeFile(cFilename);
	if (computeFile.fail()) {
		exit(EXIT_FAILURE);
	}
	string ComputeShaderCode;
	while (!computeFile.eof()) {
		string line;
		getline(computeFile, line);
		ComputeShaderCode += line + "\n";
	}

	cout << "COMPUTE SHADER:" << endl;
	cout << ComputeShaderCode << endl;

	GLuint ComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);

	char const * compute_shader_str = ComputeShaderCode.c_str();

	glShaderSource(ComputeShaderID, 1, &compute_shader_str, NULL);
	glCompileShader(ComputeShaderID);
	checkShader(ComputeShaderID); // code moved to gl_utils.cpp
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, ComputeShaderID);
	glLinkProgram(ProgramID);
	checkProgram(ProgramID);

	glDeleteShader(ComputeShaderID);

	return ProgramID;
}


GLuint loadShaders(string vFilename, string fFilename) {
	//vertex shader and fragment shader
	ifstream vFile(vFilename);
	if (vFile.fail()) {
		return 0;
	}
	string vShaderCode = "";
	while (!vFile.eof()) {
		string line;
		getline(vFile, line);
		vShaderCode += line + "\n";
	}
	vFile.close();

	ifstream fFile(fFilename);
	if (fFile.fail()) {
		return 0;
	}
	string fShaderCode = "";
	while (!fFile.eof()) {
		string line;
		getline(fFile, line);
		fShaderCode += line + "\n";
	}
	fFile.close();

	//display shader code, just comment out to stop
	cout << "VERTEX SHADER:" << endl << vShaderCode << endl;
	cout << "FRAGMENT SHADER:" << endl << fShaderCode << endl;

	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	char const * VertexSourcePointer = vShaderCode.c_str();
	char const * FragmentSourcePointer = fShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	checkShader(VertexShaderID);
	checkShader(FragmentShaderID);

	GLuint ProgramID = glCreateProgram();

	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);

	glLinkProgram(ProgramID);

	checkProgram(ProgramID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;

}


//Functions used for error checking Shader code
bool checkShader(GLuint shaderID) {
	GLint result = GL_FALSE;
	int infoLogLength;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> shaderErrorMessage(std::max(infoLogLength, int(1)));
	glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
	cout << &shaderErrorMessage[0] << endl;
	return result;
};


bool checkProgram(GLuint localProgramID) {
	GLint result = GL_FALSE;
	int infoLogLength;
	glGetProgramiv(localProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(localProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> ProgramErrorMessage(std::max(infoLogLength, int(1)));
	glGetProgramInfoLog(localProgramID, infoLogLength, NULL, &ProgramErrorMessage[0]);
	cout << &ProgramErrorMessage[0] << endl;

	return result;
};

GLuint allocateTexture(int width, int height) {
	GLuint tex_output = 0;
	
	// create the texture
	glGenTextures(1, &tex_output);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_output);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// linear allows us to scale the window up retaining reasonable quality
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// same internal format as compute shader input
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	// bind to image unit so can write to specific pixels from the shader
	glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex_output;
}

