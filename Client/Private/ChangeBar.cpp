#include "stdafx.h"
#include "ChangeBar.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SoundMgr.h"

CChangeBar::CChangeBar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CChangeBar::CChangeBar(const CChangeBar & rhs)
	: CUI(rhs)
{
}

HRESULT CChangeBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChangeBar::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));


	if (m_ThrowUIinfo.vRot >= 0 && m_ThrowUIinfo.vRot <= 360 && !m_ThrowUIinfo.bPlyCheck)
		m_pTransformCom->Turn2(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(m_ThrowUIinfo.vRot));
	else
	{
		if(m_ThrowUIinfo.iLayerNum == 0)
			m_pTransformCom->Turn2(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(166.f));
		if (m_ThrowUIinfo.iLayerNum == 1)
			m_pTransformCom->Turn2(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(134.5f));
	}


	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (!m_ThrowUIinfo.bPlyCheck)
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	else
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	return S_OK;
}

void CChangeBar::Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);


	if (!m_ThrowUIinfo.bPlyCheck)
	{
		m_fFriendBar = pUI_Manager->Get_1P()->Get_PlayerInfo().iFriendBar;
		m_fFriendMaxBar = pUI_Manager->Get_1P()->Get_PlayerInfo().iFriendMaxBar;
	}
	else
	{
		m_fFriendBar = pUI_Manager->Get_2P()->Get_PlayerInfo().iFriendBar;
		m_fFriendMaxBar = pUI_Manager->Get_2P()->Get_PlayerInfo().iFriendMaxBar;
	}
	 

	if (m_ThrowUIinfo.iLayerNum == 0)
	{
		if (m_fFriendBar < 500.f)
		{
			m_bSound = false;
			if (!m_bCurPerBarCheck)
			{
				_float fPerCurBar = (m_fFriendBar * 0.002f) * 100.f;
				m_fTime = 500.f;
				m_fTime -= 30.83f * (fPerCurBar * 0.01f);
				m_bCurPerBarCheck = true;
			}

			m_fTime -= 0.06166f;
			if(!m_ThrowUIinfo.bPlyCheck)
				pUI_Manager->Get_1P()->Set_FriendSkillBar(1.f);
			else 
				pUI_Manager->Get_2P()->Set_FriendSkillBar(1.f);

			if (m_fTime <= 469.17f)
			{
				m_fTime = 469.17f;
				m_bCurPerBarCheck = false;
			}
		}
		else if (m_fFriendBar >= 500.f)
		{
			if (!m_bSound)
			{
				CSoundMgr::Get_Instance()->PlayEffect(TEXT("UI_ChangeBar.wav"), g_fEffect);
				m_bSound = true;
			}
			m_fTime = 469.17f;
		}
	}
	else if (m_ThrowUIinfo.iLayerNum == 1)
	{
		if(m_fFriendBar >= 500.f)
		{
			if (!m_bCurPerBarCheck)
			{
				_float fPerCurBar = ((m_fFriendBar - 500.f) * 0.002f) * 100.f;
				m_fTime -= 30.83f * (fPerCurBar * 0.01f);
				m_bCurPerBarCheck = true;
			}
			if (m_fFriendBar <= 500.f)
			{
				m_fTime = 500.f;
				m_bSound = false;
			}
			if (m_fFriendBar < 1000)
			{
				m_fTime -= 0.06166f;

				if (!m_ThrowUIinfo.bPlyCheck)
					pUI_Manager->Get_1P()->Set_FriendSkillBar(1.f);
				else
					pUI_Manager->Get_2P()->Set_FriendSkillBar(1.f);

				if (!m_bSound && m_fTime <= 469.17f)
				{
					CSoundMgr::Get_Instance()->PlayEffect(TEXT("UI_ChangeBar.wav"), g_fEffect);
					m_bSound = true;
				}
			}
			else if (m_fFriendBar >= 1000.f)
			{
				m_fTime = 469.17f;
				m_bCurPerBarCheck = false;
			}
		}
		else if (m_fFriendBar < 500.f)
			m_fTime = 500.f;
	}
	

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
}

void CChangeBar::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CChangeBar::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	//if(!m_ThrowUIinfo.bPlyCheck)

	m_pShaderCom->Begin(23);

	if(m_ThrowUIinfo.iLevelIndex != LEVEL_BATTLEENMU)
		m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CChangeBar::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChangeGaugeBar"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CChangeBar::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fTime", &m_fTime, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	return S_OK;
}

CChangeBar * CChangeBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CChangeBar*	pInstance = new CChangeBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CChangeBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CChangeBar::Clone(void * pArg)
{
	CChangeBar*	pInstance = new CChangeBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CChangeBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChangeBar::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
