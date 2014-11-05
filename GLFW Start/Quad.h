#include "../dependencies/glm/glm.hpp"

class Quad
{
public:
	Quad(glm::vec3 left_top, glm::vec3 left_bottom, glm::vec3 right_bottom, glm::vec3 right_top);
	~Quad();

private:
	glm::vec3 m_left_top;
	glm::vec3 m_left_bottom;
	glm::vec3 m_right_bottom;
	glm::vec3 m_right_top;
};