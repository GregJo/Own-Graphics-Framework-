#include "ScreenFillingQuad.h"
#include "../dependencies/GLFW/include/GLFW/glfw3.h"
#include "../dependencies/glm/gtc/matrix_transform.hpp"

ScreenFillingQuad::ScreenFillingQuad()
{
	m_left_top = glm::vec3(0,1,0);
	m_left_bottom = glm::vec3(0,0,0);
	m_right_bottom = glm::vec3(1,0,0);
	m_right_top = glm::vec3(1,1,0);
}