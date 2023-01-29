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
	void Load_Data(string sLoadName);
	void Add_P1_PersonHpUI();
	void Add_P1_OniHpUI();
	void Add_P2_PersonHpUI();
	void Add_P2_OniHpUI();
	void Add_Select_CharUI();
	void Add_Logo_Title();
	void Add_Menu();
	void Add_Loading();
	HRESULT Add_Btl_PlayerUI(CUI::THROWUIINFO iter);
	HRESULT Add_SelectUI(CUI::THROWUIINFO iter);
	HRESULT Add_LogoUI(CUI::THROWUIINFO iter);
	HRESULT Add_MenuUI(CUI::THROWUIINFO iter);
	HRESULT Add_LoadingUI(CUI::THROWUIINFO iter);

	_bool P1_Oni_Check();
	_bool P2_Oni_Check();
	void	Set_1P(CCharacters* p1P) { m_p1P = p1P; }
	void	Set_2P(CCharacters* p2P) { m_p2P = p2P; }

	void	Set_1P_2(CCharacters* p1P_2) { m_p1P_2 = p1P_2; }
	void	Set_2P_2(CCharacters* p2P_2) { m_p2P_2 = p2P_2; }

	CUI*	Get_1PCursor() { return m_p1PUI; }
	void	Set_1PCursor (CUI* p1PCursor) { m_p1PUI = p1PCursor; }

	CUI*	Get_2PCursor() { return m_p2PUI; }
	void	Set_2PCursor(CUI* p2PCursor) { m_p2PUI = p2PCursor; }

	CUI*	Get_1PChar() { return m_p1PChar; } // 레벨 캐릭터 넘겨줄때 이거사용 Get_ImgNum() 사용
	void	Set_1PChar(CUI* p1PChar) { m_p1PChar = p1PChar; }

	CUI*	Get_2PChar() { return m_p2PChar; } //레벨 캐릭터 넘겨줄때 이거사용 Get_ImgNum() 사용
	void	Set_2PChar(CUI* p2PChar) { m_p2PChar = p2PChar; }

	CUI*	Get_1P_2Char() { return m_p1P_2Char; } // 레벨 캐릭터 넘겨줄때 이거사용 Get_ImgNum() 사용
	void	Set_1P_2Char(CUI* p1P_2Char) { m_p1P_2Char = p1P_2Char; }

	CUI*	Get_2P_2Char() { return m_p2P_2Char; } //레벨 캐릭터 넘겨줄때 이거사용 Get_ImgNum() 사용
	void	Set_2P_2Char(CUI* p2P_2Char) { m_p2P_2Char = p2P_2Char; }

	CUI*	Get_Sel1PMain() { return m_pSel1PMain; }
	void	Set_Sel1PMain(CUI* p1PMain) { m_pSel1PMain = p1PMain; }

	CUI*	Get_Sel2PMain() { return m_pSel2PMain; }
	void	Set_Sel2PMain(CUI* p2PMain) { m_pSel2PMain = p2PMain; }

	CUI*	Get_SelectFrame(_uint iLayerNum) { return m_pSelectFrame[iLayerNum]; }
	void	Set_SelectFrame(CUI* pTarget, _uint iLayerNum) { m_pSelectFrame[iLayerNum] = pTarget; }

	_uint   Get_Sel1P() { return m_i1p; }
	void	Set_Sel1P(_uint _i1p) { m_i1p = _i1p; }

	_uint   Get_Sel1P_2() { return m_i1p_2; }
	void	Set_Sel1P_2(_uint _i1p_2) { m_i1p_2 = _i1p_2; }
	

	_uint   Get_Sel2P() { return m_i2p; }
	void	Set_Sel2P(_uint _i2p) { m_i2p = _i2p; }

	_uint   Get_Sel2P_2() { return m_i2p_2; }
	void	Set_Sel2P_2(_uint _i2p_2) { m_i2p_2 = _i2p_2; }
	//0일때 아카자
	//1일때 쿄쥬로
	//2일때 루이
	//3일때 탄지로

	CUI*   Get_MenuCursor() { return m_pMenuCursor; }
	void	Set_MenuCursor(CUI* MenuCursor) { m_pMenuCursor = MenuCursor; }


	void	Set_CharNameUIZero() { m_iCharNameLayerNum = 0; }

private:
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pContext = nullptr;
	list<CUI::LOADUIINFO>				P1_P_LOADDATALIST;
	list<CUI::LOADUIINFO>				P1_O_LOADDATALIST;
	list<CUI::LOADUIINFO>				P2_P_LOADDATALIST;
	list<CUI::LOADUIINFO>				P2_O_LOADDATALIST;
	list<CUI::LOADUIINFO>				SELECT_LOADDATALIST;
	list<CUI::LOADUIINFO>				LOGOTITLE_LOADDATALIST;
	list<CUI::LOADUIINFO>				MENU_LOADDATALIST;
	list<CUI::LOADUIINFO>				LOADING_LOADDATALIST;

	list<CUI::THROWUIINFO>				P1_P_DATALIST;
	list<CUI::THROWUIINFO>				P1_O_DATALIST;
	list<CUI::THROWUIINFO>				P2_P_DATALIST;
	list<CUI::THROWUIINFO>				P2_O_DATALIST;
	list<CUI::THROWUIINFO>				SELECT_DATALIST;
	list<CUI::THROWUIINFO>				LOGOTITLE_DATALIST;
	list<CUI::THROWUIINFO>				MENU_DATALIST;
	list<CUI::THROWUIINFO>				LOADING_DATALIST;

	string							m_sFilePath;
	CCharacters*					m_p1P;
	CCharacters*					m_p2P;
	CCharacters*					m_p1P_2;
	CCharacters*					m_p2P_2;
	CUI*							m_p1PUI;
	CUI*							m_p2PUI;
	CUI*							m_p1PChar;
	CUI*							m_p2PChar;
	CUI*							m_p1P_2Char;
	CUI*							m_p2P_2Char;
	CUI*							m_pSelectFrame[6];
	CUI*							m_pSel1PMain;
	CUI*							m_pSel2PMain;
	CUI*							m_pMenuCursor = nullptr;

	CUI::THROWUIINFO				m_ThrowInfo;
	_uint							m_iCharIconLayerNum = 0;
	_uint							m_iCharNameLayerNum = 0;
	_uint							m_iCharFrameLayerNum = 0;
	_uint							m_i1PIconLayerNum = 0;
	_uint							m_i2PIconLayerNum = 0;
	_uint							m_iUltBarLayerNum = 0;
	_uint                           m_i1p = 0;
	_uint                           m_i2p = 0;
	_uint                           m_i1p_2 = 0;
	_uint                           m_i2p_2 = 0;
	_uint							m_iLogoButtonNum = 0;

public:
	virtual void Free() override;
};

END