#pragma once
#include "../dependencies/gl/include/glew.h"
#include "../dependencies/GLFW/include/GLFW/glfw3.h"

//Errors to cover:
class GraphicsWindow
{
public:

	GraphicsWindow();
	GraphicsWindow(int width, int height, const char* name, GLFWmonitor* monitor, GLFWwindow* share, GLFWkeyfun cbfun);
	~GraphicsWindow();

	void release();

	GLFWwindow* getWindowHandle();
	
	int getWindowHeight();
	int getWindowWidth();

	//! \brief Checking if the window should close flag should be set.
	//! \return Returns the int should close flag.
	int shouldClose();
	
	//! \brief Make window current context.
	void makeContextCurrent();
	
	void swapBuffers();

private:

	GLFWwindow* m_windowHandle;
	int m_height;
	int m_width; 
};

static void setErrorCallbackAndInit(GLFWerrorfun cbfun)
{
	glfwSetErrorCallback(cbfun);

	if (!glfwInit())
		exit(1);
}