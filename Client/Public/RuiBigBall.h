#pragma once
#include "Client_Defines.h"
#include "CollBox.h"
#include "Characters.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CRuiBigBall : public CCollBox
{
public:
	typedef struct tagRuiBigInfo {
		CCharacters*		pPlayer;
		CCharacters*		pTarget;
	}RUIBIGBALLINFO;
protected:
	CRuiBigBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRuiBigBall(const CRuiBigBall& rhs);
	virtual ~CRuiBigBall() = default;
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
	RUIBIGBALLINFO	 m_ShootInfo;
	_float		   m_fDeadTime = 0.f;
	_bool		   m_bHit = false;
	_bool		   m_bEffectDead = false;
public:
	static CRuiBigBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
