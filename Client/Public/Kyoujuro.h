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


		// ATTACK
		ANIM_ATTACK_1 = 10,
		ANIM_ATTACK_2 = 11,
		ANIM_ATTACK_3 = 12,
		ANIM_ATTACK_4 = 15,

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

		ANIM_END = 100
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
	CModel* Get_Model() const { return m_pModelCom; }

	ANIMID Get_AnimIndex() const { return m_eAnimID; }
	void   Set_AnimIndex(ANIMID iAnimIndex) { m_eAnimID = iAnimIndex; }

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();
	void	Set_ShadowLightPos();
	HRESULT Ready_Parts();
	HRESULT Ready_Parts2();
	void	Set_Info();

public:
	virtual void  Take_Damage(_float _fPow,_bool _bJumpHit = 0);
	virtual void  Get_GuardHit(_int eType);
public:
	void Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue);
private:
	CModel*					m_pModelCom = nullptr;
	
	CGameObject*			m_pWeapon = nullptr;
	CGameObject*			m_pSheath = nullptr;

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

