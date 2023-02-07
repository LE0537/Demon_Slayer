#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Data_Manager.h"	// 추가

#include "Camera_Dynamic.h"
#include "BackGround.h"
#include "Terrain.h"
#include "SkyBox.h"

//Map
#include "BattleField.h"
#include "MeshObj_Static.h"
#include "MeshObj_Static_Inst.h"

//Player 
#include "Player.h"
//char
#include "Rui.h"
#include "Tanjiro.h"
#include "Kyoujuro.h"
#include "Akaza.h"
#include "Nezuko.h"
#include "Shinobu.h"
#include "MenuModel.h"
//parts
#include "KyoujuroWeapon.h"
#include "KyoujuroSheath.h"
#include "TanjiroWeapon.h"
#include "TanjiroSheath.h"
#include "ShinobuWeapon.h"
#include "ShinobuSheath.h"
#include "KyoujuroWeaponMenu.h"
#include "ShinobuWeaponMenu.h"
//UI
#include "HpBar.h"
#include "HpBarBack.h"
#include "HpBarDeco.h"
#include "SkillBar.h"
#include "GaugeBase.h"
#include "CharIcon.h"
#include "CharNameUI.h"
#include "SkillBarParts.h"
#include "ChangeBaseDeco.h"
#include "ChangeSprt.h"
#include "ChangeIcon.h"
#include "SelectBG.h"
#include "PatternWind.h"
#include "PatternOne.h"
#include "CloudUI.h"
#include "PlayerNumIcon.h"
#include "CharSelBg.h"
#include "WindowBase.h"
#include "WindowRight.h"
#include "WindowLeft.h"
#include "CharFrameEff.h"
#include "SelNameShadow.h"
#include "CharFrame.h"
#include "SelP1Cursor.h"
#include "SelP2Cursor.h"
#include "OniIcon.h"
#include "1PIcon.h"
#include "2PIcon.h"
#include "SelOniLight.h"
#include "1PMainOnBase.h"
#include "2PMainOnBase.h"
#include "LogoBackEff.h"
#include "LogoBackLight.h"
#include "LogoButton.h"
#include "LogoFixedImg.h"
#include "MenuBackDeco.h"
#include "MenuCursor.h"
#include "MenuDarkCloud.h"
#include "MenuFrontCloud.h"
#include "MenuFixedImg.h"
#include "MenuChar.h"
#include "MenuTitle.h"
#include "InkEff.h"
#include "BtlFixedImg.h"
#include "GaugeLight.h"
#include "OniChanBarEff.h"
#include "PlyChanBarEff.h"
#include "UltBar.h"
#include "UltStockBase.h"
#include "UltGaugeDeco.h"
#include "UltStockEff.h"
#include "UltStockFrame.h"
#include "UltStockNum.h"
#include "KeyUI.h"
#include "SelStamp.h"
#include "RoundIcon.h"
#include "RoundUI.h"
#include "NumTimer.h"
#include "ResultCheckUI.h"
#include "ResultFrame.h"
#include "ResultCloud.h"
#include "ResultUnder.h"
#include "TxtWinLight.h"
#include "TxtWinUI.h"
#include "RankEff.h"
#include "RankIcon.h"
#include "RankFuda.h"
#include "CharSelMsg.h"
#include "ProgressBar.h"
#include "ComboBase.h"
#include "ComboHitBase.h"
#include "ComboHitDeco.h"
#include "ComboHitTxt.h"
#include "ComboNum.h"
//Effect
#include "Effect.h"
#include "Effect_Manager.h"
#include "Effect_Texture.h"
#include "Effect_Mesh.h"
#include "Effect_Particle.h"
//CollBox
#include "BaseAtk.h"
#include "WaterMill.h"
#include "WindMill.h"
#include "KyoujuroSkill.h"
#include "RuiAtk.h"
#include "AkazaShoot.h"
#include "RuiShoot.h"
#include "RuiSphere.h"
#include "KyoujuroJumpSkill.h"
#include "KaguraSkill.h"
#include "RuiBall.h"
#include "RuiBigBall.h"
CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

unsigned int APIENTRY Thread_Main(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_SELECTCHAR:
		pLoader->Loading_ForSelectCharLevel();
		break;
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlayLevel();
		break;
	case LEVEL_GAMERESULT:
		pLoader->Loading_ForGameResult();
		break;
	}
	
	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	CoInitializeEx(nullptr, 0);

	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	_matrix			PivotMatrix = XMMatrixIdentity();



	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("                       텍스쳐 로딩 중."));

