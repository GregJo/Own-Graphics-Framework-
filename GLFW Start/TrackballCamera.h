#pragma once
#include "Camera.h"
#include "../dependencies/GLFW/include/GLFW/glfw3.h"

//No controls on this camera type controls yet
class TrackballCamera : public Camera 
{
public:
	// Constructor with symmetrical view frustum (perspective, orthogonal).
	TrackballCamera(GLdouble fovy, GLdouble aspect, GLdouble left, GLdouble right, GLdouble near, GLdouble far, glm::vec3 position, glm::vec3 up_dir, glm::vec3 look_point, glm::vec3 m_rotat_axis, bool perspective);
	// Constructor with variable view frustum.
	TrackballCamera(GLdouble left, GLdouble right, GLdouble top, GLdouble down, GLdouble near, GLdouble far, glm::vec3 position, glm::vec3 up_dir, glm::vec3 look_point, glm::vec3 m_rotat_axis);
	~TrackballCamera();

	void update(float time);

	glm::vec3 getCamPos() { return m_tmp_position; }

private:
	glm::vec3 m_rotat_axis;

	glm::vec3 m_tmp_position;
	glm::vec3 m_tmp_look_point;
	glm::vec3 m_tmp_up_point;
	glm::vec3 m_tmp_right_point;
};