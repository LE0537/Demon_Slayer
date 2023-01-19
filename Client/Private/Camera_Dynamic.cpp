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

	Move_CamPos(fTimeDelta);

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

	XMStoreFloat4(&m_vPoint, vPos);

	_vector vRight = XMVector3Normalize(vPos - vTarget);
	//_vector vUp = { 0.f,1.f,0.f,0.f };
	//_vector vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

	//_matrix	RotationMatrixX = XMMatrixRotationAxis(vRight, XMConvertToRadians(15.f));

	//vRight = XMVector3Normalize(XMVector3TransformNormal(vRight, RotationMatrixX));
	//vUp = XMVector3Normalize(XMVector3TransformNormal(vUp, RotationMatrixX));
	//vLook = XMVector3Normalize(XMVector3TransformNormal(vLook, RotationMatrixX));

	//m_pSubTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	//m_pSubTransform->Set_State(CTransform::STATE_UP, vUp);
	//m_pSubTransform->Set_State(CTransform::STATE_LOOK, vLook);

	_vector vAtPos = vPos;

	vAtPos.m128_f32[1] += 2.f;

	m_pSubTransform->LookAt(vAtPos);

	m_pSubTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pSubTransform->Set_Rotation(_float3(0.f, m_fAngle, 0.f));

	_vector vLook = XMVector3Normalize(m_pSubTransform->Get_State(CTransform::STATE_LOOK));
	vPos -= vLook * 30.f;
	vPos.m128_f32[1] = 0.f;
	vPos.m128_f32[1] += 5.f;
	m_pSubTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);

	
}

void CCamera_Dynamic::Move_CamPos(_float fTimeDelta)
{
	ConvertToViewPort();

	_vector vPoint = XMLoadFloat4(&m_vPoint);

	if (m_f1pX < 200.f)
	{
		m_fAngle += 3.f;
	}
	else if (m_f1pX > 1080.f)
	{
		//m_fAngle -= 3.f;
	/*	m_pSubTransform->Set_State(CTransform::STATE_TRANSLATION, vPoint);
		m_pSubTransform->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-2.f));
		_vector vLook = XMVector3Normalize(m_pSubTransform->Get_State(CTransform::STATE_LOOK));
		vPoint -= vLook * 30.f;
		vPoint.m128_f32[1] = 0.f;
		vPoint.m128_f32[1] += 10.f;
		m_pSubTransform->Set_State(CTransform::STATE_TRANSLATION, vPoint);*/
	}
}

void CCamera_Dynamic::Lerp_SubCam(_float fTimeDelta)
{
	_float	fWeight = 5.f;
	_matrix matWorld = XMMatrixIdentity();

	_vector vLerp = XMVector3Normalize(XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_LOOK), m_pSubTransform->Get_State(CTransform::STATE_LOOK), fTimeDelta * fWeight));
	matWorld.r[CTransform::STATE_LOOK] = vLerp;

	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransform->Get_State(CTransform::STATE_LOOK)));
	_vector vUp = XMVector3Cross(m_pTransform->Get_State(CTransform::STATE_LOOK), m_pTransform->Get_State(CTransform::STATE_RIGHT));
	matWorld.r[CTransform::STATE_RIGHT] = vRight;
	matWorld.r[CTransform::STATE_UP] = vUp;

	_vector vLerp_Pos = XMVector3Normalize(XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), m_pSubTransform->Get_State(CTransform::STATE_TRANSLATION), fTimeDelta * fWeight));
	matWorld.r[CTransform::STATE_TRANSLATION] = vLerp_Pos;

	m_pTransform->Set_WorldMatrix(matWorld);
}

void CCamera_Dynamic::ConvertToViewPort()
{
	_vector vPlayerPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPos = m_pSubTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPlayerLook = vPos - vPlayerPos;
	_vector vTargetLook = vPos - vTargetPos;

	_float fPlayerDist = XMVectorGetX(XMVector3Length(vPlayerLook));
	_float fTargetDist = XMVectorGetX(XMVector3Length(vTargetLook));
	/////////////////////////////////////////////////////////////////////////////위에가 소트
	_matrix matPlayerWorld = m_pPlayer->Get_Transform()->Get_WorldMatrix();
	_matrix matTargetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();
	_matrix matView = XMMatrixInverse(nullptr, m_pTransform->Get_WorldMatrix());
	_matrix matProj = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	_matrix matWVP = matPlayerWorld * matView * matProj;
	_matrix matWVP2 = matTargetWorld * matView * matProj;
	_float  m_fScalingbyDepth = XMVectorGetW(matWVP.r[3]);
	matWVP.r[3] /= m_fScalingbyDepth;

	XMStoreFloat4(&m_vPlayerPos, matWVP.r[3]);
	
	m_vPlayerPos.x += 1.f;
	m_vPlayerPos.x *= g_iWinSizeX / 2.f;

	XMStoreFloat4(&m_vTargetPos, matWVP2.r[3]);

	m_vTargetPos.x += 1.f;
	m_vTargetPos.x *= g_iWinSizeX / 2.f;


	if (fTargetDist >= fPlayerDist)
	{
		m_f1pX = m_vPlayerPos.x;
		m_f2pX = m_vTargetPos.x;
	}
	else
	{
		m_f1pX = m_vTargetPos.x;
		m_f2pX = m_vPlayerPos.x;
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
