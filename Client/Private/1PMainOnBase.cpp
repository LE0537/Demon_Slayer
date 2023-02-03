#include "stdafx.h"
#include "1PMainOnBase.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SelP1Cursor.h"

C1PMainOnBase::C1PMainOnBase(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

C1PMainOnBase::C1PMainOnBase(const C1PMainOnBase & rhs)
	: CUI(rhs)
{
}

HRESULT C1PMainOnBase::Initialize_Prototype()
{
	return S_OK;
}

HRESULT C1PMainOnBase::Initialize(void * pArg)
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
		m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, m_ThrowUIinfo.vRot));

	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (!m_ThrowUIinfo.bReversal)
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	else
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	pUI_Manager->Set_Sel1PMain(this, m_ThrowUIinfo.iLayerNum);

	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

void C1PMainOnBase::Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	_bool bFirstCheck = dynamic_cast<CSelP1Cursor*>(pUI_Manager->Get_1PCursor())->Get_FirstSelCheck();
	_bool bSecondCheck = dynamic_cast<CSelP1Cursor*>(pUI_Manager->Get_1PCursor())->Get_SecondSelCheck();
	_bool bOni = dynamic_cast<CSelP1Cursor*>(pUI_Manager->Get_1PCursor())->Get_SelectUIInfo().bOni;

	if (m_ThrowUIinfo.iLayerNum == 0)
		Move_FirstMain(fTimeDelta, bFirstCheck, bSecondCheck, bOni);
	else if (m_ThrowUIinfo.iLayerNum == 1)
		Move_SecondMain(fTimeDelta, bFirstCheck, bSecondCheck, bOni);
	
	
	RELEASE_INSTANCE(CUI_Manager);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (_float)g_iWinSizeX * 0.5f, -m_fY + (_float)g_iWinSizeY * 0.5f, 0.f, 1.f));
}

void C1PMainOnBase::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIPOKE, this);
}

HRESULT C1PMainOnBase::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (!m_ThrowUIinfo.bReversal)
		m_pShaderCom->Begin();
	else
		m_pShaderCom->Begin(1);

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	
	CSelP1Cursor* pSelP1Cursor = dynamic_cast<CSelP1Cursor*>(pUI_Manager->Get_1PCursor());

	if(m_ThrowUIinfo.iLayerNum == 0)
		m_pVIBufferCom->Render();
	else
	{
		if (pSelP1Cursor != nullptr)
		{
			if (pSelP1Cursor->Get_FirstSelCheck() && !pSelP1Cursor->Get_SecondSelCheck() && !pSelP1Cursor->Get_SelectUIInfo().bOni)
				m_pVIBufferCom->Render();
			if(pSelP1Cursor->Get_SecondSelCheck() && !pSelP1Cursor->Get_SelectUIInfo().bOni)
				m_pVIBufferCom->Render();
		}
	}
	
	RELEASE_INSTANCE(CUI_Manager);
	return S_OK;
}

