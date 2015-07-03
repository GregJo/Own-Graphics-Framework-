#include "SkyBox.h"

SkyBox::SkyBox(glm::vec3 center, std::string texImageFileName, float scale)
{
	m_quads = new Quad*[6];
	m_scale = 1.0f;
	m_center = glm::vec3(0,0,0);

	m_texCoordsBottom = nullptr;
	m_texCoordsTop = nullptr;

	m_texCoordsFront = nullptr;
	m_texCoordsBack = nullptr;

	m_texCoordsLeft = nullptr;
	m_texCoordsRight = nullptr;

	glm::vec3 skyboxCube[8] = {glm::vec3(-1.0f,-1.0f,-1.0f),glm::vec3(1.0f,-1.0f,-1.0f),glm::vec3(1.0f,-1.0f,1.0f),glm::vec3(-1.0f,-1.0f,1.0f),
								glm::vec3(-1.0f,1.0f,-1.0f),glm::vec3(1.0f,1.0f,-1.0f),glm::vec3(1.0f,1.0f,1.0f),glm::vec3(-1.0f,1.0f,1.0f)};

	if (scale!=1.0f)
	{
		for (int i = 0; i < 8; i++)
		{
			skyboxCube[i]*=scale;
		}
	}

	glm::vec3 position(0);

	m_quads[0]=new Quad( skyboxCube[1]+m_center,  skyboxCube[0]+m_center,  skyboxCube[3]+m_center,  skyboxCube[2]+m_center, position, 0, 0, 0);
	m_quads[1]=new Quad( skyboxCube[4]+m_center,  skyboxCube[5]+m_center,  skyboxCube[6]+m_center,  skyboxCube[7]+m_center, position, 0, 0, 0);
	m_quads[2]=new Quad( skyboxCube[0]+m_center,  skyboxCube[1]+m_center,  skyboxCube[5]+m_center,  skyboxCube[4]+m_center, position, 0, 0, 0);
	m_quads[3]=new Quad( skyboxCube[2]+m_center,  skyboxCube[3]+m_center,  skyboxCube[7]+m_center,  skyboxCube[6]+m_center, position, 0, 0, 0);
	m_quads[4]=new Quad( skyboxCube[3]+m_center,  skyboxCube[0]+m_center,  skyboxCube[4]+m_center,  skyboxCube[7]+m_center, position, 0, 0, 0);
	m_quads[5]=new Quad( skyboxCube[1]+m_center,  skyboxCube[2]+m_center,  skyboxCube[6]+m_center,  skyboxCube[5]+m_center, position, 0, 0, 0);

	m_shaderProg.initShaderProgram("SkyboxVert.glsl","","","","SkyboxFrag.glsl");
}

SkyBox::~SkyBox()
{
	for (int i = 0; i < 6; i++)
	{
		m_quads[i]->~Quad();
	}

	delete[] m_quads;

	delete[] m_texCoordsBottom;
	delete[] m_texCoordsTop;

	delete[] m_texCoordsFront;
	delete[] m_texCoordsBack;

	delete[] m_texCoordsLeft;
	delete[] m_texCoordsRight;
}

