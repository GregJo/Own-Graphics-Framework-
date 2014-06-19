#pragma once
#define _USE_MATH_DEFINES 
#include "TrackballCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

TrackballCamera::TrackballCamera(GLdouble fovy, GLdouble aspect, 
									GLdouble left, GLdouble right, 
									GLdouble near, GLdouble far, 
									glm::vec3 position, glm::vec3 up_dir, 
									glm::vec3 look_point, glm::vec3 rotat_axis, 
									bool perspective)
	: Camera(fovy, aspect, left, right, near, far, position, up_dir, look_point, perspective),
	m_rotat_axis(glm::normalize(rotat_axis))
{

}

TrackballCamera::TrackballCamera(GLdouble left, GLdouble right, 
									GLdouble top, GLdouble bottom, 
									GLdouble near, GLdouble far, 
									glm::vec3 position, glm::vec3 up_dir, 
									glm::vec3 look_point, glm::vec3 rotat_axis)
	: Camera(left, right, top, bottom, near, far, position, up_dir, look_point),
	m_rotat_axis(glm::normalize(rotat_axis))
{

}

TrackballCamera::~TrackballCamera()
{
	Camera::~Camera();
}

void TrackballCamera::update(float time)
{
	m_tmp_position = glm::rotate(m_position, time, m_rotat_axis);
	m_tmp_up_point = glm::rotate(m_up_point, time, m_rotat_axis);
	m_tmp_right_point = glm::rotate(m_right_point, time, m_rotat_axis);

	m_look_dir = glm::normalize((m_look_point-m_tmp_position));
	m_up_dir = glm::normalize((m_tmp_up_point-m_tmp_position));
	m_right_dir = glm::normalize((m_tmp_right_point-m_tmp_position));

	glm::mat4 view = glm::lookAt(m_tmp_position, m_look_point, m_up_dir);

	m_vp = m_projection * view;
}