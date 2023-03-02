#pragma once
#include "Client_Defines.h"
#include "Characters.h"
#include <queue>

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
class CRui : public CCharacters
{
public:
	enum ANIMID
	{
		// IDLE
		ANIM_IDLE = 60,

		// MOVE
		ANIM_MOVE_START = 65,
		ANIM_MOVE_END = 66,


		// DASH
		ANIM_DASH_AB = 67,
		ANIM_DASH_AF = 68,
		ANIM_DASH_AL = 69,
		ANIM_DASH_AR = 70,
		ANIM_DASH_B = 71,
		ANIM_DASH_F = 72,
		ANIM_DASH_L_01 = 73,
		ANIM_DASH_L_02 = 74,
		ANIM_DASH_R_01 = 75,
		ANIM_DASH_R_02 = 76,

		ANIM_JUMPATTACK = 5,
		ANIM_JUMP_MOVE_ATTACK_0 = 10,
		ANIM_JUMP_MOVE_ATTACK_1 = 11,
		ANIM_JUMP_MOVE_ATTACK_2 = 12,

		ANIM_TARGET_RUSH_0 = 45,
		ANIM_TARGET_RUSH_1 = 46,
		ANIM_TARGET_RUSH_2 = 47,

		// ATTACK
		ANIM_ATTACK_1 = 13,
		ANIM_ATTACK_2 = 15,
		ANIM_ATTACK_3 = 18,
		ANIM_ATTACK_4 = 24,

		ANIM_JUMPSKILL_0 = 6,
		ANIM_JUMPSKILL_1 = 6,

		ANIM_MOVESKILL = 5,
		// JUMP
		ANIM_JUMP_START = 57,
		ANIM_JUMP_LOOP = 58,
		ANIM_JUMP_END = 59,

		ANIM_JUMP_LOOP_END = 58,

		// change
		ANIM_CHANGE_UP = 57,
		ANIM_CHANGE_DOWN_START = 58,
		ANIM_CHANGE_DOWN_END = 59,

		// GUARD
		ANIM_GUARD_0 = 50,
		ANIM_GUARD_1 = 51,
		ANIM_GUARD_2 = 52,

		// GUARD ADV
		ANIM_GUARD_ADV = 53,

		// GUARD STATE -> HIT
		ANIM_GUARD_HIT_0 = 54,
		ANIM_GUARD_HIT_1 = 55,
		// Hit

		ANIM_HIT = 87,


		// SKILL

		// SHOOT Net
		ANIM_SKILL_SHOOTNET_0 = 25,
		ANIM_SKILL_SHOOTNET_1 = 26,
		ANIM_SKILL_SHOOTNET_2 = 27,

		// SHOOT 
		ANIM_SKILL_SHOOT_0 = 9,
		ANIM_SKILL_SHOOT_1 = 13,
		ANIM_SKILL_SHOOT_2 = 9,

		// SHOOT SPHERE
		ANIM_SKILL_SPHERE_0 = 33,
		ANIM_SKILL_SPHERE_1 = 34,
		ANIM_SKILL_SPHERE_2 = 35,


		// FRIEND SKILL
		ANIM_ADVSKILL_COMMON = 23,
		ANIM_ADVSKILL_MOVE = 20,

		ANIM_BATTLE_START = 83,

		ANIM_HIT_DMG_F = 88,
		ANIM_HIT_DMG_L = 89,
		ANIM_HIT_DMG_R = 90,
		ANIM_HIT_DMG_U = 91,
		ANIM_HIT_DMG_AF = 92,
		ANIM_HIT_DMG_AL = 93,
		ANIM_HIT_DMG_AR = 94,
		ANIM_HIT_DMG_AU = 95,
		ANIM_HIT_DMG2_F = 96,
		ANIM_HIT_DMG2_G = 97,
		ANIM_HIT_DMG_DOWN_0 = 98,
		ANIM_HIT_DMG_DOWN_1 = 99,
		ANIM_HIT_DMG_DOWN_2 = 100,
		ANIM_HIT_DMG_DOWN_COL = 101,
		ANIM_HIT_DMG_UPPER_0 = 102,
		ANIM_HIT_DMG_UPPER_1 = 103,
		ANIM_HIT_DMG_UPPER_2 = 104,
		ANIM_HIT_DMG_DEATH = 105,

		ANIM_HIT_DMG_RETURN_1 = 49,
		ANIM_HIT_DMG_RETURN_0 = 48,

		ANIM_DEAD =105,

		ANIM_SPLSKL_START_0 = 23,

		ANIM_SPLSKL_END = 113,

		ANIM_END = 106
	};

private:
	CRui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRui(const CRui& rhs);
	virtual ~CRui() = default;
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
	void	Set_Start() { m_bStart = true; }
	void	Set_AiMode(_bool _bAiMode) { m_bAiState = _bAiMode; }
	_bool   Get_IsAIMode() const { return m_bAiState; }
	ANIMID Get_AnimIndex() const { return m_eAnimID; }
	void   Set_AnimIndex(ANIMID iAnimIndex) { m_eAnimID = iAnimIndex; }
	void   Set_QueueCombo(queue<_uint> eQueue) { m_queueDash = eQueue; }
	queue<_uint> Get_QueueCombo() { return m_queueDash; }
	_bool Get_RuiHit();
	void Set_RuiHit(_bool bHit) { m_bIsHit = bHit; }
	void	Set_NextRound(_bool bNextRound) { m_bNextRound = bNextRound; }
	_bool   Get_NextRound() { return m_bNextRound; }
	void	Set_StorySpl() { m_bStorySpl = true; }
	_bool   Get_StorySpl() { return m_bStorySpl; }
public:
	void Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue);
	void Set_StoryDelay(_float _fTime) { m_StoryTime = _fTime; }
private:
	void HandleInput(_float fTimeDelta);
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);

	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();

	void	Boss_Tick(_float fTimeDelta);
	void	Boss_LateTick(_float fTimeDelta);
	void	Boss_Render();
	void	Set_Info();
	void    Check_Spl();
	void    StorySpl(_float fTimeDelta);
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


	CGameObject*			m_pTrail = nullptr;


	_float					m_g_fEffectTime = 0.f;

	_float					m_StoryTime = 0.f;
private:
	OBJDIR m_eDirection = OBJDIR::DIR_END;
	ANIMID m_eAnimID = ANIMID::ANIM_END;
 
	class CRuiState* m_pRuiState = nullptr;
	
	_bool m_bHeal = false;
	_int  m_iHp = 0;

	_bool m_bStart = false;
	_bool m_bAiState = false;
	_bool m_bIsHit = false;
	queue<_uint> m_queueDash;
	
	_bool m_bNextRound = false;
	_bool m_bStorySpl = false;
	_float m_fStoryTime = 0.f;
	list <class CRuiState*> m_AIStateList;
public:
	static CRui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
