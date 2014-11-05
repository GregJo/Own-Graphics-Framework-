#include "../dependencies/glm/glm.hpp"
#include "Quad.h"

class ScreenFillingQuad : public Quad
{
public:
	ScreenFillingQuad() : Quad(glm::vec3(0,1,0), glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(1,1,0)) { }
	~ScreenFillingQuad();

	void draw();
private:

};