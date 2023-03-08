#pragma once
#include "GameObj.h"
#include "Characters.h"
BEGIN(Engine)
class CModel;

END

BEGIN(Client)

class CEndingText final : public CGameObj
{
public:
	typedef struct tagTEXT_Desc
	{
		_float4		vPos;

		_uint		iModelIndex;
	
	}TEXT_DESC;

private:
	CEndingText(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEndingText(const CEndingText& rhs);
	virtual ~CEndingText() = default;


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
	TEXT_DESC			m_tMyDesc;
	_float4x4	m_matProjOrigin;


private:
	HRESULT Delete_Component(const _tchar* pComponentTag);

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

	HRESULT Ready_ModelComponent();

public:
	static CEndingText* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
