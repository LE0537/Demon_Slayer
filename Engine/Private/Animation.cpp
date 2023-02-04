#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(CModel* pModel, aiAnimation * pAIAnimation)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_fDuration = (_float)pAIAnimation->mDuration;
	m_fTickPerSecond = (_float)pAIAnimation->mTicksPerSecond;

	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim*		pAIChannel = pAIAnimation->mChannels[i];

		CChannel*		pChannel = CChannel::Create(pModel, pAIAnimation->mChannels[i]);

		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Invalidate_TransformationMatrix(_float fTimeDelta)
{
	//
	if (m_pFrames != nullptr)
	{
		if (m_fCurrentTime >= m_pFrames[m_iCurrentFrame].fStartTime &&
			m_fCurrentTime < m_pFrames[m_iCurrentFrame].fEndTime)
		{
			m_fTestTime = m_pFrames[m_iCurrentFrame].fFrameTime;
		}
		else if (m_iCurrentFrame < m_iFrameNum && m_fCurrentTime > m_pFrames[m_iCurrentFrame].fEndTime)
		{
			++m_iCurrentFrame;
		}
	}

	/* 현재 재생중인 시간. */
	m_fCurrentTime += m_fTickPerSecond * fTimeDelta * m_fTestTime;

	if (m_iCurrentFrame > m_iEndFrame || m_fCurrentTime >= m_fDuration)
	{
		//m_pFrames 초기화 하기.
		m_iCurrentFrame = m_iStartFrame;

		m_fCurrentTime = 0.f;

		m_isFinished = true;
		m_bEnd = true;
	}
	if (!m_isLoop && m_bEnd)
		return;

	for (auto& pChannel : m_Channels)
	{
		if (true == m_isFinished && true == m_isLoop)
			pChannel->Reset();

		pChannel->Invalidate_TransformationMatrix(m_fCurrentTime);
	}

	if (true == m_isFinished && true == m_isLoop)
		m_isFinished = false;

}

void CAnimation::Invalidate_TransformationMatrix2(_float fTimeDelta, vector<class CChannel*> _Channel)
{
	/* 현재 재생중인 시간. */
	m_fCurrentTime2 += fTimeDelta;

	if (m_fCurrentTime2 >= m_flinearTime)
	{
		if (m_pFrames != nullptr)
		{
			m_fCurrentTime2 = m_pFrames[m_iStartFrame].fStartTime;
		}
		else
			m_fCurrentTime2 = 0.f;

		int a = 10;
		m_bAnimEnd = true;

		if (m_bAnimEnd)
			Reset2();

	}
	else
	{
		auto& Currentiter = m_Channels.begin();
		auto& Previter = _Channel.begin();
		for (_uint i = 0; i < m_Channels.size(); ++i)
		{
			Currentiter[i]->Invalidate_TransformationMatrix2(m_fCurrentTime2, Previter[i]->Get_Key()[0]);
		}
	}
	
}

void CAnimation::Invalidate_TransformationMatrix3(_float fTimeDelta)
{
	//
	if (m_pFrames != nullptr)
	{
		if (m_fCurrentTime >= m_pFrames[m_iCurrentFrame].fStartTime &&
			m_fCurrentTime < m_pFrames[m_iCurrentFrame].fEndTime)
		{
			m_fTestTime = m_pFrames[m_iCurrentFrame].fFrameTime;
		}
		else if (m_iCurrentFrame < m_iFrameNum && m_fCurrentTime > m_pFrames[m_iCurrentFrame].fEndTime)
		{
			++m_iCurrentFrame;
		}
	}

	/* 현재 재생중인 시간. */
	m_fCurrentTime += m_fTickPerSecond * fTimeDelta * m_fTestTime;

	if (m_iCurrentFrame > m_iEndFrame || m_fCurrentTime >= m_fDuration)
	{
		//m_pFrames 초기화 하기.
		m_iCurrentFrame = m_iStartFrame;

		m_fCurrentTime = 0.f;

		m_isFinished = true;
		m_bEnd = true;
	}
	if (!m_isLoop && m_bEnd)
		return;

	for (auto& pChannel : m_Channels)
	{
		

		pChannel->Invalidate_TransformationMatrix(m_fCurrentTime);
	}

	if (true == m_isFinished && true == m_isLoop)
		m_isFinished = false;
}