#pragma region UI
	{
#pragma region BattleUI
		//Battle
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ComboHitTimeMask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Combo/Hit_Gauge_Mask.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ComboNum"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Combo/Num_Combo%d.png"), 10))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ComboHitDeco"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Combo/Hit_Gauge_Shadow.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ComboHitBase"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Combo/HUD_Hit_Base.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ComboHitTxt"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Combo/Text_Hit.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ComboBase"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Combo/Base_Combo.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TxtWin"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Txt_Win.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TxtWinEff"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Result_Win_White.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ResultUnder"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Result_Under_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ResultFrame"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Result_Frame.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ResultCheckUI"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Reslt_Ply_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RankFuda"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Rank_Fuda_%d.png"), 5))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RankIcon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Rank_%d.png"), 5))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RankIconEff"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Eff_Rank_%d.png"), 5))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ResultCloud"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/cloud_%d.png"), 5))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NumTimer"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Num_Timer/Num_Timer%d.png"), 10))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RoundIcon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Icon_Round_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RoundUI"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/RoundUI/Round_%d.png"), 5))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RoundJudgmentUI"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/RoundUI/Round_Judgment_%d.png"), 3))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RoundFightUI"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/RoundUI/RoundFight_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RoundWinUI"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/RoundUI/Lang_Round_Win_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharMask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Mask_Chara_%d.png"), 4))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_KeyUI"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/kb_U_00.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltStockNum"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Lang_Num_Stock.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltStockFrame"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Ult_Stock_Frame_%d.png"), 6))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltStockEff"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Ult_Stock_Eff_%d.png"), 7))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltGaugeDeco"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Utl_Gauge_Deco_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltStockBase"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Ult_Stock_Base_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Ult_Gauge_Tex.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PlyChanBarEff"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Support_Gauge_Eff_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_OniChanBarEff"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Oni_Gauge_Eff_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GaugeLight"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Gauge_Color.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BtlFixedImg"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Fixed_Img_%d.png"), 13))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Gauge_Bar/Bar_Life_%d.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpBarDeco"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Gauge_Deco/HpBar_Deco_%d.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpBarBack"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Gauge_Bar/Bar_LifeBack_%d.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GaugeBase"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Gauge_Base_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharIcon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Char_Icon/C_Icon_%d.png"), 52))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharNameUI"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Char_Name/Name_%d.png"), 6))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkillBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Gauge_Bar/Bar_Skill_%d.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkillBarParts"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Gauge_Bar/Gauge_Scaleparts.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChangeBaseDeco"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Change/Base_Deco.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChangeSprt"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Change/Base_Sprt.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChangeIcon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Change/Icon_Change_Btl_%d.png"), 3))))
			return E_FAIL;
#pragma endregion BattleUI

#pragma region SelectUI
		//SelectChar
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MaskWindowLeft"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Char_Window_Mask1.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MaskWindowRight"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Char_Window_Mask2.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharSelMsg"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MsgUI/Profile_Voice_Base.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelectBG"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/CharSel_Bg.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharIconMaskMap"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Mask_Chara1.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PatternWind"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Wind00.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pattern1"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Pattern00.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CloudUI"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Charsel_Cloud_%d.png"), 7))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PalyerNumIcon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/PlayerNumIcon_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharSelBg"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/CharSel_Bg4.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WindowBase"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Char_Window_Base.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WindowRight"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Char_Window2.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WindowLeft"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Char_Window1.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelFrameEff"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Xef_Fade01.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelNameShdow"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Name_Shadow.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelFrame"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Char_Icon_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelCursor"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Char_Cursor_%d.png"), 3))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_OniIcon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Icon_Oni_Chara.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_1P_SelIcon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Cursor_1p_Icon.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_2P_SelIcon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Cursor_2p_Icon.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_OniLight"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Oni_Light.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_1P_MainOnBase"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/1PMain_On_Base.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_2P_MainOnBase"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/2PMain_On_Base.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelStamp"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/Charsel_stamp02.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharSel_BgWind"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/CharSel_Bg_Ani2.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharSel_BgGara"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/SelectChar_UI/CharSel_Bg_Gara.png"), 1))))
			return E_FAIL;


#pragma endregion SelectUI

#pragma region LogoTitleUI
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LogoFixedImg"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Logo/Logo_FixedImg_%d.png"), 4))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LogoBackEff"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Logo/Back_Eff.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LogoBackLight"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Logo/Back_Light.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LogoButton"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Logo/Logo_Button_%d.png"), 2))))
			return E_FAIL;
