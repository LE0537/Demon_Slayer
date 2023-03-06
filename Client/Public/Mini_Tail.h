#pragma once

#include "Client_Defines.h"
#include "GameObj.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CMini_Tail final : public CGameObj
{
private:
	CMini_Tail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMini_Tail(const CMini_Tail& rhs);
	virtual ~CMini_Tail() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void  Set_Target(CGameObj* _pTarget) { m_pTarget = _pTarget; }
private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources();

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;

	CGameObj*				m_pTarget = nullptr;
	_int					m_iTextrueNum = 0;
public:
	static CMini_Tail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
