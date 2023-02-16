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

class CRuiDad : public CCharacters
{
public:
	enum ANIMID
	{
		ANIM_IDLE = 38,

		ANIM_ATTACK_0 = 4,
		ANIM_ATTACK_1 = 5,
		ANIM_ATTACK_2 = 6,

		ANIM_SKILL0 = 10,

		ANIM_SKILL1 = 11,

		ANIM_SKILL2_0 = 12,
		ANIM_SKILL2_1 = 13,
		ANIM_SKILL2_2 = 14,
	

		// 점프해서 플레이어에 내려찍기
		ANIM_SKILL3_0 = 19,
		ANIM_SKILL3_1 = 20,
		ANIM_SKILL3_2 = 21,

	



		ANIM_RUSH_0 = 26,
		ANIM_RUSH_1 = 27,
		ANIM_RUSH_2 = 28,
		ANIM_RUSH_3 = 29,


		ANIM_GUARD_0 = 32,
		ANIM_GUARD_1 = 33,
		ANIM_GUARD_2 = 34,


		ANIM_GUARDHIT_0 = 35,

		ANIM_JUMP_0 = 36,
		ANIM_JUMP_1 = 37,

		ANIM_MOVE_0 = 45,
		ANIM_MOVE_1 = 46,
		ANIM_MOVE_2 = 47,

		ANIM_DASH_B = 48,
		ANIM_DASH_F = 49,
		ANIM_DASH_L = 50,
		ANIM_DASH_R = 51,

		ANIM_HIT = 52,
		ANIM_HIT_FULL = 53,

		ANIM_END = 54
	};

public:
	CRuiDad(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRuiDad(const CRuiDad& rhs);
	virtual ~CRuiDad() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();


public:
	//	CTransform* Get_Transfrom() const { return m_pTransformCom; }
	CModel* Get_Model() const { return m_pModelCom; }

	ANIMID Get_AnimIndex() const { return m_eAnimID; }
	void   Set_AnimIndex(ANIMID iAnimIndex) { m_eAnimID = iAnimIndex; }

	void   Set_ShadowAlphaIncrease(_bool bShadow) { m_bShadowAlphaIncrease = bShadow; }
	void   Set_ShadowAlphaDecrease(_bool bShadow) { m_bShadowAlphaDecrease = bShadow; }
	void   Set_RuiDadHit(_bool bHit) { m_bRuiDadHit = bHit; }
	_bool  Get_RuiDadHit() const { return m_bRuiDadHit; }
	void   Set_RuiDadAiMode(_bool bAIMode) { m_bAiMode = bAIMode; }
	_bool  Get_RuiDadAiMode() const { return m_bAiMode; }
private:
	void HandleInput();
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();

	void	Set_Info();


public:
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);
	virtual void  Player_TakeDown(_float _fPow, _bool _bJump = 0);
	virtual void  Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower);

private:
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	_float					m_fEffectTime = 0.f;
private:
	OBJDIR m_eDirection = OBJDIR::DIR_END;
	ANIMID m_eAnimID = ANIMID::ANIM_END;

	_bool	m_bShadowAlphaIncrease = false;
	_bool	m_bShadowAlphaDecrease= false;

	_matrix m_ShadowMatrix;

	_bool   m_bRuiDadHit = false;
	_bool	m_bStart = false;
	_bool   m_bAiMode = false;


	_bool					m_bQuestStart = false;
	_bool					m_bQuestStop = false;
	class CRuiDadState* m_pRuiDadState = nullptr;
public:
	static CRuiDad* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;


};

END