#pragma endregion LogoTitleUI

#pragma region MenuUI
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuFixedImg"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/MenuFixedImg_%d.png"), 4))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuDarkCloud"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Cloud_Dark_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuFrontCloud"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Cloud_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuCursor"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Cursor.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuBackDeco"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Mode_Back_Deco00_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ModeTitle_Adv"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Lang_ModeTitle_Adv_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ModeTitle_Vs"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Lang_ModeTitle_Vs_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuChar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/MenuChar_%d.png"), 2))))
			return E_FAIL;
#pragma endregion MenuUI

#pragma region UIEff
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_InkEff"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/UIEff/Xcmn_Ink_%d.png"), 2))))
			return E_FAIL;
#pragma endregion UIEff

#pragma region LoadingUI
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingFixedImg"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Fixed_%d.png"), 4))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingCloud"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Cloud_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingShoji"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Shoji%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingShojiL"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Shoji_L.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingShojiR"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Shoji_R.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingBaseTitle"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Base_Title01.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingTxt"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Base_Txt.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Gauge_Nakami.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingAnim"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Load_Anim.png"), 1))))
			return E_FAIL;
#pragma endregion LoadingUI

	}
#pragma endregion UI


	/* 모델 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("                     모델 로딩 중."));


#pragma region Static Objects
	{
		PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

		////	Tree
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_BigTree1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/BigTree1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_BigTree2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/BigTree2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_BigTree3",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/BigTree3.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeFar1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/FarTree1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeWillow",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeWillow.fbx", PivotMatrix))))
		//	return E_FAIL;
		////	Tree Instancing
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_BigTree1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/BigTree1.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_BigTree2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/BigTree2.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_BigTree3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/BigTree3.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeFar1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/FarTree1.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeWillow_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeWillow.fbx", PivotMatrix))))	return E_FAIL;

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeWillow"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeWillow_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	TreeBroken
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken1", 
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken1.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken3",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken3.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken4",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken4.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken5",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken5.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken6",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken6.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken7",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken7.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken8",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken8.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken9",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken9.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken10",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken10.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken1.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken2.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken3.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken4_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken4.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken5_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken5.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken6_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken6.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken7_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken7.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken8_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken8.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken9_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken9.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeBroken10_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeBroken10.fbx", PivotMatrix))))	return E_FAIL;

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken4"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken5"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken6"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken7"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken8"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken9"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken10"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken4_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken5_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken6_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken7_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken8_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken9_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken10_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	Tree Root
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeRoot1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeRoot1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeRoot2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeRoot2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeRoot3",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/TreeRoot3.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeRoot1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeRoot1.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeRoot2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeRoot2.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeRoot3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/TreeRoot3.fbx", PivotMatrix))))	return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	Rock
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Rock1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Rock2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock3",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Rock3.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock4",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Rock4.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock5",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Rock5.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock6",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Rock6.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock7",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Rock7.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_RockSmall",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/RockSmall1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Rock1.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Rock2.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Rock3.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock4_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Rock4.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock5_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Rock5.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock6_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Rock6.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rock7_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Rock7.fbx", PivotMatrix))))	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_RockSmall_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/RockSmall1.fbx", PivotMatrix))))	return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock4"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock5"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock6"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock7"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock_Small"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock4_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock5_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock6_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock7_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock_Small_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	cliff
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff3",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff3.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff_Small",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff_Small.fbx", PivotMatrix))))
		//	return E_FAIL;
		////	cliff Instancing
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff2.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff3.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff_Small_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff_Small.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff_Small"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff_Small_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	grass
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass3",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass3.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass4",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass4.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass5",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/grass1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass6",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/grass2.fbx", PivotMatrix))))
		//	return E_FAIL;
		////	grass Instancing
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass2.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass3.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass4_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass4.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass5_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/grass1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass6_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/grass2.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass4"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass5"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass6"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass4_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass5_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass6_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	Flower
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Lavender",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/lavender.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flower1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Flower1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flower2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Flower2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flower3",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Flower3.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Lavender_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/lavender.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flower1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Flower1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flower2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Flower2.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flower3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Flower3.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Lavender"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Lavender_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	Leaf
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Leaf1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Leaf1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Leaf2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Leaf2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Leaf3",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Leaf3.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Leaf4",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Leaf4.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Leaf1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Leaf1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Leaf2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Leaf2.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Leaf3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Leaf3.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Leaf4_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Leaf4.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf4"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf4_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	Hill Far
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/Hill_Far1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/Hill_Far2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far3",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Far1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far4",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Far2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far5",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/FarHill1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far6",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/FarHill2.fbx", PivotMatrix))))
		//	return E_FAIL;
		////	Hill Far Instancing
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/Hill_Far1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/Hill_Far2.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Far1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far4_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Far2.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far5_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/FarHill1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hill_Far6_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/FarHill2.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far4"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far5"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far6"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far4_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far5_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far6_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	Wall
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Wall1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Wall1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Wall2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Wall2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Wall1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Wall1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Wall2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Wall2.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Wall1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Wall2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Wall1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Wall2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	Spider Web
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/SpiderWeb1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/SpiderWeb2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb3",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/SpiderWeb3.fbx", PivotMatrix))))
		//	return E_FAIL;
		////	Spider Web Instancing
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/SpiderWeb1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/SpiderWeb2.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/SpiderWeb3.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	Bush
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Bush1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Bush2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush3",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Bush3.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush4",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Bush4.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush5",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Bush5.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush6",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Bush6.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush7",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Bush7.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush8",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Bush8.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush9",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/Bush9.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Bush1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Bush2.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Bush3.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush4_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Bush4.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush5_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Bush5.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush6_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Bush6.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush7_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Bush7.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush8_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Bush8.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Bush9_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/Bush9.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush4"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush5"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush6"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush7"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush8"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush9"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush4_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush5_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush6_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush7_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush8_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush9_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		////	Home
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_HomeSmall1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/HomeSmall1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_HomeSmall2",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/HomeSmall2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_HomeSmall1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/HomeSmall1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_HomeSmall2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/HomeSmall2.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("HomeSmall1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("HomeSmall2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("HomeSmall1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("HomeSmall2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	Rice Field
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_RiceField1",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/common/RiceField1.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_RiceField1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/common/RiceField1.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RiceField"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RiceField_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	Ground
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_RuiGround",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/Ground.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_UrokodakiGround",
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/urokodakinoiwa/Ground2.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_RuiGround_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui/Ground.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_UrokodakiGround_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/urokodakinoiwa/Ground2.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiGround"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("UrokodakiGround"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiGround_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("UrokodakiGround_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		////	Else
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_RuiGround2", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Ground.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Home1", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Home1.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rubble1", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Rubble1.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rubble2", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Rubble2.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb4", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb1.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb5", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb2.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb6", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb3.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb7", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb4.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb8", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb5.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb9", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb6.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Tree_Jenitsu", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Tree_Jenitsu.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeFar2", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/TreeFar2.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeFar3", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui_new/TreeFar3.fbx", PivotMatrix)))) 
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_RuiGround2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Ground.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Home1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Home1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rubble1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Rubble1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Rubble2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Rubble2.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb4_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb1.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb5_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb2.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb6_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb3.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb7_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb4.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb8_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb5.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_SpiderWeb9_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/SpiderWeb6.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Tree_Jenitsu_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Tree_Jenitsu.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeFar2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/TreeFar2.fbx", PivotMatrix)))) return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreeFar3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/TreeFar3.fbx", PivotMatrix)))) return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiGround2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Home1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rubble1"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rubble2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb4"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb5"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb6"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb7"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb8"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb9"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Tree_Jenitsu"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar2"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar3"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiGround2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Home1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rubble1_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rubble2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb4_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb5_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb6_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb7_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb8_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb9_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Tree_Jenitsu_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar2_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar3_Instancing"), LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM_INSTANCING);

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Moon", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/Rect/Moon/Moon.fbx", PivotMatrix)))) 
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_MoonLight", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/Rect/Moon/MoonLight.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Moon_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Ground.fbx", PivotMatrix)))) return E_FAIL;


		//	MeshObj_Static
		/*For.Prototype_GameObject_MeshObj_Static */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MeshObj_Static"),
			CMeshObj_Static::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		//	MeshObj_Static_Insatncing
		/*For.Prototype_GameObject_MeshObj_Static_Instancing */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MeshObj_Static_Instancing"),
			CMeshObj_Static_Inst::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		
		PivotMatrix = XMMatrixIdentity();
	}
