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

class CAkaza : public CCharacters
{
public:
	enum ANIMID
	{
		// IDLE
		ANIM_IDLE = 48,

		// MOVE
		ANIM_MOVE_START = 51,
		ANIM_MOVE_END = 52,

		// DASH
		ANIM_DASH_AB = 53,
		ANIM_DASH_AF = 54,
		ANIM_DASH_AL = 55,
		ANIM_DASH_AR = 56,
		ANIM_DASH_B = 57,
		ANIM_DASH_F = 58,
		ANIM_DASH_L_01 = 59,
		ANIM_DASH_L_02 = 60,
		ANIM_DASH_R_01 = 61,
		ANIM_DASH_R_02 = 62,


		ANIM_HIT_DMG_RETURN_0 = 36,
		ANIM_HIT_DMG_RETURN_1 = 37,

		ANIM_SKILL_JUMPCOMMON_0 = 20,
		ANIM_SKILL_JUMPCOMMON_1 = 21,
		ANIM_SKILL_JUMPCOMMON_2 = 22,
		ANIM_SKILL_JUMPMOVE_0 = 28,
		ANIM_SKILL_JUMPMOVE_1 = 27,
		ANIM_SKILL_JUMPMOVE_2 = 47,


		// ATTACK
		ANIM_ATTACK_1 = 11,
		ANIM_ATTACK_2 = 12,
		ANIM_ATTACK_3 = 13,
		ANIM_ATTACK_4 = 16,

		ANIM_JUMPATTACK = 3,
		ANIM_JUMP_MOVE_ATTACK_0 = 8,
		ANIM_JUMP_MOVE_ATTACK_1 = 9,
		ANIM_JUMP_MOVE_ATTACK_2 = 10,

		ANIM_TARGET_RUSH_0 = 33,
		ANIM_TARGET_RUSH_1 = 34,
		ANIM_TARGET_RUSH_2 = 35,


		// JUMP
		ANIM_JUMP_START = 45,
		ANIM_JUMP_LOOP = 46,
		ANIM_JUMP_END = 47,


		ANIM_JUMP_LOOP_END = 46,

		// change
		ANIM_CHANGE_UP = 45,
		ANIM_CHANGE_DOWN_START = 46,
		ANIM_CHANGE_DOWN_END = 47,

		// GUARD
		ANIM_GUARD_0 = 38,
		ANIM_GUARD_1 = 39,
		ANIM_GUARD_2 = 40,

		// GUARD ADV
		ANIM_GUARD_ADV = 41,

		// GUARD STATE -> HIT
		ANIM_GUARD_HIT_0 = 42,
		ANIM_GUARD_HIT_1 = 43,

		// Hit

		ANIM_HIT = 65,


		// SKILL

		// SHOOT
		ANIM_SKILL_SHOOT_0 = 17,
		ANIM_SKILL_SHOOT_1 = 18,
		ANIM_SKILL_SHOOT_2 = 19,

		// PUNCH

		ANIM_SKILL_PUNCH_0 = 25,
		ANIM_SKILL_PUNCH_1 = 26,
		ANIM_SKILL_PUNCH_2 = 27,

		// DESTROY

		ANIM_SKILL_DESTROY_0 = 32,
		ANIM_SKILL_DESTROY_1 = 15,

		// FRIEND SKILL
		ANIM_ADVSKILL_COMMON_0 = 29,
		ANIM_ADVSKILL_COMMON_1 = 30,
		ANIM_ADVSKILL_COMMON_2 = 31,
		ANIM_ADVSKILL_MOVE_0 = 24,
		ANIM_ADVSKILL_MOVE_1 = 25,
		ANIM_ADVSKILL_MOVE_2= 27,
		

		ANIM_BATTLE_START = 64,


		ANIM_HIT_DMG_F = 66,
		ANIM_HIT_DMG_L = 67,
		ANIM_HIT_DMG_R = 68,
		ANIM_HIT_DMG_U = 69,
		ANIM_HIT_DMG_AF = 70,
		ANIM_HIT_DMG_AL = 71,
		ANIM_HIT_DMG_AR = 72,
		ANIM_HIT_DMG_AU = 73,
		ANIM_HIT_DMG2_F = 74,
		ANIM_HIT_DMG2_G = 75,
		ANIM_HIT_DMG_DOWN_0 = 76,
		ANIM_HIT_DMG_DOWN_1 = 77,
		ANIM_HIT_DMG_DOWN_2 = 78,
		ANIM_HIT_DMG_DOWN_COL = 79,
		ANIM_HIT_DMG_UPPER_0 = 80,
		ANIM_HIT_DMG_UPPER_1 = 81,
		ANIM_HIT_DMG_UPPER_2 = 82,
		ANIM_HIT_DMG_DEATH = 83,

		ANIM_SPLSKL_START_0 = 5,
		ANIM_SPLSKL_START_1 = 7,
		ANIM_SPLSKL_END = 2, // or 63


		ANIM_DEAD = 83,
		ANIM_END = 100
	};
private:
	CAkaza(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAkaza(const CAkaza& rhs);
	virtual ~CAkaza() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

	void Boss_Tick(_float fTimeDelta);


public:
	//	CTransform* Get_Transfrom() const { return m_pTransformCom; }
	virtual CModel* Get_Model() const { return m_pModelCom; }

	ANIMID Get_AnimIndex() const { return m_eAnimID; }
	void   Set_AnimIndex(ANIMID iAnimIndex) { m_eAnimID = iAnimIndex; }
	void	Set_AiMode(_bool _bAiMode) { m_bAiState = _bAiMode; }
	_bool   Get_IsAIMode() const { return m_bAiState; }


	_bool Get_IsFarAI() const { return m_bFarAI; }
	void Set_FarAI(_bool bAi) { m_bFarAI = bAi; }

	_bool Get_IsDashPattern() const { return m_bDashPattern; }
	void Set_DashPatten(_bool bPattern) {m_bDashPattern = bPattern;}
	void Play_AkazaScene();
public:
	void Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue);
private:
	void HandleInput();
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();
	void	Set_ShadowLightPos();
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
	_float					m_fEffectTime = 0.f;

	_bool					m_bAiState = false;
	_bool					m_bFarAI = false;
	_bool					m_bDashPattern = false;
private:
	OBJDIR m_eDirection = OBJDIR::DIR_END;
	ANIMID m_eAnimID = ANIMID::ANIM_END;

	class CAkazaState* m_pAkazaState = nullptr;
public:
	static CAkaza* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
