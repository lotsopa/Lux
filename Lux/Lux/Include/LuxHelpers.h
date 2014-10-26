#ifndef LUX_HELPERS_H
#define LUX_HELPERS_H

namespace Lux
{
	namespace Utility
	{
		enum PlatformType
		{
			OPEN_GL = 0,
			DIRECTX_11
		};

		struct AppInitOptions
		{
			AppInitOptions::AppInitOptions() :
				m_AntiAliasing(0),
				m_LoggingLevel(TLogLevel::logDEBUG4),
				m_PlatformType(OPEN_GL),
				m_WindowCaption("Lux Application"),
				m_WindowHeight(600),
				m_WindowWidth(800)
			{

			}

			AppInitOptions::~AppInitOptions()
			{

			}

			unsigned int m_WindowWidth;
			unsigned int m_WindowHeight;
			String m_WindowCaption;
			unsigned int m_AntiAliasing;
			TLogLevel m_LoggingLevel;
			PlatformType m_PlatformType;
		};

		static void ErrorCallbackFunc(int error, const char* description)
		{
			LUX_LOG(Lux::Utility::logERROR) << "An Error occurred. " << "Error Code: " << error << ". " << "Error description: " << description;
		}

		template<typename T>
		static void SafePtrDelete(T* a_Ptr)
		{
			if (a_Ptr)
			{
				delete a_Ptr;
				a_Ptr = nullptr;
			}
		}

		template<typename T>
		static void SafeArrayDelete(T* a_Array)
		{
			if (a_Array)
			{
				delete[] a_Array;
				a_Array = nullptr;
			}
		}

		static mat4x4 ConvertMatrixAssimpToGLM(aiMatrix4x4 a_AssimpMat)
		{
			// GLM is column-major and Assimp uses Row-major matrices
			a_AssimpMat = a_AssimpMat.Transpose(); // Make it column major

			// Not checked
			mat4x4 retMat;
			retMat[0][0] = a_AssimpMat[0][0];
			retMat[0][1] = a_AssimpMat[0][1];
			retMat[0][2] = a_AssimpMat[0][2];
			retMat[0][3] = a_AssimpMat[0][3];

			retMat[1][0] = a_AssimpMat[1][0];
			retMat[1][1] = a_AssimpMat[1][1];
			retMat[1][2] = a_AssimpMat[1][2];
			retMat[1][3] = a_AssimpMat[1][3];

			retMat[2][0] = a_AssimpMat[2][0];
			retMat[2][1] = a_AssimpMat[2][1];
			retMat[2][2] = a_AssimpMat[2][2];
			retMat[2][3] = a_AssimpMat[2][3];

			retMat[3][0] = a_AssimpMat[3][0];
			retMat[3][1] = a_AssimpMat[3][1];
			retMat[3][2] = a_AssimpMat[3][2];
			retMat[3][3] = a_AssimpMat[3][3];

			return retMat;
		}

		static vec4 ConvertVec4AssimpToGLM(aiColor4D& a_Vec)
		{
			return vec4(a_Vec.r, a_Vec.g, a_Vec.b, a_Vec.a);
		}

		static vec3 ConvertVec3AssimpToGLM(aiVector3D& a_Vec)
		{
			return vec3(a_Vec.x, a_Vec.y, a_Vec.z);
		}

		static quat ConvertQuatAssimpToGLM(aiQuaternion& a_Quat)
		{
			return quat(a_Quat.x, a_Quat.y, a_Quat.z, a_Quat.w);
		}

		static void ThrowError(const String a_String)
		{
			LUX_LOG(logERROR) << a_String;
#ifdef _WIN32
			MessageBox(nullptr, a_String.c_str(), "Error", MB_ICONERROR | MB_SETFOREGROUND);
#endif
			throw std::logic_error(a_String);
		}
	}
	}

#endif