void CAnimation::Reset2()
{
	for (auto& pChannel : m_Channels)
	{
		if (!m_isLoop)
			break;

		pChannel->Reset();
	}
	m_fCurrentTime = 0.f;
}
void CAnimation::Reset3()
{
	for (auto& pChannel : m_Channels)
	{
		pChannel->Reset();
	}
	if (m_pFrames != nullptr)
	{
		m_fCurrentTime = m_pFrames[m_iStartFrame].fStartTime;
	}
	else
		m_fCurrentTime = 0.f;
}

_bool CAnimation::Is_KeyFrame(char * pChannelName, _uint iKeyFrame)
{
	auto iter = find_if(m_Channels.begin(), m_Channels.end(), [&](CChannel* pChannel)
	{
		return !strcmp(pChannelName, pChannel->Get_ChannelName());
	});

	if (iter == m_Channels.end())
		return false;
	else
	{
		if ((*iter)->Get_CurrentKeyframe() == iKeyFrame)
			return true;
		else
			return false;
	}
}


HRESULT CAnimation::Set_FrameNum(_uint iFrameNum)
{
	if (iFrameNum < 0)
		return E_FAIL;

	m_iFrameNum = iFrameNum;
	m_pFrames = new FRAMEDESC[iFrameNum];

	for (_uint i = 0; i < iFrameNum; ++i)
	{
		FRAMEDESC FrameDesc;
		FrameDesc.fStartTime = m_fDuration * i / iFrameNum;
		FrameDesc.fEndTime = m_fDuration * (i + 1) / iFrameNum;

		m_pFrames[i] = FrameDesc;
	}
	m_iStartFrame = 0;
	m_iEndFrame = iFrameNum - 1;

	m_vecFrames.push_back(m_pFrames);

	return S_OK;
}

void CAnimation::Set_FrameTime(_uint iStartFrame, _uint iEndFrame, _float fFrameTime)
{
	if (iStartFrame < 0 || iEndFrame > m_iFrameNum || iStartFrame > m_iFrameNum)
		return;

	for (_uint i = iStartFrame; i < iEndFrame; ++i)
	{
		m_pFrames[i].fFrameTime = fFrameTime;
	}

}

void CAnimation::Set_UsingFrame(_uint iStartFrame, _uint iEndFrame)
{
	m_iCurrentFrame = m_iStartFrame = iStartFrame;
	m_iEndFrame = iEndFrame;
	m_fCurrentTime = m_pFrames[m_iStartFrame].fStartTime;

	Set_LinearStartTime();
}

void CAnimation::Set_LinearStartTime()
{		/* 현재 재생중인 시간. */
	if (m_pFrames != nullptr)
	{
		m_fCurrentTime2 = m_pFrames[m_iStartFrame].fStartTime;
	}
}


CAnimation * CAnimation::Create(CModel* pModel, aiAnimation * pAIAnimation)
{
	CAnimation*	pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pModel, pAIAnimation)))
	{
		ERR_MSG(TEXT("Failed to Created : CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
HRESULT CAnimation::Bin_Initialize(DATA_BINANIM * pAIAnimation, CModel* pModel)
{
	strcpy_s(m_szName, pAIAnimation->szName);

	m_fDuration = pAIAnimation->fDuration;

	m_fTickPerSecond = pAIAnimation->fTickPerSecond;

	/* 현재 애니메이션에서 제어해야할 뼈들의 갯수를 저장한다. */
	m_iNumChannels = pAIAnimation->iNumChannels;

	m_isLoop = pAIAnimation->bLoop;

	/* 현재 애니메이션에서 제어해야할 뼈정보들을 생성하여 보관한다. */
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel*		pChannel = CChannel::Bin_Create(&pAIAnimation->pBinChannel[i], pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}
CAnimation * CAnimation::Bin_Create(DATA_BINANIM * pAIAnimation, CModel* pModel)
{
	CAnimation*			pInstance = new CAnimation();

	if (FAILED(pInstance->Bin_Initialize(pAIAnimation, pModel)))
	{
		ERR_MSG(TEXT("Failed To Created : CAnimation_Bin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CAnimation::Get_AnimData(DATA_BINANIM * pAnimData)
{
	pAnimData->fDuration = m_fDuration;
	pAnimData->iNumChannels = m_iNumChannels;
	pAnimData->fTickPerSecond = m_fTickPerSecond;
	pAnimData->bLoop = m_isLoop;
	strcpy_s(pAnimData->szName, m_szName);

	pAnimData->pBinChannel = new DATA_BINCHANNEL[m_iNumChannels];

	for (_int i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i]->Get_ChannelData(&pAnimData->pBinChannel[i]);
	}
}
void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	for (auto& iter : m_vecFrames)
		Safe_Delete(iter);

	m_vecFrames.clear();

	m_Channels.clear();
}