#pragma endregion Static Objects

#pragma region Map
	{
	//	Skybox
		/*For.Prototype_Component_Texture_SkyBox */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkyBox"), CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/SkyBox/SkyBox_11.dds", 1)))) return E_FAIL;
		/*For.Prototype_GameObject_NewTerrain */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkyBox"), CSkyBox::Create(m_pDevice, m_pContext)))) return E_FAIL;


		//	Terrain
		/*For.Prototype_Component_Texture_TerrainDiffuse */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TerrainDiffuse"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Map/Textures/Diffuse_%d.png", 12))))
			return E_FAIL;
		/*For.Prototype_Component_Texture_TerrainNormal */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TerrainNormal"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Map/Textures/Normal_%d.png", 12))))
			return E_FAIL;


		//	MeshObj_Static
		/*For.Prototype_GameObject_NewTerrain */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NewTerrain"),
			CTerrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}
#pragma endregion Map

#pragma region Effect
	{
		/* Texture */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Effect/Texture/Particle/Particle%d.png"), 42))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Noise */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Noise"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Effect/Texture/NoiseTest.png"), 1))))
			return E_FAIL;

#pragma region Effect Texture

		Load_Texture("Hit_Effect", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Common", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Dissolve", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Water", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Ground Mask", "../Bin/Resources/Effect/Texture/");

		Load_Texture("Shock", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Slash", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Skl_Water_6", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Skl_Water_2", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Skl_Water_8", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Fire", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Line", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Rui", "../Bin/Resources/Effect/Mesh/MeshTexture/");

		Load_Texture("Else", "../Bin/Resources/Effect/Mesh/MeshTexture/");

	
#pragma endregion Eeffect Texture

#pragma region Effect Model
		/* 모델 */
		PivotMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationX(XMConvertToRadians(90.f));

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_EffectSlash1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_EffectSlash2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_EffectSlash3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_EffectSlash4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_EffectSlash5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash5.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_EffectSlash6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash6.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_EffectSlash7"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash7.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_EffectSlash8"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash8.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_6_Ground"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Water/6/Water_6_Ground.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_6_Main"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Water/6/Water_6_Main.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_6_Wind"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Water/6/Water_6_Wind.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_2_Side"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Water/2/Water_2_Side.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_2_Ring"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Water/2/Water_2_Ring.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_BrokenGround1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/EnvironmentEffect/BrokenGround1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_BrokenGround2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/EnvironmentEffect/BrokenGround2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_UpSwing"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/1_Siranui/UpSwing.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Nobori"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/2_UpperDouble/2_Nobori.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_OgiAtk1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Slash/Ogi_Atk.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_OgiAtk2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Slash/Ogi_Atk2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Swing"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Rengoku_Swing.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Tiger"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Tiger/Tiger.fbx", PivotMatrix)))) return E_FAIL;
		
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring5.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring6.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring14"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring14.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring15"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring15.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring16"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring16.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring17_1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring17_1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring17_2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring17_2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring18"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring18.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring19"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring19.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring20"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring20.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Smoke1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Smoke/Smoke1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Smoke2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Smoke/Smoke2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Smoke3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Smoke/Smoke3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Twist1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Twist/Twist1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Twist2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Twist/Twist2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Wind"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Wind/Wind.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Wind1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Wind/Wind1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Wind2_L"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Wind/Wind2_L.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Wind3_L"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Wind/Wind3_L.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash9"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash9.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash10"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash10.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Wind4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Wind/Wind4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Wind5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Wind/Wind5.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Wind6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Wind/Wind6.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Wind7"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Wind/Wind7.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring21"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring21.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring22"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring22.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring23"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring23.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring24"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring24.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordTrail1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/SwordTrail/SwordTrail1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordTrail2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/SwordTrail/SwordTrail2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sphere1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Sphere/Sphere1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sphere2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Sphere/Sphere2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash11"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash11.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash12"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash12.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash13"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash13.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash14"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash14.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash15"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash15.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash16"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash16.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water8_0"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Water/8/Water8_0.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water8_1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Water/8/Water8_1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water8_3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Water/8/Water8_3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water8_4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Water/8/Water8_4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring_ShockWave"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring_ShockWave1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_TigerFire"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/TigerFire/TigerMainFire.fbx", PivotMatrix)))) return E_FAIL;
		
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_6_Wind2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Water/6/Water_6_Wind2.fbx", PivotMatrix)))) return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring_RuiSkl3_1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Ring_RuiSkl3_1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring_RuiSkl3_2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Ring_RuiSkl3_2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_BigWeb3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_BigWeb3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Dome1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Dome1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Dome2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Dome2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Dome3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Dome3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Dome4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Dome4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_DomeEnd1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_DomeEnd1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_DomeEnd4_1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_DomeEnd4_1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_DomeEnd4_2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_DomeEnd4_2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Fragment1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Fragment1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Fragment2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Fragment2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Fragment3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Fragment3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Fragment4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Fragment4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Fragment5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Fragment5.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Fragment6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Fragment6.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Line3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Line3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Line4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Line4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Line5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Line5.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Line6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Line6.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Line7"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Line7.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Line8"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Line8.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Line10"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Line10.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Plane1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Plane1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Ring1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Ring1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Ring2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Ring2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Skl3Line3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Skl3Line3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Skl3Line4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Skl3Line4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Smoke3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Smoke3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_UpWeb5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_UpWeb5.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_UpWeb6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_UpWeb6.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Web3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Web3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Web4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Web4.fbx", PivotMatrix)))) return E_FAIL;



