#ifndef LUX_RESOURCE_HANDLER_DX11_H
#define LUX_RESOURCE_HANDLER_DX11_H

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

		namespace Internal
		{
			class ResourceHandlerDX11 : public ResourceHandler
			{
			public:
				virtual ~ResourceHandlerDX11();

				virtual Mesh* CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags);
				virtual Mesh* CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags);
				virtual Texture* CreateTextureFromFile(const String& a_File, const String& a_TexName);
				virtual Texture* CreateTextureFromMemory(FileInfo* a_Info, const String& a_TexName) ;
				virtual Shader* CreateShaderFromFile(const String& a_File, const String& a_ShaderName);
				virtual Material* CreateMaterial(const String& a_Name);

				virtual Mesh* GetMesh(const String& a_Name);
				virtual Shader* GetShader(const String& a_Name);
				virtual Material* GetMaterial(const String& a_Name);
				virtual Texture* GetTexture(const String& a_Name);
				virtual bool MaterialExists(const String& a_Name);
				virtual bool MeshExists(const String& a_Name);
				virtual bool ShaderExists(const String& a_Name);
				virtual bool TextureExists(const String& a_Name);
				virtual bool DeleteTexture(const String& a_Name);

			private:

			};
		}
	}
}

#endif