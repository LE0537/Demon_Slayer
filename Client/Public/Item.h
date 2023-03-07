#pragma once

#include "Client_Defines.h"
#include "GameObj.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CItem final : public CGameObj
{
private:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem(const CItem& rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources();

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;

	CGameObj*				m_pPlayer = nullptr;
	CGameObj*				m_pTarget = nullptr;
	_int					m_iTextrueNum = 0;

	_float					m_fDeadTime = 0.f;
	_float					m_fRenderTime = 0.f;
	_bool					m_bRenderItem = false;
public:
	static CItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
