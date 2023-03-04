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
class CKyoujuro : public CCharacters
{
public:
	enum ANIMID
	{
		// IDLE
		ANIM_IDLE = 57,

		// MOVE
		ANIM_MOVE_START = 63,
		ANIM_MOVE_END = 64,
		

		// JUMP
		ANIM_JUMP_START = 53,
		ANIM_JUMP_LOOP_START = 54,
		ANIM_JUMP_LOOP_END = 55,
		ANIM_JUMP_END = 56,



		// change
		ANIM_CHANGE_UP = 54,
		ANIM_CHANGE_DOWN_START = 55,
		ANIM_CHANGE_DOWN_END = 56,

		// ATTACK
		ANIM_ATTACK_1 = 10,
		ANIM_ATTACK_2 = 11,
		ANIM_ATTACK_3 = 12,
		ANIM_ATTACK_4 = 15,

		ANIM_JUMPATTACK = 2,
		ANIM_JUMP_MOVE_ATTACK_0 = 7,
		ANIM_JUMP_MOVE_ATTACK_1 = 8,
		ANIM_JUMP_MOVE_ATTACK_2 = 9,

		ANIM_TARGET_RUSH_0 = 42,
		ANIM_TARGET_RUSH_1 = 43,
		ANIM_TARGET_RUSH_2 = 44,


		// HIT
		ANIM_HIT = 19,

		// GUARD
		ANIM_GUARD_0 = 47,
		ANIM_GUARD_1 = 48,
		ANIM_GUARD_2 = 49,

		// GUARD ADV
		ANIM_GUARD_ADV = 50,

		// GUARD STATE -> HIT
		ANIM_GUARD_HIT_0 = 51,
		ANIM_GUARD_HIT_1 = 52,

		// DASH
		ANIM_DASH_AB = 65,
		ANIM_DASH_AF = 66,
		ANIM_DASH_AL = 67,
		ANIM_DASH_AR = 68,
		ANIM_DASH_B = 69,
		ANIM_DASH_F = 70,
		ANIM_DASH_L_01 = 71,
		ANIM_DASH_L_02 = 72,
		ANIM_DASH_R_01 = 73,
		ANIM_DASH_R_02 = 74,


		// SKILL

		// DASH SLASH
		ANIM_SKILL_DASHSLASH = 27,

		// DOUBLE UPPER
		ANIM_SKILL_DOUBLEUPPER = 36,

		// COMMON
		ANIM_SKILL_COMMON = 21,

		//JUMP SKILL
		ANIM_SKILL_JUMPCOMMON = 23,
		ANIM_SKILL_JUMPMOVE = 29,


		ANIM_BATTLE_START = 82,

		ANIM_HIT_DMG_F = 83,
		ANIM_HIT_DMG_L = 84,
		ANIM_HIT_DMG_R = 85,
		ANIM_HIT_DMG_U = 86,
		ANIM_HIT_DMG_AF = 87,
		ANIM_HIT_DMG_AL = 88,
		ANIM_HIT_DMG_AR = 89,
		ANIM_HIT_DMG_AU = 90,
		ANIM_HIT_DMG2_F = 91,
		ANIM_HIT_DMG2_G = 92,
		ANIM_HIT_DMG_DOWN_0 = 93,
		ANIM_HIT_DMG_DOWN_1 = 94,
		ANIM_HIT_DMG_DOWN_2 = 95,
		ANIM_HIT_DMG_DOWN_COL = 96,
		ANIM_HIT_DMG_UPPER_0 = 97,
		ANIM_HIT_DMG_UPPER_1 = 98,
		ANIM_HIT_DMG_UPPER_2 = 99,
		ANIM_HIT_DMG_DEATH = 100,


		ANIM_HIT_DMG_RETURN_0 = 58,
		ANIM_HIT_DMG_RETURN_1 = 56,
			
		ANIM_DEAD = 100,


		ANIM_SPLSKL_START_0 = 76,
		ANIM_SPLSKL_START_1 = 78,
		ANIM_SPLSKL_END = 75,

		ANIM_END = 101
	};

private:
	CKyoujuro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKyoujuro(const CKyoujuro& rhs);
	virtual ~CKyoujuro() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

public:
//	CTransform* Get_Transfrom() const { return m_pTransformCom; }
	virtual	CModel* Get_Model() const { return m_pModelCom; }

	ANIMID Get_AnimIndex() const { return m_eAnimID; }
	void   Set_AnimIndex(ANIMID iAnimIndex) { m_eAnimID = iAnimIndex; }

	void   Set_SplWeapon(_bool _bTrue) { m_bSplWeapon = _bTrue; }
	void   Reset_SplWeapon();
	_bool  Get_SplWeapon() { return m_bSplWeapon; }
private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();
	void	Set_ShadowLightPos();
	HRESULT Ready_Parts();
	HRESULT Ready_Parts2();
	void	Set_Info();
	void    Check_Spl();

public:
	virtual void  Take_Damage(_float _fPow,_bool _bJumpHit = 0);
	virtual void  Get_GuardHit(_int eType);
	virtual void  Player_TakeDown(_float _fPow, _bool _bJump = 0);
	virtual void  Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower);
	virtual void  Play_Scene();

public:
	void Play_AkazaScene();
	void Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue);
private:
	CModel*					m_pModelCom = nullptr;
	
	CGameObject*			m_pWeapon = nullptr;
	CGameObject*			m_pSheath = nullptr;
	_float					m_fEffectTime = 0.f;

	_bool					m_bStop = false;
	_bool					m_bQuest1 = false;
	_bool					m_bQuest1MSG = false;
	_bool					m_bQuest2 = false;
	_bool					m_bQuest2MSG = false;
	_bool					m_bQuest2_1MSG = false;
	_bool					m_bQuest2_2MSG = false;

	_bool					m_bSplWeapon = false;
	_bool					m_bWeaponTurn = false;
private:
	void HandleInput();
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);


private:
	OBJDIR m_eDirection = OBJDIR::DIR_END;
	ANIMID m_eAnimID = ANIMID::ANIM_END;

	class CKyoujuroState* m_pKyoujuroState = nullptr;

public:
	static CKyoujuro* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END

