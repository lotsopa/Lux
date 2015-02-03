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
		class Texture1D;
		class Texture2D;
		class Texture3D;
		class TextureSampler;
		struct TextureSamplerOptions;
		struct FileInfo;
		class RenderWindow;

		class ResourceHandler
		{
		public:

			static ResourceHandler* Create(Utility::PlatformType a_PlatformType, RenderWindow* a_RenderWindow);
			virtual ~ResourceHandler();

			virtual Mesh* CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags) = 0;
			virtual Mesh* CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags) = 0;
			virtual Texture2D* CreateTexture2DFromFile(const String& a_File, const String& a_TexName) = 0;
			virtual Texture2D* CreateTexture2DFromMemory(FileInfo* a_Info, const String& a_TexName) = 0;
			virtual Texture1D* CreateTexture1DFromFile(const String& a_File, const String& a_TexName) = 0;
			virtual Texture1D* CreateTexture1DFromMemory(FileInfo* a_Info, const String& a_TexName) = 0;
			virtual Texture3D* CreateTexture3DFromFile(const String& a_File, const String& a_TexName) = 0;
			virtual Texture3D* CreateTexture3DFromMemory(FileInfo* a_Info, const String& a_TexName) = 0;
			virtual Shader* CreateShaderFromFile(const String& a_File, const String& a_ShaderName) = 0;
			virtual Material* CreateMaterial(const String& a_Name) = 0;
			virtual TextureSampler* CreateTextureSampler(const String& a_Name,TextureSamplerOptions& a_InitOptions) = 0;

			virtual Mesh* GetMesh(const String& a_Name) = 0;
			virtual Shader* GetShader(const String& a_Name) = 0;
			virtual Material* GetMaterial(const String& a_Name) = 0;
			virtual Texture2D* GetTexture2D(const String& a_Name) = 0;
			virtual Texture3D* GetTexture3D(const String& a_Name) = 0;
			virtual Texture1D* GetTexture1D(const String& a_Name) = 0;
			virtual TextureSampler* GetTextureSampler(const String& a_Name) = 0;
			virtual bool MaterialExists(const String& a_Name) = 0;
			virtual bool MeshExists(const String& a_Name) = 0;
			virtual bool ShaderExists(const String& a_Name) = 0;
			virtual bool Texture2DExists(const String& a_Name) = 0;
			virtual bool Texture1DExists(const String& a_Name) = 0;
			virtual bool Texture3DExists(const String& a_Name) = 0;
			virtual bool TextureSamplerExists(const String& a_Name) = 0;
			virtual bool DeleteTexture2D(const String& a_Name) = 0;
			virtual bool DeleteTexture1D(const String& a_Name) = 0;
			virtual bool DeleteTexture3D(const String& a_Name) = 0;
			virtual bool DeleteTextureSampler(const String& a_Name) = 0;

		protected:
			ResourceHandler();
			ResourceHandler(ResourceHandler const&);// Don't Implement
			void operator=(ResourceHandler const&);// Don't implement

		};
	}
}

#endif