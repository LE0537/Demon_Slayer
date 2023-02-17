#pragma once
#include "Client_Defines.h"
#include "CollBox.h"
#include "Characters.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CRuiDadSmallStone : public CCollBox
{
public:
	typedef struct tagRuiDadSmallInfo {
		CCharacters*		pPlayer;
		CCharacters*		pTarget;
		_float4				vPos;
		_int				iIndex;
	}RUIDADSMALLSTONE;
protected:
	CRuiDadSmallStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRuiDadSmallStone(const CRuiDadSmallStone& rhs);
	virtual ~CRuiDadSmallStone() = default;
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
	void	Jump(_float fTimeDelta);
private:
	RUIDADSMALLSTONE	 m_ShootInfo;
	_float		   m_fDeadTime = 0.f;
	_bool		   m_bHit = false;
	_bool		   m_bEffectDead = false;
	_float4		   m_vTargetLook;
	_float		   m_fY = 0.f;
	_bool		   m_bReady = false;
	_float		   m_fCurrentDuration = 0.f;
	_bool          m_bLook = false;
	_bool		   m_bThrow = false;
	//Jump
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fGravity = 9.8f;
	_float m_fCurrentPosY = 0.f;
public:
	static CRuiDadSmallStone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
