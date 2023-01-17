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
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string sLoadName);
	
public:
	void Load_Data(string sLoadName);
	void Add_P1_PersonHpUI();
	void Add_P1_OniHpUI();
	void Add_P2_PersonHpUI();
	void Add_P2_OniHpUI();
	HRESULT Add_Obj(CUI::THROWUIINFO iter);


	_bool P1_Oni_Check();
	_bool P2_Oni_Check();
	void	Set_1P(CCharacters* p1P) { m_p1P = p1P; }
	void	Set_2P(CCharacters* p2P) { m_p2P = p2P; }
private:
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pContext = nullptr;
	list<CUI::LOADUIINFO>				P1_P_LOADDATALIST;
	list<CUI::LOADUIINFO>				P1_O_LOADDATALIST;
	list<CUI::LOADUIINFO>				P2_P_LOADDATALIST;
	list<CUI::LOADUIINFO>				P2_O_LOADDATALIST;

	list<CUI::THROWUIINFO>				P1_P_DATALIST;
	list<CUI::THROWUIINFO>				P1_O_DATALIST;
	list<CUI::THROWUIINFO>				P2_P_DATALIST;
	list<CUI::THROWUIINFO>				P2_O_DATALIST;

	string							m_sFilePath;
	CCharacters*					m_p1P;
	CCharacters*					m_p2P;
	CUI::THROWUIINFO				m_ThrowInfo;

public:
	virtual void Free() override;

private:
	//플레이어 인포 만들기 그리고 함수 써서 정보 가져오기 hp바 생성할때 던져주기
};

END