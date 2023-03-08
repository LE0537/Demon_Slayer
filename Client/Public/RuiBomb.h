#pragma once
#include "Client_Defines.h"
#include "CollBox.h"
#include "Characters.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CRuiBomb : public CCollBox
{
public:

typedef struct tagBombDesc {
		CCharacters*		pPlayer;
		CCharacters*		pTarget;
		_vector				vPosition;
		_int				iIndex;
	}BOMBDESC;
protected:
	CRuiBomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRuiBomb(const CRuiBomb& rhs);
	virtual ~CRuiBomb() = default;
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
	BOMBDESC	 m_tBombInfo;
	_bool  m_bEffect = false;
	_bool		   m_bHit = false;
	_int		   m_iHit = 0;
	_float		   m_fDeadTime = 0.f;
	_float4 m_vTargetPos;
	_bool	m_bTargetPos = false;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_float m_fMove = 0.f;
public:
	static CRuiBomb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
