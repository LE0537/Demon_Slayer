#pragma once
#include "Client_Defines.h"
#include "CollBox.h"
#include "Characters.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CRuiDadBigStone : public CCollBox
{
public:
	typedef struct tagRuiDadInfo {
		CCharacters*		pPlayer;
		CCharacters*		pTarget;
		_int				iIndex;
	}RUIDADBIGSTONE;
protected:
	CRuiDadBigStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRuiDadBigStone(const CRuiDadBigStone& rhs);
	virtual ~CRuiDadBigStone() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
private:
	HRESULT Ready_Components();
	void	Ready_Throw(_float fTimeDelta);
	void	Throw(_float fTimeDelta);
	void    Throw_End(_float fTimeDelta);
private:
	RUIDADBIGSTONE	 m_ShootInfo;
	_float		   m_fDeadTime = 0.f;
	_bool		   m_bHit = false;
	_bool		   m_bEffectDead = false;
	_float4		   m_vTargetLook;
	_float		   m_fY = 0.f;
	_bool		   m_bReady = false;

	
public:
	static CRuiDadBigStone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
