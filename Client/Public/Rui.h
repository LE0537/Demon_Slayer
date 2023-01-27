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
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit);
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
