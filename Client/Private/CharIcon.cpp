#include "stdafx.h"
#include "CharIcon.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CCharIcon::CCharIcon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CCharIcon::CCharIcon(const CCharIcon & rhs)
	: CUI(rhs)
{
}

HRESULT CCharIcon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharIcon::Initialize(void * pArg)
{
	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	
	if (m_ThrowUIinfo.iLevelIndex == LEVEL_SELECTCHAR)
	{
		Icon_Selected_SelectChar(m_ThrowUIinfo.iLayerNum);
		m_fSizeX = m_ThrowUIinfo.vScale.x * 1.1f;
		m_fSizeY = m_ThrowUIinfo.vScale.y * 1.1f;
		m_fY -= 2.f;
	}

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	if (m_ThrowUIinfo.vRot >= 0 && m_ThrowUIinfo.vRot <= 360)
		m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, m_ThrowUIinfo.vRot));

	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (!m_ThrowUIinfo.bReversal)
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	else
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));


	return S_OK;
}

void CCharIcon::Tick(_float fTimeDelta)
{

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMEPLAY || m_ThrowUIinfo.iLevelIndex == LEVEL_BATTLEENMU || m_ThrowUIinfo.iLevelIndex == LEVEL_BOSSENMU)
	{
		if (pUI_Manager->Get_BattleTypeCheck())
		{
			if (!m_ThrowUIinfo.bPlyCheck)
			{
				if (m_ThrowUIinfo.iLayerNum == 0)
					Icon_Selected_GamePlay(pUI_Manager->Get_1P()->Get_PlayerInfo().strName);
				else
					Icon_Selected_GamePlay(pUI_Manager->Get_1P_2()->Get_PlayerInfo().strName);
			}
			if (m_ThrowUIinfo.bPlyCheck)
			{
				if (m_ThrowUIinfo.iLayerNum == 0)
					Icon_Selected_GamePlay(pUI_Manager->Get_2P()->Get_PlayerInfo().strName);
				else
					Icon_Selected_GamePlay(pUI_Manager->Get_2P_2()->Get_PlayerInfo().strName);
			}
		}
		else
		{
			if (!m_ThrowUIinfo.bPlyCheck)
			{
				if (m_ThrowUIinfo.iLayerNum == 0)
					Icon_Selected_GamePlay(pUI_Manager->Get_1P()->Get_PlayerInfo().strName);
				else
					Icon_Selected_GamePlay(pUI_Manager->Get_1P_2()->Get_PlayerInfo().strName);
			}
			else
				Icon_Selected_GamePlay(pUI_Manager->Get_2P()->Get_PlayerInfo().strName);
		}
		
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));
	}

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_SELECTCHAR)
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.6f, 1.f));
	}
	RELEASE_INSTANCE(CUI_Manager);
}

void CCharIcon::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CCharIcon::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMEPLAY || m_ThrowUIinfo.iLevelIndex == LEVEL_BATTLEENMU || m_ThrowUIinfo.iLevelIndex == LEVEL_BOSSENMU)
		m_pShaderCom->Begin(16);
	else if(m_ThrowUIinfo.iLevelIndex == LEVEL_SELECTCHAR)
		m_pShaderCom->Begin(9);

	m_pVIBufferCom->Render();

	RELEASE_INSTANCE(CUI_Manager);
	return S_OK;
}

void CCharIcon::Icon_Selected_GamePlay(wstring strName)
{
	if (strName == TEXT("쿄주로"))
		m_iImgNum = 19;
	else if (strName == TEXT("탄지로"))
		m_iImgNum = 35;
	else if (strName == TEXT("루이"))
		m_iImgNum = 27;
	else if (strName == TEXT("아카자"))
		m_iImgNum = 0;
	else if (strName == TEXT("네즈코"))
		m_iImgNum = 25;
	else if (strName == TEXT("시노부"))
		m_iImgNum = 30;
	else if(strName == TEXT("아빠 거미"))
		m_iImgNum = 2;
	else if (strName == TEXT("엔무"))
		m_iImgNum = 11;
	else if (strName == TEXT("엔무(각성)"))
		m_iImgNum = 11;
}

void CCharIcon::Icon_Selected_SelectChar(_uint iLayerNum)
{
	if (0 == iLayerNum)
		m_iImgNum = 35;
	else if(1 == iLayerNum)
		m_iImgNum = 19;
	else if (2 == iLayerNum)
		m_iImgNum = 27;
	else if (3 == iLayerNum)
		m_iImgNum = 0;
	else if (4 == iLayerNum)
		m_iImgNum = 25;
	else if (5 == iLayerNum)
		m_iImgNum = 30;
}

HRESULT CCharIcon::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharIcon"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_SELECTCHAR)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharIconMaskMap"), (CComponent**)&m_pTextureMaskCom)))
			return E_FAIL;
	}

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMEPLAY || m_ThrowUIinfo.iLevelIndex == LEVEL_BATTLEENMU || m_ThrowUIinfo.iLevelIndex == LEVEL_BOSSENMU)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharMask"), (CComponent**)&m_pTextureMaskCom)))
			return E_FAIL;
	}

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharIcon::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iImgNum))))
		return E_FAIL;
	
	if (m_ThrowUIinfo.iLevelIndex == LEVEL_SELECTCHAR)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_MaskTexture", m_pTextureMaskCom->Get_SRV(0))))
			return E_FAIL;
	}
	if (!m_ThrowUIinfo.bPlyCheck)
	{
		if (m_ThrowUIinfo.iLayerNum == 0)
			m_iMaskImgNum = 0;
		else if (m_ThrowUIinfo.iLayerNum == 1)
			m_iMaskImgNum = 1;
	}
	else if (m_ThrowUIinfo.bPlyCheck)
	{
		if (m_ThrowUIinfo.iLayerNum == 0)
			m_iMaskImgNum = 2;
		else if (m_ThrowUIinfo.iLayerNum == 1)
			m_iMaskImgNum = 3;
	}

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMEPLAY || m_ThrowUIinfo.iLevelIndex == LEVEL_BATTLEENMU || m_ThrowUIinfo.iLevelIndex == LEVEL_BOSSENMU)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_MaskTexture", m_pTextureMaskCom->Get_SRV(m_iMaskImgNum))))
			return E_FAIL;
	}

	return S_OK;
}

CCharIcon * CCharIcon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCharIcon*	pInstance = new CCharIcon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCharIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCharIcon::Clone(void * pArg)
{
	CCharIcon*	pInstance = new CCharIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCharIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharIcon::Free()
{
	__super::Free();

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_SELECTCHAR)
		Safe_Release(m_pTextureMaskCom);
	else if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMEPLAY || m_ThrowUIinfo.iLevelIndex == LEVEL_BATTLEENMU || m_ThrowUIinfo.iLevelIndex == LEVEL_BOSSENMU)
		Safe_Release(m_pTextureMaskCom);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
