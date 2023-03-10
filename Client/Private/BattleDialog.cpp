#include "stdafx.h"
#include "BattleDialog.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SoundMgr.h"

CBattleDialog::CBattleDialog(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CBattleDialog::CBattleDialog(const CBattleDialog & rhs)
	: CUI(rhs)
{
}

HRESULT CBattleDialog::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBattleDialog::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fNameX = 400.f;
	m_fNameY = 550.f;
	m_fX = 640.f;
	m_fY = 615.f;
	m_fSplX = 640.f;
	m_fSplY = 610.f;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	pUI_Manager->Set_DialogUI(this);

	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

void CBattleDialog::Tick(_float fTimeDelta)
{
	if (m_bFontFadeCheck)
	{
		m_fFontFade += 0.1f;
		if (m_fFontFade >= 1.f)
			m_fFontFade = 1.f;
	}
	else
	{
		m_fFontFade -= 0.1f;
		if (m_fFontFade <= 0.f)
			m_fFontFade = 0.f;
	}
	m_g_fDialogStartTime += fTimeDelta;

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (g_iLevel == LEVEL_GAMEPLAY && !pUI_Manager->Get_BattleTypeCheck() && pUI_Manager->Get_2P()->Get_PlayerInfo().strName == TEXT("루이") && pUI_Manager->Get_MsgCount() >= 8)
		m_fDelay += fTimeDelta;
	else if (g_iLevel == LEVEL_BOSSENMU)
	{
		m_fDelay += fTimeDelta;
		if (pUI_Manager->Get_MsgCount() == 3)
			m_fDelay2 += fTimeDelta;
	}
	else if (g_iLevel == LEVEL_GAMEPLAY && !pUI_Manager->Get_BattleTypeCheck() && pUI_Manager->Get_2P()->Get_PlayerInfo().strName != TEXT("루이"))
	{
		m_fDelay += fTimeDelta;
		if (pUI_Manager->Get_MsgCount() == 2)
			m_fDelay2 += fTimeDelta;
	}
	else if(g_iLevel == LEVEL_GAMEPLAY && pUI_Manager->Get_BattleTypeCheck() && m_bSplDialogStart)
		m_fDelay += fTimeDelta;

	RELEASE_INSTANCE(CUI_Manager);
}

void CBattleDialog::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CBattleDialog::Render()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	if (!pUI_Manager->Get_BattleTypeCheck())
	{
		if (!pUI_Manager->Get_StroyEvent(0) && !pUI_Manager->Get_BattleTypeCheck() && m_g_fDialogStartTime >= 3.f && pUI_Manager->Get_2P()->Get_PlayerInfo().strName == TEXT("루이") && !m_bDialogCheck)
			Battle_RuiDialog();
		else if (pUI_Manager->Get_StroyEvent(0) && !pUI_Manager->Get_StroyEvent(1) && !pUI_Manager->Get_BattleTypeCheck() && pUI_Manager->Get_2P()->Get_PlayerInfo().strName == TEXT("루이") && !m_bDialog3Start)
			Battle_RuiDialog2();
		else if (m_bDialog3Start)
			Battle_RuiDialog3();
		else if (g_iLevel == LEVEL_BATTLEENMU && m_bStartDialog)
			Battle_EnmuDialog();
		else if (g_iLevel == LEVEL_BOSSENMU && m_fDelay >= 3.f && !m_bEndDialog)
			Battle_BossEnmuDialog();
		else if (g_iLevel == LEVEL_GAMEPLAY && pUI_Manager->Get_2P()->Get_PlayerInfo().strName == TEXT("아카자") && m_fDelay >= 9.f)
			Battle_Akaza();
	}
	else
	{
		if (g_iLevel == LEVEL_GAMEPLAY && m_bSplDialogStart)
			Spl_Dialog();
	}

	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

void CBattleDialog::Battle_RuiDialog()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	switch (pUI_Manager->Get_MsgCount())
	{
	case 0:
		g_fEffect = 0.4f;
		g_fVoice = 0.f;
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("넌....."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog_0.wav"), g_fDialog);
			m_bFontFadeCheck = true;
			m_bSoundCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 1:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("단숨에 널 죽이지는 않을 거야...!\n토막을 낸 후에 잘게 썰어 주마..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog_1.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 2:
		
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("하지만 아까 그 말을 취소한다면\n단번에 죽여줄게..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog_2.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 3:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄지로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("취소 안 해!! 내가 한 말은 틀리지 않았어!!\n이상한 건, 너다!!"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog_3.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 4:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄지로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("틀린 건, 너야!!"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog_4.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 5:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("...그래. 그럼 아까 말한 대로\n널 토막 낸 후에 잘게 썰어줄게..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog_5.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
			{
				g_fEffect = 0.8f;
				g_fVoice = 0.7f;
				pUI_Manager->Set_MsgCount(1);
			}
		}
		break;
	default:
		CSoundMgr::Get_Instance()->Listen_Voice();
		pUI_Manager->Reset_MsgCount();
		g_fEffect = 0.8f;
		g_fVoice = 0.7f;
		m_bDialogCheck = true;
		break;
	}


	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CBattleDialog::Battle_RuiDialog2()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (pUI_Manager->Get_MsgCount())
	{
	case 0:
		if (m_bDialogSwitch)
		{
			g_fEffect = 0.4f;
			g_fVoice = 0.f;
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("이봐, 실의 강도가 이게 끝이라고 생각한 거야?"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade , m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			if (!m_bSoundCheck)
			{
				CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog_6.wav"), g_fDialog);
				m_bSoundCheck = true;
				m_bFontFadeCheck = true;
			}
			CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
			if (!m_bIsPlaying)
			{
				m_bFontFadeCheck = false;
				if (m_fFontFade <= 0.f)
				{
					pUI_Manager->Set_MsgCount(1);
					m_bDialogSwitch = false;
				}
			}
		}
		break;
	case 1:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("넌 이제 됐어. 잘 가라"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog_7.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		
		}
		break;
	default:
		//pUI_Manager->Reset_MsgCount();
		g_fEffect = 0.8f;
		g_fVoice = 0.4f;
		break;
	}
	
	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CBattleDialog::Battle_RuiDialog3()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (pUI_Manager->Get_MsgCount())
	{
	case 0:
		if (!m_bSoundValumCheck)
		{
			g_fBGM -= 0.05f;
			g_fEffect -= 0.05f;
			g_fVoice -= 0.05f;

			if (g_fBGM <= 0.f)
				g_fBGM = 0.f;
			if (g_fEffect <= 0.f)
				g_fEffect = 0.f;
			if (g_fVoice <= 0.f)
				g_fVoice = 0.f;
			if (g_fVoice <= 0.f && g_fVoice <= 0.f && g_fVoice <= 0.f)
			{
				g_fBGM = 1.f;
				g_fEffect = 0.4f;
				CSoundMgr::Get_Instance()->Effect_Stop(SOUND_BGM);
				CSoundMgr::Get_Instance()->PlayBGM(TEXT("Tanjiro_noUta.wav"), g_fBGM);
				m_bSoundValumCheck = true;
			}
		}

		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄쥬로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("탄지로, 호흡이다. 호흡을 가다듬고..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog2_0.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 1:
		m_bSoundValumCheck = false;
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄쥬로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("히노카미 님이 되는 거야"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog2_1.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 2:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄지로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(진정해, 감정적으로 굴지 마!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog2_2.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 3:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄지로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(집중해라! 호흡을 가다듬어라!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog2_3.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 4:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄지로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(눈 깜빡할 새도 없이 벌써 새로운 실이 펼쳐졌다)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog3_0.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 5:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄지로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(만약 지금 일단 물러선다 해도)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog3_1.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 6:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄지로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(물의 호흡에서 히노카미 카구라 호흡으로\n억지로 바꾼 반동이 온다!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog3_2.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 7:
		if (pUI_Manager->Get_2P()->Get_PlayerInfo().iHp <= 0)
		{
			g_fBGM = 1.f;
			g_fDialog = 1.f;
			g_fEffect = 0.f;
			g_fVoice = 0.f;
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄지로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(보였다!! 틈새의 실!!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			if (!m_bSoundCheck)
			{
				CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog3_3.wav"), g_fDialog);
				m_bSoundCheck = true;
				m_bFontFadeCheck = true;
			}
			CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
			if (!m_bIsPlaying)
			{
				m_bFontFadeCheck = false;
				if (m_fFontFade <= 0.f)
				{
					pUI_Manager->Set_MsgCount(1);
					g_fEffect = 0.5f;
					g_fVoice = 0.5f;
					m_bDelayStart = true;
					m_fDelay = 0.f;
				}
			}
		}
		else if(pUI_Manager->Get_2P()->Get_PlayerInfo().iHp <= pUI_Manager->Get_2P()->Get_PlayerInfo().iMaxHp * 0.5f)
		{
			if (m_bSoundCheck)
			{
				g_fEffect = 0.5f;
				g_fVoice = 0.5f;
				g_fBGM = 1.f;
				CSoundMgr::Get_Instance()->Effect_Stop(SOUND_BGM);
				CSoundMgr::Get_Instance()->PlayBGM(TEXT("Tanjiro_noUta2.wav"), g_fBGM);
				m_bSoundCheck = false;
			}
		}
		break;
	case 8:
		if (m_fDelay >= 4.f)
		{
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄지로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("널, 지금 여기서 쓰러뜨린다!!"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			if (m_bSoundCheck)
			{
				CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog3_4.wav"), g_fDialog);
				m_bSoundCheck = false;
				m_bFontFadeCheck = true;
			}
			CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
			if (!m_bIsPlaying)
			{
				m_bFontFadeCheck = false;
				if (m_fFontFade <= 0.f)
				{
					pUI_Manager->Set_MsgCount(1);
					m_fDelay = 0.f;
				}
			}
		}
		break;
	case 9:
		if (m_fDelay >= 4.f)
		{
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(따뜻해.. 햇살처럼..)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			if (!m_bSoundCheck)
			{
				CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleRui_Dialog3_5.wav"), g_fDialog);
				m_bSoundCheck = true;
				m_bFontFadeCheck = true;
			}
			CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
			if (!m_bIsPlaying)
			{
				m_bFontFadeCheck = false;
				if (m_fFontFade <= 0.f)
					pUI_Manager->Set_MsgCount(1);
			}
		}
		break;
	default:
		break;
	}

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CBattleDialog::Battle_EnmuDialog()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (pUI_Manager->Get_MsgCount())
	{
	case 0:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄쥬로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(베는 느낌이 거의 없어... 혹시 이것도 꿈인가?)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleEnmu_Dialog_0.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 1:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[엔무]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("목을 뱄는데도 왜 안 죽는지 알고 싶지?"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleEnmu_Dialog_1.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 2:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[엔무]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("좋아. 난 지금 엄청 흥분했거든\n갓난아기도 알 만큼 단순한 거야. 우후후~"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleEnmu_Dialog_2.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 3:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[엔무]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("그게 더 이상 본체가 아니기 때문이지"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleEnmu_Dialog_3.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 4:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[엔무]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("나는 이 기차와 '융합'했거든!"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleEnmu_Dialog_4.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 5:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[엔무]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("너는 여기서 잠이 드는 거야..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleEnmu_Dialog_5.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 6:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[엔무]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("아주 깊~게 잠들어라..우후후후후..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("BattleEnmu_Dialog_6.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	default:
		//pUI_Manager->();
		m_bEndDialog = true;
		break;
	}

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CBattleDialog::Battle_BossEnmuDialog()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (pUI_Manager->Get_MsgCount())
	{
	case 0:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[엔무]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("너는 내 목에 상처를 낼 수 없어"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Boss_Enmu_Dialog_0.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 1:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[엔무]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("악몽을 꾸고 괴로워하며 죽을 테니까... 우후후..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Boss_Enmu_Dialog_1.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 2:
		if (m_bStartDialog)
		{
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄쥬로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(빨리 목을!!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			if (!m_bSoundCheck)
			{
				CSoundMgr::Get_Instance()->PlayDialog(TEXT("Boss_Enmu_Dialog_2.wav"), g_fDialog);
				m_bSoundCheck = true;
				m_bFontFadeCheck = true;
			}
			CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
			if (!m_bIsPlaying)
			{
				m_bFontFadeCheck = false;
				if (m_fFontFade <= 0.f)
					pUI_Manager->Set_MsgCount(1);
			}
		}
		break;
	case 3:
		if (m_fDelay2 >= 3.f)
		{
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄쥬로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(이 일격으로 뼈를 베어야 해!!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			if (m_bSoundCheck)
			{
				CSoundMgr::Get_Instance()->PlayDialog(TEXT("Boss_Enmu_Dialog_3.wav"), g_fDialog);
				m_bSoundCheck = false;
				m_bFontFadeCheck = true;
			}
			CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
			if (!m_bIsPlaying)
			{
				m_bFontFadeCheck = false;
				if (m_fFontFade <= 0.f)
					pUI_Manager->Set_MsgCount(1);
			}
		}
		break;
	case 4:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄쥬로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(히노카미 카구라! 푸른 비단 하늘!!!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Boss_Enmu_Dialog_4.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 5:
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Boss_Enmu_Dialog_5.wav"), g_fDialog);
			m_bSoundCheck = false;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
			pUI_Manager->Set_MsgCount(1);
		break;
	default:
		pUI_Manager->Reset_MsgCount();
		m_bEndDialog = true;
		break;
	}

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CBattleDialog::Battle_Akaza()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (pUI_Manager->Get_MsgCount())
	{
	case 0:
		g_fVoice = 0.f;
		g_fEffect = 0.4f;
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄쥬로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(상현......3?!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Battle_Akaza_Dialog_0.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 1:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄쥬로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(왜 지금 여기에...?)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Battle_Akaza_Dialog_1.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 2:
		if (m_fDelay2 >= 3.f)
		{
			g_fVoice = 0.f;
			g_fEffect = 0.4f;
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[렌고쿠 쿄쥬로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("왜 부상당한 사람부터 노리는지 이해가 안 된다"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			if (!m_bSoundCheck)
			{
				CSoundMgr::Get_Instance()->PlayDialog(TEXT("Battle_Akaza_Dialog_2.wav"), g_fDialog);
				m_bSoundCheck = true;
				m_bFontFadeCheck = true;
			}
			CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
			if (!m_bIsPlaying)
			{
				m_bFontFadeCheck = false;
				if (m_fFontFade <= 0.f)
					pUI_Manager->Set_MsgCount(1);
			}
		}
		else
		{
			g_fVoice = 0.7f;
			g_fEffect = 0.8f;
		}
		break;
	case 3:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[아카자]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("너와 나 사이의 대화에 방해가 될 것 같았거든"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Battle_Akaza_Dialog_3.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 4:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[렌고쿠 쿄쥬로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("너와 내가 무슨 대화를 하지?\n초면이지만 난 이미 네가 싫은데"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Battle_Akaza_Dialog_4.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 5:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[아카자]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("그래? 나도 약한 인간이 정말 싫어\n약자를 보면 신물이 올라오지"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Battle_Akaza_Dialog_5.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 6:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[아카자]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("끝내주는 제안을 하마\n너도 혈귀가 되지 않겠나?"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Battle_Akaza_Dialog_6.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 7:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[렌고쿠 쿄쥬로]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("싫다"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Battle_Akaza_Dialog_7.wav"), g_fDialog);
			m_bSoundCheck = false;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	case 8:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[아카자]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("그래? 혈귀가 되지 않겠다면 죽이겠다"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		if (!m_bSoundCheck)
		{
			CSoundMgr::Get_Instance()->PlayDialog(TEXT("Battle_Akaza_Dialog_8.wav"), g_fDialog);
			m_bSoundCheck = true;
			m_bFontFadeCheck = true;
		}
		CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
		if (!m_bIsPlaying)
		{
			m_bFontFadeCheck = false;
			if (m_fFontFade <= 0.f)
				pUI_Manager->Set_MsgCount(1);
		}
		break;
	default:
		//pUI_Manager->Reset_MsgCount();
		g_fVoice = 0.7f;
		g_fEffect = 0.8f;
		m_bEndDialog = true;
		break;
	}

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CBattleDialog::Spl_Dialog()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (pUI_Manager->Get_MsgCount())
	{
	case 0:
		if (m_iCharNum == 0)
		{
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("이야아아아아아아압!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			m_bFontFadeCheck = true;
			if (m_fDelay >= 3.f)
				m_bFontFadeCheck = false;
		}
		else if (m_iCharNum == 1)
		{
			if (m_fDelay >= 1.f)
			{
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("오의!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_bFontFadeCheck = true;
				if (m_fDelay >= 3.f)
				{
					m_bFontFadeCheck = false;
					pUI_Manager->Set_MsgCount(1);
				}
			}
		}
		else if (m_iCharNum == 2)
		{
			if (m_fDelay >= 4.f)
			{
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("혈귀술"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_bFontFadeCheck = true;
				if (m_fDelay >= 6.f)
				{
					m_bFontFadeCheck = false;
					pUI_Manager->Set_MsgCount(1);
				}
			}
		}
		else if (m_iCharNum == 3)
		{
			if (m_fDelay >= 0.5f)
			{
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("술식 전개"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_bFontFadeCheck = true;
				if (m_fDelay >= 2.5f)
				{
					m_bFontFadeCheck = false;
					pUI_Manager->Set_MsgCount(1);
				}
			}
		}
		else if (m_iCharNum == 4)
		{
			if (m_fDelay >= 7.f)
			{
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("혈귀술!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_bFontFadeCheck = true;
				if (m_fDelay >= 9.f)
				{
					m_bFontFadeCheck = false;
					pUI_Manager->Set_MsgCount(1);
				}
			}
		}
		else if (m_iCharNum == 5)
		{
			if (m_fDelay >= 3.f)
			{
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("벌레의 호흡"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_bFontFadeCheck = true;
				if (m_fDelay >= 4.f)
				{
					m_bFontFadeCheck = false;
					pUI_Manager->Set_MsgCount(1);
				}
			}
		}
		break;
	case 1:
		if (!m_bResetCheck)
		{
			m_fDelay = 0.f;
			m_bResetCheck = true;
		}

		if (m_iCharNum == 0)
		{
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("히노카미 카구라!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			m_bFontFadeCheck = true;
			if (m_fDelay >= 3.f)
			{
				m_bFontFadeCheck = false;
				m_bResetCheck = false;
				pUI_Manager->Set_MsgCount(1);
			}
		}
		else if (m_iCharNum == 1)
		{
			if (m_fDelay >= 1.f)
			{
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("제9형 연옥!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_bFontFadeCheck = true;
				if (m_fDelay >= 4.f)
				{
					m_bFontFadeCheck = false;
					m_bResetCheck = false;
					pUI_Manager->Set_MsgCount(1);
				}
			}
		}
		else if (m_iCharNum == 2)
		{
			if (m_fDelay >= 1.f)
			{
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("각사윤전"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_bFontFadeCheck = true;
				if (m_fDelay >= 3.f)
				{
					m_bFontFadeCheck = false;
					m_bResetCheck = false;
					pUI_Manager->Set_MsgCount(3);
				}
			}
		}
		else if (m_iCharNum == 3)
		{
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("파괴살·나침"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			m_bFontFadeCheck = true;
			if (m_fDelay >= 2.f)
			{
				m_bFontFadeCheck = false;
				m_bResetCheck = false;
				pUI_Manager->Set_MsgCount(1);
			}
		}
		else if (m_iCharNum == 4)
		{
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("폭혈!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			m_bFontFadeCheck = true;
			if (m_fDelay >= 2.f)
			{
				m_bFontFadeCheck = false;
				m_bResetCheck = false;
				pUI_Manager->Set_MsgCount(3);
			}
		}
		else if (m_iCharNum == 5)
		{
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("나비의 춤"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			m_bFontFadeCheck = true;
			if (m_fDelay >= 2.f)
			{
				m_bFontFadeCheck = false;
				m_bResetCheck = false;
				pUI_Manager->Set_MsgCount(1);
			}
		}
		break;
	case 2:
		if (!m_bResetCheck)
		{
			m_fDelay = 0.f;
			m_bResetCheck = true;
		}

		if (m_iCharNum == 0)
		{
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("푸른 비단 하늘!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			m_bFontFadeCheck = true;
			if (m_fDelay >= 2.f)
			{
				m_bFontFadeCheck = false;
				m_bResetCheck = false;
				pUI_Manager->Set_MsgCount(2);
			}
		}
		else if (m_iCharNum == 1)
		{
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("이야아아아아아압!!!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			m_bFontFadeCheck = true;
			if (m_fDelay >= 3.f)
			{
				m_bFontFadeCheck = false;
				m_bResetCheck = false;
				pUI_Manager->Set_MsgCount(2);
			}
		}
		else if (m_iCharNum == 3)
		{
			if (m_fDelay >= 4.f)
			{
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("파괴살"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_bFontFadeCheck = true;
				if (m_fDelay >= 6.f)
				{
					m_bFontFadeCheck = false;
					m_bResetCheck = false;
					pUI_Manager->Set_MsgCount(1);
				}
			}
		}
		else if (m_iCharNum == 5)
		{
			if (m_fDelay >= 2.f)
			{
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("'장난'"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_bFontFadeCheck = true;
				if (m_fDelay >= 4.f)
				{
					m_bFontFadeCheck = false;
					m_bResetCheck = false;
					pUI_Manager->Set_MsgCount(2);
				}
			}
		}
		break;
	case 3:
		if (!m_bResetCheck)
		{
			m_fDelay = 0.f;
			m_bResetCheck = true;
		}

		if (m_iCharNum == 3)
		{
			if (m_fDelay >= 2.f)
			{
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("멸식!!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_bFontFadeCheck = true;
				if (m_fDelay >= 4.f)
				{
					m_bFontFadeCheck = false;
					m_bResetCheck = false;
					pUI_Manager->Set_MsgCount(1);
				}
			}
		}
		break;
	default:
		pUI_Manager->Reset_MsgCount();
		m_bSplDialogStart = false;
		m_fDelay = 0.f;
		break;
	}

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

HRESULT CBattleDialog::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UIVtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBattleDialog::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	return S_OK;
}

CBattleDialog * CBattleDialog::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBattleDialog*	pInstance = new CBattleDialog(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CBattleDialog"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CBattleDialog::Clone(void * pArg)
{
	CBattleDialog*	pInstance = new CBattleDialog(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CBattleDialog"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattleDialog::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
