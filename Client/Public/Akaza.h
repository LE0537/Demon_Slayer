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


		// ATTACK
		ANIM_ATTACK_1 = 11,
		ANIM_ATTACK_2 = 12,
		ANIM_ATTACK_3 = 13,
		ANIM_ATTACK_4 = 16,

		// JUMP
		ANIM_JUMP_START = 45,
		ANIM_JUMP_LOOP = 46,
		ANIM_JUMP_END = 47,

		// Hit

		ANIM_HIT = 0,

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
	void	Set_ShadowLightPos();
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

	class CAkazaState* m_pAkazaState = nullptr;
public:
	static CAkaza* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
