#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CNumTimer final : public CUI
{
private:
	CNumTimer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNumTimer(const CNumTimer& rhs);
	virtual ~CNumTimer() = default;

public:
	void Set_TimerStart() { m_bTimerOnoff = true; }
	void Set_TimerStop() { m_bTimerOnoff = false; }

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
	_float				m_fTimer = 100.f;
	_bool				m_bTimerOnoff = false;
	_uint				m_iFirstNum = 9;
	_uint				m_iSecondNum = 9;


public:
	static CNumTimer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END