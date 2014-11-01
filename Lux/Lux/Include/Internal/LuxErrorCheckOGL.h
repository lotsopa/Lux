#ifndef LUX_ERROR_CHECK_OGL_H
#define LUX_ERROR_CHECK_OGL_H

namespace Lux
{
	namespace Utility
	{
		namespace Internal
		{
			/*
			Checks for an openGL error. If there is one, the function throws.
			*/
			static void CheckOGLError()
			{
#ifdef _DEBUG
				int err = glGetError();

				if (err != GL_NO_ERROR)
				{
					switch (err)
					{
					case GL_INVALID_ENUM:
						ThrowError("OpenGL Error. Invalid Enum.");
						break;

					case GL_INVALID_VALUE:
						ThrowError("OpenGL Error. Invalid Value");
						break;

					case GL_INVALID_OPERATION:
						ThrowError("OpenGL Error. Invalid Operation");
						break;

					case GL_INVALID_FRAMEBUFFER_OPERATION:
						ThrowError("OpenGL Error. Invalid Frame Buffer Operation");
						break;

					case GL_OUT_OF_MEMORY:
						ThrowError("OpenGL Error. Out of Memory.");
						break;

					case GL_STACK_UNDERFLOW:
						ThrowError("OpenGL Error. Stack Underflow.");
						break;

					case GL_STACK_OVERFLOW:
						ThrowError("OpenGL Error. Stack Overflow.");
						break;
					}
				}
#endif
			}

			static void ClearAllOGLErrors()
			{
				int err = glGetError();

				while (err != GL_NO_ERROR)
				{
					err = glGetError();
				}
			}

			static void CheckFBOError(GLenum target)
			{
				int err = glCheckFramebufferStatus(target);

				if (err != GL_FRAMEBUFFER_COMPLETE)
				{
					std::string errStr("Frame Buffer Error.");

					switch (err)
					{
					case GL_FRAMEBUFFER_UNDEFINED:
						errStr.append("Framebuffer undefined.");
						break;

					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
						errStr.append("Framebuffer incomplete attachment");
						break;

					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
						errStr.append("Incomplete missing attachment");
						break;

					case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
						errStr.append("Incomplete draw buffer");
						break;

					case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
						errStr.append("Incomplete read buffer");
						break;

					case GL_FRAMEBUFFER_UNSUPPORTED:
						errStr.append("Framebuffer unsupported");
						break;

					case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
						errStr.append("Incomplete multisample");
						break;

					case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
						errStr.append("Incomplete layer targets");
						break;

					default:
						errStr.append("Unknown error.");
						break;
					}
					ThrowError(errStr);
				}
			}
		}
	}
}

#endif