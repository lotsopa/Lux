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
		class TextureSampler;
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
				virtual Texture2D* CreateTexture2DFromFile(const String& a_File, const String& a_TexName);
				virtual Texture2D* CreateTexture2DFromMemory(FileInfo* a_Info, const String& a_TexName) ;
				virtual Texture1D* CreateTexture1DFromFile(const String& a_File, const String& a_TexName);
				virtual Texture1D* CreateTexture1DFromMemory(FileInfo* a_Info, const String& a_TexName);
				virtual Texture3D* CreateTexture3DFromFile(const String& a_File, const String& a_TexName);
				virtual Texture3D* CreateTexture3DFromMemory(FileInfo* a_Info, const String& a_TexName);
				virtual Shader* CreateShaderFromFile(const String& a_File, const String& a_ShaderName);
				virtual Material* CreateMaterial(const String& a_Name);
				virtual TextureSampler* CreateTextureSampler(const String& a_Name, TextureSamplerOptions& a_InitOptions);

				virtual Mesh* GetMesh(const String& a_Name);
				virtual Shader* GetShader(const String& a_Name);
				virtual Material* GetMaterial(const String& a_Name);
				virtual Texture2D* GetTexture2D(const String& a_Name);
				virtual Texture1D* GetTexture1D(const String& a_Name);
				virtual Texture3D* GetTexture3D(const String& a_Name);
				virtual TextureSampler* GetTextureSampler(const String& a_Name);
				virtual bool MaterialExists(const String& a_Name);
				virtual bool MeshExists(const String& a_Name);
				virtual bool ShaderExists(const String& a_Name);
				virtual bool Texture2DExists(const String& a_Name);
				virtual bool Texture1DExists(const String& a_Name);
				virtual bool Texture3DExists(const String& a_Name);
				virtual bool TextureSamplerExists(const String& a_Name);
				virtual bool DeleteTexture2D(const String& a_Name);
				virtual bool DeleteTexture1D(const String& a_Name);
				virtual bool DeleteTexture3D(const String& a_Name);
				virtual bool DeleteTextureSampler(const String& a_Name);
			private:
				ResourceHandlerDX11(ResourceHandlerDX11 const&);// Don't Implement
				void operator=(ResourceHandlerDX11 const&);// Don't implement
				ResourceHandlerDX11(RenderWindowDX11* a_RenderWindow);
				RenderWindowDX11* m_RenderWindow;
				friend class ResourceHandler;

				typedef std::map<Key, std::unique_ptr<Texture2D>> Texture2DMap;
				typedef std::map<Key, std::unique_ptr<Texture1D>> Texture1DMap;
				typedef std::map<Key, std::unique_ptr<Texture3D>> Texture3DMap;
				typedef std::map<Key, std::unique_ptr<Mesh>> MeshMap;
				typedef std::map<Key, Mesh*> MeshMapSimple;
				typedef std::map<Key, std::unique_ptr<Material>> MaterialMap;
				typedef std::map<Key, std::unique_ptr<Shader>> ShaderMap;
				typedef std::map<Key, Microsoft::WRL::ComPtr<ID3D11InputLayout>> InputLayoutMap;
				typedef std::map<Key, std::unique_ptr<TextureSampler>> SamplerMap;
				MeshMap m_MeshMap;
				MeshMapSimple m_LoadedFilenameMeshes;
				MaterialMap m_MaterialMap;
				Texture2DMap m_Texture2DMap;
				Texture3DMap m_Texture3DMap;
				Texture1DMap m_Texture1DMap;
				ShaderMap m_ShaderMap;
				InputLayoutMap m_InputLayouts;
				SamplerMap m_SamplerMap;

#if LUX_THREAD_SAFE == TRUE
				std::mutex m_MeshMapMutex;
				std::mutex m_MaterialMapMutex;
				std::mutex m_TextureMapMutex;
				std::mutex m_ShaderMapMutex;
				std::mutex m_InputLayoutMutex;
#endif
				void AddSamplerToMap(const String& a_Str, TextureSampler* a_Sampler);
				void AddMeshToMap(const String& a_Str, Mesh* a_Ent);
				void AddFileNameToMap(const String& a_Str, Mesh* a_Ent);
				Mesh* GetLoadedMesh(const String& a_FileStr);
				void AddMaterialToMap(const String& a_Str, Material* a_Mat);
				void AddTexture2DToMap(const String& a_Str, Texture2D* a_Tex);
				void AddTexture1DToMap(const String& a_Str, Texture1D* a_Tex);
				void AddTexture3DToMap(const String& a_Str, Texture3D* a_Tex);
				void LoadAllTexturesOfTypeFromMaterial(aiMaterial* a_Mat, aiTextureType a_TexType);
				void AddShaderToMap(const String& a_Str, Shader* a_Shader);
				void AddInputLayoutToMap(const String& a_Str, ID3D11InputLayout* a_Layout);
				void LoadImageData(const String& a_Path, unsigned int& outWidth, unsigned int& outHeight, unsigned char* outData);
				void LoadImageData(FileInfo* a_File, unsigned int& outWidth, unsigned int& outHeight, unsigned char* outData);
				HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);
			};
		}
	}
}

#endif