void SkyBox::initSkyBox()
{	
	if(m_texCoordsBottom == 0 && m_texCoordsTop == 0 && m_texCoordsFront == 0 && m_texCoordsBack == 0 && m_texCoordsLeft == 0 && m_texCoordsRight == 0)
	{
		m_texCoordsBottom = new GLfloat[8];
		m_texCoordsTop = new GLfloat[8];

		m_texCoordsFront = new GLfloat[8];
		m_texCoordsBack = new GLfloat[8];

		m_texCoordsLeft = new GLfloat[8];
		m_texCoordsRight = new GLfloat[8];

		GLfloat textureCoordsBottom[8]={1.0f/4.0f, 1.0f/3.0f, 1.0f/4.0f, 0.0f, 2.0f/4.0f, 0.0f, 2.0f/4.0f, 1.0f/3.0f};
		GLfloat textureCoordsTop[8]={1.0f/4.0f, 1.0f, 1.0f/4.0f, 2.0f/3.0f, 2.0f/4.0f, 2.0f/3.0f, 2.0f/4.0f, 1.0f};

		GLfloat textureCoordsFront[8]={0.0f, 1.0f/3.0f, 1.0f/4.0f, 1.0f/3.0f, 1.0f/4.0f, 2.0f/3.0f, 0.0f, 2.0f/3.0f};
		GLfloat textureCoordsBack[8]={1.0f/2.0f, 1.0f/3.0f, 3.0f/4.0f, 1.0f/3.0f, 3.0f/4.0f, 2.0f/3.0f, 1.0f/2.0f, 2.0f/3.0f};

		GLfloat textureCoordsLeft[8]={3.0f/4.0f, 1.0f/3.0f, 1.0f, 1.0f/3.0f, 1.0f, 2.0f/3.0f, 3.0f/4.0f, 2.0f/3.0f};
		GLfloat textureCoordsRight[8]={1.0f/4.0f, 1.0f/3.0f, 1.0f/2.0f, 1.0f/3.0f, 1.0f/2.0f, 2.0f/3.0f, 1.0f/4.0f, 2.0f/3.0f};

		memcpy(m_texCoordsBottom,textureCoordsBottom,8*sizeof(GLfloat));
		memcpy(m_texCoordsTop,textureCoordsTop,8*sizeof(GLfloat));

		memcpy(m_texCoordsFront,textureCoordsFront,8*sizeof(GLfloat));
		memcpy(m_texCoordsBack,textureCoordsBack,8*sizeof(GLfloat));

		memcpy(m_texCoordsLeft,textureCoordsLeft,8*sizeof(GLfloat));
		memcpy(m_texCoordsRight,textureCoordsRight,8*sizeof(GLfloat));
	}

	//Bottom
	m_quads[0]->setTexCoords(m_texCoordsBottom);
	
	//Top
	m_quads[1]->setTexCoords(m_texCoordsTop);

	//Front
	m_quads[2]->setTexCoords(m_texCoordsFront);

	//Back
	m_quads[3]->setTexCoords(m_texCoordsBack);

	//Left
	m_quads[4]->setTexCoords(m_texCoordsLeft);

	//Right
	m_quads[5]->setTexCoords(m_texCoordsRight);

	m_quads[0]->initQuad();
	m_quads[1]->initQuad();
	m_quads[2]->initQuad();
	m_quads[3]->initQuad();
	m_quads[4]->initQuad();
	m_quads[5]->initQuad();

	m_quads[0]->setGLSLProgram(m_shaderProg);
	m_quads[1]->setGLSLProgram(m_shaderProg);
	m_quads[2]->setGLSLProgram(m_shaderProg);
	m_quads[3]->setGLSLProgram(m_shaderProg);
	m_quads[4]->setGLSLProgram(m_shaderProg);
	m_quads[5]->setGLSLProgram(m_shaderProg);
}

bool SkyBox::setTexImage(std::string texImageFileName)
{
	bool success = m_texImage.loadFile(texImageFileName);

	m_skyboxTexID;

	if (success) {
        /* Create and load texture to OpenGL */
		glGenTextures(1, &m_skyboxTexID);
		glBindTexture(GL_TEXTURE_2D, m_skyboxTexID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texImage.getWidth(),
			m_texImage.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE,
			m_texImage.getData());
    }
	else
	{
		Logger::GetInstance().Log("Couldn't load Image: %s\n", "../Skybox Texture/Above_The_Sea_Copy.jpg");
		return success = false;
	}
	return success;
}

void SkyBox::setTexCoords(GLfloat* bottom, GLfloat* top, GLfloat* front, GLfloat* back, GLfloat* left, GLfloat* right)
{
	m_texCoordsBottom = bottom;
	m_texCoordsTop = top;

	m_texCoordsFront = front;
	m_texCoordsBack = back;

	m_texCoordsLeft = left;
	m_texCoordsRight = right;
}

void SkyBox::drawSkyBox()
{
	glDepthMask(0);
	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_skyboxTexID);
		m_quads[i]->draw();
	}
	glDepthMask(1);
}