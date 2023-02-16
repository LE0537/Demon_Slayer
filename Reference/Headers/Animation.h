#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
	typedef struct FrameDesc {
		_float	fStartTime;
		_float	fEndTime;
		_float	fFrameTime = 1.f;
	} FRAMEDESC;

private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(class CModel* pModel, aiAnimation* pAIAnimation);
	HRESULT Bin_Initialize(DATA_BINANIM* pAIAnimation, class CModel* pModel); // �߰�
	void Invalidate_TransformationMatrix(_float fTimeDelta);
	void Invalidate_TransformationMatrix2(_float fTimeDelta, vector<class CChannel*> _Channel);
	void Invalidate_TransformationMatrix3(_float fTimeDelta);
	_bool	Get_AnimEnd() { return m_bAnimEnd; }
	_bool	Get_End() { return m_bEnd; }
	void	Set_AnimEnd() { m_bAnimEnd = false; }
	void	Set_End() { m_bEnd = false; }
	void	Set_Loop(_bool bIsLoop = false) { m_isLoop = bIsLoop; }

	void    Set_LinearTime(_float flinearTime) { m_flinearTime = flinearTime; }
	void	Clear_Frame()
	{
		for (auto& iter : m_vecFrames)
			Safe_Delete(iter);

		m_vecFrames.clear();
	};
	vector<class CChannel*> Get_Channel() { return m_Channels; }
	void	Reset2();
	void	Reset3();
private:
	char				m_szName[MAX_PATH] = "";

	/* ���� �ִϸ��̼� �ϳ��� ����ϴµ� �ɸ��� �� �ð�. */
	_float				m_fDuration = 0.f;

	/* 1�ʿ� ����Ǿ���� �ִϸ��̼��� �ӵ�. */
	_float				m_fTickPerSecond = 0.f;
	_float				m_fCurrentTime = 0.f;
	_float				m_fCurrentTime2 = 0.f;

	_float				m_flinearTime = 0.2f;
	/* Channel : ��. */

	/* �� �ִϸ��̼��� �����ϴµ� �ʿ��� ���� ����. */
	_uint							m_iNumChannels = 0;

	vector<class CChannel*>			m_Channels;
	
	_bool							m_isFinished = false;
	_bool							m_bAnimEnd = false;

	_bool							m_isLoop = true;
	_bool							m_bEnd = false;
public: // For. Data �߰�
	void Get_AnimData(DATA_BINANIM* pAnimData);




public: // ���� �߰� Ű������ ��������
	_bool Is_KeyFrame(char* pChannelName, _uint iKeyFrame);
	_float Get_Duration() { return m_fDuration; }
	_float Get_CurrentTime() {return m_fCurrentTime;}
	void Set_CurrentTime2(_float _fTime) { m_fCurrentTime = _fTime; }
	_uint Get_CurrentFrame() { return m_iCurrentFrame; }
	_uint Get_AllFrame() { return m_iFrameNum; }
	void Set_CurrentTime(_float fCurrentTime) { m_fCurrentTime = fCurrentTime; }
	void Set_Duration(_float fDurationTime) { m_fDuration = fDurationTime; }
	char* Get_AnimName() { return m_szName; }
	//Test
	HRESULT	Set_FrameNum(_uint iFrameNum);
	void	Set_FrameTime(_uint iStartFrame, _uint iEndFrame, _float fFrameTime);
	void	Set_UsingFrame(_uint iStartFrame, _uint iEndFrame);
	void	Set_LinearStartTime();
private:
	//Frame���� ���� TEst
	_uint							m_iFrameNum;
	_uint							m_iCurrentFrame = 0;
	FRAMEDESC*						m_pFrames = nullptr;
	_float							m_fTestTime = 1.f;
	_uint							m_iStartFrame = 0;
	_uint							m_iEndFrame = 0;

	vector<FRAMEDESC*>				m_vecFrames;

public:
	static CAnimation* Create(class CModel* pModel, aiAnimation* pAIAnimation);
	static CAnimation* Bin_Create(DATA_BINANIM* pAIAnimation, class CModel* pModel); // �߰�
	virtual void Free() override;
};

END