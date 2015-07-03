#pragma once

#define _USE_MATH_DEFINES 

#include "../dependencies/gl/include/glew.h"
#include "../dependencies/GLFW/include/GLFW/glfw3.h"
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
#include "../dependencies/FreeImage/include/FreeImage.h"
#include "../dependencies/glm/glm.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"
#include "ScreenFillingQuad.h"
#include "SkyBox.h"
#include "Scene.h"
#include "GBuffer.h"
#include "PointLight.h"
#include "DefferedRenderer.h"

//TMP
#include "Image.h"

float time_passed = 0.0f;

// Rotation axis for the trackball cam
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

static std::vector<GeometryObject*> loadContent()
{
	std::vector<GeometryObject*> models;
	// Shaders
	GLSLProgram* shaderProg =  new GLSLProgram();
	shaderProg->initShaderProgram("VertToonShader.glsl", "GeomToonShader.glsl", "", "", "FragToonShader.glsl");

	GLSLProgram* shaderProg2 =  new GLSLProgram();
	shaderProg2->initShaderProgram("Vert.glsl", "", "", "", "Frag.glsl");

	//GLSLProgram* shaderProgDR =  new GLSLProgram();
	//shaderProgDR->initShaderProgram("GeomPassVert.glsl", "", "", "", "GeomPassFrag.glsl");

	// Models
	Model* test_model = new Model(glm::vec3(0));
	test_model->setShaderProgram(shaderProg);
	test_model->setVertAlignment(GL_TRIANGLES_ADJACENCY);
	test_model->importLoadModel("../Models/IronMan/Red Snapper.obj", processFlagsOnModelImport);
	//test_model->setPosition(glm::vec3(0,0,100));

	Quad* floorQuad = new Quad(glm::vec3(10,-1,10), glm::vec3(10,-1,-10), glm::vec3(-10,-1,-10), glm::vec3(-10,-1,10), glm::vec3(0), 1200, 800, 0);
	floorQuad->initQuad();
	floorQuad->setGLSLProgram(*shaderProg2);
	floorQuad->initDefaultTexture();

	models.push_back(test_model);
	models.push_back(floorQuad);

	return models;
}

