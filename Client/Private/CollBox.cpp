#include "stdafx.h"
#include "..\Public\CollBox.h"

#include "GameInstance.h"

CCollBox::CCollBox(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CCollBox::CCollBox(const CCollBox & rhs)
	: CGameObj(rhs)
{
}

HRESULT CCollBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollBox::Initialize(void * pArg)
{
	return S_OK;
}

void CCollBox::Tick(_float fTimeDelta)
{

}

void CCollBox::Late_Tick(_float fTimeDelta)
{

}

HRESULT CCollBox::Render()
{
	return S_OK;
}

HRESULT CCollBox::Render_ShadowDepth()
{
	return S_OK;
}

CCollBox * CCollBox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCollBox*	pInstance = new CCollBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCollBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCollBox::Clone(void * pArg)
{
	CGameObject*	pInstance = new CCollBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCollBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollBox::Free()
{
	__super::Free();
}

