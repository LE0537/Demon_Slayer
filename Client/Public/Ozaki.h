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

class COzaki : public CCharacters
{
public:
	COzaki(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	COzaki(const COzaki& rhs);
	virtual ~COzaki() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();


public:
	//	CTransform* Get_Transfrom() const { return m_pTransformCom; }
	virtual CModel* Get_Model() const { return m_pModelCom; }



private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();
	void Check_Event(_float fTimeDelta);

public:
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);
	virtual void  Player_TakeDown(_float _fPow, _bool _bJump = 0);
	virtual void  Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower);
	virtual void  Play_Scene();
private:
	CModel*					m_pModelCom = nullptr;

	_float					m_fEffectTime = 0.f;
	_bool					m_bQuestStop = false;
	_bool					m_MsgReset = false;
	_bool					m_bMsgStart = false;
	_bool					m_bMsgEnd = false;
	_bool					m_bSoundCheck = false;
	_bool					m_bIsPlaying = false;
	_float					m_fMsgTime = 0.f;
public:
	static COzaki* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;


};

END
