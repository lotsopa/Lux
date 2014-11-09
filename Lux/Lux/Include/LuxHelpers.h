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

		static quat RotationBetweenVectors(vec3 start, vec3 dest)
		{
			start = normalize(start);
			dest = normalize(dest);

			float cosTheta = dot(start, dest);
			vec3 rotationAxis;

			if (cosTheta < -1 + 0.001f)
			{
				// special case when vectors in opposite directions:
				// there is no "ideal" rotation axis
				// So guess one; any will do as long as it's perpendicular to start
				rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
				if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
					rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

				rotationAxis = normalize(rotationAxis);
				return angleAxis(180.0f, rotationAxis);
			}

			rotationAxis = cross(start, dest);

			float s = sqrtf((1 + cosTheta) * 2);
			float invs = 1 / s;

			return quat(
				s * 0.5f,
				rotationAxis.x * invs,
				rotationAxis.y * invs,
				rotationAxis.z * invs
				);
		}

		static quat RotateTowards(quat q1, quat q2, float maxAngle)
		{
			if (maxAngle < 0.001f){
				// No rotation allowed. Prevent dividing by 0 later.
				return q1;
			}

			float cosTheta = dot(q1, q2);

			// q1 and q2 are already equal.
			// Force q2 just to be sure
			if (cosTheta > 0.9999f){
				return q2;
			}

			// Avoid taking the long path around the sphere
			if (cosTheta < 0){
				q1 = q1*-1.0f;
				cosTheta *= -1.0f;
			}

			float angle = acos(cosTheta);

			// If there is only a 2° difference, and we are allowed 5°,
			// then we arrived.
			if (angle < maxAngle){
				return q2;
			}

			float fT = maxAngle / angle;
			angle = maxAngle;

			quat res = (sin((1.0f - fT) * angle) * q1 + sin(fT * angle) * q2) / sin(angle);
			res = normalize(res);
			return res;
		}

		static void ThrowError(const String a_String)
		{
			LUX_LOG(logERROR) << a_String;
#ifdef _WIN32
			MessageBox(nullptr, a_String.c_str(), "Error", MB_ICONERROR | MB_SETFOREGROUND);
#endif
			throw std::logic_error(a_String);
		}

		static void ErrorCallbackFunc(int error, const char* description)
		{
			LUX_LOG(Lux::Utility::logERROR) << "An Error occurred. " << "Error Code: " << error << ". " << "Error description: " << description;
			Lux::Utility::ThrowError(description);
		}
	}
}

#endif