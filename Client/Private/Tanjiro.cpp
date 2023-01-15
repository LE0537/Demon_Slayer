#include "stdafx.h"
#include "..\Public\Tanjiro.h"
#include "Layer.h"
#include "GameInstance.h"
#include "Camera_Dynamic.h"

// state
#include "TanjiroState.h"
#include "TanjiroIdleState.h"
#include "TanjiroMoveState.h"

using namespace Tanjiro;


CTanjiro::CTanjiro(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CTanjiro::CTanjiro(const CTanjiro & rhs)
	: CGameObj(rhs)
{
}

HRESULT CTanjiro::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTanjiro::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;



	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-3.f, 0.f, 0.f, 1.f));

	CTanjiroState* pState = new CIdleState();
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);


	return S_OK;
}

void CTanjiro::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Set_ShadowLightPos();

	

	HandleInput();
	TickState(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta);
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CTanjiro::Late_Tick(_float fTimeDelta)
{

	LateTickState(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTanjiro::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;

		//aiTextureType_AMBIENT
	}

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CTanjiro::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;


	_vector			vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
	_vector			vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
	_vector			vLightUp = { 0.f, 1.f, 0.f ,0.f };
	_matrix			matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(matLightView), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
			return E_FAIL;

	}

	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}

HRESULT CTanjiro::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CTanjiro::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Tanjiro"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vPosition = _float3(0.f, 2.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vPosition = _float3(0.f, 2.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}



void CTanjiro::Set_ShadowLightPos()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	_float4 vAt = vPos;

	vPos.x -= 15.f;
	vPos.y += 30.f;
	vPos.z -= 30.f;

	pGameInstance->Set_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW, vPos, vAt);

	RELEASE_INSTANCE(CGameInstance);
}

void CTanjiro::Tanjiro_GoStraight(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, L"Layer_Camera", L"Com_Transform");

	_vector vPlayerLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vCameraLook = pTransform->Get_State(CTransform::STATE_LOOK);
	vCameraLook = XMVectorSetY(vCameraLook, XMVectorGetY(vPlayerLook));
	_vector vLookDot = XMVector3AngleBetweenVectors(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_float fAngle = XMConvertToDegrees(XMVectorGetX(vLookDot));

	//해킹 완료 배고파서 6시팟 밥머그러 감 ㅋ ㅋ

	// 외적, 내적으로 angle 보정
	_vector vCross = XMVector3Cross(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_vector vDot = XMVector3Dot(XMVector3Normalize(vCross), XMVector3Normalize(XMVectorSet(0.f, 1.f, 0.f, 0.f)));

	_float fScala = XMVectorGetX(vDot);

	if (fScala < 0.f)
		fAngle = 360.f - fAngle;


	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), fAngle - 5.f);
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);
	m_eCurKeyState = KEY_END;

	int i = fAngle;

	_tchar	m_szFPS[MAX_PATH] = L"";
	wsprintf(m_szFPS, L"fAngle : L %d", i);
	OutputDebugString(m_szFPS);

	RELEASE_INSTANCE(CGameInstance);
}

void CTanjiro::Tanjiro_GoBackWard(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, L"Layer_Camera", L"Com_Transform");

	_vector vPlayerLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vCameraLook = pTransform->Get_State(CTransform::STATE_LOOK);
	vCameraLook = XMVectorSetY(vCameraLook, XMVectorGetY(vPlayerLook));
	vCameraLook *= -1.f;
	_vector vLookDot = XMVector3AngleBetweenVectors(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_float fAngle = XMConvertToDegrees(XMVectorGetX(vLookDot));

	// 외적, 내적으로 angle 보정
	_vector vCross = XMVector3Cross(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_vector vDot = XMVector3Dot(XMVector3Normalize(vCross), XMVector3Normalize(XMVectorSet(0.f, 1.f, 0.f, 0.f)));

	_float fScala = XMVectorGetX(vDot);

	if (fScala < 0.f)
		fAngle = 360.f - fAngle;


	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), fAngle - 5.f);
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);
	m_eCurKeyState = KEY_END;

	RELEASE_INSTANCE(CGameInstance);
}

void CTanjiro::Tanjiro_GoRight(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, L"Layer_Camera", L"Com_Transform");

	m_eCurKeyState = KEY_RIGHT;

	_vector vPlayerLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vCameraLook = pTransform->Get_State(CTransform::STATE_LOOK);
	vCameraLook = XMVectorSetY(vCameraLook, XMVectorGetY(vPlayerLook));
	_vector tempVec = XMVector3AngleBetweenVectors(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_float fAngle = XMConvertToDegrees(XMVectorGetX(tempVec));

	if (m_ePreKeyState == KEY_LEFT)
		fAngle += 180.f;
	else
		fAngle = 90.f - fAngle;

	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), fAngle);
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);
	m_ePreKeyState = m_eCurKeyState;


	RELEASE_INSTANCE(CGameInstance);
}

void CTanjiro::Tanjiro_GoLeft(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, L"Layer_Camera", L"Com_Transform");

	m_eCurKeyState = KEY_LEFT;

	_vector vPlayerLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vCameraLook = pTransform->Get_State(CTransform::STATE_LOOK);
	vCameraLook = XMVectorSetY(vCameraLook, XMVectorGetY(vPlayerLook));
	_vector tempVec = XMVector3AngleBetweenVectors(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_float fAngle = XMConvertToDegrees(XMVectorGetX(tempVec));




	if (m_ePreKeyState == KEY_RIGHT)
		fAngle += 180.f;
	else
		fAngle += 270.f;

	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), fAngle);
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);
	m_ePreKeyState = m_eCurKeyState;

	RELEASE_INSTANCE(CGameInstance);
}

