#pragma once
#include "../dependencies/gl/include/glew.h"
#include "../dependencies/GLFW/include/GLFW/glfw3.h"
#include <stdlib.h>
#include <stdio.h>
#include "GraphicsWindow.h"

GraphicsWindow::GraphicsWindow(int width, int height, const char* name, GLFWmonitor* monitor, GLFWwindow* share, GLFWkeyfun cbfun) : m_height(height), m_width(width)
{
	m_windowHandle = glfwCreateWindow(width, height, name, monitor, share);

	if (!m_windowHandle)
    {
        glfwTerminate();
		printf( "Window Initialization failed!\n") ;
        exit(EXIT_FAILURE);
    }

	makeContextCurrent();

	glfwSetKeyCallback(m_windowHandle, cbfun);
}

GraphicsWindow::~GraphicsWindow()
{
	release();
}

GLFWwindow* GraphicsWindow::getWindowHandle()
{
	return m_windowHandle;
}

int GraphicsWindow::getWindowHeight()
{
	int width, height;
	glfwGetWindowSize(m_windowHandle, &width, &height);
	return height; 
}

int GraphicsWindow::getWindowWidth()
{
	int width, height;
	glfwGetWindowSize(m_windowHandle, &width, &height);
	return width; 
}

int GraphicsWindow::shouldClose()
{
	return glfwWindowShouldClose(m_windowHandle) != 0;
}

void GraphicsWindow::swapBuffers()
{
	glfwSwapBuffers(m_windowHandle);
}

void GraphicsWindow::makeContextCurrent()
{
	glfwMakeContextCurrent(m_windowHandle);
}

void GraphicsWindow::release()
{
	glfwDestroyWindow(m_windowHandle);
	m_windowHandle = nullptr;
}