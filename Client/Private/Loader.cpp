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
#include "MeshObj_Smell.h"
#include "MeshObj_Smell_Inst.h"
#include "Door.h"
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
#include "RuiDad.h"
#include "Enmu.h"
#include "Enmu_Chaos_Head.h"
#include "Enmu_Shield.h"
#include "Enmu_Right_Hand.h"
#include "Enmu_Left_Hand.h"
#include "Enmu_Chok.h"
#include "Enmu_ChokChok.h"
#include "AkazaBody.h"
#include "AkazaLeg.h"
//parts
#include "KyoujuroWeapon.h"
#include "KyoujuroSheath.h"
#include "TanjiroWeapon.h"
#include "TanjiroSheath.h"
#include "ShinobuWeapon.h"
#include "ShinobuSheath.h"
#include "KyoujuroWeaponMenu.h"
#include "ShinobuWeaponMenu.h"
#include "TanjiroWeapon2.h"
#include "RuiHead.h"
#include "Train_Head.h"
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
#include "ChangeBar.h"
#include "ComboBase.h"
#include "ComboHitBase.h"
#include "ComboHitDeco.h"
#include "ComboHitTxt.h"
#include "ComboNum.h"
#include "OniSpecialSkillBar.h"
#include "ButtonEff.h"
#include "AdcMenuBg.h"
#include "AdcMenuBgDeco.h"
#include "AdcMenuCloud.h"
#include "AdcMenuCursor.h"
#include "AdcMenuDarkCloud.h"
#include "AdcMenuNameBar.h"
#include "AdcMenuSelFrame.h"
#include "AdcMenuSelImg.h"
#include "AdcMenuSelTxt.h"
#include "UltFire.h"
#include "UltGaugeFrame.h"
#include "UltOpenBar.h"
#include "UltNumEff.h"
#include "UltStockFadeEff.h"
#include "ResultScoreBar.h"
#include "ResultScoreBase.h"
#include "ResultTotalBase.h"
#include "LineFrame.h"
#include "SelMapIconFrame.h"
#include "SelMapFixedImg.h"
#include "SelMapNameBase.h"
#include "SelMapIcon.h"
#include "SelMapCursor.h"
#include "SelMapEff.h"
#include "SelMapImg.h"
#include "SelMapListPattern.h"
#include "QuiestGuideBase.h"
#include "QuiestKeyUI.h"
#include "QuiestMainBase.h"
#include "QuiestStampIcon.h"
#include "QuiestSubBase.h"
#include "QuiestSubIcon.h"
#include "QuiestMainIcon.h"
#include "FeedArrow.h"
#include "MsgNameBase.h"
#include "MsgTextBase.h"
#include "MapNameBar.h"
#include "Adv_ResultBase.h"
#include "FadeUIEff.h"
#include "BattleDialog.h"
//Effect
#include "Effect.h"
#include "Effect_Manager.h"
#include "Effect_Texture.h"
#include "Effect_Mesh.h"
#include "Effect_AnimMesh.h"
#include "Effect_Particle.h"
#include "Effect_Particle_New.h"
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
#include "RuiMoveSkill.h"
#include "RuiDadJump.h"
#include "RuiDadBigStone.h"
#include "RuiDadSmallStone.h"
#include "InteractionUI.h"
#include "EnmuShoot.h"
#include "RuiSplColl.h"
#include "StoneSphere.h"
#include "MGameHeart.h"
//Ani
#include "Butterfly.h"
#include "Deer.h"
#include "Deer2.h"
#include "Bird.h"
#include "BigBird.h"
#include "Hoke.h"
// Test Target 
#include "TargetCircle.h"
//NPC
#include "Murata.h"
#include "Goto.h"
#include "Ozaki.h"
#include "TrainNPC.h"
#include "RuiSister.h"
//CamAction
#include "CamLine.h"
#include "RuiBomb.h"

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
	case LEVEL_MENU:
		pLoader->Loading_ForMenu();
		break;
	case LEVEL_STORYMENU:
		pLoader->Loading_ForStoryMenu();
		break;
	case LEVEL_ADVRUI:
		pLoader->Loading_ForAdvRui();
		break;
	case LEVEL_SELECTMAP:
		pLoader->Loading_ForSelectMap();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	g_fLoading = 0.f;
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





#pragma region UI
	{
#pragma region BattleUI
		/* 텍스쳐 로딩 중. */
		lstrcpy(m_szLoadingText, TEXT("                       텍스쳐 로딩 중."));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MGameHeart"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MGame/Heart.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MiniGame"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/MiniGame/%d.png"), 11))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Dissolve/%d.png"), 3))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NezukoMask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Meshes/Anim/NezukoMask/%d.png"), 3))))
			return E_FAIL;
		g_fLoading = 3.f;
		//Battle
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltGaugeFrame"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Ult_Gauge_Frame_%d.png"), 4))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_OpeningUltBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Ult_Gauge_Base.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltGaugeFire"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Eff_Awk%d.png"), 4))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_OniSpecialBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/OniBarFrame.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_OniSpecialBarMask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Oni_Mask.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChangeGaugeBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/ProgressBar.png"), 1))))
			return E_FAIL;
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
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ResultScoreBase"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Result_BarBase.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ResultScoreBar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Result_BarSelect.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ResultTotalBase"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Result_Base2.png"), 1))))
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
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Adv_ResultBase"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/BattleResult/Result_Base05.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NumTimer"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Num_Timer/Num_Timer%d.png"), 10))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AdvNumTimer"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Num_Timer/Num_Timer_Inf.png"), 1))))
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
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Key_%d.png"), 2))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltStockNum"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Lang_Num_Stock.png"), 1))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltStockFrame"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Ult_Stock_Frame_%d.png"), 6))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltStockEff"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Ult_Stock_Eff_%d.png"), 6))))
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
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Btl_UI/Char_Name/Name_%d.png"), 8))))
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
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Enmu_Normal"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Meshes/Anim/Normal/%d.png"), 4))))
			return E_FAIL;
		g_fLoading = 10.f;
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

		g_fLoading = 15.f;
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

