#include "stdafx.h"
#include "..\Public\Characters.h"

#include "GameInstance.h"

CCharacters::CCharacters(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CCharacters::CCharacters(const CCharacters & rhs)
	: CGameObj(rhs)
{
}

HRESULT CCharacters::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharacters::Initialize(void * pArg)
{

	return S_OK;
}

void CCharacters::Tick(_float fTimeDelta)
{
}

void CCharacters::Late_Tick(_float fTimeDelta)
{
}

HRESULT CCharacters::Render()
{
	return S_OK;
}

HRESULT CCharacters::Render_ShadowDepth()
{
	return S_OK;
}

CCharacters * CCharacters::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCharacters*	pInstance = new CCharacters(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCharacters"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCharacters::Clone(void * pArg)
{
	CGameObject*	pInstance = new CCharacters(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCharacters"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharacters::Free()
{
	__super::Free();


}

