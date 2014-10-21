#include "LuxPCH.h"
#include "LuxMeshAnimation.h"

Lux::Core::MeshAnimation::NodeChannel::NodeChannel(String& a_Name, unsigned int a_NumPosKeys, unsigned int a_NumScaleKeys, unsigned int a_NumRotKeys) :
m_Name(a_Name),
m_NumPositionKeys(a_NumPosKeys),
m_NumRotationKeys(a_NumRotKeys),
m_NumScalingKeys(a_NumScaleKeys)
{
	m_PreState = ANIM_BEHAVIOUR_DEFAULT;
	m_PostState = ANIM_BEHAVIOUR_DEFAULT;

	m_PositionKeys = new VectorKey[m_NumPositionKeys];
	m_RotationKeys = new QuaternionKey[m_NumRotationKeys];
	m_ScalingKeys = new VectorKey[m_NumScalingKeys];
}

Lux::Core::MeshAnimation::NodeChannel::NodeChannel(aiNodeAnim& a_NodeAnim)
{
	m_NumPositionKeys = a_NodeAnim.mNumPositionKeys;
	m_NumRotationKeys = a_NodeAnim.mNumRotationKeys;
	m_NumScalingKeys = a_NodeAnim.mNumScalingKeys;
	m_PreState = ConvertAssimpAnimBehaviour(a_NodeAnim.mPreState);
	m_PostState = ConvertAssimpAnimBehaviour(a_NodeAnim.mPostState);
	m_Name = a_NodeAnim.mNodeName.C_Str();

	if (m_NumPositionKeys)
	{
		m_PositionKeys = new VectorKey[m_NumPositionKeys];

		for (unsigned int i = 0; i < m_NumPositionKeys; i++)
		{
			m_PositionKeys[i].Set(a_NodeAnim.mPositionKeys[i]);
		}
	}

	if (m_NumRotationKeys)
	{
		m_RotationKeys = new QuaternionKey[m_NumRotationKeys];

		for (unsigned int i = 0; i < m_NumRotationKeys; i++)
		{
			m_RotationKeys[i].Set(a_NodeAnim.mRotationKeys[i]);
		}
	}

	if (m_NumScalingKeys)
	{
		m_ScalingKeys = new VectorKey[m_NumScalingKeys];

		for (unsigned int i = 0; i < m_NumScalingKeys; i++)
		{
			m_ScalingKeys[i].Set(a_NodeAnim.mScalingKeys[i]);
		}
	}
}

Lux::Core::MeshAnimation::NodeChannel::NodeChannel(const NodeChannel& a_Channel)
{
	m_NumPositionKeys = a_Channel.m_NumPositionKeys;
	m_NumRotationKeys = a_Channel.m_NumRotationKeys;
	m_NumScalingKeys = a_Channel.m_NumScalingKeys;
	m_PreState = a_Channel.m_PreState;
	m_PostState = a_Channel.m_PostState;
	m_Name = a_Channel.m_Name;

	if (m_NumPositionKeys)
	{
		m_PositionKeys = new VectorKey[m_NumPositionKeys];

		for (unsigned int i = 0; i < m_NumPositionKeys; i++)
		{
			m_PositionKeys[i].Set(a_Channel.m_PositionKeys[i]);
		}
	}

	if (m_NumRotationKeys)
	{
		m_RotationKeys = new QuaternionKey[m_NumRotationKeys];

		for (unsigned int i = 0; i < m_NumRotationKeys; i++)
		{
			m_RotationKeys[i].Set(a_Channel.m_RotationKeys[i]);
		}
	}
	
	if (m_NumScalingKeys)
	{
		m_ScalingKeys = new VectorKey[m_NumScalingKeys];

		for (unsigned int i = 0; i < m_NumScalingKeys; i++)
		{
			m_ScalingKeys[i].Set(a_Channel.m_ScalingKeys[i]);
		}
	}
}

