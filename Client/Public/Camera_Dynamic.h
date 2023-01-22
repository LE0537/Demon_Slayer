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
	void	ConvertToViewPort();
	void	Set_BattleTarget();
public:
	void Set_Player(CCharacters* _pPlayer) { m_pPlayer = _pPlayer; }
	void Set_Target(CCharacters* _pTarget) { m_pTarget = _pTarget; }
private:
	CCharacters*				m_pPlayer = nullptr;
	CCharacters*				m_pTarget = nullptr;

	_float4					m_vPlayerPos, m_vTargetPos;
	_float4					m_vPoint;
	_float					m_f1pX = 0.f;
	_float					m_f2pX = 0.f;
	_float					m_fAngle = 0.f;
	_float4					m_v1pCamAt, m_v2pCamAt;
	_bool					m_bBattle = false;
private:/* For.Check_Clockwise */
	_bool			m_bClockwise = false;

public:
	static CCamera_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END