#ifndef LUX_RESOURCE_HANDLER_H
#define LUX_RESOURCE_HANDLER_H

namespace Lux
{
	namespace Core
	{
		class Material;
		class Shader;
		class SubMesh;
		class Mesh;
		class Key;
		class Texture;
		struct FileInfo;
		class RenderWindow;

		class ResourceHandler
		{
		public:

			static ResourceHandler* Create(Utility::PlatformType a_PlatformType, RenderWindow* a_RenderWindow);
			virtual ~ResourceHandler();

			virtual Mesh* CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags) = 0;
			virtual Mesh* CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags) = 0;
			virtual Texture* CreateTextureFromFile(const String& a_File, const String& a_TexName) = 0;
			virtual Texture* CreateTextureFromMemory(FileInfo* a_Info, const String& a_TexName) = 0;
			virtual Shader* CreateShaderFromFile(const String& a_File, const String& a_ShaderName) = 0;
			virtual Material* CreateMaterial(const String& a_Name) = 0;

			virtual Mesh* GetMesh(const String& a_Name) = 0;
			virtual Shader* GetShader(const String& a_Name) = 0;
			virtual Material* GetMaterial(const String& a_Name) = 0;
			virtual Texture* GetTexture(const String& a_Name) = 0;
			virtual bool MaterialExists(const String& a_Name) = 0;
			virtual bool MeshExists(const String& a_Name) = 0;
			virtual bool ShaderExists(const String& a_Name) = 0;
			virtual bool TextureExists(const String& a_Name) = 0;
			virtual bool DeleteTexture(const String& a_Name) = 0;

		protected:
			ResourceHandler();
			ResourceHandler(ResourceHandler const&);// Don't Implement
			void operator=(ResourceHandler const&);// Don't implement

		};
	}
}

#endif