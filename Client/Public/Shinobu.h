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

END


BEGIN(Client)

class CShinobu : public CCharacters
{
public:
	enum ANIMID
	{
		// IDLE
		ANIM_IDLE = 66,

		// MOVE
		ANIM_MOVE_START = 70,
		ANIM_MOVE_LOOP = 70,
		ANIM_MOVE_END = 71,

		// JUMP
		ANIM_JUMP_START = 63,
		ANIM_JUMP_LOOP = 64,
		ANIM_JUMP_END = 65,

		ANIM_HIT_DMG_RETURN_0 = 80,
		ANIM_HIT_DMG_RETURN_1 = 82,
		// change
		ANIM_CHANGE_UP = 63,
		ANIM_CHANGE_DOWN_START = 64,
		ANIM_CHANGE_DOWN_END = 65,

		// ATTACK
		ANIM_ATTACK_1 = 11,
		ANIM_ATTACK_2 = 12,
		ANIM_ATTACK_3 = 13,
		ANIM_ATTACK_4 = 22,

		ANIM_JUMPATTACK = 1,
		ANIM_JUMP_MOVE_ATTACK_0 = 8,
		ANIM_JUMP_MOVE_ATTACK_1 = 9,
		ANIM_JUMP_MOVE_ATTACK_2 = 10,

		ANIM_TARGET_RUSH_0 = 51,
		ANIM_TARGET_RUSH_1 = 52,
		ANIM_TARGET_RUSH_2 = 53,

		ANIM_JUMP_LOOP_END = 64,

		// HIT
		ANIM_HIT = 26,


		// GUARD
		ANIM_GUARD_0 = 56,
		ANIM_GUARD_1 = 57,
		ANIM_GUARD_2 = 58,

		// GUARD ADV
		ANIM_GUARD_ADV = 59,

		// GUARD STATE -> HIT
		ANIM_GUARD_HIT_0 = 60,
		ANIM_GUARD_HIT_1 = 61,

		ANIM_JUMP_SKILL_COMMON_0 = 31,
		ANIM_JUMP_SKILL_COMMON_1 = 32,
		ANIM_JUMP_SKILL_COMMON_2 = 33,


		ANIM_JUMP_SKILL_MOVE_0 = 40,
		ANIM_JUMP_SKILL_MOVE_1 = 41,
		ANIM_JUMP_SKILL_MOVE_2 = 42,
		ANIM_JUMP_SKILL_MOVE_3 = 35,
		ANIM_JUMP_SKILL_MOVE_4 = 36,


		// DASH
		ANIM_DASH_AB = 72,
		ANIM_DASH_AF = 73,
		ANIM_DASH_AL = 74,
		ANIM_DASH_AR = 75,
		ANIM_DASH_B = 76,
		ANIM_DASH_F = 77,
		ANIM_DASH_L_01 = 78,
		ANIM_DASH_L_02 = 79,
		ANIM_DASH_R_01 = 80,
		ANIM_DASH_R_02 = 81,



		// SKILL

		// Common
		ANIM_SKILL_COMMON_0 = 28,
		ANIM_SKILL_COMMON_1 = 29,
		ANIM_SKILL_COMMON_2 = 30,

		// Move
		ANIM_SKILL_MOVE_0 = 34,
		ANIM_SKILL_MOVE_1 = 35,
		ANIM_SKILL_MOVE_2 = 36,

		// U + I
		ANIM_SKILL_UPPER_0 = 37,
		ANIM_SKILL_UPPER_1 = 38,
		ANIM_SKILL_UPPER_2 = 39,

		ANIM_BATTLE_START = 89,

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


		ANIM_SPLSKL_START_0 = 83,
		ANIM_SPLSKL_START_1 = 85,
		ANIM_SPLSKL_END = 86,

		ANIM_DEAD = 108,
		ANIM_END = 114
	};

private:
	CShinobu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShinobu(const CShinobu& rhs);
	virtual ~CShinobu() = default;
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
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);
	virtual void  Player_TakeDown(_float _fPow, _bool _bJump = 0);
	virtual void  Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower);
	virtual void  Play_Scene();

public:
	void Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue);
	_int Get_WaterMillHit() { return m_iWaterMillHit; }
	void Set_WaterMillHit() { ++m_iWaterMillHit; }
	void Reset_WaterMillHit() { m_iWaterMillHit = 0; }
	_int Get_WindMillHit() { return m_iWindMillHit; }
	void Set_WindMillHit() { ++m_iWindMillHit; }
	void Reset_WindMillHit() { m_iWindMillHit = 0; }
	_int Get_SkillHit() { return m_iSkillHit; }
	void Set_SkillHit() { ++m_iSkillHit; }
	void Reset_SkillHit() { m_iSkillHit = 0; }
	void Set_Render(_bool _bRender) { m_bRender = _bRender; }

	void Set_KaguraMode(_bool bKagura) { m_bIsKagura = bKagura; }
	_bool Get_KaguraMode() const { return m_bIsKagura; }

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();



private:
	void HandleInput(_float fTimeDelta);
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);
	HRESULT Ready_Parts();
	HRESULT Ready_Parts2();
	void	Set_Info();
private:
	CModel*					m_pModelCom = nullptr;

	CGameObject*			m_pWeapon = nullptr;
	CGameObject*			m_pSheath = nullptr;
	_float					m_fHpTime = 0.f;
	_int					m_iSkillHit = 0;
	_int					m_iWaterMillHit = 0;
	_int					m_iWindMillHit = 0;
	_float					m_fEffectTime = 0.f;

	_bool					m_bRender = true;
private:
	OBJDIR m_eDirection = OBJDIR::DIR_END;
	ANIMID m_eAnimID = ANIMID::ANIM_END;

	class CShinobuState* m_pShinobuState = nullptr;
	_bool m_bIsKagura = false;

private:
	OBJKEYSTATE m_ePreKeyState = KEY_END;
	OBJKEYSTATE m_eCurKeyState = KEY_END;

public:
	static CShinobu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
