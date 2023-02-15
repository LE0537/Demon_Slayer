#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CRenderer;
class CTransform;
class CModel;

END

BEGIN(Client)

class CTargetCircle final : public CGameObject
{
public:
	typedef struct tagRangeCircleDesc
	{
		_float3 vPosition;
	}RANGECIRCLEDESC;

private:
	CTargetCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTargetCircle(const CTargetCircle& rhs);
	virtual ~CTargetCircle() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CModel*					m_pModelCom = nullptr;

private:
	RANGECIRCLEDESC			m_tRangeCircleInfo;

private:
	_float					m_fRangeCircleTime = 0.f;
	_float					m_fCircleDeadTime = 0.f;

	_bool					m_bCreateChain = false;
	_bool					m_bCircleTime = false;
	_bool					m_bShaderChange = false;

	_int					m_iShaderPass = 0;


private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

public:
	static CTargetCircle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END