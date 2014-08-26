#include "../dependencies/glm/glm.hpp"

class ScreenFillingQuad
{
public:
	ScreenFillingQuad();
	~ScreenFillingQuad(){}

	void draw();
private:
	glm::vec3 m_left_top;
	glm::vec3 m_left_bottom;
	glm::vec3 m_right_bottom;
	glm::vec3 m_right_top;
};