Lux::Core::MeshAnimation::NodeChannel::~NodeChannel()
{
	Utility::SafeArrayDelete(m_PositionKeys);
	Utility::SafeArrayDelete(m_RotationKeys);
	Utility::SafeArrayDelete(m_ScalingKeys);
}

Lux::Core::MeshAnimation::AnimBehaviour Lux::Core::MeshAnimation::NodeChannel::ConvertAssimpAnimBehaviour(aiAnimBehaviour b)
{
	AnimBehaviour ret;
	switch (b)
	{
	case aiAnimBehaviour_CONSTANT:
		ret = ANIM_BEHAVIOUR_CONSTANT;
		break;

	case aiAnimBehaviour_DEFAULT:
		ret = ANIM_BEHAVIOUR_DEFAULT;
		break;

	case aiAnimBehaviour_LINEAR:
		ret = ANIM_BEHAVIOUR_LINEAR;
		break;

	case aiAnimBehaviour_REPEAT:
		ret = ANIM_BEHAVIOUR_REPEAT;
		break;

	default:
		LUX_LOG(Utility::logWARNING) << "Unrecognized Animation state when converting.";
		break;
	}
	return ret;
}

Lux::Core::MeshAnimation::MeshChannel::MeshChannel(unsigned int a_NumMeshKeys, String& a_Name) : m_Name(a_Name), m_NumKeys(a_NumMeshKeys)
{
	m_MeshKeys = new MeshKey[m_NumKeys];
}

Lux::Core::MeshAnimation::MeshChannel::MeshChannel(aiMeshAnim& a_MeshAnim)
{
	m_Name = a_MeshAnim.mName.C_Str();
	m_NumKeys = a_MeshAnim.mNumKeys;
	m_MeshKeys = new MeshKey[m_NumKeys];
	for (unsigned int i = 0; i < m_NumKeys; i++)
	{
		m_MeshKeys[i].Set(a_MeshAnim.mKeys[i]);
	}
}

Lux::Core::MeshAnimation::MeshChannel::MeshChannel(const MeshChannel& a_Channel)
{
	m_Name = a_Channel.m_Name;
	m_NumKeys = a_Channel.m_NumKeys;
	m_MeshKeys = new MeshKey[m_NumKeys];
	for (unsigned int i = 0; i < m_NumKeys; i++)
	{
		m_MeshKeys[i].Set(a_Channel.m_MeshKeys[i]);
	}
}

Lux::Core::MeshAnimation::MeshChannel::~MeshChannel()
{
	Utility::SafeArrayDelete(m_MeshKeys);
}

Lux::Core::MeshAnimation::VectorKey::VectorKey(aiVectorKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = Utility::ConvertVec3AssimpToGLM(a_Key.mValue);
}

Lux::Core::MeshAnimation::VectorKey::VectorKey() : m_Time(0.0), m_Value()
{

}

Lux::Core::MeshAnimation::VectorKey::~VectorKey()
{

}

void Lux::Core::MeshAnimation::VectorKey::Set(aiVectorKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = Utility::ConvertVec3AssimpToGLM(a_Key.mValue);
}

void Lux::Core::MeshAnimation::VectorKey::Set(const VectorKey& a_Key)
{
	m_Time = a_Key.m_Time;
	m_Value = a_Key.m_Value;
}

Lux::Core::MeshAnimation::QuaternionKey::QuaternionKey(aiQuatKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = Utility::ConvertQuatAssimpToGLM(a_Key.mValue);
}

Lux::Core::MeshAnimation::QuaternionKey::QuaternionKey() : m_Time(0.0), m_Value()
{

}

Lux::Core::MeshAnimation::QuaternionKey::~QuaternionKey()
{

}

void Lux::Core::MeshAnimation::QuaternionKey::Set(aiQuatKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = Utility::ConvertQuatAssimpToGLM(a_Key.mValue);
}

