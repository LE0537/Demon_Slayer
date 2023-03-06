#pragma once

#include "Client_Defines.h"
#include "GameObj.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CMini_Monster final : public CGameObj
{
private:
	CMini_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMini_Monster(const CMini_Monster& rhs);
	virtual ~CMini_Monster() = default;

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
	_bool					m_bMonsterDead = false;
public:
	static CMini_Monster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
