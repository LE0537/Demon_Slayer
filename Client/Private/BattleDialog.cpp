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

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

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
	if (!pUI_Manager->Get_StroyEvent(0) && !pUI_Manager->Get_BattleTypeCheck() && m_g_fDialogStartTime >= 3.f && pUI_Manager->Get_2P()->Get_PlayerInfo().strName == TEXT("루이"))
		Battle_RuiDialog();

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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(350.f, 540.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("넌....."), XMVectorSet(620.f, 595.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(350.f, 540.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("단숨에 널 죽이지는 않을 거야...!\n토막을 낸 후에 잘게 썰어 주마..."), XMVectorSet(460.f, 595.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(350.f, 540.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("하지만 아까 그 말을 취소한다면\n단번에 죽여줄게..."), XMVectorSet(460.f, 595.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄지로]"), XMVectorSet(350.f, 540.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("취소 안 해!! 내가 한 말은 틀리지 않았어!!\n이상한 건, 너다!!"), XMVectorSet(420.f, 595.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[카마도 탄지로]"), XMVectorSet(350.f, 540.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("틀린 건, 너야!!"), XMVectorSet(570.f, 595.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(350.f, 540.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("...그래. 그럼 아까 말한 대로\n널 토막 낸 후에 잘게 썰어줄게..."), XMVectorSet(460.f, 595.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
	case 6:
		if (pUI_Manager->Get_2P()->Get_PlayerInfo().iHp <= pUI_Manager->Get_2P()->Get_PlayerInfo().iMaxHp * 0.1f)
		{
			g_fEffect = 0.4f;
			g_fVoice = 0.f;
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(350.f, 540.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("이봐, 실의 강도가 이게 끝이라고 생각한 거야?"), XMVectorSet(460.f, 595.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
					pUI_Manager->Set_MsgCount(1);
			}
		}
		break;
	case 7:
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("[루이]"), XMVectorSet(350.f, 540.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("넌 이제 됐어. 잘 가라"), XMVectorSet(460.f, 595.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
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
		CSoundMgr::Get_Instance()->Listen_Voice();
		g_fEffect = 0.8f;
		g_fVoice = 0.7f;
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
