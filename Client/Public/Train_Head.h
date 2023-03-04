#pragma once
#include "GameObj.h"
#include "Characters.h"
BEGIN(Engine)
class CModel;

END

BEGIN(Client)

class CTrain_Head final : public CGameObj
{
public:
	typedef struct tagDoor_Desc
	{
		_float4x4	matWorld;

		_uint		iModelIndex;
		CCharacters*  m_pPlayer;
	}DOOR_DESC;

public:
	_uint Get_ModelIndex() { return m_tMyDesc.iModelIndex; }

	_bool Get_Turn() { return m_bTurn; }
	void Set_ModelNum() { ++m_iModelNum; }
private:
	CTrain_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrain_Head(const CTrain_Head& rhs);
	virtual ~CTrain_Head() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

private:
	CModel*		m_pModelCom[3] = { nullptr };

private:
	DOOR_DESC			m_tMyDesc;
	_float4x4	m_matProjOrigin;

	_bool     m_bTurn = false;
	_bool	  m_bInteractionCheck = false;

	_bool     m_bNonAlpha = false;

	_int	  m_iModelNum = 0;
private:
	HRESULT Delete_Component(const _tchar* pComponentTag);

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

	HRESULT Ready_ModelComponent();

public:
	static CTrain_Head* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
