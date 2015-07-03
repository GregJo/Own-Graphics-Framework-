#pragma once
#include "Camera.h"
#include "Logger.h"
#include "../dependencies/GLFW/include/GLFW/glfw3.h"
#include "../dependencies/glm/gtc/matrix_transform.hpp"

Camera::Camera(GLdouble fovy, GLdouble aspect, GLdouble left, GLdouble right, GLdouble near, GLdouble far, glm::vec3 position, 
				glm::vec3 up_dir, glm::vec3 look_point, bool perspective):
																m_fovy(fovy),
																m_aspect(aspect),
																m_left(left),
																m_right(right),
																m_near(near),
																m_far(far),
																m_up_dir(glm::normalize(up_dir)),
																m_position(position),
																m_look_point(look_point)

{
	if(m_up_dir == glm::vec3(0))
	{
		Logger::GetInstance().Log("Camera up vector must not be zero vector.");
		//Either exit(1) or set default values.
		exit(1);
	}

	if(m_position == m_look_point)
	{
		Logger::GetInstance().Log("Camera position vector must not be equal to .");
		//Either exit(1) or set default values.
		exit(1);
	}

	m_look_dir = glm::normalize(m_look_point-m_position);

	m_right_dir = glm::normalize(glm::cross(m_up_dir, m_look_dir));

	m_up_point = m_position + m_up_dir;

	m_right_point = m_position + m_right_dir;

	m_view = glm::lookAt(m_position, m_look_point, m_up_dir);

	if(perspective)
		m_projection = glm::perspective(m_fovy, m_aspect, m_near, m_far);
	else
		m_projection = glm::ortho(m_left, m_right, m_near, m_far);

	m_vp = m_projection * m_view;
}

Camera::Camera(GLdouble left, GLdouble right, GLdouble top, GLdouble bottom, GLdouble near, GLdouble far, glm::vec3 position, glm::vec3 up_dir, glm::vec3 look_point):
																m_left(left),
																m_right(right),
																m_top(top),
																m_bottom(bottom),
																m_near(near),
																m_far(far),
																m_position(position),
																m_up_dir(glm::normalize(up_dir))
{
	if(m_up_dir == glm::vec3(0))
	{
		Logger::GetInstance().Log("Camera up vector must not be zero vector.");
		//Either exit(1) or set default values.
		exit(1);
	}

	if(m_position == m_look_point)
	{
		Logger::GetInstance().Log("Camera position vector must not be equal to .");
		//Either exit(1) or set default values.
		exit(1);
	}

	m_look_dir = m_look_point-m_position;

	m_right_dir = glm::normalize(glm::cross(m_up_dir, m_look_dir));

	m_up_point = m_position + m_up_dir;

	m_right_point = m_position + m_right_dir;

	glm::mat4 view = glm::lookAt(m_position, m_look_point, m_up_dir);

	m_projection = glm::frustum(m_left, m_right, m_bottom, m_top, m_near, m_far);

	m_vp = m_projection * view;
}

Camera::~Camera()
{}

void Camera::setViewport(unsigned int width, unsigned int height, float pixeloffset_x, float pixeloffset_y)
{
	m_height = height;
	m_width = width;
	m_pixeloffset_x = pixeloffset_x;
	m_pixeloffset_y = pixeloffset_y;

	glViewport(pixeloffset_x, pixeloffset_y, width, height);
}

glm::vec3 Camera::getCamPos(){return m_position;}
	
void Camera::update(float time){}
	
void Camera::update(){}
	
void Camera::camControll(GLFWwindow* window){}