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

class CTanjiro : public CCharacters
{
public:
	enum ANIMID
	{
		// IDLE
		ANIM_IDLE = 69,

		// MOVE
		ANIM_MOVE_START = 72,
		ANIM_MOVE_LOOP = 73,
		ANIM_MOVE_END = 74,

		// JUMP
		ANIM_JUMP_START = 65,
		ANIM_JUMP_LOOP_START = 66,
		ANIM_JUMP_LOOP_END = 67,
		ANIM_JUMP_END = 68,

		// change
		ANIM_CHANGE_UP = 66,
		ANIM_CHANGE_DOWN_START = 67,
		ANIM_CHANGE_DOWN_END = 68,

		// ATTACK
		ANIM_ATTACK_1 = 12,
		ANIM_ATTACK_2 = 13,
		ANIM_ATTACK_3 = 14,
		ANIM_ATTACK_4 = 17,


		ANIM_JUMPATTACK = 4,
		ANIM_JUMP_MOVE_ATTACK_0 = 9,
		ANIM_JUMP_MOVE_ATTACK_1 = 10,
		ANIM_JUMP_MOVE_ATTACK_2 = 11,

		ANIM_TARGET_RUSH_0 = 54,
		ANIM_TARGET_RUSH_1 = 55,
		ANIM_TARGET_RUSH_2 = 56,

		ANIM_KAGURA_ATTACK_1 = 100,
		ANIM_KAGURA_ATTACK_2 = 101,
		ANIM_KAGURA_ATTACK_3 = 102,
		ANIM_KAGURA_ATTACK_4 = 103,

		//JUMP SKILL
		ANIM_SKILL_JUMPCOMMON_0 = 25,
		ANIM_SKILL_JUMPCOMMON_1 = 26,
		ANIM_SKILL_JUMPCOMMON_2 = 27,
		ANIM_SKILL_JUMPMOVE_0 = 25,
		ANIM_SKILL_JUMPMOVE_1 = 26,
		ANIM_SKILL_JUMPMOVE_2 = 27,

		// HIT
		ANIM_HIT = 22,


		// GUARD
		ANIM_GUARD_0 = 59,
		ANIM_GUARD_1 = 60,
		ANIM_GUARD_2 = 61,

		// GUARD ADV
		ANIM_GUARD_ADV = 62,

		// GUARD STATE -> HIT
		ANIM_GUARD_HIT_0 = 63,
		ANIM_GUARD_HIT_1 = 64,


		// DASH
		ANIM_DASH_AB = 75,
		ANIM_DASH_AF = 76,
		ANIM_DASH_AL = 77,
		ANIM_DASH_AR = 78,
		ANIM_DASH_B = 79,
		ANIM_DASH_F = 80,
		ANIM_DASH_L_01 = 81,
		ANIM_DASH_L_02 = 82,
		ANIM_DASH_R_01 = 83,
		ANIM_DASH_R_02 = 84,


		// SKILL

		// WATER MILL
		ANIM_SKILL_WATERMILL_0 = 25,
		ANIM_SKILL_WATERMILL_1 = 26,
		ANIM_SKILL_WATERMILL_2 = 27,

		// WIND MILL
		ANIM_SKILL_WINDMILL_0 = 29,
		ANIM_SKILL_WINDMILL_1 = 30,
		ANIM_SKILL_WINDMILL_2 = 31,

		// COMMON
		ANIM_SKILL_COMMON = 33,


		// KAGURA COMMON
		ANIM_SKILL_KAGURA_COMMON = 104,
		ANIM_SKILL_KAGURA_MOVE = 105,
		ANIM_SKILL_KAGURA_SPHERE = 107,
		ANIM_SKILL_KAGURA_SPHERE_1 = 108,
		ANIM_SKILL_KAGURA_SPHERE_2 = 109,

		ANIM_BATTLE_START = 99,