#pragma endregion Effect Model

#pragma region Effect Object
		/* 객체 */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectTexture"),
			CEffect_Texture::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectMesh"),
			CEffect_Mesh::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectParticle"),
			CEffect_Particle::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		
		CEffect_Manager* pEffect_Manager = GET_INSTANCE(CEffect_Manager);

		pEffect_Manager->Load_Effect(TEXT("HitEffect1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack1_GroundMask"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack2_1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack2_2"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack2_2_GroundMask"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack3"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack3_GroundMask"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack4"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack4_GroundMask"));
		pEffect_Manager->Load_Effect(TEXT("Water2"));
		pEffect_Manager->Load_Effect(TEXT("Water2_Ground"));
		pEffect_Manager->Load_Effect(TEXT("Water6"));
		pEffect_Manager->Load_Effect(TEXT("Water8"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_NormAtk_1_Cl"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_NormAtk_2_1_Cl"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_NormAtk_2_2_Cl"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_NormAtk_3_Cl"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_NormAtk_4_Cl"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_3Kien_Start"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_3Kien_Start_Fire"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_3Kien_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_2Nobori_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_2Nobori_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_5Tiger_Charge"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_5Tiger_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_5TigerColl_Particle"));	//	0205.18:20

		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_Jump3Kien_Start"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_Jump3Kien_StartFire"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_Jump3Kien_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_Jump5Tiger_Charge"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_Jump5Tiger_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_Jump5Tiger_After"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_Jump5Tiger_End"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk2_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk2_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk3_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk3_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk4"));

		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_FriendCom_Start"));
		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_FriendCom_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Coll_FriendMove"));

		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_ShotBall_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_ShotBall_Start_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_ShotBall_Start_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_ShotBall_Start_3"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_ShotNet_StartEff"));	//	0205.18:20
		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_ShotNet_Main"));	//	0205.18:20

		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_JumpBall_Start"));	//	0206.18:00
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Coll_JumpBall_Main"));	//	0206.18:00
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_JumpShotNet_StartEff"));	//	0206.18:00
		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_JumpShotNet_Main"));	//	0206.18:00

		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_JumpAtkMove_PullMain"));	//	0206.18:00
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_JumpAtkMove_PlayerMain"));	//	0206.18:00
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_JumpAtkMove_Main"));	//	0206.18:00

		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_JumpAtk_Main"));	//	0206.18:00

		RELEASE_INSTANCE(CEffect_Manager);
