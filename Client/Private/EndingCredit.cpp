#include "stdafx.h"
#include "EndingCredit.h"
#include "GameInstance.h"

CEndingCredit::CEndingCredit(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CEndingCredit::CEndingCredit(const CEndingCredit & rhs)
	: CUI(rhs)
{
}

HRESULT CEndingCredit::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEndingCredit::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEndingCredit::Tick(_float fTimeDelta)
{
	m_fDelayTime += fTimeDelta;
}

void CEndingCredit::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CEndingCredit::Render()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	Ending();
	return S_OK;
}

void CEndingCredit::Ending()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	if (m_fDelayTime >= 5.f && m_fDelayTime <= 8.f)
	{
		pGameInstance->Render_Font(TEXT("Font_Nexon2"), TEXT("[ÆÀÀå ÀÌÀçÈÆ]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("´ã´ç : ÇÁ·¹ÀÓ ¿öÅ©"), XMVectorSet(720.f, 300.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));
	}
	else if (m_fDelayTime >= 15.f && m_fDelayTime <= 18.f)
	{
		/*pGameInstance->Render_Font(TEXT("Font_Nexon2"), TEXT("[ÆÀ¿ø Çã¼ºÁØ]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("´ã´ç : ½¦ÀÌ´õ, SubÀÌÆåÆ®"), XMVectorSet(740.f, 300.f, 0.f, 1.f), XMVectorSet(0.5451f, 0.f, 1.f, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));*/
		pGameInstance->Render_Font(TEXT("Font_Nexon2"), TEXT("[ÆÀ¿ø Çã¼ºÁØ]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("´ã´ç : ½¦ÀÌ´õ, SubÀÌÆåÆ®"), XMVectorSet(720.f, 300.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));
	}
	else if (m_fDelayTime >= 24.f && m_fDelayTime <= 26.f)
	{
		/*pGameInstance->Render_Font(TEXT("Font_Nexon2"), TEXT("[ÆÀ¿ø ÃÖ¹ÎÁØ]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("´ã´ç : ¾Ö´Ï¸ÞÀÌ¼Ç"), XMVectorSet(740.f, 300.f, 0.f, 1.f), XMVectorSet(0.f, 0.3725f, 1.f, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));*/
		pGameInstance->Render_Font(TEXT("Font_Nexon2"), TEXT("[ÆÀ¿ø ÃÖ¹ÎÁØ]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("´ã´ç : ¾Ö´Ï¸ÞÀÌ¼Ç"), XMVectorSet(720.f, 300.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));
	}
	else if (m_fDelayTime >= 31.f && m_fDelayTime <= 35.f)
	{
		/*pGameInstance->Render_Font(TEXT("Font_Nexon2"), TEXT("[ÆÀ¿ø ¼®À±Áö]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("´ã´ç : MainÀÌÆåÆ®"), XMVectorSet(740.f, 300.f, 0.f, 1.f), XMVectorSet(1.f, 0.2f, 0.6f, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));*/
		pGameInstance->Render_Font(TEXT("Font_Nexon2"), TEXT("[ÆÀ¿ø ¼®À±Áö]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("´ã´ç : MainÀÌÆåÆ®"), XMVectorSet(720.f, 300.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));
	}
	else if (m_fDelayTime >= 37.f && m_fDelayTime <= 40.f)
	{
		pGameInstance->Render_Font(TEXT("Font_Nexon2"), TEXT("[ÆÀ¿ø Á¤¼®ÈÆ]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("´ã´ç : UI"), XMVectorSet(720.f, 300.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));
	}
	else if(m_fDelayTime >= 44.f )
		pGameInstance->Render_Font(TEXT("Font_Nexon2"), TEXT("130±â ÆÀ Æ÷Æ®Æú¸®¿À ³¡!\n°í»ýÇÏ¼Ì½À´Ï´Ù~"), XMVectorSet(300.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CEndingCredit::Ready_Components()
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

HRESULT CEndingCredit::SetUp_ShaderResources()
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

CEndingCredit * CEndingCredit::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEndingCredit*	pInstance = new CEndingCredit(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEndingCredit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CEndingCredit::Clone(void * pArg)
{
	CEndingCredit*	pInstance = new CEndingCredit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEndingCredit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEndingCredit::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
