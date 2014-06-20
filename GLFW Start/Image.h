#pragma once
#include "../dependencies/FreeImage/include/FreeImage.h"
#include <fstream>
#include <memory>

//Errors to cover:
class Bitmap
{
public:

	Bitmap();
	~Bitmap();

	//! \brief Load image.
	//! \return Returns true if succeeded.
	bool loadFile(std::string FileName);
	int getHeight();
	int getWidth();
	int getSizeInBytes();
	//! \brief Get image data in bytes.
	//! \return Returns image data in bytes.
	unsigned char* getData();

private:
	unsigned int m_size;
	unsigned int m_width, m_height;
	unsigned int m_imageSize;
	std::shared_ptr<FIBITMAP> m_image;
};
