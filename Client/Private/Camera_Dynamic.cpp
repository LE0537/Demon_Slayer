#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"
#include "GameObj.h"

CCamera_Dynamic::CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::Initialize(void* pArg)
{
	m_pPlayer = (CGameObj*)*(&((CAMERADESC_DERIVED*)pArg)->CameraDesc.pTarget);
	
	if (FAILED(__super::Initialize(&((CAMERADESC_DERIVED*)pArg)->CameraDesc)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc = m_pTransform->Get_TransformDesc();
	if (FAILED(__super::Add_Components(TEXT("Com_SubTransform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pSubTransform, &TransformDesc)))
		return E_FAIL;

	m_pSubTransform->Set_WorldMatrix(m_pTransform->Get_WorldMatrix());


	return S_OK;
}

void CCamera_Dynamic::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Set_CamPos();

	Lerp_SubCam(fTimeDelta);

	if (FAILED(Bind_OnPipeLine()))
		return;
}

void CCamera_Dynamic::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

HRESULT CCamera_Dynamic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	
	return S_OK;
}

void CCamera_Dynamic::Set_CamPos()
{
	if (nullptr == m_pPlayer ||
		nullptr == m_pTarget)
	{
		return;
	}

	_vector vPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTarget = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vLook2 = vPos - vTarget;

	_float fDist = XMVectorGetX(XMVector3Length(vLook2));

	vPos -= XMVector3Normalize(vLook2) * (fDist * 0.5f);

	_vector vRight = XMVector3Normalize(vPos - vTarget);
	_vector vUp = { 0.f,1.f,0.f,0.f };
	_vector vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

	_matrix	RotationMatrixX = XMMatrixRotationAxis(vRight, XMConvertToRadians(15.f));

	vRight = XMVector3Normalize(XMVector3TransformNormal(vRight, RotationMatrixX));
	vUp = XMVector3Normalize(XMVector3TransformNormal(vUp, RotationMatrixX));
	vLook = XMVector3Normalize(XMVector3TransformNormal(vLook, RotationMatrixX));

	m_pSubTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pSubTransform->Set_State(CTransform::STATE_UP, vUp);
	m_pSubTransform->Set_State(CTransform::STATE_LOOK, vLook);

	_vector vAtPos = vPos;

	vAtPos.m128_f32[1] += 1.f;

	m_pSubTransform->LookAt(vAtPos);

	vPos -= vLook * 30.f;

	m_pSubTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	
}

void CCamera_Dynamic::Lerp_SubCam(_float fTimeDelta)
{
	//	Have Bug. PlayerPos == TargetPos, Display looks like Motion Blur

	for (_int i = 0; i < 4; ++i)
	{
		_float	fWeight = (4 == i ? 1.5f : 3.f);
		CTransform::STATE	iTemp = CTransform::STATE(i);
		_vector vLerp = XMVector3Normalize(XMVectorLerp(m_pTransform->Get_State(iTemp), m_pSubTransform->Get_State(iTemp), fTimeDelta * fWeight));
		m_pTransform->Set_State(CTransform::STATE(i), vLerp);
	}

}

CCamera_Dynamic * CCamera_Dynamic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void* pArg)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Dynamic::Free()
{
	__super::Free();

	Safe_Release(m_pSubTransform);
}
