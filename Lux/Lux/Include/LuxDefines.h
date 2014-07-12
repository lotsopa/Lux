#ifndef LUX_DEFINES_H
#define LUX_DEFINES_H

#define LuxAssert(a) assert(a)

namespace Lux
{
	using namespace glm;

	// Some typedefs
	typedef std::string String;
	typedef long long int64;
	
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

	static void ThrowError(const String a_String)
	{
		LUX_LOG(logERROR) << a_String;
		MessageBox(nullptr, a_String.c_str(), "Error", MB_ICONERROR | MB_SETFOREGROUND);
		throw std::logic_error(a_String);
	}
}

#endif