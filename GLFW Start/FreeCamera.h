#pragma once
#include "Camera.h"
#include <glfw3.h>

class FreeCamera : public Camera 
{
public:
	//! \brief Camera constructor with symmetrical view frustum (perspective, orthogonal).
	FreeCamera(GLdouble fovy, GLdouble aspect, GLdouble left, GLdouble right, GLdouble near, GLdouble far, float sensitivity_rotate, float sensitivity_move, glm::vec3 position, glm::vec3 up_dir, glm::vec3 look_point, bool perspective);
	// Constructor with variable view frustum.
	FreeCamera(GLdouble left, GLdouble right, GLdouble top, GLdouble down, GLdouble near, GLdouble far, float sensitivity_rotate, float sensitivity_move, glm::vec3 position, glm::vec3 up_dir, glm::vec3 look_point);
	~FreeCamera();

	//! \brief Camera controls with mouse.
	//! \param window Current window as active context.
	void camRotation(GLFWwindow* window);
	//! \brief Camera controls with keyboard.
	//! \param window Current window as active context.
	void camTranslation(GLFWwindow* window);
	//! \brief Puts camRotation(...) and camTranslation(...) together.
	//! \param window Current window as active context.
	void camControll(GLFWwindow* window);

	//! \brief Updating camera position and rotation.
	void update();

	glm::vec3 getCamPos() { return m_tmp_position; }

private:

	double m_alpha; //!< stores value retrieved from vertical movement from the mouse for rotation of the camera. 
	double m_beta;  //!< stores value retrieved from horizontal movement from the mouse for rotation of the camera. 

	float m_sensitivity_rotate;
	float m_sensitivity_move;

	glm::vec3 m_tmp_look_dir;
	glm::vec3 m_tmp_up_dir;
	glm::vec3 m_tmp_right_dir;
	glm::vec3 m_move_vec;

	glm::vec3 m_tmp_position;
	glm::vec3 m_tmp_look_point;
	glm::vec3 m_tmp_up_point;
	glm::vec3 m_tmp_right_point;

	void setCursorPos(GLFWwindow* window, double setX, double setY);
	void setCursorPos(GLFWwindow* window);
};