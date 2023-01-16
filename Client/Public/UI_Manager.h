#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "UI.h"


BEGIN(Client)

class CUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

public:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

public:
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	
public:
	void Load_Data();
	HRESULT Add_Obj(CUI::UIINFO CloneInfo);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	list<CUI::UIINFO>				DATATEXTURELIST;

public:
	virtual void Free() override;
};

END