#include "stdafx.h"
#include "MsgTextBase.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CMsgTextBase::CMsgTextBase(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CMsgTextBase::CMsgTextBase(const CMsgTextBase & rhs)
	: CUI(rhs)
{
}

HRESULT CMsgTextBase::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMsgTextBase::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y + 10.f;

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

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	pUI_Manager->Set_MsgTextBase(this);

	RELEASE_INSTANCE(CUI_Manager);
	return S_OK;
}

void CMsgTextBase::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);


	if (pGameInstance->Key_Down(DIK_PGUP))
		pUI_Manager->Set_MsgOn();
	else if (pGameInstance->Key_Down(DIK_PGDN))
		pUI_Manager->Set_MsgOff();

	if(!m_bFirstMsgCheck)
		m_fFirstMsg += fTimeDelta;
	if (m_fFirstMsg >= 5.f)
		m_bFirstMsgCheck = true;
	if (m_bFirstMsgCheck && m_fFirstMsg >= 5.f)
	{
		m_iMoveCount += 1;
		m_fFadeTime += 0.2f;
		m_fY -= 2.f;
		if (m_iMoveCount >= 5)
		{
			m_iMoveCount = 10;
			m_fFadeTime = 1.f;
			m_fY = m_ThrowUIinfo.vPos.y;
			pUI_Manager->Set_MsgOff();
		}

		m_fFirstMsg = 0.f;
	}


	if (pUI_Manager->Get_MsgOnOff())
		m_bMoveCheck = true;
	else
		m_bMoveCheck = false;

	

	if (m_bMoveCheck)
	{
		m_iMoveCount += 1;
		m_fFadeTime += 0.2f;
		m_fY -= 2.f;
		if (m_iMoveCount >= 5)
		{
			m_iMoveCount = 10;
			m_fFadeTime = 1.f;
			m_fY = m_ThrowUIinfo.vPos.y;
		}

		if (pGameInstance->Key_Down(DIK_RETURN))
		{
			pUI_Manager->Set_MsgCount(1);

			pUI_Manager->Set_MsgOff();
		}
	}
	else
	{
		m_iMoveCount -= 1;
		m_fFadeTime -= 0.2f;
		m_fY += 2.f;
		if (m_iMoveCount <= 0)
		{
			m_iMoveCount = 0;
			m_fFadeTime = 0.f;
			m_fY = m_ThrowUIinfo.vPos.y + 10.f;
		}
	}

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CMsgTextBase::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CMsgTextBase::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	pGameInstance->Render_Font(TEXT("Font_Nexon"), pUI_Manager->Get_Msg().c_str(), XMVectorSet(m_fX - 450.f, m_fY - 30.f, 0.f, 1.f), XMVectorSet(m_fFadeTime, m_fFadeTime, m_fFadeTime, m_fFadeTime), XMVectorSet(0.9f, 0.9f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMsgTextBase::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MsgTextBase"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMsgTextBase::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fFadeTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	return S_OK;
}

CMsgTextBase * CMsgTextBase::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMsgTextBase*	pInstance = new CMsgTextBase(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMsgTextBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMsgTextBase::Clone(void * pArg)
{
	CMsgTextBase*	pInstance = new CMsgTextBase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMsgTextBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMsgTextBase::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
