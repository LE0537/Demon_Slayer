#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CCharIcon final : public CUI
{
private:
	CCharIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharIcon(const CCharIcon& rhs);
	virtual ~CCharIcon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Icon_Selected_GamePlay(wstring strName);
	void Icon_Selected_SelectChar(_uint iLayerNum);

private:
	CTexture*			m_pTextureMaskCom;

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	_uint				m_iMaskImgNum = 0;

public:
	static CCharIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END