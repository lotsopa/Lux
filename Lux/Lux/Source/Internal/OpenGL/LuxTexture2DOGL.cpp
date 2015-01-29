#include "LuxPCH.h"
#include "LuxTexture.h"
#include "LuxTexture2D.h"
#include "LuxTexture2DOGL.h"
#include "LuxErrorCheckOGL.h"

Lux::Core::Internal::Texture2DOGL::Texture2DOGL(unsigned int a_ImgWidth, unsigned int a_ImgHeight, unsigned char* a_Bits) : m_TextureID(-1)
{
	glGenTextures(1, &m_TextureID);

	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	//store the texture data for OpenGL use (FreeImage loads in BGRA order)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, a_ImgWidth, a_ImgHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, a_Bits);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	Utility::Internal::CheckOGLError();
}

Lux::Core::Internal::Texture2DOGL::~Texture2DOGL()
{
	glDeleteTextures(1, &m_TextureID);
}