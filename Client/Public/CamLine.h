#pragma once
#include "GameObj.h"

BEGIN(Engine)
class CModel;
class CVIBuffer;
class CVIBuffer_Line;
END

BEGIN(Client)

class CCamLine final : public CGameObj
{
public:
	typedef struct tagCamLineDesc
	{
		CCamLine*	pMe = nullptr;

		_float3 vPos[4];
		_float3 vColor;

	}CAMLINEDESC;
private:
	CCamLine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamLine(const CCamLine& rhs);
	virtual ~CCamLine() = default;

public:
	void Set_Pos(_float fX, _float fY, _float fZ);
	void Set_Pos(_float3 vPos[4]);
	void	Set_Rendering(_bool bTrueisOn) { m_bRender = bTrueisOn; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	_float3					m_vPos[4];
	_float3					m_vColor;
	CVIBuffer_Line*			m_pVIBuffer_LineCom = nullptr;

	_bool		m_bRender = true;

private:/*Cube*/
	CShader*			m_pShader_ModelCom = nullptr;
	CModel*				m_pModelCom = nullptr;

public:
	static CCamLine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END