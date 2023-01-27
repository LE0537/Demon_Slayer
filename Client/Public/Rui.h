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


		// ATTACK
		ANIM_ATTACK_1 = 13,
		ANIM_ATTACK_2 = 15,
		ANIM_ATTACK_3 = 18,
		ANIM_ATTACK_4 = 24,

		// JUMP
		ANIM_JUMP_START = 57,
		ANIM_JUMP_LOOP = 58,
		ANIM_JUMP_END = 59,


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

		ANIM_HIT = 0,

		ANIM_END = 100
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
	CModel* Get_Model() const { return m_pModelCom; }

	ANIMID Get_AnimIndex() const { return m_eAnimID; }
	void   Set_AnimIndex(ANIMID iAnimIndex) { m_eAnimID = iAnimIndex; }
public:
	void Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue);
private:
	void HandleInput();
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);

	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();

	void	Set_ShadowLightPos();
	void	Set_Info();

public:
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);

private:
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;


	CGameObject*			m_pTrail = nullptr;

private:
	OBJDIR m_eDirection = OBJDIR::DIR_END;
	ANIMID m_eAnimID = ANIMID::ANIM_END;

	class CRuiState* m_pRuiState = nullptr;

public:
	static CRui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
