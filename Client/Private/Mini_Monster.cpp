#include "stdafx.h"
#include "..\Public\Mini_Monster.h"

#include "GameInstance.h"
#include "UI_Manager.h"
#include "Mini_Player.h"
CMini_Monster::CMini_Monster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CMini_Monster::CMini_Monster(const CMini_Monster & rhs)
	: CGameObj(rhs)
{
}

HRESULT CMini_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMini_Monster::Initialize(void * pArg)
{
	m_pPlayer = *(CGameObj**)pArg;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	_int iDest = rand() % 4;

	switch (iDest)
	{
	case 0:
		m_fX = 0.f;
		m_fY = 0.f;
		break;
	case 1:
		m_fX = 0.f;
		m_fY = 720.f;
		break;
	case 2:
		m_fX = 1280.f;
		m_fY = 0.f;
		break;
	case 3:
		m_fX = 1280.f;
		m_fY = 720.f;
		break;
	default:
		break;
	}


	m_fSizeX = 60.f;
	m_fSizeY = 60.f;


	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));


	return S_OK;
}

void CMini_Monster::Tick(_float fTimeDelta)
{
	if (g_bMiniGame)
		return;
	if (!m_bMonsterDead)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos2 = vPos;
		_vector vTargetPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		vPos2 += XMVector3Normalize(vTargetPos - vPos2) * 100.f * fTimeDelta;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos2);
		if (!dynamic_cast<CMini_Player*>(m_pPlayer)->Get_PlayerDead())
		{
			_int iCollNum = 0;

			for (auto& iter : dynamic_cast<CMini_Player*>(m_pPlayer)->Get_Tail())
			{
				if (iCollNum != 0)
				{
					_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
					_vector vTargetPos = iter->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - vPos));
					if (fDist < 40.f)
					{
						Set_Dead();
						return;
					}
				}
				++iCollNum;
			}
		}

		_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - vPos));
		if (fDist < 40.f)
		{
			dynamic_cast<CMini_Player*>(m_pPlayer)->Set_PlayerDead();
			m_bMonsterDead = true;
		}
	}
}

void CMini_Monster::Late_Tick(_float fTimeDelta)
{
	if (g_bMiniGame)
		Set_Dead();
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIPOKE, this);

}

HRESULT CMini_Monster::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;


	return S_OK;
}



HRESULT CMini_Monster::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 150.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UIVtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MiniGame"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMini_Monster::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin();
	m_pVIBufferCom->Render();

	return S_OK;
}

CMini_Monster * CMini_Monster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMini_Monster*	pInstance = new CMini_Monster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMini_Monster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMini_Monster::Clone(void * pArg)
{
	CMini_Monster*	pInstance = new CMini_Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMini_Monster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMini_Monster::Free()
{
	__super::Free();


}