#pragma region Quiest
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuiestMainIcon"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Quiest/FIcon_Main01.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuiestSubIcon"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Quiest/FIcon_Minievent.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuiestStampIcon"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Quiest/Stamp.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuiestSubBase"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Quiest/Mission_Base.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuiestMainBase"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Quiest/Mission_Main_Base.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuiestGuideBase"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Quiest/MiniTitle_Base2.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuiestKeyUI"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Quiest/Key_%d.png"), 3)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MsgNameBase"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MsgUI/Msg_Name_Base02.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MsgTextBase"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MsgUI/Msg_Text_Base.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FeedArrow"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MsgUI/LineFeed_Arrow.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MapNameBar"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/MsgUI/Profile_Info_Base.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_InteractionUI"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Interaction/Base_Title01.png"), 1)))) return E_FAIL;
#pragma endregion Quiest

#pragma region SelectMap
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelMapFixedImg"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Map_Sel/Fixed_%d.png"), 6)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelMapEff"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Map_Sel/Map_SelEff_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelMapCursor"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Map_Sel/Cursor_Stage.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelMapIconFrame"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Map_Sel/Frame_Icon_Stage.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelMapImg"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Map_Sel/Image_BG_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelMapIconImg"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Map_Sel/Image_Icon_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LineFrame"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Map_Sel/Line_Frame_Stage.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelMapImgMask"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Map_Sel/Mask_Image_Stage.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StageNameBase"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Map_Sel/Name_Base_Stage.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelMapPatternMask"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Map_Sel/Stage_Mask02.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelMapPattern"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Map_Sel/Stage_Pattern01.png"), 1)))) return E_FAIL;
#pragma endregion SelectMap

#pragma region Adc_Menu
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Adc_MenuBG"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Adc_Menu/Lang_StoryBG_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Adc_MenuDeco"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Adc_Menu/Story_Bg_Deco_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Adc_MenuSelTxt"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Adc_Menu/Lang_Title_Stage.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Adc_MenuSelFrame"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Adc_Menu/Frame_Icon_Stage.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Adc_MenuSelImg"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Adc_Menu/Image_Icon_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Adc_MenuSelCursor"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Adc_Menu/Cursor_Stage.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Adc_MenuStageNameBar"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Adc_Menu/Name_Base_Stage.png"), 1)))) return E_FAIL;
#pragma endregion Adc_Menu

#pragma region MenuUI
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuFixedImg"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/MenuFixedImg_%d.png"), 4)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuDarkCloud"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Cloud_Dark_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuFrontCloud"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Cloud_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuCursor"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Cursor.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuBackDeco"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Mode_Back_Deco00_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ModeTitle_Adv"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Lang_ModeTitle_Adv_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ModeTitle_Vs"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/Lang_ModeTitle_Vs_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuChar"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Menu/MenuChar_%d.png"), 2)))) return E_FAIL;
#pragma endregion MenuUI

#pragma region UIEff
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_InkEff"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/UIEff/Xcmn_Ink_%d.png"), 2)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ButtonEff"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/UIEff/Xef_Base00.png"), 1)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fade"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/UIEff/Fade.png"), 1)))) return E_FAIL;
#pragma endregion UIEff
		g_fLoading = 20.f;
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
	g_fLoading = 25.f;

	/* 모델 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("                     모델 로딩 중."));
#pragma region Static Objects
	{
		_matrix PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeWillow"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigTree3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeWillow_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken4"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken5"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken6"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken7"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken8"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken9"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken10"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken4_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken5_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken6_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken7_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken8_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken9_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeBroken10_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeRoot3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock4"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock5"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock6"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock7"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock_Small"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock4_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock5_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock6_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock7_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rock_Small_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff_Small"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cliff_Small_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass4"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass5"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass6"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass4_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass5_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass6_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Lavender"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Lavender_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Flower3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf4"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Leaf4_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far4"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far5"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far6"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far4_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far5_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hill_Far6_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Wall1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Wall2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Wall1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Wall2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush4"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush5"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush6"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush7"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush8"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush9"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush4_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush5_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush6_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush7_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush8_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bush9_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("HomeSmall1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("HomeSmall2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("HomeSmall1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("HomeSmall2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RiceField"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RiceField_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiGround"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("UrokodakiGround"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiGround_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("UrokodakiGround_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiGround2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Home1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rubble1"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rubble2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb4"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb5"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb6"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb7"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb8"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb9"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Tree_Jenitsu"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar3"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiGround2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Home1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rubble1_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rubble2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb4_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb5_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb6_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb7_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb8_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("SpiderWeb9_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Tree_Jenitsu_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar2_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TreeFar3_Instancing"), LEVEL_STATIC, CData_Manager::DATA_NONANIM_INSTANCING);

		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Moon"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("MoonLight"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Moon_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/rui_new/Ground.fbx", PivotMatrix))))
			return E_FAIL;
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hut"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiMap"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Enmu_Ground", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/Enmu/Enmu_Ground.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Enmu_NeckShield1", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/Enmu/Enmu_NeckShield1.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Enmu_NeckShield2", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/Enmu/Enmu_NeckShield2.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Enmu_Train", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/Enmu/Enmu_Train.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Enmu_Wall", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/Enmu/Enmu_Wall.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Enmu_Wall_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/Enmu/Enmu_Wall.fbx", PivotMatrix))))
			return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Hut", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/Hut/Hut.fbx", PivotMatrix))))
		//	return E_FAIL;
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_RuiMap", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/RuiMap/RuiMap.fbx", PivotMatrix))))
		//	return E_FAIL;


		g_fLoading = 35.f;
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
		//Door
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Door"),
			CDoor::Create(m_pDevice, m_pContext))))
			return E_FAIL;


		//	Smell
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Smell1", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/Smell/Smell1.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Smell2", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/Smell/Smell2.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Smell3", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/Smell/Smell3.fbx", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Smell1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/Smell/Smell1.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Smell2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/Smell/Smell2.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Smell3_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/Smell/Smell3.fbx", PivotMatrix))))
			return E_FAIL;

		//AkazaMap
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BrokenTrain1", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/BrokenTrain1.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BrokenTrain2", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/BrokenTrain2.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BrokenTrain3", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/BrokenTrain3.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BrokenTrain4", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/BrokenTrain4.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BrokenTrain5", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/BrokenTrain5.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Enmu_Neck0", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/Enmu_Neck0.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Enmu_Neck1", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/Enmu_Neck1.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Enmu_Neck2", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/Enmu_Neck2.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Rail1", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/Rail1.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Rail2", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/Rail2.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Train1", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/Train1.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Train2", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/Train2.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Train3", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/Train3.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Trainbottom1", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/Trainbottom1.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Trainbottom2", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/Trainbottom2.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_TrainChiar1", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/TrainChiar1.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_TrainChiar2", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/TrainChiar2.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_TrainDoor", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/TrainDoor.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_TrainMap", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/TrainMap.fbx", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_TrainChiar1_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/TrainChiar1.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_TrainChiar2_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/TrainChiar2.fbx", PivotMatrix))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_TrainDoor_Instancing", CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCING, "../Bin/Resources/Meshes/NonAnim/static/TrainMap/TrainDoor.fbx", PivotMatrix))))
			return E_FAIL;

		/*For.Prototype_GameObject_MeshObj_Smell */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MeshObj_Smell"),
			CMeshObj_Smell::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		//	MeshObj_Static_Insatncing
		/*For.Prototype_GameObject_MeshObj_Smell_Instancing */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MeshObj_Smell_Instancing"),
			CMeshObj_Smell_Inst::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		g_fLoading = 45.f;
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

		/*For.Prototype_Component_Texture_Noise_Smell */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Noise_Smell"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Meshes/NonAnim/static/Smell/T_d_e_Cmn_AuraSmoke_01_Smell_%d.png"), 2))))
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
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Effect/Texture/Particle/Particle%d.png"), 108))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Noise */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Noise"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Effect/Texture/NoiseTest.png"), 1))))
			return E_FAIL;
		g_fLoading = 48.f;
