#include "..\Public\Channel.h"
#include "Model.h"
#include "HierarchyNode.h"


CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(CModel* pModel, aiNodeAnim * pAIChannel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	m_iNumKeyframes = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyframes = max(m_iNumKeyframes, pAIChannel->mNumPositionKeys);

	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		KEYFRAME				Keyframe;
		ZeroMemory(&Keyframe, sizeof(KEYFRAME));

		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			Keyframe.fTime = (_float)pAIChannel->mScalingKeys[i].mTime;
		}

		if (i < pAIChannel->mNumRotationKeys)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			Keyframe.fTime = (_float)pAIChannel->mRotationKeys[i].mTime;
		}

		if (i < pAIChannel->mNumPositionKeys)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			Keyframe.fTime = (_float)pAIChannel->mPositionKeys[i].mTime;
		}

		Keyframe.vScale = vScale;
		Keyframe.vRotation = vRotation;
		Keyframe.vPosition = vPosition;

		m_KeyFrames.push_back(Keyframe);
	}

	m_pBoneNode = pModel->Get_BonePtr(m_szName);
	Safe_AddRef(m_pBoneNode);

	return S_OK;
}

void CChannel::Invalidate_TransformationMatrix(_float fCurrentTime)
{
	_vector			vScale, vRotation, vPosition;

	KEYFRAME		LastKeyframe = m_KeyFrames.back();

	if (fCurrentTime >= LastKeyframe.fTime)
	{
		vScale = XMLoadFloat3(&LastKeyframe.vScale);
		vRotation = XMLoadFloat4(&LastKeyframe.vRotation);
		vPosition = XMLoadFloat3(&LastKeyframe.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}
	else
	{
		while (fCurrentTime >= m_KeyFrames[m_iCurrentKeyFrameIndex + 1].fTime)
			++m_iCurrentKeyFrameIndex;

		_vector		vSourScale, vSourRotation, vSourPosition;
		_vector		vDestScale, vDestRotation, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vScale);
		vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrameIndex].vRotation);
		vSourPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vPosition);

		vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vPosition);

		_float		fRatio = (fCurrentTime - m_KeyFrames[m_iCurrentKeyFrameIndex].fTime) /
			(m_KeyFrames[m_iCurrentKeyFrameIndex + 1].fTime - m_KeyFrames[m_iCurrentKeyFrameIndex].fTime);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBoneNode->Set_TransformationMatrix(TransformationMatrix);
	//m_LinearKeyframe
}

void CChannel::Invalidate_TransformationMatrix2(_float fCurrentTime, KEYFRAME _key)
{
	_vector			vScale, vRotation, vPosition;

	_vector		vSourScale, vSourRotation, vSourPosition;
	_vector		vDestScale, vDestRotation, vDestPosition;

	vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vScale);
	vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrameIndex].vRotation);
	vSourPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vPosition);

	vDestScale = XMLoadFloat3(&_key.vScale);
	vDestRotation = XMLoadFloat4(&_key.vRotation);
	vDestPosition = XMLoadFloat3(&_key.vPosition);

	_float		fRatio = fCurrentTime / 0.2f;

	vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
	vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
	vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
	vPosition = XMVectorSetW(vPosition, 1.f);

	//linearkeyframe = 
	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBoneNode->Set_TransformationMatrix(TransformationMatrix);

}

void CChannel::Reset()
{
	m_iCurrentKeyFrameIndex = 0;
}

CChannel * CChannel::Create(CModel* pModel, aiNodeAnim * pAIChannel)
{
	CChannel*	pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pModel, pAIChannel)))
	{
		ERR_MSG(TEXT("Failed to Created : CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
HRESULT CChannel::Bin_Initialize(DATA_BINCHANNEL * pAIChannel, CModel* pModel)
{
	strcpy_s(m_szName, pAIChannel->szName);

	m_iNumKeyframes = pAIChannel->iNumKeyFrames;

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		KEYFRAME			KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		KeyFrame = pAIChannel->pKeyFrames[i];

		m_KeyFrames.push_back(KeyFrame);
	}

	m_pBoneNode = pModel->Get_BonePtr(m_szName);
	Safe_AddRef(m_pBoneNode);

	return S_OK;
}
void CChannel::Get_ChannelData(DATA_BINCHANNEL * pChannelData)
{
	memcpy(&pChannelData->szName, m_szName, sizeof(char)*MAX_PATH);
	pChannelData->iNumKeyFrames = m_iNumKeyframes;

	pChannelData->pKeyFrames = new KEYFRAME[m_iNumKeyframes];

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		memcpy(&pChannelData->pKeyFrames[i], &m_KeyFrames[i], sizeof(KEYFRAME));
	}
}
CChannel * CChannel::Bin_Create(DATA_BINCHANNEL * pAIChannel, CModel* pModel)
{
	CChannel*			pInstance = new CChannel();

	if (FAILED(pInstance->Bin_Initialize(pAIChannel, pModel)))
	{
		ERR_MSG(TEXT("Failed To Created : CChannel_Bin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CChannel::Free()
{
	Safe_Release(m_pBoneNode);
}