int main(void)
{
	// Initialize Logger
	//Logger::GetInstance().Init("Framework Logger");
	//Logger::GetInstance().Log("Logger initialized.");

	setErrorCallbackAndInit(error_callback);
	
	GraphicsWindow* window_n1 = new GraphicsWindow(1200, 800, "Cell Shading", NULL, NULL, key_callback);
	//GraphicsWindow* window_n2 = new GraphicsWindow(800, 600, "2nd Window", NULL, NULL);

	// Initialize GLEW
	initGlew();

	// Camera
	//______________________________________________________________________________________________________________________________________________________
	float ratio; 
	unsigned int width = 1200, height = 800;

	FreeCamera* cam = new FreeCamera(45.0f, 16.0f/9.0f, 
										0.0f, 0.0f, 
										0.1f, 10000.0f, 
										0.0004f, 0.35f,
										glm::vec3(0,0,-50), 
										glm::vec3(0,1,0), 
										glm::vec3(0,0,0), 
										true);

	cam->setViewport(width,height,0.5f,0.5f);

	/*
	TrackballCamera* cam = new TrackballCamera(M_PI/2.0f, 4.0f/3.0f, 
												0.0f, 0.0f, 
												10.0f, 10000.0f, 
												glm::vec3(0,0,-8000), 
												glm::vec3(0,1,0), 
												glm::vec3(0,1,0), 
												rotat_axis, true);
												*/
	//______________________________________________________________________________________________________________________________________________________
	SkyBox* sBox = new SkyBox(glm::vec3(0),"../Skybox Texture/Above_The_Sea.jpg",1.0f);
	sBox->setTexImage("../Skybox Texture/Above_The_Sea.jpg");
	sBox->initSkyBox();
	
	Scene scene(cam, sBox, glm::vec3(0.1), glm::vec3(0));

	std::vector<GeometryObject*> models = loadContent();
	for (int i = 0; i < models.size(); i++)
	{
		scene.addModel(models[i]);
	}
	scene.sortByGLSLProgram();

	//______________________________________________________________________________________________________________________________________________________
	// Initalize screen filling quad
	//______________________________________________________________________________________________________________________________________________________
	//ScreenFillingQuad* test_SFQ = new ScreenFillingQuad (width,height,glm::vec3(0.4f,0.6f,0.94f),(RenderTargetTextureCoponents::Color|RenderTargetTextureCoponents::Depth));
	//test_SFQ->initScreenFillingQuad();
	//______________________________________________________________________________________________________________________________________________________

	bool shouldClose_window_n1 = false;
	//bool shouldClose_window_n2 = false;

	glfwSetCursorPos(window_n1->getWindowHandle(), (double) (width/2.0), (double) (height/2.0));

	glClearColor(0.4f,0.6f,0.94f,1.0f);
	//glClearColor(0.0f,0.0f,0.0f,1.0f);

	/*
	//DefferedRenderer* defferedRenderer = new DefferedRenderer();
	//defferedRenderer->InitDefferedRenderer(window_n1, processFlagsOnModelImport);

	//std::srand(time(NULL));
	//float LO = -8.0f;
	//float HI = 8.0f;
	//const unsigned int numLights = 500;
	//glm::vec3 bSphereWorldPositions[numLights];
	//float bSphereScales[numLights];
	//glm::mat4 bSphereScaleMatrices[numLights];
	//
	//float rands[numLights];
	//glm::vec3 randVecs[numLights];
	//for (int i = 0; i < numLights; i++)
	//{
	//	float randPosX = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	//	float randPosY = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	//	float randPosZ = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

	//	rands[i] = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	//	randVecs[i] = glm::vec3(randPosX,randPosY,randPosZ);
	//	bSphereWorldPositions[i] = randVecs[i];

	//	float LOSphereScale = 2.0f;
	//	float HISphereScale = 3.0f;
	//	bSphereScales[i] = LOSphereScale + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HISphereScale-LOSphereScale)));
	//	bSphereScaleMatrices[i] = glm::scale(glm::mat4(1),glm::vec3(bSphereScales[i],bSphereScales[i],bSphereScales[i]));
	//}
	//glm::vec3 randColors[numLights];
	//LO = 0.0f;
	//HI = 1.0f;
	//float intensity = 1.0f;
	//for (int i = 0; i < numLights; i++)
	//{
	//	float randColorR = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	//	float randColorG = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	//	float randColorB = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	//	
	//	randColors[i] = intensity*glm::vec3(randColorR,randColorG,randColorB);
	//}
	//
	//glEnable(GL_TEXTURE_2D);
	//_______________________________________________________________________________________________________________________________________________________
	*/
	while (!shouldClose_window_n1)// || !shouldClose_window_n2)
    {
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		scene.updateCamera(window_n1->getWindowHandle(), time_passed);
		//std::printf("Camera Position: %f, %f, %f \n", scene.getCamera()->getCamPos().x, cam->getCamPos().y, cam->getCamPos().z);
		//cam->camControll(window_n1->getWindowHandle());
		//cam->update();
		//cam->update(time_passed);
		//___________________________________________________________________________________________________________________________________________________

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		//window_n1->makeContextCurrent();

		// GL stuff window 1
		//ratio =  height/width;
		//glViewport(0.5f, 0.5f, width, height);
		
		/*
		test_SFQ->getCurrentShaderProgram()->setUniform("time", time_passed);

		test_SFQ->enableScreenFillingQuadRenderTarget();

		scene.drawScene();

		test_SFQ->disableScreenFillingQuadRenderTarget();

		test_SFQ->getCurrentShaderProgram()->use();

		test_SFQ->draw();
		*/

		/*
		//Deferred Rendering
		defferedRenderer->GeometryPass(scene, *window_n1);
		//defferedRenderer->BeginLightPasses();
		defferedRenderer->LightPass(*cam, bSphereWorldPositions,randColors,bSphereScales,glm::vec3(1.0f),*window_n1,numLights);
		//defferedRenderer->LightPass(*cam, bSphereWorldPositions,bSphereWorldPositions,bSphereScales,glm::vec3(0.5f),*window_n1,numLights);
		defferedRenderer->FinalPass(*window_n1);
		defferedRenderer->unbindInternal();
		*/
		
		sBox->getShaderProgram().use();
		sBox->getShaderProgram().setUniform("VPMatrix",scene.getCamera()->getVPMatrix()*glm::translate(glm::mat4x4(1.0),scene.getCamera()->getCamPos()));

		sBox->drawSkyBox();

		scene.drawScene();

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

		//time_passed += 1.0f/60.0f;
		//
		//for (int i = 0; i < numLights; i++)
		//{
		//	bSphereWorldPositions[i] = (randVecs[i]*std::abs(std::sin(time_passed+rands[i])));
		//	bSphereWorldPositions[i] = glm::vec3((randVecs[i].x*std::sin(time_passed+rands[i])),randVecs[i].y,randVecs[i].z*std::cos(time_passed+rands[i]));
		//}
		
    }
	glfwTerminate();
	window_n1->release();
	//window_n2->release();
    
    exit(0);
}