#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CInkEff final : public CUI
{
private:
	CInkEff(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInkEff(const CInkEff& rhs);
	virtual ~CInkEff() = default;

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
	_bool				m_bDownCheck = false;
	_uint				m_iFrame = 0;
	_uint				m_iNumTextureU = 0;
	_uint				m_iNumTextureV = 0;
	_float				m_fSpriteTime = 0.f;				

public:
	static CInkEff* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END