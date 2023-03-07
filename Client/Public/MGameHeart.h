#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CMGameHeart final : public CUI
{
private:
	CMGameHeart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMGameHeart(const CMGameHeart& rhs);
	virtual ~CMGameHeart() = default;

public:
	_bool Get_RenderOn() { return m_bRender; }
	void Set_RenderOn(_bool bCheck) { m_bRender = bCheck; }

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
	_uint			m_iLayerNum = 0;
	_bool			m_bRender = false;


public:
	static CMGameHeart* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