#pragma region Effect Texture

		Load_Texture("Hit_Effect", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Common", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Dissolve", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Water", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Ground Mask", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Smoke", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Unity", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Useful", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Fire", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Aura", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Tanjiro_Hinokami_Action", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Rui_Action", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Rengoku_Action", "../Bin/Resources/Effect/Texture/");
		Load_Texture("Akaza_Action", "../Bin/Resources/Effect/Texture/");

		g_fLoading = 55.f;

		Load_Texture("Shock", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Slash", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Skl_Water_6", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Skl_Water_2", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Skl_Water_8", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Fire", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Line", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Rui", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("RuiDad", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Else", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Akaza", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Bullet", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Hinokami", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Shinobu", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Nezuko", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Enmu", "../Bin/Resources/Effect/Mesh/MeshTexture/");
		Load_Texture("Shinobu_Spl", "../Bin/Resources/Effect/Mesh/MeshTexture/");

		g_fLoading = 60.f;
#pragma endregion Eeffect Texture

#pragma region Effect Model
		/* 모델 */
		PivotMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationX(XMConvertToRadians(90.f));

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash5.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash6.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash7"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash7.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash8"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Slash8.fbx", PivotMatrix)))) return E_FAIL;

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
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ring_ShockWave1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Ring/Ring_ShockWave1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_TigerMainFire"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/TigerFire/TigerMainFire.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_6_Wind2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Water/6/Water_6_Wind2.fbx", PivotMatrix)))) return E_FAIL;

		g_fLoading = 65.f;

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
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_DashSmoke_L"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/DashSmoke_L.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_DashSmoke_R"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/DashSmoke_R.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SmokeMesh1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Smoke1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SmokeMesh2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Smoke2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SmokeMesh3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Smoke3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SmokeMesh5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Smoke5.fbx", PivotMatrix)))) return E_FAIL;

		//	Akaza
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Angry1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Angry1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Angry2_1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Angry2_1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Angry2_2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Angry2_2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Angry3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Angry3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Compass1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Compass1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Compass2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Compass2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Compass3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Compass3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Fist1_1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Fist1_1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Fist1_2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Fist1_2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Fist2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Fist2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Fist3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Fist3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Fist4_Dist"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Fist4_Dist.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Fist6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Fist6.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Ring2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Ring2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Ring3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Ring3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Ring4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Ring4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Stone1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Stone1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Stone2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Stone2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Stone3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Stone3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Wind1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Wind1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Wind2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Wind2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Wind3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Wind4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Wind4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Wind4.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Akaza_Angry3_1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Akaza/Akaza_Angry3_1.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_BreathFire"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_BreathFire.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Cneter_Slash01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Cneter_Slash01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Enb_CenterSlashRing1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Enb_CenterSlashRing1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Enb_Slash01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Enb_Slash01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Enb_Slash02"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Enb_Slash02.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Enb_SlashRing1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Enb_SlashRing1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Slash02"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Slash02.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Slash04"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Slash04.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Slash05_1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Slash05_1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Slash05_2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Slash05_2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Stab"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Stab.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Arch"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Arch.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Aura001_1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Aura001_1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Aura001_2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Aura001_2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Aura001_3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Aura001_3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Aura001_4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Aura001_4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Board1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Board1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_ChAura1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/ChAura1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Cone1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Cone1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_FireRing1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/FireRing1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_GroundRock00"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/GroundRock00.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_HSphere"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/HSphere.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Plane001"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Plane001.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Plane002"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Plane002.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_CmnRing1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Ring1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_CmnRing2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Ring2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_CmnRing3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Ring3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rock001"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Rock001.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Slash006"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Slash006.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SlashWind0"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/SlashWind0.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Spark1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Spark1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Spark2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Spark2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_CmnSphere1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Sphere1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Spike1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Spike1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Spike2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Spike2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Spiral1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Spiral1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Unique1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Unique1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_CmnWind2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Wind2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_CmnWind2_L"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Wind2_L.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_CmnWind3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Wind3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_CmnWind4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Wind4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_CmnWind5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Wind5.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_CmnWind6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/Wind6.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_WindSpiral"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Cmn/WindSpiral.fbx", PivotMatrix)))) return E_FAIL;


		// Test Target 나중에 바꿔줘
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_TargetCircle"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/TargetTest/Floor.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TargetCircle"),
			CTargetCircle::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		g_fLoading = 70.f;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Blow1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Blow1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_CrossPlane"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_CrossPlane.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_GhostCoat"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_GhostCoat.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_PlaneSix1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_PlaneSix1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_PlaneSix2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_PlaneSix2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Ring1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Ring1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Ring2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Ring2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Ring4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Ring4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Spiral1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Spiral1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Sword1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Sword1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Unique1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Unique1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Unique2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Unique2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Wind1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Wind1.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Unique3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Unique3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Plane"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Plane.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Spiral2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Spiral2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Slash"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Slash.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_LongCross"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_LongCross.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Wind2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu/Shinobu_Wind2.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NeClaw1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Nezuko/NeClaw1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NeGroundRock1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Nezuko/NeGroundRock1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NeRock1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Nezuko/NeRock1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NeShock1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Nezuko/NeShock1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NeWind1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Nezuko/NeWind1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NeWind2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Nezuko/NeWind2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NeWind3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Nezuko/NeWind3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_PlaneClose1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Nezuko/PlaneClose1.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_GroundStone1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Nezuko/GroundStone1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_GroundStone2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Nezuko/GroundStone2.fbx", PivotMatrix)))) return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Circle1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Circle1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Cylinder1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Cylinder1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Rock1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Rock1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Rock2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Rock2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Stone1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Stone1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Thorn1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Thorn1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Torus1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Torus1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Wind1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Wind1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Wind2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Wind2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Wind3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Wind3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Wind4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Wind4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiDad_Wind5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/RuiDad/RuiDad_Wind5.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Spl_Circle"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Spl_Circle.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Spl_FireTrailPlane_01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Spl_FireTrailPlane_01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Spl_GroundFlame"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Spl_GroundFlame.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Spl_Hekira_01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Spl_Hekira_01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Spl_Hekira_04"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Spl_Hekira_04.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Spl_Hekira_05"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Spl_Hekira_05.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Spl_Hekira_07"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Spl_Hekira_07.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Spl_Stone"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Spl_Stone.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Spl_Wind"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Spl_Wind.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hi_Spl_FireTrail02"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Hinokami/Hi_Spl_FireTrail02.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Spl_Circle01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Spl_Circle01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Spl_Cone01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Spl_Cone01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Spl_HalfCircle01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Spl_HalfCircle01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Spl_HalfSphere01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Spl_HalfSphere01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Spl_Leaf01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Spl_Leaf01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Spl_Plane01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Spl_Plane01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Spl_Slash01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Spl_Slash01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Spl_Sphere01"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Spl_Sphere01.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Spl_Sphere02"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Spl_Sphere02.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Spl_Sphere03"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Spl_Sphere03.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Wind1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Wind1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Wind2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Wind2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_WebLine2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_WebLine2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_WebLine1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_WebLine1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_WebDome1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_WebDome1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Web7"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Web7.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Web6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Web6.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Web5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Web5.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Web2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Web2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Web1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Web1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Tornade1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Tornade1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_Spl_WebRing1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_Spl_WebRing1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_AtkSkl02_Web1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_AtkSkl02_Web1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_AtkSpl_Web1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_AtkSpl_Web1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui_AtkThrow_Web1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rui/Rui_AtkThrow_Web1.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Meat1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Meat1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Meat2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Meat2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Meat3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Meat3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Parts1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Parts1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Ring1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Ring1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Ring2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Ring2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Ring3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Ring3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Ring4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Ring4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Ring5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Ring5.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Ring6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Ring6.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Sphere1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Sphere1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Sphere2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Sphere2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Sphere3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Sphere3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Sphere4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Sphere4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Spotlight1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Spotlight1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Wind1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Wind1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Wind2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Wind2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Wind3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Wind3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Wind4"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Wind4.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Wind5"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Wind5.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Wind6"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Enmu/Enmu_Wind6.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Wind8"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Wind/Wind8.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Wind9"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Wind/Wind9.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Wind10"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Wind/Wind10.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_078_Bg_11"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Spl/Rengoku_Spl_078_Bg_11.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_078_Bg_12"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Spl/Rengoku_Spl_078_Bg_12.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_078_Debri1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Spl/Rengoku_Spl_078_Debri1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_078_Dome1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Spl/Rengoku_Spl_078_Dome1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_Tornado"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Spl/Rengoku_Spl_Tornado.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_078_Smoke3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Spl/Rengoku_Spl_078_Smoke3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_078_Wind1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Spl/Rengoku_Spl_078_Wind1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_Circle1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Spl/Rengoku_Spl_Circle1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_GroundRock1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Spl/Rengoku_Spl_GroundRock1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_GroundRock2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Spl/Rengoku_Spl_GroundRock2.fbx", PivotMatrix)))) return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Spl_CrossTri"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu_Spl/Shinobu_Spl_CrossTri.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Spl_Fly"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu_Spl/Shinobu_Spl_Fly.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Spl_Ground"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu_Spl/Shinobu_Spl_Ground.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Spl_Rock1"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu_Spl/Shinobu_Spl_Rock1.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Spl_Rock2"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu_Spl/Shinobu_Spl_Rock2.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Spl_Rock3"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu_Spl/Shinobu_Spl_Rock3.fbx", PivotMatrix)))) return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Spl_Spark"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Effect/Mesh/Shinobu_Spl/Shinobu_Spl_Spark.fbx", PivotMatrix)))) return E_FAIL;

		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_Tornado"), CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin//Effect/Mesh/Rengoku/Spl/Rengoku_Spl_Tornado.fbx", PivotMatrix)))) return E_FAIL;
		PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_078_FlameTrail01_11"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Effect/Mesh/Rengoku/Spl/Rengoku_Spl_078_FlameTrail01_11.fbx", PivotMatrix))))
			return E_FAIL;

		g_fLoading = 75.f;

		PivotMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
