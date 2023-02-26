#include "stdafx.h"
#include "OniSpecialSkillBar.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SoundMgr.h"

COniSpecialSkillBar::COniSpecialSkillBar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

COniSpecialSkillBar::COniSpecialSkillBar(const COniSpecialSkillBar & rhs)
	: CUI(rhs)
{
}

HRESULT COniSpecialSkillBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT COniSpecialSkillBar::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	if (m_ThrowUIinfo.vRot >= 0 && m_ThrowUIinfo.vRot <= 360)
		m_pTransformCom->Turn2(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(m_ThrowUIinfo.vRot));

	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (!m_ThrowUIinfo.bReversal)
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);
	else
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight );

	_vector vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * -1.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));


	return S_OK;
}

void COniSpecialSkillBar::Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (!m_ThrowUIinfo.bPlyCheck)
	{
		m_fSpecialSkillBar = pUI_Manager->Get_1P()->Get_PlayerInfo().iFriendBar;
		m_fSpecialSkillMaxBar = pUI_Manager->Get_1P()->Get_PlayerInfo().iFriendMaxBar;
	}
	else
	{
		m_fSpecialSkillBar = pUI_Manager->Get_2P()->Get_PlayerInfo().iFriendBar;
		m_fSpecialSkillMaxBar = pUI_Manager->Get_2P()->Get_PlayerInfo().iFriendMaxBar;
	}

	if (m_ThrowUIinfo.iLayerNum == 0)
	{
		if (m_fSpecialSkillBar < 500.f)
		{
			m_bSound = false;
			m_fSpecialSkillBar += fTimeDelta * 40.f;
			
			if (!m_ThrowUIinfo.bPlyCheck)
				pUI_Manager->Get_1P()->Set_FriendSkillBar(fTimeDelta * 40.f);
			else
				pUI_Manager->Get_2P()->Set_FriendSkillBar(fTimeDelta * 40.f);
		}
		else if (m_fSpecialSkillBar >= 500.f)
		{
			if (!m_bSound)
			{
				CSoundMgr::Get_Instance()->PlayEffect(TEXT("UI_ChangeBar.wav"), fEFFECT);
				m_bSound = true;
			}
			m_fSpecialSkillBar = 500.f;
		}
			
	}
	else if (m_ThrowUIinfo.iLayerNum == 1)
	{
		if (m_fSpecialSkillBar >= 500)
		{
			if (m_fSpecialSkillBar < 1000)
			{
				m_bSound = false;
				m_fSpecialSkillBar -= 500.f;
				m_fSpecialSkillBar += fTimeDelta * 40.f;
				if (!m_ThrowUIinfo.bPlyCheck)
					pUI_Manager->Get_1P()->Set_FriendSkillBar(fTimeDelta * 40.f);
				else
					pUI_Manager->Get_2P()->Set_FriendSkillBar(fTimeDelta * 40.f);


			}
			else if (m_fSpecialSkillBar >= 1000.f)
			{
				m_fSpecialSkillBar = 500.f;
				if (!m_bSound)
				{
					CSoundMgr::Get_Instance()->PlayEffect(TEXT("UI_ChangeBar.wav"), fEFFECT);
					m_bSound = true;
				}
			}
		}
		else if (m_fSpecialSkillBar < 500)
			m_fSpecialSkillBar = 0.f;

	}


	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
}

void COniSpecialSkillBar::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT COniSpecialSkillBar::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(24);

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (pUI_Manager->Get_BattleTypeCheck())
		m_pVIBufferCom->Render();

	RELEASE_INSTANCE(CUI_Manager);


	return S_OK;
}

HRESULT COniSpecialSkillBar::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_OniSpecialBar"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_OniSpecialBarMask"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT COniSpecialSkillBar::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurBar", &m_fSpecialSkillBar, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_MaskTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;


	return S_OK;
}

COniSpecialSkillBar * COniSpecialSkillBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	COniSpecialSkillBar*	pInstance = new COniSpecialSkillBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : COniSpecialSkillBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * COniSpecialSkillBar::Clone(void * pArg)
{
	COniSpecialSkillBar*	pInstance = new COniSpecialSkillBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : COniSpecialSkillBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COniSpecialSkillBar::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