void Lux::Core::MeshAnimation::QuaternionKey::Set(const QuaternionKey& a_Key)
{
	m_Time = a_Key.m_Time;
	m_Value = a_Key.m_Value;
}

Lux::Core::MeshAnimation::MeshKey::MeshKey(aiMeshKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = a_Key.mValue;
}

Lux::Core::MeshAnimation::MeshKey::MeshKey() : m_Time(0.0), m_Value(0)
{

}

Lux::Core::MeshAnimation::MeshKey::~MeshKey()
{

}

void Lux::Core::MeshAnimation::MeshKey::Set(aiMeshKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = a_Key.mValue;
}

void Lux::Core::MeshAnimation::MeshKey::Set(const MeshKey& a_Key)
{
	m_Time = a_Key.m_Time;
	m_Value = a_Key.m_Value;
}

Lux::Core::MeshAnimation::MeshAnimation() :
m_MeshChannels(nullptr),
m_NodeChannels(nullptr),
m_NumMeshChannels(0),
m_NumNodeChannels(0),
m_Duration(-1.0),
m_TicksPerSecond(0.0)
{

}

Lux::Core::MeshAnimation::MeshAnimation(aiAnimation& a_Anim)
{
	m_Duration = a_Anim.mDuration;
	m_NumMeshChannels = a_Anim.mNumMeshChannels;
	m_NumNodeChannels = a_Anim.mNumChannels;
	m_TicksPerSecond = a_Anim.mTicksPerSecond;
	m_Name = a_Anim.mName.C_Str();

	if (m_NumMeshChannels)
	{
		m_MeshChannels = new MeshChannel*[m_NumMeshChannels];

		for (unsigned int i = 0; i < m_NumMeshChannels; i++)
		{
			m_MeshChannels[i] = new MeshChannel(*a_Anim.mMeshChannels[i]);
		}
	}

	if (m_NumNodeChannels)
	{
		m_NodeChannels = new NodeChannel*[m_NumNodeChannels];

		for (unsigned int i = 0; i < m_NumNodeChannels; i++)
		{
			m_NodeChannels[i] = new NodeChannel(*a_Anim.mChannels[i]);
		}
	}

}

Lux::Core::MeshAnimation::MeshAnimation(const MeshAnimation& a_Anim)
{
	m_Duration = a_Anim.m_Duration;
	m_NumMeshChannels = a_Anim.m_NumMeshChannels;
	m_NumNodeChannels = a_Anim.m_NumNodeChannels;
	m_Name = a_Anim.m_Name;
	m_TicksPerSecond = a_Anim.m_TicksPerSecond;
	
	if (m_NumMeshChannels)
	{
		m_MeshChannels = new MeshChannel*[m_NumMeshChannels];

		for (unsigned int i = 0; i < m_NumMeshChannels; i++)
		{
			m_MeshChannels[i] = new MeshChannel(*a_Anim.m_MeshChannels[i]);
		}
	}

	if (m_NumNodeChannels)
	{
		m_NodeChannels = new NodeChannel*[m_NumNodeChannels];

		for (unsigned int i = 0; i < m_NumNodeChannels; i++)
		{
			m_NodeChannels[i] = new NodeChannel(*a_Anim.m_NodeChannels[i]);
		}
	}
}

Lux::Core::MeshAnimation::~MeshAnimation()
{
	if (m_NodeChannels && m_NumNodeChannels)
	{
		for (unsigned int i = 0; i < m_NumNodeChannels; i++)
		{
			Utility::SafePtrDelete(m_NodeChannels[i]);
		}
		Utility::SafeArrayDelete(m_NodeChannels);
	}

	if (m_MeshChannels && m_NumMeshChannels)
	{
		for (unsigned int i = 0; i < m_NumMeshChannels; i++)
		{
			Utility::SafePtrDelete(m_MeshChannels[i]);
		}
		Utility::SafeArrayDelete(m_MeshChannels);
	}
}