#pragma endregion Effect Object
	}
#pragma endregion Effect

	/* 카메라 객체 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* 케릭터 객체 */

	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Player/Player.fbx", PivotMatrix))))
		return E_FAIL;*/
//	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Player"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
//	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
//		CPlayer::Create(m_pDevice, m_pContext))))
//		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Rui/Rui.fbx", PivotMatrix))))
		return E_FAIL;*/
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rui"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rui"),
		CRui::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Tanjiro"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tanjiro"),
		CTanjiro::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Kyoujuro"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Kyoujuro"),
		CKyoujuro::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Akaza"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Akaza"),
		CAkaza::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Nezuko"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Nezuko"),
		CNezuko::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Shinobu"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Shinobu"),
		CShinobu::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("KyoujuroWeapon"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KyoujuroWeapon"),
		CKyoujuroWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("KyoujuroSheath"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KyoujuroSheath"),
		CKyoujuroSheath::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TanjiroWeapon"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TanjiroWeapon"),
		CTanjiroWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TanjiroSheath"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TanjiroSheath"),
		CTanjiroSheath::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("ShinobuWeapon"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShinobuWeapon"),
		CShinobuWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("ShinobuSheath"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShinobuSheath"),
		CShinobuSheath::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	_matrix PivotMatrix2 = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_KyoujuroWeaponMenu"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/KyoujuroWeapon(Menu)/KyoujuroWeapon(Menu).fbx", PivotMatrix2))))
	//return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_ShinobuWeaponMenu"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/ShinobuWeapon(Menu)/ShinobuWeapon(Menu).fbx", PivotMatrix2))))
	//return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("KyoujuroWeapon(Menu)"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KyoujuroWeaponMenu"),
		CKyoujuroWeaponMenu::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("ShinobuWeapon(Menu)"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShinobuWeaponMenu"),
		CShinobuWeaponMenu::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_TanjiroMenu"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Tanjiro(Menu)/Tanjiro(Menu).fbx", PivotMatrix2))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_KyoujuroMenu"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Kyoujuro(Menu)/Kyoujuro(Menu).fbx", PivotMatrix2))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiMenu"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Rui(Menu)/Rui(Menu).fbx", PivotMatrix2))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_AkazaMenu"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Akaza(Menu)/Akaza(Menu).fbx", PivotMatrix2))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NezukoMenu"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Nezuko(Menu)/Nezuko(Menu).fbx", PivotMatrix2))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_ShinobuMenu"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Shinobu(Menu)/Shinobu(Menu).fbx", PivotMatrix2))))
	//	return E_FAIL;
 	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Tanjiro(Menu)"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Kyoujuro(Menu)"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rui(Menu)"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Akaza(Menu)"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Nezuko(Menu)"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Shinobu(Menu)"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MenuModel"),
		CMenuModel::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//CollBox
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BaseAtk"),
		CBaseAtk::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WaterMill"),
		CWaterMill::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WindMill"),
		CWindMill::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KyoujuroSkill"),
		CKyoujuroSkill::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiAtk"),
		CRuiAtk::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AkazaShoot"),
		CAkazaShoot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiShoot"),
		CRuiShoot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiSphere"),
		CRuiSphere::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KyoujuroJumpSkill"),
		CKyoujuroJumpSkill::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KaguraSkill"),
		CKaguraSkill::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiBall"),
		CRuiBall::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiBigBall"),
		CRuiBigBall::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//Map
	//CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BattleField"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BattleField"),
	//	CBattleField::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;



	// Navi
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Rui"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Navigation/11_Map_Rui.nav")))))
		return E_FAIL;

