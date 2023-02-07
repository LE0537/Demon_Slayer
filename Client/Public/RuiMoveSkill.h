#pragma once
#include "Client_Defines.h"
#include "CollBox.h"
#include "Characters.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CRuiMoveSkill : public CCollBox
{

protected:
	CRuiMoveSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRuiMoveSkill(const CRuiMoveSkill& rhs);
	virtual ~CRuiMoveSkill() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
private:
	HRESULT Ready_Components();
private:

	_bool  m_bEffect = false;
	_bool		   m_bHit = false;
	_int		   m_iHit = 0;
	_float		   m_fDeadTime = 0.f;

public:
	static CRuiMoveSkill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
