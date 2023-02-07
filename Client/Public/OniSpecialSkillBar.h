#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class COniSpecialSkillBar final : public CUI
{
private:
	COniSpecialSkillBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	COniSpecialSkillBar(const COniSpecialSkillBar& rhs);
	virtual ~COniSpecialSkillBar() = default;

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
	CTexture*					m_pTextureCom1 = nullptr;

private:
	_float			m_fSpecialSkillBar = 0.f;
	_float			m_fSpecialSkillMaxBar = 0.f;
	_bool			m_bCurPerBarCheck = false;

public:
	static COniSpecialSkillBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
