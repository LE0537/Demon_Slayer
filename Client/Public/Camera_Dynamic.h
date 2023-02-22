#pragma once

#include "Client_Defines.h"
#include "Camera.h"
#include "Characters.h"

BEGIN(Engine)
END

BEGIN(Client)

class CCamera_Dynamic final : public CCamera
{
public:
	typedef struct tagCameraDesc_Derived
	{
		CCamera::CAMERADESC			CameraDesc;
	}CAMERADESC_DERIVED;
	enum SHAKE{ SHAKE_DOWN,SHAKE_HIT,SHAKE_END};
	enum ZOOM { ZOOM_LOW,ZOOM_MIDDLE ,ZOOM_HIGH, ZOOM_END };
	enum CAMTURN { CAM_RIGHT, CAM_LEFT, CAM_TARGETRIGHT, CAM_TARGETLEFT, CAM_END };
private:
	CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Dynamic(const CCamera_Dynamic& rhs);
	virtual ~CCamera_Dynamic() = default;

public:
	_bool 	Play_CutScene(vector<_float4> vecPositions, vector<_float4> vecLookAts, vector<_float> vecUseTime, _float* pOut, _float fTimeDelta);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

private:
	CTransform*			m_pSubTransform = nullptr;

private:
	_bool   Check_Level(_vector vPos);
	void	Move_TrainCam(_float fTimeDelta);
	void	Set_CamPos();
	void	Move_CamPos(_float fTimeDelta);
	void	Lerp_SubCam(_float fTimeDelta);
	void	ConvertToViewPort(_float fTimeDelta);
	void	Set_BattleTarget(_float fTimeDelta);
	_bool	CheckSubChar();
	void	Set_StartPos(_float fTimeDelta);
	void	Set_BattleStart(_float fTimeDelta);

public:
	void Set_Player(CCharacters* _pPlayer) { m_pPlayer = _pPlayer; }
	void Set_Target(CCharacters* _pTarget) { m_pTarget = _pTarget; }
	_bool	Get_1PCam() { return m_b1P; }
	void Set_Shake(SHAKE eShake, _float _fTime) { m_ShakeInfo = eShake, m_ShakeTime = _fTime; }
	void Set_Zoom(ZOOM eZoom) { m_ZoomInfo = eZoom, m_bZoom = true; }
	void Set_CamType(_bool _bStory) { m_bStory = _bStory; }
	void	Key_Input(_float fTimeDelta);
	//Blur
	void Blur_VeryLow(CRenderer* _pRenderer);
	void Blur_Low(CRenderer* _pRenderer);
	void Blur_High(CRenderer* _pRenderer);
	//Quest
	void Set_QusetCam() { m_bQuestCam = true; }
	_bool Get_QuestCam() { return m_bQuestCam; }
	void Set_QuestNPC(CCharacters* _pNPC) { m_pNPC = _pNPC; }
	_bool Get_QuestBattleCam() { return m_bQuestBattleCam; }
private:
	//Shake
	void Check_Shake(_float fTimeDelta);
	void Camera_ShakeDown(_float fTimeDelta);
	void Camera_ShakeHit(_float fTimeDelta);
	void Check_Model();
	//Zoom
	void Check_Zoom(_float fTimeDelta);
	void Zoom_Low(_float fTimeDelta);
	void Zoom_Middle(_float fTimeDelta);
	void Zoom_High(_float fTimeDelta);
	//Turn
	void Check_Trun(_float fTimeDelta);
	void Check_TargetTrun(_float fTimeDelta);
	//Quest
	void QuestCam(_float fTimeDelta);
	void QuestBattleCam(_float fTimeDelta);


	void Check_StoryCam();
private:
	CCharacters*				m_pPlayer = nullptr;
	CCharacters*				m_pTarget = nullptr;
	_float					m_fDist = 0.f;
	_float4					m_vPlayerPos, m_vTargetPos;
	_float					m_f1pX, m_f1pY = 0.f;
	_float					m_f2pX, m_f2pY = 0.f;
	_float					m_fAngle = 0.f;
	_bool					m_bBattle = false;
	_bool					m_bBattleSound = false;
	_float					m_fCamDist = 0.f;
	_float					m_fLookY = 0.f;
	CCharacters*			m_p1P = nullptr;
	CCharacters*			m_p2P = nullptr;
	_bool					m_b1P = false;
	_int					m_iIniti = 0;
	_float					m_fFov = 0.f;
	_float					m_fFovTime = 0.f;
	_float					m_fLookAtY = 0.f;
	_bool					m_bStartBattle = false;
	_bool					m_bEffect = false;
	_float3 				m_vAtPos;
	//Start
	_bool  m_bStart = false;
	_bool  m_bLerp = false;
	_float m_fStartTime = 0.f;
	_float m_fLerpTime = 0.f;
	_float4 m_vCamPos, m_vLerpLook;
	_int   m_iAnimIndex = 0;
	//shake 
	_float m_fShakeAmount = 0.f;
	_float m_fShakeFrequency = 0.f;
	_float m_ShakeTime = 0.f;
	SHAKE  m_ShakeInfo;
	//CamType
	_bool m_bStory = false;
	_float		 m_FovAngle = 0.f;
	_bool	m_bStoryBattle = false;
	//ZOOM
	_bool	m_bZoom = false;
	_bool	m_bZoomIn = false;
	ZOOM    m_ZoomInfo;
	_float  m_fZoomAngle = 0.f;
	//Turn
	_float fTurnAngle = 0.f;
	_float fAngleDot = 0.f;
	_bool  m_bTurn = false;
	_bool  m_bTargetTurn = false;
	_float m_fTurnCol = 0.f;
	CAMTURN m_eTurn = CAM_END;

	//Quest Cam
	_bool m_bQuestCam = false;
	CCharacters* m_pNPC = nullptr;
	_float m_fQuestAngle = 0.f;
	_float m_fQuestFov = 0.f;
	_float m_fCamY = 0.f;
	_bool  m_bCamTurn = false;
	_bool  m_bQuestBattleCam = false;
	_float m_fBattleAngle = 0.f;
	_float m_fBattleCamY = 0.f;
	_float m_fBattleCamZ = 0.f;

	//TrainCam

public:
	static CCamera_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END