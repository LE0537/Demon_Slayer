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


		// ATTACK
		ANIM_ATTACK_1 = 90,
		ANIM_ATTACK_2 = 0,
		ANIM_ATTACK_3 = 1,
		ANIM_ATTACK_4 = 2,

		// JUMP
		ANIM_JUMP_START = 31,
		ANIM_JUMP_LOOP = 32,
		ANIM_JUMP_END = 33,


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

		ANIM_HIT = 65,


		// SKILL

		// SHOOT
		ANIM_SKILL_COMMON_0 = 60,
		ANIM_SKILL_COMMON_1 = 61,
		ANIM_SKILL_COMMON_2 = 62,

		// PUNCH

		ANIM_SKILL_PUNCH_0 = 25,
		ANIM_SKILL_PUNCH_1 = 26,
		ANIM_SKILL_PUNCH_2 = 27,

		// DESTROY

		ANIM_SKILL_DESTROY_0 = 32,
		ANIM_SKILL_DESTROY_1 = 15,


		ANIM_END = 100
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

	void	Set_Info();
public:
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);
private:
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;

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