#pragma region UI객체
	//UI
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ComboBase"),
		CComboBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ComboHitBase"),
		CComboHitBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ComboHitDeco"),
		CComboHitDeco::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ComboTxt"),
		CComboHitTxt::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ComboNum"),
		CComboNum::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CharSelMsg"),
		CCharSelMsg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ResultUnder"),
		CResultUnder::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ResultCloud"),
		CResultCloud::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TxtWinLight"),
		CTxtWinLight::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TxtWinUI"),
		CTxtWinUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ResultCheckUI"),
		CResultCheckUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RankEff"),
		CRankEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RankIcon"),
		CRankIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RankFuda"),
		CRankFuda::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ResultFrame"),
		CResultFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NumTimer"),
		CNumTimer::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RoundIcon"),
		CRoundIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RoundUI"),
		CRoundUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SelStamp"),
		CSelStamp::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KeyUI"),
		CKeyUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UltStockNum"),
		CUltStockNum::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UltStockFrame"),
		CUltStockFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UltStockEff"),
		CUltStockEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UltGaugeDeco"),
		CUltGaugeDeco::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UltStockBase"),
		CUltStockBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UltBar"),
		CUltBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PersonChanBarEff"),
		CPlyChanBarEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_OniChanBarEff"),
		COniChanBarEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GaugeLight"),
		CGaugeLight::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BtlFixedImg"),
		CBtlFixedImg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_InkEff"),
		CInkEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MenuBackDeco"),
		CMenuBackDeco::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MenuChar"),
		CMenuChar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MenuCursor"),
		CMenuCursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MenuDarkCloud"),
		CMenuDarkCloud::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MenuFixedImg"),
		CMenuFixedImg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MenuFrontCloud"),
		CMenuFrontCloud::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MenuTitle"),
		CMenuTitle::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LogoBackEff"),
		CLogoBackEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LogoBackLight"),
		CLogoBackLight::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LogoButton"),
		CLogoButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LogoFixedImg"),
		CLogoFixedImg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SelectBg"),
		CSelectBG::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PatternWind"),
		CPatternWind::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PatternOne"),
		CPatternOne::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CloudUI"),
		CCloudUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerNumIcon"),
		CPlayerNumIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CharSelBg"),
		CCharSelBg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WindowBase"),
		CWindowBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WindowLeft"),
		CWindowLeft::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WindowRight"),
		CWindowRight::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CharFrameEff"),
		CCharFrameEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SelNameShadow"),
		CSelNameShadow::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CharFrame"),
		CCharFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CharIcon"),
		CCharIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_P1Cursor"),
		CSelP1Cursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_P2Cursor"),
		CSelP2Cursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_OniIcon"),
		COniIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_1P_Icon"),
		C1PIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_2P_Icon"),
		C2PIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_OniLight"),
		CSelOniLight::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_1P_MainOnBase"),
		C1PMainOnBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_2P_MainOnBase"),
		C2PMainOnBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CharNameUI"),
		CCharNameUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HpBar"),
		CHpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HpBarDeco"),
		CHpBarDeco::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HpBarBack"),
		CHpBarBack::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GaugeBase"),
		CGaugeBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkillBar"),
		CSkillBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkillBarParts"),
		CSkillBarParts::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChangeBaseDeco"),
		CChangeBaseDeco::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChangeSprt"),
		CChangeSprt::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChangeIcon"),
		CChangeIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ProgressBar"),
		CProgressBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion UI객체

	lstrcpy(m_szLoadingText, TEXT("                        로딩이 완료되었습니다."));

	m_isFinished = true;
	RELEASE_INSTANCE(CData_Manager);
	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLoader::Loading_ForSelectCharLevel()
{
	/* 객체 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("                       객체 생성 중."));
	
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	

	lstrcpy(m_szLoadingText, TEXT("                       로딩이 완료되었습니다."));

	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);


	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("                       텍스쳐 로딩 중."));



	/* 객체 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("                       객체 생성 중."));

	/* UI 객체 */



	lstrcpy(m_szLoadingText, TEXT("                        로딩이 완료되었습니다."));
	
	m_isFinished = true; 

	Safe_Release(pGameInstance);
	
	return S_OK;
}

