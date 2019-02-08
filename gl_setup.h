//File that holds the prototypes of functions that are used to set up and use OpenGL
//Brian Malsan

#include <iostream>
#include <cstring>
#include <thread>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Mouse.h"

using namespace std;

extern GLFWwindow* window;

//error callback function
static void error_callback(int error, const char* description);
//key_callback for now just to close out of the window easily
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
bool init_gl(int &windowWidth,int &windowHeight ,int &framebufferWidth ,int &framebufferHeight );
void stop_gl();
GLuint loadComputeShader(string cFilename);
GLuint loadShaders(string vFilename, string fFilename);
bool checkShader(GLuint shaderID);
bool checkProgram(GLuint localProgramID);
GLuint allocateTexture(int width, int height);