#pragma endregion Effect Model

#pragma region Effect Object
		/* 객체 */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectTexture"),
			CEffect_Texture::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectMesh"),
			CEffect_Mesh::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectAnimMesh"),
			CEffect_AnimMesh::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectParticle"),
			CEffect_Particle::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectParticleNew"),
			CEffect_Particle_New::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		CEffect_Manager* pEffect_Manager = GET_INSTANCE(CEffect_Manager);

		pEffect_Manager->Load_Effect(TEXT("HitEffect1"));
		pEffect_Manager->Load_Effect(TEXT("HitEffect2"));
		pEffect_Manager->Load_Effect(TEXT("HitEffect3"));
		pEffect_Manager->Load_Effect(TEXT("HitEffect4"));
		pEffect_Manager->Load_Effect(TEXT("HitEffect5"));

		pEffect_Manager->Load_Effect(TEXT("Character_Change_DOWN"));
		pEffect_Manager->Load_Effect(TEXT("Character_Change_UP"));
		pEffect_Manager->Load_Effect(TEXT("Guard1"));
		pEffect_Manager->Load_Effect(TEXT("Guard2"));
		pEffect_Manager->Load_Effect(TEXT("Guard3"));
		pEffect_Manager->Load_Effect(TEXT("Guard3_Broken"));
		pEffect_Manager->Load_Effect(TEXT("Jump_DOWN"));
		pEffect_Manager->Load_Effect(TEXT("Jump_UP"));
		pEffect_Manager->Load_Effect(TEXT("Run"));
		pEffect_Manager->Load_Effect(TEXT("Rush_Start"));
		pEffect_Manager->Load_Effect(TEXT("Rush_Move"));
		pEffect_Manager->Load_Effect(TEXT("Rush_Hit"));
		pEffect_Manager->Load_Effect(TEXT("Power_Charging"));
		pEffect_Manager->Load_Effect(TEXT("Power_Up"));
		pEffect_Manager->Load_Effect(TEXT("Power_Up_Player"));
		pEffect_Manager->Load_Effect(TEXT("Perfect_Power_UP_Player"));
		pEffect_Manager->Load_Effect(TEXT("Game_Start"));

		pEffect_Manager->Load_Effect(TEXT("Dash_Tanjiro_Move"));
		pEffect_Manager->Load_Effect(TEXT("Dash_Tanjiro_Stop"));
		pEffect_Manager->Load_Effect(TEXT("Dash_Tanjiro_MoveFB"));
		pEffect_Manager->Load_Effect(TEXT("Dash_Tanjiro_StopFB"));

		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack1_GroundMask"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack2_1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack2_2"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack2_2_GroundMask"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack3"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack3_GroundMask"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack4"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Attack4_GroundMask"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_Jump_Attack"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_JumpMoveAttack"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro_JumpMoveAttack2"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Heno_Kami)_Attack1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Heno_Kami)_Attack2_1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Heno_Kami)_Attack2_2"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Heno_Kami)_Attack3_1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Heno_Kami)_Attack3_2"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Heno_Kami)_Attack4_1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Heno_Kami)_Attack4_2"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Heno_Kami)_Jump_Attack"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Hinokami)_JumpMoveAttack2"));
		pEffect_Manager->Load_Effect(TEXT("Water2"));
		pEffect_Manager->Load_Effect(TEXT("Water2_Ground"));
		pEffect_Manager->Load_Effect(TEXT("Water6"));
		pEffect_Manager->Load_Effect(TEXT("Water6_Ground"));
		pEffect_Manager->Load_Effect(TEXT("Water8"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Hinokami)_Common_1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Hinokami)_Common_2"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Hinokami)_Move1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Hinokami)_Move2"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Hinokami)_Move3"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Hinokami)_Move4"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Hinokami)_Move5"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Hinokami)_Sphere_Main1"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Hinokami)_Sphere_Main2"));
		pEffect_Manager->Load_Effect(TEXT("Tanjiro(Hinokami)_Sphere_Start"));

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

		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_JumpAtk_Main"));		//	0208.00:55
		pEffect_Manager->Load_Effect(TEXT("Effect_Rengoku_JumpMoveAtk_NonFollow"));		//	0208.00:55

		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk2_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk2_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk3_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk3_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk4"));

		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_FriendCom_Start"));
		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_FriendCom_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_FriendMove"));

		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_Skl_Com_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Skl_Com_Start_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Skl_Com_Start_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Skl_Com_Start_3"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Skl_Move_StartEff"));
		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_Skl_Move_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_Skl_Move_ColEveryTime"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Skl_Jump_Start"));
		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_Skl_Jump_Main"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Skl_JumpMove_StartEff"));
		pEffect_Manager->Load_Effect(TEXT("Effect_RuiColl_Skl_JumpMove_Main"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk_JumpMove_PullMain"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk_JumpMove_PlayerMain"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk_JumpMove_Main"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Rui_Atk_Jump_Main"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Atk1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Atk2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Atk3"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Atk4"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Atk_Jump_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Atk_JumpMove_NonFollow"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Atk_JumpMove_Follow"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Atk_JumpMove_End"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Com_Start"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Com_Hand_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Com_Hand_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Com_Hand_3"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Com_Hand_4"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Com_Hand_5"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Coll_Skl_Com_Main"));


		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Move_StartFollowPlc"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Move_RunSmoke"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Move_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Move_Final"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_JumpCom_StartEff"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_JumpMove_StartEff"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_JumpMove_StartFollow"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_JumpMove_Main"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_SpecialGround"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Destroy_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Destroy_SuccessFinal"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Destroy_FailedFinal"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Dash"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Friend_Com_Start"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Friend_Com_StartFollow"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Friend_Com_Ing_Follow"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Friend_Com_Main"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Friend_Move_Start_Fol"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Friend_Move_Start_NonFol"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Akaza_Skl_Friend_Move_Main"));


		//	Shinobu
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Atk1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Atk2_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Atk2_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Atk3_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Atk4_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Atk4_2"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Atk_JumpCom_Main"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Atk_JumpMove_Follow"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Atk_JumpMove_NonFollow"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Com_Main_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Com_Main_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Com_Main_3"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Com_Main_4"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Com_Main_5"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Com_Final_NonFol"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Com_Final_Fol"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Move_Start"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Move_Main_1R"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Move_Main_2L"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Move_Final_NonFol"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_JumpCom_Start_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_JumpCom_Start_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_JumpCom_Start_3"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_JumpCom_Start_4"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_JumpCom_Loop_Fol"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_JumpCom_Loop_NonFol"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_JumpCom_Loop_FinalYZero"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_JumpMove_1Loop_Fol"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_JumpMove_1Loop_NonFol"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_JumpMove_1LoopEnd_Main"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Special_NonFol"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Skl_Special_Fol"));

		//Nezuko
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Attack1"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Attack2"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Attack3_1"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Attack3_2"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Attack3_3"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Attack4_1"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Attack4_2"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Jump_Attack1"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Jump_Attack2"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_JumpMoveAttack"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_JumpMoveAttack2"));

		pEffect_Manager->Load_Effect(TEXT("Nezuko_Jump_Skill_Claw1"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Jump_Skill_Claw2"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Jump_Skill_Claw3"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Jump_Skill_Claw4"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Jump_Skill_Claw5"));

		pEffect_Manager->Load_Effect(TEXT("Nezuko_JumpKick_Ground"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_JumpKick1"));

		pEffect_Manager->Load_Effect(TEXT("Nezuko_Skill_Claw1"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Skill_Claw2"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Skill_Claw3"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Skill_Claw4"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Skill_Claw5"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Skill_Claw_Ground"));

		pEffect_Manager->Load_Effect(TEXT("Nezuko_Skill_KickDown_FootFire"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Skill_KickDown_Ground"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Skill_KickDown_Turn"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Skill_KickDown_UPFire1"));
		pEffect_Manager->Load_Effect(TEXT("Nezuko_Skill_KickDown_Wind"));

		//RuiDad
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Throwing_BigStone"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Throwing_SmallStone"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Throwing_Ground1"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Throwing_Ground2"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Throwing_Ground3"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Throwing_Ground4"));

		pEffect_Manager->Load_Effect(TEXT("RuiDad_Angry"));

		pEffect_Manager->Load_Effect(TEXT("RuiDad_Attack1_1"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Attack1_2"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Attack2_Ground"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Attack2_Slash"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Attack3_End"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Attack3_Slash"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Attack3_Start"));

		pEffect_Manager->Load_Effect(TEXT("RuiDad_Dash_EndGround"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Dash_Ground"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Dash_Player"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Dash_StartGround"));

		pEffect_Manager->Load_Effect(TEXT("RuiDad_Free"));

		pEffect_Manager->Load_Effect(TEXT("RuiDad_Jump_EndGround"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Jump_Ground_Mark"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Jump_Player_Down"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Jump_Player_Up"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Jump_Up"));

		pEffect_Manager->Load_Effect(TEXT("RuiDad_Swing_Ground"));
		pEffect_Manager->Load_Effect(TEXT("RuiDad_Swing_Slash"));

		// 오의 히노카미
		pEffect_Manager->Load_Effect(TEXT("Special_Tanjiro(Hinokami)_Start"));
		pEffect_Manager->Load_Effect(TEXT("Special_Tanjiro(Hinokami)_Slash"));
		pEffect_Manager->Load_Effect(TEXT("Special_Tanjiro(Hinokami)_EndGround"));
		pEffect_Manager->Load_Effect(TEXT("Special_Tanjiro(Hinokami)_EndPlayer"));

		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion1_Slash1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion1_Slash2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion1_Sword1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Boom1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Boom2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Boom3"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Boom4"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Boom5"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Boom6"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Proj1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Proj2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Proj3"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Proj4"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Proj5"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Slash1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Slash2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Slash3"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Slash4"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Slash5"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Slash6"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion2_Sword"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion3_Proj1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion3_Slash1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion3_Slash2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion4_Proj1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion4_Slash1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion5_Proj1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion5_Proj2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion5_Ground"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion5_Slash1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion5_Slash2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Tan(Hi)_Motion5_Slash3"));

		//루이 오의
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion1_Web1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion2_Proj1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion2_RedWeb1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion2_TreadL1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion2_TreadR1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion3_RedWeb1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion3_TreadL1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion3_TreadR1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion3_Web1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion4_Proj1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion4_RedWeb1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion4_RedWeb2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion4_TreadL1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Rui_Motion4_TreadR1"));

		//Enmu
		pEffect_Manager->Load_Effect(TEXT("Enmu_Attack1"));
		pEffect_Manager->Load_Effect(TEXT("Enmu_Attack2_1"));
		pEffect_Manager->Load_Effect(TEXT("Enmu_Attack2_2"));
		pEffect_Manager->Load_Effect(TEXT("Enmu_Attack3_1"));
		pEffect_Manager->Load_Effect(TEXT("Enmu_Attack3_2"));
		pEffect_Manager->Load_Effect(TEXT("Enmu_Attack3_3"));
		pEffect_Manager->Load_Effect(TEXT("Enmu_Skill_Ball1"));
		pEffect_Manager->Load_Effect(TEXT("Enmu_Skill_Start"));

		//렌고쿠 오의
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion1_Sword1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion2_BG1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion2_Sword1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion2_Aura1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion2_Aura2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion3_Ground1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion4_BG1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion4_BG2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion4_Player1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion5_BG1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion5_Fire1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion6_BG1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion6_Hit"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion6_HitBG"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion7_BG"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion7_Fire1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion7_Fire2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion9_Boom"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion9_Ground"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion9_Proj1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion9_Proj2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion9_Tornado"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Ren_Motion10_Flash"));

		//Enmu_Boss
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat1_Flash"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat2_Chok"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat2_Flash"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat2_Flash2"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat2_Ground"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat3_Flash"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat3_Flash2"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat3_Ground"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat4_Flash"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat4_Flash2"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat4_Ground"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat4_Hand"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat5_Ground"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat5_Ground2"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat5_Hand"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat5_Wind"));
		pEffect_Manager->Load_Effect(TEXT("EnmuBoss_Pat5_Flash"));

		//아카자 오의
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion1_Hit"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion2_Ground"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion3_Ground"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion4_Dist"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion5_Dash1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion5_Hand1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion5_Hit1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion5_Hit2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion5_Kick1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion5_Run1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion6_Proj1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion6_Slash1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion6_Slash2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion7_BackLight"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion7_BG1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion7_BG2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion7_Hand"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion7_Hand2"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion7_Hand3"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion7_Proj1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion8_BG1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion8_Hand1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion8_Proj1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion9_Aura1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion9_BG1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion9_Proj1"));
		pEffect_Manager->Load_Effect(TEXT("Spl_Aka_Motion9_Boom1"));

		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Spl_1"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Spl_2"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Spl_3"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Spl_4_NonFol"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Spl_4_Ptc"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Spl_4_Weapon"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Spl_5"));
		pEffect_Manager->Load_Effect(TEXT("Effect_Shinobu_Spl_5_Weapon"));

		pEffect_Manager->Load_Effect(TEXT("Train_Smoke"));
		pEffect_Manager->Load_Effect(TEXT("Train_Wind"));
		pEffect_Manager->Load_Effect(TEXT("Fade"));

		RELEASE_INSTANCE(CEffect_Manager);
#pragma endregion Effect Object
	}
#pragma endregion Effect
	g_fLoading = 80.f;
	/* 카메라 객체 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	// 애니멀 객체
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Butterfly"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Butterfly/Butterfly.fbx", PivotMatrix))))
	//return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Butterfly"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Butterfly"),
		CButterfly::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Deer"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Deer1/Deer1.fbx", PivotMatrix))))
	//return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Deer"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Deer"),
		CDeer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Deer2"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Deer2/Deer2.fbx", PivotMatrix))))
	//return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Deer2"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Deer2"),
		CDeer2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bird"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Bird/Bird.fbx", PivotMatrix))))
	//return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Bird"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bird"),
		CBird::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_BigBird"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/BigBird/BigBird.fbx", PivotMatrix))))
		return E_FAIL;*/
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BigBird"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BigBird"),
		CBigBird::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Hoke"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Hoke/Hoke.fbx", PivotMatrix))))
	//	return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hoke"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hoke"),
		CHoke::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//NPC
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Murata"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Murata/Murata.fbx", PivotMatrix))))
	//	return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Murata"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Murata"),
		CMurata::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ozaki"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Ozaki/Ozaki.fbx", PivotMatrix))))
		return E_FAIL;*/
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Ozaki"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ozaki"),
		COzaki::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Goto"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Goto"),
		CGoto::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("AkazaBody"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AkazaBody"),
		CAkazaBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("AkazaLeg"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AkazaLeg"),
		CAkazaLeg::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RuiSister"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/RuiSister/RuiSister.fbx", PivotMatrix))))
		return E_FAIL;*/
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiSister"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiSister"),
		CRuiSister::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Train_Head"),
		CTrain_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.0f));


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Kyou"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/Kyou.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_KyouBro"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/KyouBro.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_KyouDad"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/KyouDad.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Mom"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/Mom.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_NPC0"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/NPC0.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_NPCF1"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/NPCF1.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_NPCF2"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/NPCF2.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_NPCF3"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/NPCF3.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_NPCM1"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/NPCM1.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_NPCM2"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/NPCM2.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_RuiSis"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/RuiSis.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Slayer1"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/Slayer1.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Slayer2"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/Slayer2.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Yosi"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/NPC/Yosi.fbx", PivotMatrix))))
	//	return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Kyou"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("KyouBro"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("KyouDad"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Mom"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("NPC0"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("NPCF1"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("NPCF2"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("NPCF3"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("NPCM1"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("NPCM2"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiSis"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Slayer1"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Slayer2"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Yosi"), LEVEL_STATIC, CData_Manager::DATA_ANIM);

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TrainNPC"),
		CTrainNPC::Create(m_pDevice, m_pContext))))
		return E_FAIL;

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

	// 아빠 거미
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("RuiDad"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiDad"),
		CRuiDad::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player"),
	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Player/Player.fbx", PivotMatrix))))
	return E_FAIL;*/
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Enmu"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enmu"),
		CEnmu::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Chaos_Head"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Enmu_Chaos_Head/Enmu_Chaos_Head.fbx", PivotMatrix))))
		return E_FAIL;
	//CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Enmu_Chaos_Head"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enmu_Chaos_Head"),
		CEnmu_Chaos_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Shield"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Enmu_Shield/Enmu_Shield.fbx", PivotMatrix))))
		return E_FAIL;
	//CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Enmu_Shield"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enmu_Shield"),
		CEnmu_Shield::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Right_Hand"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Enmu_Right_Hand/Enmu_Right_Hand.fbx", PivotMatrix))))
		return E_FAIL;
	//CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Enmu_Right_Hand"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enmu_Right_Hand"),
		CEnmu_Right_Hand::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Left_Hand"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Enmu_Left_Hand/Enmu_Left_Hand.fbx", PivotMatrix))))
		return E_FAIL;
	//CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Enmu_Left_Hand"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enmu_Left_Hand"),
		CEnmu_Left_Hand::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Chok"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Enmu_Chok/Enmu_Chok.fbx", PivotMatrix))))
		return E_FAIL;
	//CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Enmu_Chok"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enmu_Chok"),
		CEnmu_Chok::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enmu_ChokChok"),
		CEnmu_ChokChok::Create(m_pDevice, m_pContext))))
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

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_TanjiroWeapon2"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/TanjiroWeapon2/TanjiroWeapon2.fbx", PivotMatrix))))
	//return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TanjiroWeapon2"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TanjiroWeapon2"),
		CTanjiroWeapon2::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_TanjiroADV"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/TanjiroADV/TanjiroADV.fbx", PivotMatrix))))
	//return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TanjiroADV"), LEVEL_STATIC, CData_Manager::DATA_ANIM);

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Box"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/Box/Box.fbx", PivotMatrix))))
	//return E_FAIL;
	////CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Box"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Box"),
	//	CBox::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	g_fLoading = 90.f;
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

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiHead"),
		CRuiHead::Create(m_pDevice, m_pContext))))
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
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiMoveSkill"),
		CRuiMoveSkill::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiDadJump"),
		CRuiDadJump::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiDadBigStone"),
		CRuiDadBigStone::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiDadSmallStone"),
		CRuiDadSmallStone::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EnmuShoot"),
		CEnmuShoot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiSplColl"),
		CRuiSplColl::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_StoneSphere"),
	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/StoneSphere/StoneSphere.fbx", PivotMatrix))))
	return E_FAIL;
