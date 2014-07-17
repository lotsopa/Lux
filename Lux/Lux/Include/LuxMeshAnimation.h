#ifndef LUX_ENTITY_ANIMATION_H
#define LUX_ENTITY_ANIMATION_H

namespace Lux
{
	class MeshAnimation
	{
	public:

		enum AnimBehaviour
		{
			ANIM_BEHAVIOUR_DEFAULT = 0x0,
			ANIM_BEHAVIOUR_CONSTANT = 0x1,
			ANIM_BEHAVIOUR_LINEAR = 0x2,
			ANIM_BEHAVIOUR_REPEAT = 0x3,
			ANIM_FORECE_32_BIT = INT_MAX
		};

		struct VectorKey
		{
			VectorKey();;
			VectorKey(aiVectorKey& a_Key);
			void Set(aiVectorKey& a_Key);
			~VectorKey();
			double m_Time;
			vec3 m_Value;
		};

		struct QuaternionKey
		{
			QuaternionKey();;
			QuaternionKey(aiQuatKey& a_Key);
			void Set(aiQuatKey& a_Key);
			~QuaternionKey();
			double m_Time;
			quat m_Value;
		};

		struct MeshKey
		{
			MeshKey();;
			MeshKey(aiMeshKey& a_Key);
			void Set(aiMeshKey& a_Key);
			~MeshKey();
			double m_Time;
			unsigned int m_Value;
		};

		struct NodeChannel
		{
			NodeChannel(String& a_Name, unsigned int a_NumPosKeys, unsigned int a_NumScaleKeys, unsigned int a_NumRotKeys);
			NodeChannel(aiNodeAnim& a_NodeAnim);
			~NodeChannel();

			String m_Name;
			unsigned int m_NumPositionKeys;
			VectorKey* m_PositionKeys;
			unsigned int m_NumRotationKeys;
			QuaternionKey* m_RotationKeys;
			unsigned int m_NumScalingKeys;
			VectorKey* m_ScalingKeys;
			AnimBehaviour m_PreState;
			AnimBehaviour m_PostState;

			static AnimBehaviour ConvertAssimpAnimBehaviour(aiAnimBehaviour b);
		};

		struct MeshChannel
		{
			MeshChannel(unsigned int a_NumMeshKeys, String& a_Name);
			MeshChannel(aiMeshAnim& a_MeshAnim);
			~MeshChannel();

			String m_Name;
			unsigned int m_NumKeys;
			MeshKey* m_MeshKeys;

		};

		MeshAnimation();
		MeshAnimation(aiAnimation& a_Anim);
		~MeshAnimation();

	private:
		String m_Name;
		double m_Duration;
		double m_TicksPerSecond;
		unsigned int m_NumNodeChannels;
		NodeChannel** m_NodeChannels;
		unsigned int m_NumMeshChannels;
		MeshChannel** m_MeshChannels;

	};
}

#endif