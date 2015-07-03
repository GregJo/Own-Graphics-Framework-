#pragma once
#include "ScreenFillingQuad.h"

ScreenFillingQuad::ScreenFillingQuad(unsigned int width, unsigned int height, glm::vec3 clearColor, unsigned int mask):Quad(glm::vec3(-1.0,1.0,0), glm::vec3(-1.0,-1.0,0), glm::vec3(1.0,-1.0,0), glm::vec3(1.0,1.0,0), glm::vec3(0), width, height, mask)
{
	m_clearColor = clearColor;
}

// This might be wrong!
ScreenFillingQuad::~ScreenFillingQuad(){ Quad::~Quad(); }

void ScreenFillingQuad::initScreenFillingQuad()
{
	glClearColor(m_clearColor.r,m_clearColor.g,m_clearColor.b,1.0f);
	initQuad();
}

// For a screenfilling quad no need for an extra framebuffer.
void ScreenFillingQuad::enableScreenFillingQuadRenderTarget()
{
	enableRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ScreenFillingQuad::disableScreenFillingQuadRenderTarget()
{
	resetToDefaultTarget();
}

void ScreenFillingQuad::clearScreenFillingQuadRenderTarget()
{
	glClearColor(m_clearColor.r,m_clearColor.g,m_clearColor.b,1.0f);
}

void ScreenFillingQuad::draw()
{
	resetToDefaultTarget();
	Quad::draw();
}
