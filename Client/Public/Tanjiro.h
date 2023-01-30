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
		ANIM_ATTACK_1 =12,
		ANIM_ATTACK_2 = 13,
		ANIM_ATTACK_3 = 14,
		ANIM_ATTACK_4 = 17,

		ANIM_KAGURA_ATTACK_1 = 100,
		ANIM_KAGURA_ATTACK_2 = 101,
		ANIM_KAGURA_ATTACK_3 = 102,
		ANIM_KAGURA_ATTACK_4 = 103,

		// HIT
		ANIM_HIT= 22,


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
		ANIM_SKILL_WINDMILL = 32,


		// COMMON
		ANIM_SKILL_COMMON = 33,


		// KAGURA COMMON
		ANIM_SKILL_KAGURA_COMMON = 104,
		ANIM_SKILL_KAGURA_MOVE = 105,
		ANIM_SKILL_KAGURA_SPHERE = 107,



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
	void   Set_AnimIndex(ANIMID iAnimIndex) {m_eAnimID = iAnimIndex;}

public:
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);

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
