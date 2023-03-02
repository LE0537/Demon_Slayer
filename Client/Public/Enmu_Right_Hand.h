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

class CEnmu_Right_Hand : public CCharacters
{
public:
	enum ANIMID
	{
		ANIM_IDLE = 40,
		ANIM_PATTERN1_0 = 28,
		ANIM_PATTERN1_1 = 29,
		ANIM_PATTERN1_2 = 30,
		ANIM_PATTERN1_3 = 31,
		ANIM_PATTERN1_4 = 32,

		ANIM_PATTERN2_0 = 21,
		ANIM_PATTERN2_1 = 22,
		ANIM_PATTERN2_2 = 23,
		
		ANIM_PATTERN3_0 = 36,
		ANIM_PATTERN3_1 = 37,
		ANIM_PATTERN3_2 = 38,


		ANIM_PATTERN4_0 = 1,
		ANIM_PATTERN4_1 = 2,
		ANIM_PATTERN4_2 = 3,
		ANIM_PATTERN4_3 = 4,
		ANIM_PATTERN4_4 = 5,


		ANIM_PATTERN5_0 = 11,
		ANIM_PATTERN5_1 = 12,
		ANIM_PATTERN5_2 = 13,
		ANIM_PATTERN5_3 = 14,
		ANIM_PATTERN5_4 = 15,


	};
public:
	CEnmu_Right_Hand(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnmu_Right_Hand(const CEnmu_Right_Hand& rhs);
	virtual ~CEnmu_Right_Hand() = default;

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

	void   Set_ShadowAlphaIncrease(_bool bShadow) { m_bShadowAlphaIncrease = bShadow; }
	void   Set_ShadowAlphaDecrease(_bool bShadow) { m_bShadowAlphaDecrease = bShadow; }
	_bool  Get_QuestStop() const { return m_bQuestStop; }
	_bool  Get_Start() const { return m_bStart; }
	void   Set_Start(_bool bStart) { m_bStart = bStart; }
	void   Set_Render(_bool bRender) { m_bRender = bRender; }
	_bool  Get_Tick() const { return m_bTick; }
	void   Set_Tick(_bool bTick) { m_bTick = bTick; }
	void	Set_CollBox(_bool _bTrue) { m_bCollBox = _bTrue; }
	_float4    Check_CollPos();



	_vector GET_RightHandOriginLook() { return m_vOriginalLook; }
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
	virtual void  Player_TakeDown(_float _fPow, _bool _bJump = 0);
	virtual void  Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower);
	virtual void  Play_Scene();
private:
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	_float					m_fEffectTime = 0.f;
	CCharacters* m_pTanjiro = nullptr;
	_bool					m_bIsCreate = false;
	_bool					m_bHit = false;

private:
	ANIMID m_eAnimID;

	_bool	m_bShadowAlphaIncrease = false;
	_bool	m_bShadowAlphaDecrease = false;

	_matrix m_ShadowMatrix;

	_bool	m_bStart = false;
	_bool   m_bAiMode = false;
	_bool   m_bRender = true;

	_bool   m_bTick = false;

	_bool					m_bQuestStart = false;
	_bool					m_bQuestStop = false;

	_bool	m_bCollBox = false;

	_vector m_vOriginalLook;
public:
	static CEnmu_Right_Hand* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;


};

END
