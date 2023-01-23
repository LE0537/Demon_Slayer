#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CMenuTitle final : public CUI
{
private:
	CMenuTitle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMenuTitle(const CMenuTitle& rhs);
	virtual ~CMenuTitle() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

private:
	CTexture*							m_pTextureCom1 = nullptr;
	CVIBuffer_Rect*						m_pVIBufferCom1 = nullptr;


public:
	static CMenuTitle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END