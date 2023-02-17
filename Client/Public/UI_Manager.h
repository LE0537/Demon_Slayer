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
	typedef struct tagRankInfo
	{
		_uint iMaximumCombo;
		_uint iRemnantTime;
		_uint iUltUseCount;
		_uint iFriendUseCount;
		_uint iUseSkillCount;
		_uint iRankScore;
		_uint iRank;
	}RANKINFO;

public:
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	
public:
	void Load_Data(string sLoadName);
	void Add_P1_PersonHpUI();
	void Add_P1_OniHpUI();
	void Add_P2_PersonHpUI();
	void Add_P2_OniHpUI();
	void Add_Select_CharUI();
	void Add_Select_Map();
	void Add_Logo_Title();
	void Add_Menu();
	void Add_Loading();
	void Add_BattleUI();
	void Add_BattleResult();
	void Add_P1_Combo();
	void Add_P2_Combo();
	void Add_Adc_Menu();
	void Add_Quiest();
	void Add_AdvBattleUI();
	HRESULT Add_Btl_PlayerUI(CUI::THROWUIINFO iter);
	HRESULT Add_SelectUI(CUI::THROWUIINFO iter);
	HRESULT Add_SelectMapUI(CUI::THROWUIINFO iter);
	HRESULT Add_LogoUI(CUI::THROWUIINFO iter);
	HRESULT Add_MenuUI(CUI::THROWUIINFO iter);
	HRESULT Add_LoadingUI(CUI::THROWUIINFO iter);
	HRESULT Add_BattleResultUI(CUI::THROWUIINFO iter);
	HRESULT Add_Adc_MenuUI(CUI::THROWUIINFO iter);
	HRESULT Add_QuiestUI(CUI::THROWUIINFO iter);
	_bool P1_Oni_Check();
	_bool P2_Oni_Check();


	RANKINFO Get_RankInfo(_uint iPlyNum) { return m_RankInfo[iPlyNum]; }
	void Set_RankInfo(RANKINFO tInfo, _uint iPlyNum) { m_RankInfo[iPlyNum] = tInfo; }
	_uint Get_MaximumCombo(_uint iPlyNum) { return m_RankInfo[iPlyNum].iMaximumCombo; }
	void Set_MaximumCombo(_uint _iCombo, _uint iPlyNum) { m_RankInfo[iPlyNum].iMaximumCombo = _iCombo; }
	_uint Get_RemnantTime(_uint iPlyNum) { return m_RankInfo[iPlyNum].iRemnantTime; }
	void Set_RemnantTime(_uint _iTime, _uint iPlyNum) { m_RankInfo[iPlyNum].iRemnantTime += _iTime; }
	_uint Get_UltUseCount(_uint iPlyNum) { return m_RankInfo[iPlyNum].iUltUseCount; }
	void Set_UltUseCount(_uint _iCount, _uint iPlyNum) { m_RankInfo[iPlyNum].iUltUseCount += _iCount; }
	_uint Get_FriendUseCount(_uint iPlyNum) { return m_RankInfo[iPlyNum].iFriendUseCount; }
	void Set_FriendUseCount(_uint _iFriendCount, _uint iPlyNum) { m_RankInfo[iPlyNum].iFriendUseCount += _iFriendCount; }
	_uint Get_UseSkillCount(_uint iPlyNum) { return m_RankInfo[iPlyNum].iUseSkillCount; }
	void Set_UseSkillCount(_uint _iSkillCount, _uint iPlyNum) { m_RankInfo[iPlyNum].iUseSkillCount += _iSkillCount; }
	_uint Get_Rank(_uint iPlyNum) { return m_RankInfo[iPlyNum].iRank; }
	void Set_Rank(_uint _iRank, _uint iPlyNum) { m_RankInfo[iPlyNum].iRank = _iRank; }
	void RankInfo_ZeroMemory(_uint iPlyNum) { ZeroMemory(&m_RankInfo[iPlyNum], sizeof(RANKINFO)); }
	void Set_RankScore() {
		if (!m_ResultInfo.bPlayerWin)
		{
			m_RankInfo[0].iRankScore =
				m_RankInfo[0].iFriendUseCount * 100 +
				m_RankInfo[0].iRemnantTime * 10 +
				m_RankInfo[0].iUltUseCount * 100 +
				m_RankInfo[0].iUseSkillCount * 50 +
				m_RankInfo[0].iMaximumCombo * 100;
		}
		else
		{
			m_RankInfo[1].iRankScore =
				m_RankInfo[1].iFriendUseCount * 100 +
				m_RankInfo[1].iRemnantTime * 10 +
				m_RankInfo[1].iUltUseCount * 100 +
				m_RankInfo[1].iUseSkillCount * 50 +
				m_RankInfo[1].iMaximumCombo * 100;
		}}
	/*void Set_Rank(_uint _iWinPlyNum) {
		if()
		m_RankInfo[_iWinPlyNum].iRank;
	}*/

	CCharacters* Get_1P() { return m_p1P; }
	CCharacters* Get_1P_2() { return m_p1P_2; }
	void	Set_1P(CCharacters* p1P) { m_p1P = p1P; }
	void	Set_1P_2(CCharacters* p1P_2) { m_p1P_2 = p1P_2; }

	CCharacters* Get_2P() { return m_p2P; }
	CCharacters* Get_2P_2() { return m_p2P_2; }
	void	Set_2P(CCharacters* p2P) { m_p2P = p2P; }
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

	CUI*	Get_Sel1PMain(_uint iLayerNum) { return m_pSel1PMain[iLayerNum]; }
	void	Set_Sel1PMain(CUI* p1PMain, _uint iLayerNum) { m_pSel1PMain[iLayerNum] = p1PMain; }

	CUI*	Get_Sel2PMain(_uint iLayerNum) { return m_pSel2PMain[iLayerNum]; }
	void	Set_Sel2PMain(CUI* p2PMain, _uint iLayerNum) { m_pSel2PMain[iLayerNum] = p2PMain; }

	CUI*	Get_RoundUI() { return m_pRoundUI; }
	void	Set_RoundUI(CUI* pTargetUI) { m_pRoundUI = pTargetUI; }

	CUI*	Get_TimerUI() { return m_pTimerUI; }
	void	Set_TimerUI(CUI* pTargetUI) { m_pTimerUI = pTargetUI; }

	CUI*	Get_SelectFrame(_uint iLayerNum) { return m_pSelectFrame[iLayerNum]; }
	void	Set_SelectFrame(CUI* pTarget, _uint iLayerNum) { m_pSelectFrame[iLayerNum] = pTarget; }

	CUI*	Get_ComboUI(_uint iPlyNum) { return m_pComboNum[iPlyNum]; }
	void	Set_ComboUI(CUI* pTargetUI,_uint iPlyNum) { m_pComboNum[iPlyNum] = pTargetUI; }

	CUI*	Get_AdvFrame(_uint iFrameNum) { return m_pAdvMenuFrame[iFrameNum]; }
	void	Set_AdvFrame(CUI* pTargetUI, _uint iFrameNum) { m_pAdvMenuFrame[iFrameNum] = pTargetUI; }

	CUI*	Get_SelMapIconFrame(_uint iFrameNum) { return m_pSelMapIconFrame[iFrameNum]; }
	void	Set_SelMapIconFrame(CUI* pTargetUI, _uint iFrameNum) { m_pSelMapIconFrame[iFrameNum] = pTargetUI; }

	CUI*	Get_Window(_uint iFrameNum) { return m_pWindow[iFrameNum]; }
	void	Set_Window(CUI* pTargetUI, _uint iFrameNum) { m_pWindow[iFrameNum] = pTargetUI; }

	CUI*	Get_SubBase(_uint iFrameNum) { return m_pSubBase[iFrameNum]; }
	void	Set_SubBase(CUI* pTargetUI, _uint iFrameNum) { m_pSubBase[iFrameNum] = pTargetUI; }

	CUI*	Get_MenuCursor() { return m_pMenuCursor; }
	void	Set_MenuCursor(CUI* MenuCursor) { m_pMenuCursor = MenuCursor; }

	CUI*	Get_RankIcon() { return m_pRankIcon; }
	void	Set_RankIcon(CUI* pTarget) { m_pRankIcon = pTarget; }

	CUI*	Get_SelMapCursor() { return m_pSelMapCursor; }
	void	Set_SelMapCursor(CUI* pTarget) { m_pSelMapCursor = pTarget; }

	CUI*	Get_MsgTextBase() { return m_pMsgTextBase; }
	void	Set_MsgTextBase(CUI* pTarget) { m_pMsgTextBase = pTarget; }
	
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

	wstring Get_Msg() { return m_szMsg; }
	void	Set_Msg(wstring _szMsg) { m_szMsg = _szMsg; }

	wstring Get_MsgName() { return m_szMsgName; }
	void	Set_MsgName(wstring _szMsgName) { m_szMsgName = _szMsgName; }

	_uint   Get_SelMapNum() { return m_iSelMapNum; }
	void	Set_SelMapNum(_uint _iNum) { m_iSelMapNum = _iNum; }

	_uint   Get_AdvStageNum() { return m_iAdvStageNum; }
	void	Set_AdvStageNum(_uint _iNum) { m_iAdvStageNum = _iNum; }

	_uint   Get_MsgCount() { return m_iMsgCount; }
	void	Set_MsgCount(_uint _iNum) { m_iMsgCount += _iNum; }
	void	Reset_MsgCount() { m_iMsgCount = 0; }

	_uint   Get_QuestCount() { return m_iQuestCount; }
	void	Set_QuestCount(_uint _iNum) { m_iQuestCount = _iNum; }

	_bool   Get_MsgOnOff() { return m_bMsgOnoff; }
	void	Set_MsgOn() { m_bMsgOnoff = true; }
	void	Set_MsgOff() { m_bMsgOnoff = false; }


	void	Set_CharNameUIZero() { m_iCharNameLayerNum = 0; }

	_bool	Get_AdvMenuSelCheck() { return m_bAdvMenuSelCheck; }
	void	Set_AdvMenuSelCheck(_bool bCheck) { m_bAdvMenuSelCheck = bCheck; }

	_bool	Get_LevelResultOn() { return m_bLevelResultOn; }
	void	Set_LevelResultOn(_bool bCheck) { m_bLevelResultOn = bCheck; }

	_bool	Get_LevelMenuOn() { return m_bLevelMenu; }
	void	Set_LevelMenuOn(_bool bCheck) { m_bLevelMenu = bCheck; }

	_bool	Get_BattleTypeCheck() { return m_bBattleTypeCheck; }
	void	Set_BattleTypeCheck(_bool bCheck) { m_bBattleTypeCheck = bCheck; }

	CUI::RESULTINFO Get_Result_Info() { return m_ResultInfo; }
	void Set_Result_Info(CUI::RESULTINFO ResultInfo) { m_ResultInfo = ResultInfo; }

	//스토리 데이터 함수
	_float4 Get_PlayerPos() { return m_vPlayerPos; }
	_float4 Get_TargetPos() { return m_vTargetPos; }
	CCharacters* Get_NPC() { return m_pNPC; }
	void Set_PlayerPos(_float4 _vPos) { m_vPlayerPos = _vPos; }
	void Set_TargetPos(_float4 _vPos) { m_vTargetPos = _vPos; }
	void Set_NPC(CCharacters* _pNPC) { m_pNPC = _pNPC; }
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
	list<CUI::LOADUIINFO>				BATTLEUI_LOADDATALIST;
	list<CUI::LOADUIINFO>				BATTLERESULT_LOADDATALIST;
	list<CUI::LOADUIINFO>				P1_COMBO_LOADDATALIST;
	list<CUI::LOADUIINFO>				P2_COMBO_LOADDATALIST;
	list<CUI::LOADUIINFO>				ADC_MENU_LOADDATALIST;
	list<CUI::LOADUIINFO>				SELECTMAP_LOADDATALIST;
	list<CUI::LOADUIINFO>				QUIEST_LOADDATALIST;
	list<CUI::LOADUIINFO>				ADVBATTLEUI_LOADDATALIST;

	list<CUI::THROWUIINFO>				P1_P_DATALIST;
	list<CUI::THROWUIINFO>				P1_O_DATALIST;
	list<CUI::THROWUIINFO>				P2_P_DATALIST;
	list<CUI::THROWUIINFO>				P2_O_DATALIST;
	list<CUI::THROWUIINFO>				SELECT_DATALIST;
	list<CUI::THROWUIINFO>				LOGOTITLE_DATALIST;
	list<CUI::THROWUIINFO>				MENU_DATALIST;
	list<CUI::THROWUIINFO>				LOADING_DATALIST;
	list<CUI::THROWUIINFO>				BATTLEUI_DATALIST;
	list<CUI::THROWUIINFO>				BATTLERESULT_DATALIST;
	list<CUI::THROWUIINFO>				P1_COMBO_DATALIST;
	list<CUI::THROWUIINFO>				P2_COMBO_DATALIST;
	list<CUI::THROWUIINFO>				ADC_MENU_DATALIST;
	list<CUI::THROWUIINFO>				SELECTMAP_DATALIST;
	list<CUI::THROWUIINFO>				QUIEST_DATALIST;
	list<CUI::THROWUIINFO>				ADV_BATTLEUI_DATALIST;

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
	CUI*							m_pSel1PMain[2];
	CUI*							m_pSel2PMain[2];
	CUI*							m_pMenuCursor = nullptr;
	CUI*							m_pRoundUI = nullptr;
	CUI*							m_pTimerUI = nullptr;
	CUI*							m_pComboNum[2];
	CUI*							m_pAdvMenuFrame[2];
	CUI*							m_pSelMapIconFrame[2];
	CUI*							m_pWindow[4];
	CUI*							m_pSubBase[5];
	CUI*							m_pRankIcon = nullptr;
	CUI*							m_pSelMapCursor = nullptr;
	CUI*							m_pMsgTextBase = nullptr;

	CUI::THROWUIINFO				m_ThrowInfo;
	CUI::RESULTINFO					m_ResultInfo;
	RANKINFO						m_RankInfo[2];

	_uint							m_iCharIconLayerNum = 0;
	_uint							m_iCharNameLayerNum = 0;
	_uint							m_iCharFrameLayerNum = 0;
	_uint							m_i1PIconLayerNum = 0;
	_uint							m_i2PIconLayerNum = 0;
	_uint							m_i1PMainLayerNum = 0;
	_uint							m_iLogoButtonNum = 0;
	_uint							m_i2PMainLayerNum = 0;
	_uint							m_iUltBarLayerNum = 0;
	_uint							m_iUltNumLayerNum = 0;
	_uint							m_iTimerLayerNum = 0;
	_uint							m_iSelStampLayerNum = 0;
	_uint							m_iRoundIconLayerNum = 0;
	_uint							m_iCharSelMsgLayerNum = 0;
	_uint							m_iSelNameShadowLayerNum = 0;
	_uint							m_iComboNumLayerNum = 0;
	_uint							m_iChangeBarLayerNum = 0;
	_uint							m_iOniSepcialBarLayerNum = 0;
	_uint							m_iAdcMenuSelImgLayerNum = 0;
	_uint							m_iAdcMenuSelFrameImgLayerNum = 0;
	_uint							m_iResultCloudLayerNum = 0;
	_uint							m_iResultCloudSecondLayerNum = 0;
	_uint							m_iScoreBarLayerNum = 0;
	_uint							m_iLineFrameLayerNum = 0;
	_uint							m_iSelMapIconFrameLayerNum = 0;
	_uint							m_iMapIcomImgLayerNum = 0;
	_uint							m_iSelMapCursorEffLayerNum = 0;
	_uint							m_iSelMapCursorEff2LayerNum = 0;
	_uint							m_iWindowRLayerNum = 0;
	_uint							m_iWindowLLayerNum = 0;
	_uint							m_iAdvStageNum = 0;
	_uint							m_iSelMapNum = 0;
	_uint							m_iSubBaseNum = 0;
	_uint							m_iSubIconLayerNum = 0;
	_uint                           m_i1p = 0;
	_uint                           m_i2p = 0;
	_uint                           m_i1p_2 = 0;
	_uint                           m_i2p_2 = 0;
	
	_uint							m_iMsgCount = 0;
	_uint							m_iQuestCount = 0;
	
	_bool							m_bAdvMenuSelCheck = false;

	_bool							m_bLevelResultOn = false;
	_bool							m_bLevelMenu = false;

	_bool							m_bBattleTypeCheck = false;

	_bool							m_bMsgOnoff = false;
	wstring							m_szMsg = TEXT("신현수 바보");
	wstring							m_szMsgName = TEXT("카마도 탄지로");

	//스토리 데이터 저장
	_float4							m_vPlayerPos, m_vTargetPos;
	CCharacters*					m_pNPC = nullptr;
public:
	virtual void Free() override;
};

END