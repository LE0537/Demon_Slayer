#pragma once

#include "Client_Defines.h"
#include "GameObj.h"

BEGIN(Client)

class CHpBarDeco final : public CGameObj
{
private:
	CHpBarDeco(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHpBarDeco(const CHpBarDeco& rhs);
	virtual ~CHpBarDeco() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;
	_uint					m_iImgNum = 0;

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static CHpBarDeco* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END