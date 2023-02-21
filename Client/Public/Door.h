#pragma once
#include "GameObj.h"
#include "Characters.h"
BEGIN(Engine)
class CModel;

END

BEGIN(Client)

class CDoor final : public CGameObj
{
public:
	typedef struct tagDoor_Desc
	{
		_float4x4	matWorld;
	
		_uint		iModelIndex;
		CCharacters*  m_pPlayer;
	}DOOR_DESC;

private:
	CDoor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDoor(const CDoor& rhs);
	virtual ~CDoor() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

private:
	CModel*		m_pModelCom = nullptr;

private:
	DOOR_DESC			m_tMyDesc;
	_float4x4	m_matProjOrigin;

	_bool     m_bTurn = false;
	_float    m_fTurnAngle = 0.f;
	_bool     m_bNonAlpha = false;
private:
	HRESULT Delete_Component(const _tchar* pComponentTag);

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

	HRESULT Ready_ModelComponent();
	void	Move_Mesh(_float fTimeDelta);
public:
	static CDoor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
