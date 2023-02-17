#pragma once
#include "Client_Defines.h"
#include "CollBox.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CRuiDadJump : public CCollBox
{
protected:
	CRuiDadJump(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRuiDadJump(const CRuiDadJump& rhs);
	virtual ~CRuiDadJump() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
private:
	HRESULT Ready_Components();

public:
	static CRuiDadJump* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