		ANIM_HIT_DMG_F = 114,
		ANIM_HIT_DMG_L = 115,
		ANIM_HIT_DMG_R = 116,
		ANIM_HIT_DMG_U = 117,
		ANIM_HIT_DMG_AF = 118,
		ANIM_HIT_DMG_AL = 119,
		ANIM_HIT_DMG_AR = 120,
		ANIM_HIT_DMG_AU = 121,
		ANIM_HIT_DMG2_F = 122,
		ANIM_HIT_DMG2_G = 123,
		ANIM_HIT_DMG_DOWN_0 = 124,
		ANIM_HIT_DMG_DOWN_1 = 125,
		ANIM_HIT_DMG_DOWN_2 = 126,
		ANIM_HIT_DMG_DOWN_COL = 127,
		ANIM_HIT_DMG_UPPER_0 = 128,
		ANIM_HIT_DMG_UPPER_1 = 129,
		ANIM_HIT_DMG_UPPER_2 = 130,
		ANIM_HIT_DMG_DEATH = 131,

		ANIM_HIT_DMG_RETURN_0 = 57,
		ANIM_HIT_DMG_RETURN_1 = 58,

		ANIM_DEAD = 131,

		ANIM_END = 114
	};

private:
	CTanjiro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTanjiro(const CTanjiro& rhs);
	virtual ~CTanjiro() = default;
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
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);
	virtual void  Player_TakeDown(_float _fPow, _bool _bJumpHit = 0);
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

	void Set_KaguraMode(_bool bKagura) { m_bIsKagura = bKagura; }
	_bool Get_KaguraMode() const { return m_bIsKagura; }
	void Set_Render(_bool _bRender) { m_bRender = _bRender; }
	void Set_Quest1() { m_bQuest1 = true; }
	_bool Get_Quest1() { return m_bQuest1; }
	void Set_Quest2() { m_bQuest2 = true; }
	_bool Get_Quest2() { return m_bQuest2; }
	_bool Get_Quest2MSG() { return m_bQuest2MSG; }
	_bool Get_Quest3MSG() { return m_bQuest3MSG; }
	void Set_Stop(_bool _bStop) { m_bStop = _bStop; }
	_bool Get_Stop() { return m_bStop; }

	void Set_IDLE(_bool _bIDLE) { m_bIDLE = _bIDLE; }

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();
	void Set_Shadow();
	void Check_QuestEvent(_float fTimeDelta);
	void Check_QuestTrainEvent(_float fTimeDelta);
private:
	void HandleInput(_float fTimeDelta);
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);
	HRESULT Ready_Parts();
	HRESULT Ready_Parts2();
	HRESULT Ready_PartsBox();
	void	Set_Info();
	void    Check_Spl();
private:
	CModel*					m_pModelCom = nullptr;

	CGameObject*			m_pWeapon = nullptr;
	CGameObject*			m_pSheath = nullptr;
	CGameObject*			m_pBox = nullptr;
	_float					m_fHpTime = 0.f;
	_int					m_iSkillHit = 0;
	_int					m_iWaterMillHit = 0;
	_int					m_iWindMillHit = 0;
	_bool					m_bRender = false;
	_float					m_fEffectTime = 0.f;
	_float					m_fTest = 0.f;
	//Quest
	_bool					m_bStop = false;
	_bool					m_bQuest1 = false;
	_bool					m_bQuest1MSG = false;
	_bool					m_bQuest2 = false;
	_bool					m_bQuest2MSG = false;
	_bool					m_bQuest2_1MSG = false;
	_bool					m_bQuest2_2MSG = false;
	_bool					m_bQuest3 = false;
	_bool					m_bQuest3MSG = false;
	_bool					m_bQuest3_1MSG = false;
	_bool					m_bQuest3_2MSG = false;
	_bool					m_bSoundCheck = false;

	_bool					m_bIDLE = false;
private:
	OBJDIR m_eDirection = OBJDIR::DIR_END;
	ANIMID m_eAnimID = ANIMID::ANIM_END;

	class CTanjiroState* m_pTanjiroState = nullptr;
	_bool m_bIsKagura = false;

private:
	OBJKEYSTATE m_ePreKeyState = KEY_END;
	OBJKEYSTATE m_eCurKeyState = KEY_END;

public:
	static CTanjiro* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
