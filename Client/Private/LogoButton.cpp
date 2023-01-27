#include "stdafx.h"
#include "LogoButton.h"
#include "GameInstance.h"


CLogoButton::CLogoButton(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CLogoButton::CLogoButton(const CLogoButton & rhs)
	: CUI(rhs)
{
}

HRESULT CLogoButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLogoButton::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y;

	if (m_ThrowUIinfo.iLayerNum == 0)
		m_iImgNum = 1;
	else if (m_ThrowUIinfo.iLayerNum == 1)
		m_iImgNum = 0;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (!m_ThrowUIinfo.bReversal)
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	else
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);

	if (m_ThrowUIinfo.vRot >= 0 && m_ThrowUIinfo.vRot <= 360)
		m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, m_ThrowUIinfo.vRot));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));
	

	return S_OK;
}

void CLogoButton::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	

	m_fAlpha += fTimeDelta;

	if (m_fAlpha >= 1)
		m_fAlpha = 1.f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (!m_bMenuOn)
	{
		if (pGameInstance->Key_Down(DIK_UP))
		{
			if (m_ThrowUIinfo.iLayerNum == 0)
				m_iImgNum = 1;
			else if (m_ThrowUIinfo.iLayerNum == 1)
				m_iImgNum = 0;
		}
		else if (pGameInstance->Key_Down(DIK_DOWN))
		{
			if (m_ThrowUIinfo.iLayerNum == 0)
				m_iImgNum = 0;
			else if (m_ThrowUIinfo.iLayerNum == 1)
				m_iImgNum = 1;
		}

		if (pGameInstance->Key_Down(DIK_E))
		{
			if (m_ThrowUIinfo.iLayerNum == 0 && m_iImgNum == 1)
			{
				Add_InkEff();
				m_bMenuOn = true;
			}

		}
	}
	
	RELEASE_INSTANCE(CGameInstance);
}

void CLogoButton::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CLogoButton::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();

	Font_Color();
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if(m_ThrowUIinfo.iLayerNum == 0)
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("게임 시작"), XMVectorSet(m_fX - 70.f, m_fY - 12.f, 0.f, 1.f), XMVectorSet(m_vFontColor.x, m_vFontColor.y, m_vFontColor.z, m_fAlpha), XMVectorSet(1.f, 1.f, 0.f, 1.f));
	else if(m_ThrowUIinfo.iLayerNum == 1)
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("게임 종료"), XMVectorSet(m_fX - 70.f, m_fY - 12.f, 0.f, 1.f), XMVectorSet(m_vFontColor.x, m_vFontColor.y, m_vFontColor.z, m_fAlpha), XMVectorSet(1.f, 1.f, 0.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLogoButton::Font_Color()
{
	if (m_iImgNum == 0)
	{
		m_vFontColor.x = 1;
		m_vFontColor.y = 1;
		m_vFontColor.z = 1;
		m_vFontColor.w = 1;
	}
	else if (m_iImgNum == 1)
	{
		m_vFontColor.x = 0;
		m_vFontColor.y = 0;
		m_vFontColor.z = 0;
		m_vFontColor.w = 1;
	}
}

HRESULT CLogoButton::Add_InkEff()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_InkEff"), LEVEL_LOGO, TEXT("Layer_LogoUI"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLogoButton::Ready_Components()
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

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_LogoButton"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLogoButton::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iImgNum))))
		return E_FAIL;

	return S_OK;
}

CLogoButton * CLogoButton::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLogoButton*	pInstance = new CLogoButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CLogoButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CLogoButton::Clone(void * pArg)
{
	CLogoButton*	pInstance = new CLogoButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CLogoButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogoButton::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
