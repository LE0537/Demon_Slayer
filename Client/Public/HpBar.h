#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CHpBar final : public CUI
{
private:
	CHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHpBar(const CHpBar& rhs);
	virtual ~CHpBar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */
	
private:
	_bool			m_bHpMinusCheck = false;
	_bool			m_bBeforeCheck = false;
	_float			m_fCurHp = 0.f;
	_float			m_fMaxHp = 0.f;
	_float			m_fHpTime = 0.f;
	_float			m_fMinusHp = 0.f;
	_float			m_fMinus_BeforeHp = 0.f;
public:
	static CHpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END