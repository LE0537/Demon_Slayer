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

class CEnmu : public CCharacters
{
public:
	enum ANIMID
	{
		ANIM_IDLE = 42,

		ANIM_MOVE_0 = 46,
		ANIM_MOVE_1 = 47,

		ANIM_ATTACK_1 = 1,
		ANIM_ATTACK_2 = 2,
		ANIM_ATTACK_3 = 3,

		ANIM_SKILL_1_0 = 13,
		ANIM_SKILL_1_1 = 15,
		ANIM_SKILL_1_2 = 14,

		ANIM_SKILL_2_0 = 9,
		ANIM_SKILL_2_1 = 10,
		ANIM_SKILL_2_2 = 11,
		ANIM_SKILL_2_3 = 12,

		//JUMP 38 39
		ANIM_SKILL_JUMP_0 = 38,
		ANIM_SKILL_JUMP_1 = 39,
		ANIM_SKILL_3_0 = 16,
		ANIM_SKILL_3_1 = 17,
		ANIM_SKILL_3_2 = 18,
		ANIM_SKILL_3_3 = 19,
		ANIM_SKILL_JUMP_2  = 40,
		ANIM_SKILL_JUMP_3  = 41,


		ANIM_GUARD_0 =32,
		ANIM_GUARD_1 = 33,
		ANIM_GUARD_2 = 34,

		ANIM_GUARDHIT_0 = 36,
		ANIM_GUARDHIT_1 = 37,

		ANIM_TARGET_RUSH_0 = 27,
		ANIM_TARGET_RUSH_1 = 28,
		ANIM_TARGET_RUSH_2 = 29,

		ANIM_DASH_B = 52,
		ANIM_DASH_F = 53,
		ANIM_DASH_L = 54,
		ANIM_DASH_L_02 = 55,
		ANIM_DASH_R = 56,
		ANIM_DASH_R_02 = 57,

		ANIM_BATTLE_START = 79,
		

		ANIM_HIT_DMG_F = 60,
		ANIM_HIT_DMG_L = 61,
		ANIM_HIT_DMG_R = 62,
		ANIM_HIT_DMG_U = 59,
		//// 점프 히트
		ANIM_HIT_DMG_AF = 60,
		ANIM_HIT_DMG_AL = 61,
		ANIM_HIT_DMG_AR = 62,
		ANIM_HIT_DMG_AU = 59,
		//////////////

		ANIM_HIT_DMG2_F = 59,
		ANIM_HIT_DMG2_G = 63,
		ANIM_HIT_DMG_DOWN_0 = 64,
		ANIM_HIT_DMG_DOWN_1 = 65,
		ANIM_HIT_DMG_DOWN_2 = 69,
		ANIM_HIT_DMG_DOWN_COL = 70,
		ANIM_HIT_DMG_UPPER_0 = 71,
		ANIM_HIT_DMG_UPPER_1 = 72,
		ANIM_HIT_DMG_UPPER_2 = 73,
		ANIM_HIT_DMG_DEATH = 58,
		//SPIN 75 76 77


		ANIM_HIT_DMG_RETURN_0 = 30,
		ANIM_HIT_DMG_RETURN_1 = 31,

		ANIM_DEAD = 58,



		ANIM_END = 100,
	};

public:
	CEnmu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnmu(const CEnmu& rhs);
	virtual ~CEnmu() = default;

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

	ANIMID Get_AnimIndex() const { return m_eAnimID; }
	void   Set_AnimIndex(ANIMID iAnimIndex) { m_eAnimID = iAnimIndex; }

public:
	void Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue);


private:
	void HandleInput();
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);

	void   Set_ShadowAlphaIncrease(_bool bShadow) { m_bShadowAlphaIncrease = bShadow; }
	void   Set_ShadowAlphaDecrease(_bool bShadow) { m_bShadowAlphaDecrease = bShadow; }
	void   Set_RuiDadHit(_bool bHit) { m_bRuiDadHit = bHit; }
	_bool  Get_RuiDadHit() const { return m_bRuiDadHit; }
	void   Set_RuiDadAiMode(_bool bAIMode) { m_bAiMode = bAIMode; }
	_bool  Get_RuiDadAiMode() const { return m_bAiMode; }
	_bool  Get_QuestStop() const { return m_bQuestStop; }
	_bool  Get_Start() const { return m_bStart; }
	void   Set_Start(_bool bStart) { m_bStart = bStart; }
	void   Set_Render(_bool bRender) { m_bRender = bRender; }
	_bool  Get_Tick() const { return m_bTick; }
	void   Set_Tick(_bool bTick) { m_bTick = bTick; }

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();

	void	Set_Info();


public:
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);
	virtual void  Player_TakeDown(_float _fPow, _bool _bJump = 0);
	virtual void  Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower);
	virtual void  Play_Scene();
private:
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	_float					m_g_fEffectTime = 0.f;
private:
	ANIMID m_eAnimID;

	_bool	m_bShadowAlphaIncrease = false;
	_bool	m_bShadowAlphaDecrease = false;

	_matrix m_ShadowMatrix;

	_bool   m_bRuiDadHit = false;
	_bool	m_bStart = false;
	_bool   m_bAiMode = false;
	_bool   m_bRender = true;

	_bool   m_bTick = false;

	_bool					m_bQuestStart = false;
	_bool					m_bQuestStop = false;
	class CEnmuState*		m_pEnmuState = nullptr;


public:
	static CEnmu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;


};

END
