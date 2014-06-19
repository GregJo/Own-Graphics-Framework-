#pragma once
#include "Camera.h"

//TODO: Implement class
//Errors to cover:
class ChasingCamera : public Camera 
{
public:
	ChasingCamera(GLdouble fovy, GLdouble aspect, GLdouble near, GLdouble far, glm::vec3 position, glm::vec3 target);
	ChasingCamera(GLdouble left, GLdouble right, GLdouble top, GLdouble down, GLdouble near, GLdouble far, glm::vec3 position, glm::vec3 target);
	~ChasingCamera();

private:
	glm::vec3 m_target;
};