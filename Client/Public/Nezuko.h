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

class CNezuko : public CCharacters
{
public:
	enum ANIMID
	{
		// IDLE
		ANIM_IDLE = 29,

		// MOVE
		ANIM_MOVE_START = 15,
		ANIM_MOVE_END = 16,

		// DASH
		ANIM_DASH_AB = 66,
		ANIM_DASH_AF = 67,
		ANIM_DASH_AL = 68,
		ANIM_DASH_AR = 69,
		ANIM_DASH_B = 70,
		ANIM_DASH_F = 71,
		ANIM_DASH_L_01 = 72,
		ANIM_DASH_L_02 = 73,
		ANIM_DASH_R_01 = 74,
		ANIM_DASH_R_02 = 75,

		ANIM_SKILL_JUMPCOMMON_0 = 63,
		ANIM_SKILL_JUMPCOMMON_1 = 64,
		ANIM_SKILL_JUMPCOMMON_2 = 65,
		ANIM_SKILL_JUMPMOVE_0 = 54,
		ANIM_SKILL_JUMPMOVE_1 = 55,
		ANIM_SKILL_JUMPMOVE_2 = 56,

		// ATTACK
		ANIM_ATTACK_1 = 90,
		ANIM_ATTACK_2 = 0,
		ANIM_ATTACK_3 = 1,
		ANIM_ATTACK_4 = 2,

		// JUMP
		ANIM_JUMP_START = 31,
		ANIM_JUMP_LOOP = 32,
		ANIM_JUMP_END = 33,

		ANIM_JUMP_LOOP_END =32,

		ANIM_JUMPATTACK = 40,
		ANIM_JUMP_MOVE_ATTACK_0 = 50,
		ANIM_JUMP_MOVE_ATTACK_1 = 51,
		ANIM_JUMP_MOVE_ATTACK_2 = 52,

		ANIM_TARGET_RUSH_0 = 14,
		ANIM_TARGET_RUSH_1 = 15,
		ANIM_TARGET_RUSH_2 = 16,

		// change
		ANIM_CHANGE_UP = 31,
		ANIM_CHANGE_DOWN_START = 32,
		ANIM_CHANGE_DOWN_END = 33,

		// GUARD
		ANIM_GUARD_0 = 22,
		ANIM_GUARD_1 = 23,
		ANIM_GUARD_2 = 24,

		// GUARD ADV
		ANIM_GUARD_ADV = 25,

		// GUARD STATE -> HIT
		ANIM_GUARD_HIT_0 = 26,
		ANIM_GUARD_HIT_1 = 27,

		// Hit

		ANIM_HIT = 80,


		// SKILL

		// COMMON
		ANIM_SKILL_COMMON_0 = 60,
		ANIM_SKILL_COMMON_1 = 61,
		ANIM_SKILL_COMMON_2 = 62,

		// MOVE

		ANIM_SKILL_MOVE_0 = 54,
		ANIM_SKILL_MOVE_1 = 55,
		ANIM_SKILL_MOVE_2 = 56,
		// GUARD + SKILL KEY

		ANIM_SKILL_FALLCUT_0 = 50,
		ANIM_SKILL_FALLCUT_1 = 51,
		ANIM_SKILL_FALLCUT_2 = 52,


		ANIM_BATTLE_START = 10,

		ANIM_HIT_DMG_F = 91,
		ANIM_HIT_DMG_L = 92,
		ANIM_HIT_DMG_R = 93,
		ANIM_HIT_DMG_U = 94,
		ANIM_HIT_DMG_AF = 95,
		ANIM_HIT_DMG_AL = 96,
		ANIM_HIT_DMG_AR = 97,
		ANIM_HIT_DMG_AU = 98,
		ANIM_HIT_DMG2_F = 99,
		ANIM_HIT_DMG2_G = 100,
		ANIM_HIT_DMG_DOWN_0 = 101,
		ANIM_HIT_DMG_DOWN_1 = 102,
		ANIM_HIT_DMG_DOWN_2 = 103,
		ANIM_HIT_DMG_DOWN_COL = 104,
		ANIM_HIT_DMG_UPPER_0 = 105,
		ANIM_HIT_DMG_UPPER_1 = 106,
		ANIM_HIT_DMG_UPPER_2 = 107,
		ANIM_HIT_DMG_DEATH = 108,
	
		ANIM_HIT_DMG_RETURN_0 = 80,
		ANIM_HIT_DMG_RETURN_1 = 82,


		ANIM_DEAD = 108,
		ANIM_END = 109
	};
private:
	CNezuko(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNezuko(const CNezuko& rhs);
	virtual ~CNezuko() = default;
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
public:
	void Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue);
private:
	void HandleInput();
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();
	void Set_Shadow();
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
private:
	OBJDIR m_eDirection = OBJDIR::DIR_END;
	ANIMID m_eAnimID = ANIMID::ANIM_END;
	class CNezukoState* m_pNezukoState = nullptr;

public:
	static CNezuko* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
