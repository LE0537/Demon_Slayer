#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CComboHitDeco final : public CUI
{
private:
	CComboHitDeco(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComboHitDeco(const CComboHitDeco& rhs);
	virtual ~CComboHitDeco() = default;

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
	CTexture*						m_pTextureCom1 = nullptr;
	CVIBuffer_Rect*					m_pVIBufferCom1 = nullptr;
	class CComboNum*				m_pComboNum = nullptr;

private:
	_float							m_fHitTime = 0.f;

public:
	static CComboHitDeco* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
