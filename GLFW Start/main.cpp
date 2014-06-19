#pragma once

#define _USE_MATH_DEFINES 

#include <GL/glew.h>
#include <glfw3.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <time.h>
#include "Model.h"
#include "GLSLShader.h"
#include "GraphicsWindow.h"
#include "FreeCamera.h"
#include "TrackballCamera.h"
#include "Logger.h"
#include <FreeImage.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

float timer = 0.0f;

// Rotation axix for the trackball cam
glm::vec3 rotat_axis = glm::normalize(glm::vec3(0,1,0));

// Post Process flags for the modelimport.
unsigned int processFlagsOnModelImport = //aiProcess_FlipUVs|
										 aiProcess_Triangulate|
										 aiProcess_JoinIdenticalVertices|
										 aiProcess_SortByPType;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void initGlew()
{
	// Initialize GLEW
	GLenum glewError = glewInit() ;
	if ( glewError != GLEW_OK )  {
		// Problem : glewInit failed, something is seriously wrong.
		printf( "Failed to initialize GLEW ! Error : %s\n", glewGetErrorString( glewError ) ) ;
		glfwTerminate() ;
		exit(1) ;
	}
}

int main(void)
{
	// Initialize Logger
	Logger::GetInstance().Init("Framework Logger");
	Logger::GetInstance().Log("Logger initialized.");

	// Camera
	//______________________________________________________________________________________________________________________________________________________
	//FreeCamera* cam = new FreeCamera(M_PI/2.0f, 4.0f/3.0f, 0.0f, 0.0f, 0.1f, 100000.0f, 0.04f, 0.04f, glm::vec3(0,0,-100), glm::vec3(0,1,0), glm::vec3(0,0,0), true);
	TrackballCamera* cam = new TrackballCamera(M_PI/2.0f, 4.0f/3.0f, 
												0.0f, 0.0f, 
												10.0f, 10000.0f, 
												glm::vec3(0,0,-4500), 
												glm::vec3(0,1,0), 
												glm::vec3(0,10,0), 
												rotat_axis, true);
	//______________________________________________________________________________________________________________________________________________________

	setErrorCallbackAndInit(error_callback);
	
	GraphicsWindow* window_n1 = new GraphicsWindow(640, 480, "Simple example", NULL, NULL, key_callback);
	//GraphicsWindow* window_n2 = new GraphicsWindow(800, 600, "2nd Window", NULL, NULL);

	// Initialize GLEW
	initGlew();

	GLSLProgram shaderProg;

	shaderProg.initShaderProgram("Vert.glsl", "", "", "", "Frag.glsl");

	shaderProg.use();

	// Assimp experimenting part
	//______________________________________________________________________________________________________________________________________________________
	Model* test_model = new Model();
	test_model->setShaderProgram(shaderProg.getHandle());
	test_model->importLoadModel("G:/Program Files/Assimp/test/models-nonbsd/X/dwarf.x", processFlagsOnModelImport);
	//______________________________________________________________________________________________________________________________________________________

	float ratio, width = 640, height = 480;

	bool shouldClose_window_n1 = false;
	//bool shouldClose_window_n2 = false;

	glfwSetCursorPos(window_n1->getWindowHandle(), (double) width /2.0, (double) height /2.0);

	cam->setViewport(width,height,0.5f,0.5f);

	while (!shouldClose_window_n1)// || !shouldClose_window_n2)
    {
		// Camera
		//__________________________________________________________________________________________________________________________________________________
		// Update
		//cam->camControll(window_n1->getWindowHandle());
		//cam->update();
		cam->update(timer);
		//__________________________________________________________________________________________________________________________________________________

		shaderProg.setUniform("lightPos", cam->getCamPos());
		shaderProg.setUniform("MVP", cam->getVPMatrix());
		shaderProg.setUniform("TInvMVP", cam->getTranspInvVPMatrix());

		//window_n1->makeContextCurrent();

		// GL stuff window 1
		ratio =  height/width;
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		
		test_model->drawModel();
 
		window_n1->swapBuffers();

		shouldClose_window_n1 = window_n1->shouldClose();

		/*
		// Second window
		window_n2->makeContextCurrent();
		width = window_n2->getWindowWidth();
		height = window_n2->getWindowHeight();

		// GL stuff window 2
		ratio = width / height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f,0.0f,1.0f,1.0f);

		window_n2->swapBuffers();

		shouldClose_window_n2 = window_n2->shouldClose();
		*/

        glfwPollEvents();

		timer += 1.0f/60.0f;
    }
	glfwTerminate();
	window_n1->release();
	//window_n2->release();
    
    exit(0);
}