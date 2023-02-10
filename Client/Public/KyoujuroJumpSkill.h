#pragma once
#include "Client_Defines.h"
#include "CollBox.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CKyoujuroJumpSkill : public CCollBox
{
protected:
	CKyoujuroJumpSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKyoujuroJumpSkill(const CKyoujuroJumpSkill& rhs);
	virtual ~CKyoujuroJumpSkill() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

private:
	_float		   m_fDeadTime = 0.f;


private:
	HRESULT Ready_Components();

public:
	static CKyoujuroJumpSkill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
