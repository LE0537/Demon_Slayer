#include "stdafx.h"
#include "..\Public\Mini_Player.h"

#include "GameInstance.h"
#include "UI_Manager.h"
#include "Mini_Tail.h"
#include "Mini_Result.h"
#include "Layer.h"
CMini_Player::CMini_Player(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CMini_Player::CMini_Player(const CMini_Player & rhs)
	: CGameObj(rhs)
{
}

HRESULT CMini_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMini_Player::Initialize(void * pArg)
{
	*(CGameObj**)pArg = this;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 60.f;
	m_fSizeY = 60.f;
	m_fX = 940.f;
	m_fY = 360.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	m_pResult = dynamic_cast<CMini_Result*>(pGameInstance->Find_Layer(LEVEL_LOADING, TEXT("Layer_MINI_Result"))->Get_LayerFront());
	m_pResult->Set_Target(this);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CMini_Player::Tick(_float fTimeDelta)
{
	if (g_bMiniGame)
	{
		Set_Dead();
		return;
	}
	if (m_bPlayerDead)
	{
		CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
		if (!m_bResultDead)
		{
			m_bResultDead = true;
			m_pResult->Set_PlayerDead(true);
			if (m_iScore > m_pResult->Get_RankScore())
				m_pResult->Set_RankScore(m_iScore);
		}
		
		if (pGameInstance->Key_Down(DIK_F))
		{
			m_pResult->Set_PlayerDead(false);
			g_bMiniGame = true;
		}
		RELEASE_INSTANCE(CGameInstance);
	}

	
	if (!m_bPlayerDead)
	{
		if (m_bEat)
			Create_Tail();

		m_pTransformCom->Go_Left(fTimeDelta);
		Key_Input(fTimeDelta);

		ConvertToViewPort();

		Save_Pos(fTimeDelta);
		if (m_iTailIndex > 0)
			Set_TailPos();

		
		Check_Coll();
	}
}

void CMini_Player::Late_Tick(_float fTimeDelta)
{
	
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIPOKE, this);


}

HRESULT CMini_Player::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;


	return S_OK;
}



HRESULT CMini_Player::Ready_Components()
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

HRESULT CMini_Player::SetUp_ShaderResources()
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

	m_pShaderCom->Begin();
	m_pVIBufferCom->Render();

	return S_OK;
}

void CMini_Player::Key_Input(_float fTimeDelta)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Pressing(DIK_LEFT))
	{
		m_pTransformCom->Turn2(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(3.f));
	}
	else if (pGameInstance->Key_Pressing(DIK_RIGHT))
	{
		m_pTransformCom->Turn2(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-3.f));
	}
	else if (pGameInstance->Key_Down(DIK_T))
	{
		m_bEat = true;
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CMini_Player::Create_Tail()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CMini_Tail* tInfo;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Mini_Tail"), LEVEL_LOADING, TEXT("Layer_MINI"), &tInfo)))
		return;

	m_listTail.push_back(tInfo);

	++m_iTailIndex;
	RELEASE_INSTANCE(CGameInstance);
	m_bEat = false;
}

void CMini_Player::Save_Pos(_float fTimeDelta)
{
	m_fSaveTime += fTimeDelta;
	if (m_fSaveTime > 0.22f)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vPos += XMVector3Normalize(vRight) * 15.f;
		m_listPos.push_front(vPos);
		if (m_iTailIndex + 3 < m_listPos.size())
			m_listPos.pop_back();
		
		m_fSaveTime = 0.f;
	}
}

void CMini_Player::Set_TailPos()
{
	auto& iterPos = m_listPos.begin();
	
	for (auto& iter : m_listTail)
	{
		iter->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, *iterPos);
		++iterPos;
	}
}

void CMini_Player::Check_Coll()
{
	_int iCollNum = 0;

	for (auto& iter : m_listTail)
	{
		if (iCollNum != 0)
		{
			_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetPos = iter->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - vPos));
			if (fDist < 40.f)
			{
				m_bPlayerDead = true;
			}
		}
		++iCollNum;
	}
}

void CMini_Player::ConvertToViewPort()
{
	_matrix matPlayerWorld = m_pTransformCom->Get_WorldMatrix();

	_matrix matView = XMLoadFloat4x4(&m_ViewMatrix);
	_matrix matProj = XMLoadFloat4x4(&m_ProjMatrix);
	_matrix matWVP = matPlayerWorld * matView * matProj;

	_float  m_fScalingbyDepth = XMVectorGetW(matWVP.r[3]);
	matWVP.r[3] /= m_fScalingbyDepth;

	XMStoreFloat4(&m_vPlayerPos, matWVP.r[3]);

	m_vPlayerPos.x += 1.f;
	m_vPlayerPos.x *= g_iWinSizeX / 2.f;

	m_vPlayerPos.y += 1.f;
	m_vPlayerPos.y *= g_iWinSizeY / 2.f;
	m_vPlayerPos.y = 720.f - m_vPlayerPos.y;

	if (m_vPlayerPos.x < 30.f)
		m_bPlayerDead = true;
	else if (m_vPlayerPos.x > 1250.f)
		m_bPlayerDead = true;
	else if(m_vPlayerPos.y < 30.f)
		m_bPlayerDead = true;
	else if (m_vPlayerPos.y > 680.f)
		m_bPlayerDead = true;
}

CMini_Player * CMini_Player::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMini_Player*	pInstance = new CMini_Player(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMini_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMini_Player::Clone(void * pArg)
{
	CMini_Player*	pInstance = new CMini_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMini_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMini_Player::Free()
{
	__super::Free();

	
}