void C1PMainOnBase::Move_FirstMain(_float fTimeDelta, _bool bFirstCheck, _bool bSecondCheck, _bool bOni)
{
	if (bFirstCheck && !m_bFirstMove && !bOni)
	{
		m_fMoveTime += fTimeDelta;

		if (m_fMoveTime >= 0.0001f && m_iMoveCount < 21)
		{
			m_fY -= 2.f;
			m_iMoveCount += 2;
			m_fMoveTime = 0.f;
			if (m_iMoveCount >= 20)
				m_bFirstMove = true;
		}
	}
	else if (m_bFirstMove && bFirstCheck && !bSecondCheck && !m_bSecondMoveCheck && !bOni)
	{
		m_fMoveTime += fTimeDelta;

		if (m_fMoveTime >= 0.0001f && m_iMoveCount < 21)
		{
			m_fY -= 2.f;
			m_iMoveCount += 2;
			m_fMoveTime = 0.f;
		}
	}
	else if (m_bFirstMove && !bFirstCheck && !bSecondCheck && !m_bSecondMoveCheck && !bOni)
	{
		m_fMoveTime += fTimeDelta;

		if (m_fMoveTime >= 0.0001f && m_iMoveCount > 0)
		{
			m_fY += 2.f;
			m_iMoveCount -= 2;
			m_fMoveTime = 0.f;
			if (m_iMoveCount <= 0)
				m_bFirstMove = false;
		}
	}
	else if (m_bFirstMove && bFirstCheck && bSecondCheck && m_bSecondMoveCheck && !bOni)
	{
		m_fMoveTime += fTimeDelta;

		if (m_fMoveTime >= 0.0001f && m_iMoveCount < 59)
		{
			m_fY -= 2.f;
			m_iMoveCount += 2;
			m_fMoveTime = 0.f;
		}
	}
	else if (m_bFirstMove && bFirstCheck && bSecondCheck && !m_bSecondMoveCheck && !bOni)
	{
		m_fMoveTime += fTimeDelta;

		if (m_fMoveTime >= 0.0001f && m_iMoveCount < 59)
		{
			m_fY -= 2.f;
			m_iMoveCount += 2;
			m_fMoveTime = 0.f;
			if (m_iMoveCount >= 58)
				m_bSecondMoveCheck = true;
		}
	}
	else if (m_bFirstMove && bFirstCheck && !bSecondCheck && m_bSecondMoveCheck && !bOni)
	{
		m_fMoveTime += fTimeDelta;

		if (m_fMoveTime >= 0.0001f && m_iMoveCount > 21)
		{
			m_fY += 2.f;
			m_iMoveCount -= 2;
			m_fMoveTime = 0.f;
			if (m_iMoveCount <= 21)
				m_bSecondMoveCheck = false;
		}
	}
	else if (m_bFirstMove && !bFirstCheck && !bSecondCheck && m_bSecondMoveCheck && !bOni)
	{
		m_fMoveTime += fTimeDelta;

		if (m_fMoveTime >= 0.0001f && m_iMoveCount > 0)
		{
			m_fY += 2.f;
			m_iMoveCount -= 2;
			m_fMoveTime = 0.f;
		}
	}
}

void C1PMainOnBase::Move_SecondMain(_float fTimeDelta, _bool bFirstCheck, _bool bSecondCheck, _bool bOni)
{
	if (bFirstCheck && !m_bFirstMove && bSecondCheck && !bOni)
	{
		m_fMoveTime += fTimeDelta;

		if (m_fMoveTime >= 0.0001f && m_iMoveCount < 25)
		{
			m_fY += 2.f;
			m_iMoveCount += 2;
			m_fMoveTime = 0.f;
			if (m_iMoveCount >= 24)
				m_bFirstMove = true;
		}
	}
	else if (bFirstCheck && m_bFirstMove && !bSecondCheck && !bOni)
	{
		m_fMoveTime += fTimeDelta;

		if (m_fMoveTime >= 0.0001f && m_iMoveCount > 0)
		{
			m_fY -= 2.f;
			m_iMoveCount -= 2;
			m_fMoveTime = 0.f;
		}
	}
	else if (bFirstCheck && m_bFirstMove && bSecondCheck && !bOni)
	{
		m_fMoveTime += fTimeDelta;

		if (m_fMoveTime >= 0.0001f && m_iMoveCount < 25)
		{
			m_fY += 2.f;
			m_iMoveCount += 2;
			m_fMoveTime = 0.f;
		}
	}
}

HRESULT C1PMainOnBase::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_1P_MainOnBase"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT C1PMainOnBase::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
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

C1PMainOnBase * C1PMainOnBase::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	C1PMainOnBase*	pInstance = new C1PMainOnBase(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : C1PMainOnBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * C1PMainOnBase::Clone(void * pArg)
{
	C1PMainOnBase*	pInstance = new C1PMainOnBase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : C1PMainOnBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void C1PMainOnBase::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
