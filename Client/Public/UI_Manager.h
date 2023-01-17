#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "UI.h"
#include "Characters.h"

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

private:
	//플레이어 인포 만들기 그리고 함수 써서 정보 가져오기 hp바 생성할때 던져주기
};

END