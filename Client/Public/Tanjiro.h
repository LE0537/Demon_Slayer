#pragma once
#include "Client_Defines.h"
#include "GameObj.h"


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

class CTanjiro : public CGameObj
{
public:
	enum ANIMID
	{
		ANIM_IDLE = 69,
		ANIM_MOVE_START = 72,
		ANIM_MOVE_LOOP = 73,
		ANIM_MOVE_END = 74,
		ANIM_END = 100
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
	CTransform* Get_Transfrom() const { return m_pTransformCom; }
	CModel* Get_Model() const { return m_pModelCom; }

	ANIMID Get_AnimIndex() const { return m_eAnimID; }
	void   Set_AnimIndex(ANIMID iAnimIndex) {m_eAnimID = iAnimIndex;}
private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();
	void	Set_ShadowLightPos();


public:
	void Tanjiro_GoStraight(_float fTimeDelta);
	void Tanjiro_GoBackWard(_float fTimeDelta);
	void Tanjiro_GoRight(_float fTimeDelta);
	void Tanjiro_GoLeft(_float fTimeDelta);

	void Tanjiro_GoLF(_float fTimeDelta);
	void Tanjiro_GoRF(_float fTimeDelta);
	void Tanjiro_GoLB(_float fTimeDelta);
	void Tanjiro_GoRB(_float fTimeDelta);
	


private:
	void HandleInput();
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);


private:
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;

private:
	OBJDIR m_eDirection = OBJDIR::DIR_END;
	ANIMID m_eAnimID = ANIMID::ANIM_END;

	class CTanjiroState* m_pTanjiroState = nullptr;

private:
	OBJKEYSTATE m_ePreKeyState = KEY_END;
	OBJKEYSTATE m_eCurKeyState = KEY_END;

public:
	static CTanjiro* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
