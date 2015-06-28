#ifndef LUX_RESOURCE_HANDLER_OGL_H
#define LUX_RESOURCE_HANDLER_OGL_H

namespace Lux
{
	namespace Core
	{
		class MaterialResource;
		class SubMesh;
		class Mesh;
		class Key;
		class Texture;
		class Texture1D;
		class Texture2D;
		class Texture3D;
		class TextureSampler;
		struct FileInfo;
		class PhysicsMaterial;

		namespace Internal
		{
			class ResourceHandlerOGL: public ResourceHandler
			{
			public:
				virtual ~ResourceHandlerOGL();

				Mesh* CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags);
				Mesh* CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags);
				Texture2D* CreateTexture2DFromFile(const String& a_File, const String& a_TexName);
				Texture2D* CreateTexture2DFromMemory(FileInfo* a_Info, const String& a_TexName);
				Texture1D* CreateTexture1DFromFile(const String& a_File, const String& a_TexName);
				Texture1D* CreateTexture1DFromMemory(FileInfo* a_Info, const String& a_TexName);
				Texture3D* CreateTexture3DFromFile(const String& a_File, const String& a_TexName);
				Texture3D* CreateTexture3DFromMemory(FileInfo* a_Info, const String& a_TexName);
				Shader* CreateShaderFromFile(const String& a_File, const String& a_ShaderName);
				TextureSampler* CreateTextureSampler(const String& a_Name, TextureSamplerOptions& a_InitOptions);
				PhysicsMaterial* CreatePhysicsMaterial(const String& a_Name, float a_Restitution = 0.0f, float a_DynamicFriction = 0.0f, float a_StaticFriction = 0.0f);

				MaterialResource* CreateMaterial(const String& a_Name);
				Mesh* GetMesh(const String& a_Name);
				MaterialResource* GetMaterial(const String& a_Name);
				Texture2D* GetTexture2D(const String& a_Name);
				Texture3D* GetTexture3D(const String& a_Name);
				Texture1D* GetTexture1D(const String& a_Name);
				TextureSampler* GetTextureSampler(const String& a_Name);
				PhysicsMaterial* GetPhysicsMaterial(const String& a_Name);
				bool MaterialExists(const String& a_Name);
				bool MeshExists(const String& a_Name);
				bool TextureSamplerExists(const String& a_Name);
				bool Texture2DExists(const String& a_Name);
				bool Texture1DExists(const String& a_Name);
				bool Texture3DExists(const String& a_Name);
				bool PhysicsMaterialExists(const String& a_Name);
				bool DeleteTexture2D(const String& a_Name);
				bool DeleteTexture1D(const String& a_Name);
				bool DeleteTexture3D(const String& a_Name);
				bool ShaderExists(const String& a_Name);
				Shader* GetShader(const String& a_Name);
				bool DeleteTextureSampler(const String& a_Name);
				bool DeletePhysicsMaterial(const String& a_Name);

			protected:
				friend class ResourceHandler;
				ResourceHandlerOGL();
				ResourceHandlerOGL(ResourceHandlerOGL const&);// Don't Implement
				void operator=(ResourceHandlerOGL const&);// Don't implement

				typedef std::map<Key, std::unique_ptr<Texture2D>> Texture2DMap;
				typedef std::map<Key, std::unique_ptr<Texture1D>> Texture1DMap;
				typedef std::map<Key, std::unique_ptr<Texture3D>> Texture3DMap;
				typedef std::map<Key, std::unique_ptr<Mesh>> MeshMap;
				typedef std::map<Key, Mesh*> MeshMapSimple;
				typedef std::map<Key, std::unique_ptr<MaterialResource>> MaterialMap;
				typedef std::map<Key, std::unique_ptr<Shader>> ShaderMap;
				typedef std::map<Key, std::unique_ptr<TextureSampler>> SamplerMap;
				typedef std::map<Key, std::unique_ptr<PhysicsMaterial>> PhysicsMaterialMap;
				MeshMap m_MeshMap;
				MeshMapSimple m_LoadedFilenameMeshes;
				MaterialMap m_MaterialMap;
				Texture2DMap m_Texture2DMap;
				Texture3DMap m_Texture3DMap;
				Texture1DMap m_Texture1DMap;
				ShaderMap m_ShaderMap;
				SamplerMap m_SamplerMap;
				PhysicsMaterialMap m_PhysicsMaterialMap;

				void AddFileNameToMap(const String& a_Str, Mesh* a_Ent);
				Mesh* GetLoadedMesh(const String& a_FileStr);
				void LoadAllTexturesOfTypeFromMaterial(aiMaterial* a_Mat, aiTextureType a_TexType);
				unsigned int LoadOGLShader(GLenum a_ShaderType, FileInfo* a_FileInfo);

				template<class T>
				void AddResourceToMap(const String& a_Str, T* a_Resource, std::map<Key, std::unique_ptr<T>>& a_Map)
				{
					a_Map.insert(std::make_pair(Key(a_Str), std::unique_ptr<T>(a_Resource)));
				}

				template<class T>
				bool ResourceExists(const String& a_Str, std::map<Key, std::unique_ptr<T>>& a_Map)
				{
					Key k(a_Str);
					unsigned int count = a_Map.count(k);

					if (count > 0)
					{
						return true;
					}

					return false;
				}

				template<class T>
				T* GetResource(const String& a_Name, std::map<Key, std::unique_ptr<T>>& a_Map)
				{
					return a_Map.at(Key(a_Name)).get();
				}

				template <class T>
				bool DeleteResource(const String& a_Name, std::map<Key, std::unique_ptr<T>>& a_Map)
				{
					if (!ResourceExists(a_Name, a_Map))
					{
						LUX_LOG(Utility::logWARNING) << "Could not delete resource with name: " << a_Name << ". Resource doesn't exist.";
						return false;
					}
					a_Map.at(Key(a_Name)).reset();
					a_Map.erase(Key(a_Name));

					return true;
				}

				template <class T>
				void DeleteMap(std::map<Key, std::unique_ptr<T>>& a_Map)
				{
					std::map<Key, std::unique_ptr<T>>::iterator it;

					for (it = a_Map.begin(); it != a_Map.end(); ++it)
					{
						it->second.reset();
					}
					a_Map.clear();
				}

				void LoadImageData(const String& a_Path, unsigned int& outWidth, unsigned int& outHeight, unsigned char* outData);
				void LoadImageData(FileInfo* a_File, unsigned int& outWidth, unsigned int& outHeight, unsigned char* outData);
			};
		}
	}
}

#endif