void CTanjiro::Tanjiro_GoLF(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, L"Layer_Camera", L"Com_Transform");

	m_eCurKeyState = KEY_LF;

	_vector vPlayerLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vCameraLook = pTransform->Get_State(CTransform::STATE_LOOK);
	vCameraLook = XMVectorSetY(vCameraLook, XMVectorGetY(vPlayerLook));
	_vector tempVec = XMVector3AngleBetweenVectors(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_float fAngle = XMConvertToDegrees(XMVectorGetX(tempVec));


	// 외적, 내적으로 angle 보정
	_vector vCross = XMVector3Cross(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_vector vDot = XMVector3Dot(XMVector3Normalize(vCross), XMVector3Normalize(XMVectorSet(0.f, 1.f, 0.f, 0.f)));

	_float fScala = XMVectorGetX(vDot);

	if (fScala < 0.f)
		fAngle = 360.f - fAngle;


	if (m_ePreKeyState == KEY_RB)
		fAngle += 180.f;
	else
		fAngle += 315.f;

	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), fAngle);
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);
	m_ePreKeyState = m_eCurKeyState;

	RELEASE_INSTANCE(CGameInstance);
}

void CTanjiro::Tanjiro_GoRF(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, L"Layer_Camera", L"Com_Transform");

	m_eCurKeyState = KEY_RF;

	_vector vPlayerLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vCameraLook = pTransform->Get_State(CTransform::STATE_LOOK);
	vCameraLook = XMVectorSetY(vCameraLook, XMVectorGetY(vPlayerLook));
	_vector tempVec = XMVector3AngleBetweenVectors(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_float fAngle = XMConvertToDegrees(XMVectorGetX(tempVec));

	// 외적, 내적으로 angle 보정
	_vector vCross = XMVector3Cross(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_vector vDot = XMVector3Dot(XMVector3Normalize(vCross), XMVector3Normalize(XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	_float fScala = XMVectorGetX(vDot);

	if (fScala < 0.f)
		fAngle = 360.f - fAngle;

	if (m_ePreKeyState == KEY_LB)
		fAngle += 180.f;
	else
		fAngle += 45.f;

	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), fAngle);
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);
	m_ePreKeyState = m_eCurKeyState;

	RELEASE_INSTANCE(CGameInstance);
}

void CTanjiro::Tanjiro_GoLB(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, L"Layer_Camera", L"Com_Transform");

	m_eCurKeyState = KEY_LB;

	_vector vPlayerLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vCameraLook = pTransform->Get_State(CTransform::STATE_LOOK);
	vCameraLook = XMVectorSetY(vCameraLook, XMVectorGetY(vPlayerLook));
	vCameraLook *= -1.f;

	_vector tempVec = XMVector3AngleBetweenVectors(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_float fAngle = XMConvertToDegrees(XMVectorGetX(tempVec));


	// 외적, 내적으로 angle 보정
	_vector vCross = XMVector3Cross(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_vector vDot = XMVector3Dot(XMVector3Normalize(vCross), XMVector3Normalize(XMVectorSet(0.f, 1.f, 0.f, 0.f)));

	_float fScala = XMVectorGetX(vDot);

	if (fScala < 0.f)
		fAngle = 360.f - fAngle;


	if (m_ePreKeyState == KEY_RF)
		fAngle += 180.f;
	else
		fAngle += 405.f;

	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), fAngle);
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);
	m_ePreKeyState = m_eCurKeyState;

	RELEASE_INSTANCE(CGameInstance);
}

void CTanjiro::Tanjiro_GoRB(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, L"Layer_Camera", L"Com_Transform");

	m_eCurKeyState = KEY_RB;

	_vector vPlayerLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vCameraLook = pTransform->Get_State(CTransform::STATE_LOOK);
	vCameraLook = XMVectorSetY(vCameraLook, XMVectorGetY(vPlayerLook));
	vCameraLook *= -1.f;

	_vector tempVec = XMVector3AngleBetweenVectors(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_float fAngle = XMConvertToDegrees(XMVectorGetX(tempVec));


	// 외적, 내적으로 angle 보정
	_vector vCross = XMVector3Cross(XMVector3Normalize(vPlayerLook), XMVector3Normalize(vCameraLook));
	_vector vDot = XMVector3Dot(XMVector3Normalize(vCross), XMVector3Normalize(XMVectorSet(0.f, 1.f, 0.f, 0.f)));

	_float fScala = XMVectorGetX(vDot);

	if (fScala < 0.f)
		fAngle = 360.f - fAngle;


	if (m_ePreKeyState == KEY_LF)
		fAngle += 180.f;
	else
		fAngle += 315.f;

	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), fAngle);
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);
	m_ePreKeyState = m_eCurKeyState;

	RELEASE_INSTANCE(CGameInstance);
}



void CTanjiro::HandleInput()
{
	CTanjiroState* pNewState = m_pTanjiroState->HandleInput(this);

	if (pNewState)
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pNewState);


}

void CTanjiro::TickState(_float fTimeDelta)
{
	CTanjiroState* pNewState = m_pTanjiroState->Tick(this, fTimeDelta);

	if (pNewState)
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pNewState);
}

void CTanjiro::LateTickState(_float fTimeDelta)
{
	CTanjiroState* pNewState = m_pTanjiroState->Late_Tick(this, fTimeDelta);

	if (pNewState)
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pNewState);
}

CTanjiro * CTanjiro::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTanjiro*	pInstance = new CTanjiro(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTanjiro"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTanjiro::Clone(void * pArg)
{
	CGameObject*	pInstance = new CTanjiro(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTanjiro"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTanjiro::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pModelCom);
	Safe_Delete(m_pTanjiroState);
}