//	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("TanjiroWeapon2"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StoneSphere"),
		CStoneSphere::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RuiBomb"),
		CRuiBomb::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	

	//Map
	//CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BattleField"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BattleField"),
	//	CBattleField::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	//	CamAction
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CamLine"),
		CCamLine::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	// Navi
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Rui"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Navigation/11_Map_Rui.nav")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_RuiStory"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Navigation/RuiStory.nav")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TrainNavi"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Navigation/TrainNavi.nav")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TrainBattle"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Navigation/TrainBattle.nav")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Enmu_Navi"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/Navigation/Enmu_Navi.nav")))))
		return E_FAIL;
	g_fLoading = 95.f;
#pragma region UI객체
	//UI
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MGameHeart"),
		CMGameHeart::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BattleDialog"),
		CBattleDialog::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ResultUI"),
		CAdv_ResultBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FadeUI"),
		CFadeUIEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_InteractionUI"),
		CInteractionUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MapNameBar"),
		CMapNameBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MsgNameBase"),
		CMsgNameBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MsgTextBase"),
		CMsgTextBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FeedArrow"),
		CFeedArrow::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QuiestGuideBase"),
		CQuiestGuideBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QuiestKeyUI"),
		CQuiestKeyUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QuiestMainBase"),
		CQuiestMainBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QuiestSubBase"),
		CQuiestSubBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QuiestMainIcon"),
		CQuiestMainIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QuiestSubIcon"),
		CQuiestSubIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QuiestStampIcon"),
		CQuiestStampIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SelectMapFixedImg"),
		CSelMapFixedImg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SelMapNameBase"),
		CSelMapNameBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MapListPattern"),
		CSelMapListPattern::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SelMapImg"),
		CSelMapImg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LineFrame"),
		CLineFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SelMapIconFrame"),
		CSelMapIconFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MapIconImg"),
		CSelMapIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SelMapCursor"),
		CSelMapCursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SelMapEff"),
		CSelMapEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ResultScoreBar"),
		CResultScoreBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ResultScoreBase"),
		CResultScoreBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ResultTotalBase"),
		CResultTotalBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UltNumEff"),
		CUltNumEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UltStockFadeEff"),
		CUltStockFadeEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UltGaugeFire"),
		CUltFire::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UltGaugeFrame"),
		CUltGaugeFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_OpeningUltBar"),
		CUltOpenBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AdcMenuBG"),
		CAdcMenuBg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AdcMenuDeco"),
		CAdcMenuBgDeco::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AdcMenuCloud"),
		CAdcMenuCloud::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AdcMenuCursor"),
		CAdcMenuCursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AdcMenuDarkCloud"),
		CAdcMenuDarkCloud::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AdcMenuSelWindow"),
		CAdcMenuNameBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AdcMenuFrame"),
		CAdcMenuSelFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AdcMenuSelImg"),
		CAdcMenuSelImg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AdcMenuSelTxt"),
		CAdcMenuSelTxt::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ButtonEff"),
		CButtonEff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_OniSpeciSkillBar"),
		COniSpecialSkillBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
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
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChangeGagueBar"),
		CChangeBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion UI객체
	g_fLoading = 100.f;
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

HRESULT CLoader::Loading_ForMenu()
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

HRESULT CLoader::Loading_ForStoryMenu()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);


	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("                       텍스쳐 로딩 중."));



	/* 객체 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("                       객체 생성 중."));


	lstrcpy(m_szLoadingText, TEXT("                        로딩이 완료되었습니다."));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForAdvRui()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);


	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("                       텍스쳐 로딩 중."));



	/* 객체 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("                       객체 생성 중."));


	lstrcpy(m_szLoadingText, TEXT("                        로딩이 완료되었습니다."));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForSelectMap()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);


	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("                       텍스쳐 로딩 중."));



	/* 객체 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("                       객체 생성 중."));


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
