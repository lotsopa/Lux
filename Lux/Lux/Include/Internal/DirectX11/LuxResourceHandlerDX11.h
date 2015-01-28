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
			class RenderWindowDX11;

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
				ResourceHandlerDX11(ResourceHandlerDX11 const&);// Don't Implement
				void operator=(ResourceHandlerDX11 const&);// Don't implement
				ResourceHandlerDX11(RenderWindowDX11* a_RenderWindow);
				RenderWindowDX11* m_RenderWindow;
				friend class ResourceHandler;

				typedef std::map<Key, std::shared_ptr<Texture>> TextureMap;
				typedef std::map<Key, std::shared_ptr<Mesh>> MeshMap;
				typedef std::map<Key, Mesh*> MeshMapSimple;
				typedef std::map<Key, std::shared_ptr<Material>> MaterialMap;
				typedef std::map<Key, std::shared_ptr<Shader>> ShaderMap;
				typedef std::map<Key, Microsoft::WRL::ComPtr<ID3D11InputLayout>> InputLayoutMap;
				MeshMap m_MeshMap;
				MeshMapSimple m_LoadedFilenameMeshes;
				MaterialMap m_MaterialMap;
				TextureMap m_TextureMap;
				ShaderMap m_ShaderMap;
				InputLayoutMap m_InputLayouts;

#if LUX_THREAD_SAFE == TRUE
				std::mutex m_MeshMapMutex;
				std::mutex m_MaterialMapMutex;
				std::mutex m_TextureMapMutex;
				std::mutex m_ShaderMapMutex;
				std::mutex m_InputLayoutMutex;
#endif

				void AddMeshToMap(const String& a_Str, Mesh* a_Ent);
				void AddFileNameToMap(const String& a_Str, Mesh* a_Ent);
				Mesh* GetLoadedMesh(const String& a_FileStr);
				void AddMaterialToMap(const String& a_Str, Material* a_Mat);
				void AddTextureToMap(const String& a_Str, Texture* a_Tex);
				void LoadAllTexturesOfTypeFromMaterial(aiMaterial* a_Mat, aiTextureType a_TexType);
				void AddShaderToMap(const String& a_Str, Shader* a_Shader);
				void AddInputLayoutToMap(const String& a_Str, ID3D11InputLayout* a_Layout);
				HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);
			};
		}
	}
}

#endif