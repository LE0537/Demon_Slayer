#pragma once
#include "Client_Defines.h"
#include "Characters.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CModel;
class CCollider;
class CNavigation;

END


BEGIN(Client)

class CTrainNPC : public CCharacters
{
public:
	CTrainNPC(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrainNPC(const CTrainNPC& rhs);
	virtual ~CTrainNPC() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();


	virtual CModel* Get_Model() const { return m_pModelCom[0]; }

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();


public:
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);
	virtual void  Player_TakeDown(_float _fPow, _bool _bJump = 0);
	virtual void  Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower);
	virtual void  Play_Scene();

private:
	CModel*					m_pModelCom[14] = { nullptr };

	_float					m_fEffectTime = 0.f;
private:


public:
	static CTrainNPC* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;


};

END
