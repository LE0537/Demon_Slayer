#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CFeedArrow final : public CUI
{
private:
	CFeedArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFeedArrow(const CFeedArrow& rhs);
	virtual ~CFeedArrow() = default;

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
	_uint						m_iMoveCount = 0;
	_bool						m_bMoveCheck = false;
	_float						m_fFadeTime = 0.f;


public:
	static CFeedArrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
