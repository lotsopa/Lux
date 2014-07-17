#include "LuxPCH.h"
#include "LuxMeshAnimation.h"

Lux::MeshAnimation::NodeChannel::NodeChannel(String& a_Name, unsigned int a_NumPosKeys, unsigned int a_NumScaleKeys, unsigned int a_NumRotKeys) :
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

Lux::MeshAnimation::NodeChannel::NodeChannel(aiNodeAnim& a_NodeAnim)
{
	m_NumPositionKeys = a_NodeAnim.mNumPositionKeys;
	m_NumRotationKeys = a_NodeAnim.mNumRotationKeys;
	m_NumScalingKeys = a_NodeAnim.mNumScalingKeys;
	m_PreState = ConvertAssimpAnimBehaviour(a_NodeAnim.mPreState);
	m_PostState = ConvertAssimpAnimBehaviour(a_NodeAnim.mPostState);
	m_Name = a_NodeAnim.mNodeName.C_Str();

	m_PositionKeys = new VectorKey[m_NumPositionKeys];
	m_RotationKeys = new QuaternionKey[m_NumRotationKeys];
	m_ScalingKeys = new VectorKey[m_NumScalingKeys];

	for (unsigned int i = 0; i < m_NumPositionKeys; i++)
	{
		m_PositionKeys[i].Set(a_NodeAnim.mPositionKeys[i]);
	}

	for (unsigned int i = 0; i < m_NumRotationKeys; i++)
	{
		m_RotationKeys[i].Set(a_NodeAnim.mRotationKeys[i]);
	}

	for (unsigned int i = 0; i < m_NumScalingKeys; i++)
	{
		m_ScalingKeys[i].Set(a_NodeAnim.mScalingKeys[i]);
	}
}

Lux::MeshAnimation::NodeChannel::~NodeChannel()
{
	SafeArrayDelete(m_PositionKeys);
	SafeArrayDelete(m_RotationKeys);
	SafeArrayDelete(m_ScalingKeys);
}

Lux::MeshAnimation::AnimBehaviour Lux::MeshAnimation::NodeChannel::ConvertAssimpAnimBehaviour(aiAnimBehaviour b)
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
		LUX_LOG(logWARNING) << "Unrecognized Animation state when converting.";
		break;
	}
	return ret;
}

Lux::MeshAnimation::MeshChannel::MeshChannel(unsigned int a_NumMeshKeys, String& a_Name) : m_Name(a_Name), m_NumKeys(a_NumMeshKeys)
{
	m_MeshKeys = new MeshKey[m_NumKeys];
}

Lux::MeshAnimation::MeshChannel::MeshChannel(aiMeshAnim& a_MeshAnim)
{
	m_Name = a_MeshAnim.mName.C_Str();
	m_NumKeys = a_MeshAnim.mNumKeys;
	m_MeshKeys = new MeshKey[m_NumKeys];
	for (unsigned int i = 0; i < m_NumKeys; i++)
	{
		m_MeshKeys[i].Set(a_MeshAnim.mKeys[i]);
	}
}

Lux::MeshAnimation::MeshChannel::~MeshChannel()
{
	SafeArrayDelete(m_MeshKeys);
}

Lux::MeshAnimation::VectorKey::VectorKey(aiVectorKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = ConvertVec3AssimpToGLM(a_Key.mValue);
}

Lux::MeshAnimation::VectorKey::VectorKey() : m_Time(0.0), m_Value()
{

}

Lux::MeshAnimation::VectorKey::~VectorKey()
{

}

void Lux::MeshAnimation::VectorKey::Set(aiVectorKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = ConvertVec3AssimpToGLM(a_Key.mValue);
}

Lux::MeshAnimation::QuaternionKey::QuaternionKey(aiQuatKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = ConvertQuatAssimpToGLM(a_Key.mValue);
}

Lux::MeshAnimation::QuaternionKey::QuaternionKey() : m_Time(0.0), m_Value()
{

}

Lux::MeshAnimation::QuaternionKey::~QuaternionKey()
{

}

void Lux::MeshAnimation::QuaternionKey::Set(aiQuatKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = ConvertQuatAssimpToGLM(a_Key.mValue);
}

Lux::MeshAnimation::MeshKey::MeshKey(aiMeshKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = a_Key.mValue;
}

Lux::MeshAnimation::MeshKey::MeshKey() : m_Time(0.0), m_Value(0)
{

}

Lux::MeshAnimation::MeshKey::~MeshKey()
{

}

void Lux::MeshAnimation::MeshKey::Set(aiMeshKey& a_Key)
{
	m_Time = a_Key.mTime;
	m_Value = a_Key.mValue;
}

Lux::MeshAnimation::MeshAnimation() :
m_MeshChannels(nullptr),
m_NodeChannels(nullptr),
m_NumMeshChannels(0),
m_NumNodeChannels(0),
m_Duration(-1.0),
m_TicksPerSecond(0.0)
{

}

Lux::MeshAnimation::MeshAnimation(aiAnimation& a_Anim)
{
	m_Duration = a_Anim.mDuration;
	m_NumMeshChannels = a_Anim.mNumMeshChannels;
	m_NumNodeChannels = a_Anim.mNumChannels;
	m_TicksPerSecond = a_Anim.mTicksPerSecond;
	m_Name = a_Anim.mName.C_Str();

	m_MeshChannels = new MeshChannel*[m_NumMeshChannels];
	m_NodeChannels = new NodeChannel*[m_NumNodeChannels];

	for (unsigned int i = 0; i < m_NumMeshChannels; i++)
	{
		m_MeshChannels[i] = new MeshChannel(*a_Anim.mMeshChannels[i]);
	}

	for (unsigned int i = 0; i < m_NumNodeChannels; i++)
	{
		m_NodeChannels[i] = new NodeChannel(*a_Anim.mChannels[i]);
	}
}

Lux::MeshAnimation::~MeshAnimation()
{
	if (m_NodeChannels && m_NumNodeChannels)
	{
		for (unsigned int i = 0; i < m_NumNodeChannels; i++)
		{
			SafePtrDelete(m_NodeChannels[i]);
		}
		SafeArrayDelete(m_NodeChannels);
	}

	if (m_MeshChannels && m_NumMeshChannels)
	{
		for (unsigned int i = 0; i < m_NumMeshChannels; i++)
		{
			SafePtrDelete(m_MeshChannels[i]);
		}
		SafeArrayDelete(m_MeshChannels);
	}
}