HRESULT CLoader::Loading_ForGameResult()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);


	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("                       텍스쳐 로딩 중."));



	/* 객체 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("                       객체 생성 중."));

	/* UI 객체 */



	lstrcpy(m_szLoadingText, TEXT("                        로딩이 완료되었습니다."));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CLoader::Load_Texture(char * szFolderName, char * szFilePath)
{
	_finddatai64_t fd;
	intptr_t hFile;

	//../Bin/Resources/Effect/Mesh/Rengoku/Tiger/Tiger.fbx
	char szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, szFilePath);
	strcat_s(szFullPath, szFolderName);
	strcat_s(szFullPath, "/*.png");

	hFile = _findfirsti64(szFullPath, &fd);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return;

	do {
		char szProtorypePath[MAX_PATH] = "Prototype_Component_Texture_";
		char szProtorypeName[MAX_PATH] = "";
		char* szTemp = "";

		strcpy_s(szProtorypeName, szProtorypePath);
		strcat_s(szProtorypeName, fd.name);
		//char szFullProtorypeName[MAX_PATH];
		strcpy_s(szProtorypeName, strtok_s(szProtorypeName, ".", &szTemp));

		_tchar		szRealName[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szProtorypeName, (_int)strlen(szProtorypeName), szRealName, MAX_PATH);

		strcpy_s(szFullPath, szFilePath);
		strcat_s(szFullPath, szFolderName);
		strcat_s(szFullPath, "/");
		strcat_s(szFullPath, fd.name);

		_tchar			szRealPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szFullPath, (_int)strlen(szFullPath), szRealPath, MAX_PATH);

		pGameInstance->Add_Prototype(LEVEL_STATIC, szRealName, CTexture::Create(m_pDevice, m_pContext, szRealPath, 1));

	} while (_findnexti64(hFile, &fd) == 0);

	_findclose(hFile);

	RELEASE_INSTANCE(CGameInstance);
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		ERR_MSG(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
