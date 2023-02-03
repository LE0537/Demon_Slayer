#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CCharNameUI final : public CUI
{
private:
	CCharNameUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharNameUI(const CCharNameUI& rhs);
	virtual ~CCharNameUI() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Name_Selected(wstring strName);
	void Set_Name_SelLevel();
	_float Select_NameReSize();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	_bool				m_bChangeBefore;
	wstring				m_sBeforeName;

	class CCharacters*  m_pModel = nullptr;

public:
	static CCharNameUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END