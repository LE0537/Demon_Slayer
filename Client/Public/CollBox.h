#pragma once
#include "Client_Defines.h"
#include "GameObj.h"

BEGIN(Engine)

class CCollider;
END

BEGIN(Client)

class CCollBox : public CGameObj
{
protected:
	CCollBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollBox(const CCollBox& rhs);
	virtual ~CCollBox() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
public:
	CCollider*		Get_Collider() {return m_pOBBCom;}
protected:
	CCollider*				m_pOBBCom = nullptr;

public:
	static CCollBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
