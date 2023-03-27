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



	m_fSizeX = 384.f;
	m_fSizeY = 288.f;
	m_fX = 950.f;
	m_fY = 550.f;


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));


	

	return S_OK;
}

void CEndingCredit::Tick(_float fTimeDelta)
{
	m_fDelayTime += fTimeDelta;

	if (m_fDelayTime >= 4.5f && m_fDelayTime <= 8.5f)
	{
		m_iTextureNum = 0;
		m_bRenderTrue = true;
		m_fFrame += fTimeDelta;
		if (m_fFrame > 0.0851)
		{
			m_fFrame = 0.f;
			++m_iFrame;
			if (m_iFrame > 46)
				m_fFrame = 46;
		}
	}
	else if (m_fDelayTime >= 14.5f && m_fDelayTime <= 18.5f)
	{
		m_iTextureNum = 1;
		m_bRenderTrue = true;
		m_fFrame += fTimeDelta;
		if (m_fFrame > 0.0888)
		{
			m_fFrame = 0.f;
			++m_iFrame;
			if (m_iFrame > 44)
				m_fFrame = 44;
		}
	}
	else if (m_fDelayTime >= 23.f && m_fDelayTime <= 27.f)
	{
		m_iTextureNum = 2;
		m_bRenderTrue = true;
		m_fFrame += fTimeDelta;
		if (m_fFrame > 0.0869)
		{
			m_fFrame = 0.f;
			++m_iFrame;
			if (m_iFrame > 45)
				m_fFrame = 45;
		}
	}
	else if (m_fDelayTime >= 30.5f && m_fDelayTime <= 34.5f)
	{
		m_iTextureNum = 3;
		m_bRenderTrue = true;
		m_fFrame += fTimeDelta;
		if (m_fFrame > 0.0888)
		{
			m_fFrame = 0.f;
			++m_iFrame;
			if (m_iFrame > 44)
				m_fFrame = 44;
		}
	}
	else if (m_fDelayTime >= 36.5f && m_fDelayTime <= 40.5f)
	{
		m_iTextureNum = 4;
		m_bRenderTrue = true;
		m_fFrame += fTimeDelta;
		if (m_fFrame > 0.0869)
		{
			m_fFrame = 0.f;
			++m_iFrame;
			if (m_iFrame > 45)
				m_fFrame = 45;
		}
	}
	else
	{
		m_bRenderTrue = false;
		m_fFrame = 0.f;
		m_iFrame = 0;
	}
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
	if (m_bRenderTrue)
	{
		if (FAILED(SetUp_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(0);
		m_pVIBufferCom->Render();
	}
	Ending();
	return S_OK;
}

void CEndingCredit::Ending()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	if (m_fDelayTime >= 4.5f && m_fDelayTime <= 8.5f)
	{
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon2"), TEXT("[팀장 이재훈]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon"), TEXT("담당 : 프레임 워크"), XMVectorSet(720.f, 300.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));
	}
	else if (m_fDelayTime >= 14.5f && m_fDelayTime <= 18.5f)
	{
		/*pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon2"), TEXT("[팀원 허성준]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon"), TEXT("담당 : 쉐이더, Sub이펙트"), XMVectorSet(740.f, 300.f, 0.f, 1.f), XMVectorSet(0.5451f, 0.f, 1.f, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));*/
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon2"), TEXT("[팀원 허성준]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon"), TEXT("담당 : 쉐이더, Sub이펙트"), XMVectorSet(720.f, 300.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));
	}
	else if (m_fDelayTime >= 23.f && m_fDelayTime <= 27.f)
	{
		/*pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon2"), TEXT("[팀원 최민준]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon"), TEXT("담당 : 애니메이션"), XMVectorSet(740.f, 300.f, 0.f, 1.f), XMVectorSet(0.f, 0.3725f, 1.f, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));*/
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon2"), TEXT("[팀원 최민준]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon"), TEXT("담당 : 애니메이션"), XMVectorSet(720.f, 300.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));
	}
	else if (m_fDelayTime >= 30.5f && m_fDelayTime <= 34.5f)
	{
		/*pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon2"), TEXT("[팀원 석윤지]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon"), TEXT("담당 : Main이펙트"), XMVectorSet(740.f, 300.f, 0.f, 1.f), XMVectorSet(1.f, 0.2f, 0.6f, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));*/
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon2"), TEXT("[팀원 석윤지]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon"), TEXT("담당 : Main이펙트"), XMVectorSet(720.f, 300.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));
	}
	else if (m_fDelayTime >= 36.5f && m_fDelayTime <= 40.5f)
	{
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon2"), TEXT("[팀원 정석훈]"), XMVectorSet(700.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(2.f, 2.f, 0.f, 1.f));
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon"), TEXT("담당 : UI"), XMVectorSet(720.f, 300.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.2f, 1.2f, 0.f, 1.f));
	}
	else if(m_fDelayTime >= 44.f )
		pGameInstance->Render_Font_Defalut(TEXT("Font_Nexon2"), TEXT("* 이 게임은 비상업적 목적으로 제작 되었습니다.\n\n           제작기간   23-01-13 ~ 23-03-13 \n\n       두달간 예비 클라이언트 프로그래머들이 \n      DirectX 11을 활용하여 개발 하였습니다.\n\n                             감사합니다."), XMVectorSet(300.f, 150.f, 0.f, 1.f), XMVectorSet(m_fFontFade, m_fFontFade, m_fFontFade, m_fFontFade), XMVectorSet(1.f, 1.f, 1.f, 1.f));

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

	if (FAILED(__super::Add_Components(TEXT("Com_Texture0"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Jaehun"), (CComponent**)&m_pTextureCom[0])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sungjun"), (CComponent**)&m_pTextureCom[1])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Minjun"), (CComponent**)&m_pTextureCom[2])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Yoonji"), (CComponent**)&m_pTextureCom[3])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture4"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sukhun"), (CComponent**)&m_pTextureCom[4])))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
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

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom[m_iTextureNum]->Get_SRV(m_iFrame))))
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
	for (_int i = 0; i < 5; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
	}
}
