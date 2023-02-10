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
private:
	CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Dynamic(const CCamera_Dynamic& rhs);
	virtual ~CCamera_Dynamic() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

private:
	CTransform*			m_pSubTransform = nullptr;

private:
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
	void Set_CamType(_bool _bStory) { m_bStory = _bStory; }
	void	Key_Input(_float fTimeDelta);
private:
	//Shake
	void Check_Shake(_float fTimeDelta);
	void Camera_ShakeDown(_float fTimeDelta);
	void Camera_ShakeHit(_float fTimeDelta);
private:
	CCharacters*				m_pPlayer = nullptr;
	CCharacters*				m_pTarget = nullptr;

	_float4					m_vPlayerPos, m_vTargetPos;
	_float					m_f1pX, m_f1pY = 0.f;
	_float					m_f2pX, m_f2pY = 0.f;
	_float					m_fAngle = 0.f;
	_bool					m_bBattle = false;
	_float					m_fCamDist = 0.f;
	_float					m_fLookY = 0.f;
	CCharacters*			m_p1P = nullptr;
	_bool					m_b1P = false;
	_int					m_iIniti = 0;
	_float					m_fFov = 0.f;
	_float					m_fFovTime = 0.f;
	_float					m_fLookAtY = 0.f;
	_bool					m_bStartBattle = false;
	_bool					m_bEffect = false;
	//Start
	_bool  m_bStart = false;
	_bool  m_bLerp = false;
	_float m_fStartTime = 0.f;
	_float m_fLerpTime = 0.f;
	_float4 m_vCamPos, m_vLerpLook;
	
	//shake 
	_float m_fShakeAmount = 0.f;
	_float m_fShakeFrequency = 0.f;
	_float m_ShakeTime = 0.f;
	SHAKE  m_ShakeInfo;
	//CamType
	_bool m_bStory = false;
	_float		 m_FovAngle = 0.f;
private:/* For.Check_Clockwise */
	_bool			m_bClockwise = false;

public:
	static CCamera_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END