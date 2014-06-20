#pragma once
#define _USE_MATH_DEFINES 
#include "FreeCamera.h"
#include "../dependencies/glm/gtc/matrix_transform.hpp"
#include "../dependencies/glm/gtx/rotate_vector.hpp"

FreeCamera::FreeCamera(GLdouble fovy, GLdouble aspect, 
						GLdouble left, GLdouble right, 
						GLdouble near, GLdouble far, 
						float sensitivity_rotate, 
						float sensitivity_move, 
						glm::vec3 position, glm::vec3 up_dir, 
						glm::vec3 look_point, bool perspective)
	: Camera(fovy, aspect, left, right, near, far, position, up_dir, look_point, perspective), 
	m_alpha(0),
	m_beta(0),
	m_sensitivity_rotate(sensitivity_rotate),
	m_sensitivity_move(sensitivity_move),
	m_tmp_look_dir(m_look_dir),
	m_tmp_up_dir(m_up_dir),
	m_tmp_right_dir(m_right_dir),
	m_move_vec(glm::vec3(0)),
	m_tmp_position(m_position)
{}

FreeCamera::FreeCamera(GLdouble left, GLdouble right, 
						GLdouble top, GLdouble bottom, 
						GLdouble near, GLdouble far, 
						float sensitivity_rotate, 
						float sensitivity_move, 
						glm::vec3 position, glm::vec3 up_dir, 
						glm::vec3 look_point)
	: Camera(left, right, top, bottom, near, far, position, up_dir, look_point),
	m_alpha(0),
	m_beta(0),
	m_sensitivity_rotate(sensitivity_rotate),
	m_sensitivity_move(sensitivity_move),
	m_tmp_look_dir(m_look_dir),
	m_tmp_up_dir(m_up_dir),
	m_tmp_right_dir(m_right_dir),
	m_move_vec(glm::vec3(0)),
	m_tmp_position(m_position)
{}

FreeCamera::~FreeCamera()
{
	Camera::~Camera();
}

void FreeCamera::camRotation(GLFWwindow* window)
{
	double currentX = 0, currentY = 0;
	glfwGetCursorPos(window, &currentX, &currentY);

	double delta_X = -(currentX - (double) (m_width /2.0)); 
	double delta_Y = (currentY - (double) (m_height /2.0));

	m_alpha += m_sensitivity_rotate * M_PI * delta_X;
	m_beta += m_sensitivity_rotate * M_PI * delta_Y;

	glfwSetCursorPos(window, (double) m_width /2.0, (double) m_height /2.0);
}

void FreeCamera::camTranslation(GLFWwindow* window)
{
	int left = glfwGetKey(window, GLFW_KEY_LEFT);
	int right = glfwGetKey(window, GLFW_KEY_RIGHT);
	int up = glfwGetKey(window, GLFW_KEY_UP);
	int down = glfwGetKey(window, GLFW_KEY_DOWN);

	m_move_vec = glm::vec3(0);

	if (up == GLFW_PRESS)
		m_move_vec += (m_tmp_look_dir*m_sensitivity_move);
	if (down == GLFW_PRESS)
		m_move_vec -= (m_tmp_look_dir*m_sensitivity_move);
	if (left == GLFW_PRESS)
		m_move_vec += (m_tmp_right_dir*m_sensitivity_move);
	if (right == GLFW_PRESS)
		m_move_vec -= (m_tmp_right_dir*m_sensitivity_move);
}

void FreeCamera::camControll(GLFWwindow* window)
{
	camRotation(window);
	camTranslation(window);
}

void FreeCamera::setCursorPos(GLFWwindow* window, double setX, double setY)
{
	glfwSetCursorPos(window, setX, setY);
}

void FreeCamera::setCursorPos(GLFWwindow* window)
{
	glfwSetCursorPos(window, (double) m_width /2, (double) m_height /2);
}

void FreeCamera::update()
{
	m_tmp_position += m_move_vec;

	m_tmp_look_dir = glm::normalize(glm::rotate(m_look_dir, (float)m_beta, m_right_dir));
	m_tmp_look_dir = glm::normalize(glm::rotate(m_tmp_look_dir, (float)m_alpha, m_up_dir));
	m_tmp_up_dir = glm::normalize(glm::rotate(m_up_dir, (float)m_beta, m_right_dir));
	m_tmp_up_dir = glm::normalize(glm::rotate(m_tmp_up_dir, (float)m_alpha, m_up_dir));
	m_tmp_right_dir = glm::normalize(glm::rotate(m_right_dir, (float)m_beta, m_right_dir));
	m_tmp_right_dir = glm::normalize(glm::rotate(m_tmp_right_dir, (float)m_alpha, m_up_dir));

	glm::mat4 view = glm::lookAt(m_tmp_position, (m_tmp_position+m_tmp_look_dir), m_tmp_up_dir);

	m_vp = m_projection * view;
}