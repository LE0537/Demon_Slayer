#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CSkillBar final : public CUI
{
private:
	CSkillBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkillBar(const CSkillBar& rhs);
	virtual ~CSkillBar() = default;

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
	_uint				m_iSkillMaxBar = 0;
	_uint				m_iSkillCurBar = 0;
	_float				m_fSkillTime = 0.f;
	_bool				m_bSKillCheck = false;

public:
	static CSkillBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END