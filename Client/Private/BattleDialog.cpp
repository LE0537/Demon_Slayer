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

	if (g_iLevel == LEVEL_GAMEPLAY && !pUI_Manager->Get_BattleTypeCheck() && pUI_Manager->Get_2P()->Get_PlayerInfo().strName == TEXT("����") && pUI_Manager->Get_MsgCount() >= 8)
		m_fDelay += fTimeDelta;
	else if (g_iLevel == LEVEL_BOSSENMU)
	{
		m_fDelay += fTimeDelta;
		if (pUI_Manager->Get_MsgCount() == 3)
			m_fDelay2 += fTimeDelta;
	}
	else if (g_iLevel == LEVEL_GAMEPLAY && !pUI_Manager->Get_BattleTypeCheck() && pUI_Manager->Get_2P()->Get_PlayerInfo().strName != TEXT("����"))
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
		if (!pUI_Manager->Get_StroyEvent(0) && !pUI_Manager->Get_BattleTypeCheck() && m_g_fDialogStartTime >= 3.f && pUI_Manager->Get_2P()->Get_PlayerInfo().strName == TEXT("����") && !m_bDialogCheck)
			Battle_RuiDialog();
		else if (pUI_Manager->Get_StroyEvent(0) && !pUI_Manager->Get_StroyEvent(1) && !pUI_Manager->Get_BattleTypeCheck() && pUI_Manager->Get_2P()->Get_PlayerInfo().strName == TEXT("����") && !m_bDialog3Start)
			Battle_RuiDialog2();
		else if (m_bDialog3Start)
			Battle_RuiDialog3();
		else if (g_iLevel == LEVEL_BATTLEENMU && m_bStartDialog)
			Battle_EnmuDialog();
		else if (g_iLevel == LEVEL_BOSSENMU && m_fDelay >= 3.f && !m_bEndDialog)
			Battle_BossEnmuDialog();
		else if (g_iLevel == LEVEL_GAMEPLAY && pUI_Manager->Get_2P()->Get_PlayerInfo().strName == TEXT("��ī��") && m_fDelay >= 9.f)
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("��....."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�ܼ��� �� �������� ���� �ž�...!\n�丷�� �� �Ŀ� �߰� ��� �ָ�..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("������ �Ʊ� �� ���� ����Ѵٸ�\n�ܹ��� �׿��ٰ�..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("��� �� ��!! ���� �� ���� Ʋ���� �ʾҾ�!!\n�̻��� ��, �ʴ�!!"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("Ʋ�� ��, �ʾ�!!"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("...�׷�. �׷� �Ʊ� ���� ���\n�� �丷 �� �Ŀ� �߰� ����ٰ�..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�̺�, ���� ������ �̰� ���̶�� ������ �ž�?"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade , m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�� ���� �ƾ�. �� ����"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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

		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź���]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("ź����, ȣ���̴�. ȣ���� ���ٵ��..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź���]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("����ī�� ���� �Ǵ� �ž�"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(������, ���������� ���� ��!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(�����ض�! ȣ���� ���ٵ���!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(�� ������ ���� ���� ���� ���ο� ���� ��������)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(���� ���� �ϴ� �������� �ص�)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(���� ȣ���� ����ī�� ī���� ȣ������\n������ �ٲ� �ݵ��� �´�!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(������!! ƴ���� ��!!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("��, ���� ���⼭ �����߸���!!"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(������.. �޻�ó��..)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź���]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(���� ������ ���� ����... Ȥ�� �̰͵� ���ΰ�?)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("���� ��µ��� �� �� �״��� �˰� ����?"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("����. �� ���� ��û ����߰ŵ�\n�����Ʊ⵵ �� ��ŭ �ܼ��� �ž�. ������~"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�װ� �� �̻� ��ü�� �ƴϱ� ��������"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("���� �� ������ '����'�߰ŵ�!"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�ʴ� ���⼭ ���� ��� �ž�..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("���� ��~�� �����..����������..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�ʴ� �� �� ��ó�� �� �� ����"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�Ǹ��� �ٰ� ���ο��ϸ� ���� �״ϱ�... ������..."), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź���]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(���� ����!!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź���]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(�� �ϰ����� ���� ����� ��!!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź���]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(����ī�� ī����! Ǫ�� ��� �ϴ�!!!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź���]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(����......3?!)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[ī���� ź���]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("(�� ���� ���⿡...?)"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[������ �����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�� �λ���� ������� �븮���� ���ذ� �� �ȴ�"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[��ī��]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�ʿ� �� ������ ��ȭ�� ���ذ� �� �� ���Ұŵ�"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[������ �����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�ʿ� ���� ���� ��ȭ�� ����?\n�ʸ������� �� �̹� �װ� ������"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[��ī��]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�׷�? ���� ���� �ΰ��� ���� �Ⱦ�\n���ڸ� ���� �Ź��� �ö����"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[��ī��]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�����ִ� ������ �ϸ�\n�ʵ� ���Ͱ� ���� �ʰڳ�?"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[������ �����]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�ȴ�"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[��ī��]"), XMVectorSet(m_fNameX, m_fNameY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�׷�? ���Ͱ� ���� �ʰڴٸ� ���̰ڴ�"), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�̾߾ƾƾƾƾƾƾ�!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			m_bFontFadeCheck = true;
			if (m_fDelay >= 3.f)
				m_bFontFadeCheck = false;
		}
		else if (m_iCharNum == 1)
		{
			if (m_fDelay >= 1.f)
			{
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("����!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("���ͼ�"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("���� ����"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("���ͼ�!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("������ ȣ��"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("����ī�� ī����!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("��9�� ����!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("��������"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�ı��졤��ħ"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("����!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("������ ��"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("Ǫ�� ��� �ϴ�!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�̾߾ƾƾƾƾƾ�!!!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("�ı���"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("'�峭'"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
				pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("���!!"), XMVectorSet(m_fSplX, m_fSplY, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 0.f, 1.f));
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
