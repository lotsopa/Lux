#include "LuxPCH.h"
#include "LuxTexture.h"
#include "LuxTexture2D.h"
#include "LuxTexture2DOGL.h"
#include "LuxErrorCheckOGL.h"
#include "LuxKey.h"
#include "LuxBufferOGL.h"
#include "LuxShader.h"
#include "LuxShaderOGL.h"

Lux::Core::Internal::Texture2DOGL::Texture2DOGL(unsigned int a_ImgWidth, unsigned int a_ImgHeight, unsigned char* a_Bits) : Texture2D(), m_TextureID(-1)
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

void Lux::Core::Internal::Texture2DOGL::Bind(unsigned int a_Slot, const Key& a_Name, Shader* a_Shader, ShaderProgram a_Program)
{
	ShaderOGL* glShader = (ShaderOGL*)a_Shader;
	unsigned int texLoc = glShader->GetUniformLocation(a_Name);
	m_LastSlot = a_Slot;
	glActiveTexture(GL_TEXTURE0 + m_LastSlot);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glUniform1i(texLoc, m_LastSlot);
	Utility::Internal::CheckOGLError();
}

void Lux::Core::Internal::Texture2DOGL::Unbind()
{
	glActiveTexture(GL_TEXTURE0 + m_LastSlot);
	glBindTexture(GL_TEXTURE_2D, 0);
}
