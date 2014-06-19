#pragma once
#include "Image.h" 

Bitmap::Bitmap() : m_image(nullptr)
{
	m_image = std::shared_ptr<FIBITMAP>(nullptr);
}

Bitmap::~Bitmap()
{

}

bool Bitmap::loadFile(std::string FileName)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(FileName.c_str(), 0);

	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(FileName.c_str());

	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
	{
		return false;
	}

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		m_image = std::shared_ptr<FIBITMAP>(FreeImage_Load(fif, FileName.c_str()), [](FIBITMAP* dib){ FreeImage_Unload(dib); });

	//if the image failed to load, return failure
	if(!m_image)
	{
		return false;
	}

	//get the image width and height
	m_width = FreeImage_GetWidth(m_image.get());
	m_height = FreeImage_GetHeight(m_image.get());
	m_size = FreeImage_GetDIBSize(m_image.get());

	//if this somehow one of these failed (they shouldn't), return failure
	if((FreeImage_GetBits(m_image.get()) == 0) || (m_width == 0) || (m_height == 0) || (m_size == 0))
	{
		return false;
	}

	//return success
	return true;
}

int Bitmap::getWidth()
{
	return m_width;
}

int Bitmap::getHeight()
{
	return m_height;
}

int Bitmap::getSizeInBytes()
{
	return m_size;
}

unsigned char* Bitmap::getData()
{
	return FreeImage_GetBits(m_image.get());
}