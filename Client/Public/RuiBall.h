#pragma once
#include "Client_Defines.h"
#include "CollBox.h"
#include "Characters.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CRuiBall : public CCollBox
{
public:
	typedef struct tagRuiInfo {
		CCharacters*		pPlayer;
		CCharacters*		pTarget;
	}RUISHOOTINFO;
protected:
	CRuiBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRuiBall(const CRuiBall& rhs);
	virtual ~CRuiBall() = default;
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
	RUISHOOTINFO	 m_ShootInfo;
	_float		   m_fDeadTime = 0.f;
	_bool		   m_bHit = false;
public:
	static CRuiBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
