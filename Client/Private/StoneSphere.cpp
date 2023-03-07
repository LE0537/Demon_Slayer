#include "stdafx.h"
#include "..\Public\StoneSphere.h"

#include "GameInstance.h"
#include "Layer.h"
#include "Tanjiro.h"
#include "SoundMgr.h"
CStoneSphere::CStoneSphere(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CStoneSphere::CStoneSphere(const CStoneSphere & rhs)
	: CCollBox(rhs)
{
}

HRESULT CStoneSphere::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStoneSphere::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Stone("Stone1"))) return E_FAIL;
	if (FAILED(Ready_Stone("Stone2"))) return E_FAIL;
	if (FAILED(Ready_Stone("Stone3"))) return E_FAIL;

	_int iIndex = *(_int*)pArg;

	switch (iIndex)
	{
	case 0:
		m_eStone = STONEPOS_LEFT;
		break;
	case 1:
		m_eStone = STONEPOS_CENTER;
		break;
	case 2:
		m_eStone = STONEPOS_RIGHT;
		break;
	default:
		break;
	}
	m_pTransformCom->Set_Scale(XMVectorSet(3.5f, 3.5f, 3.5f, 0.f));

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayer = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CStoneSphere::Tick(_float fTimeDelta)
{
	if (!m_bStone)
		m_bStone = true;
	if (m_bStone)
		Move_Stone(m_eStone, fTimeDelta);

	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_fTurnAngle += 10.f;
	if (m_fTurnAngle >= 360.f)
		m_fTurnAngle = 0.f;
	m_pTransformCom->Turn2(XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)), XMConvertToRadians(m_fTurnAngle));
	
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	vPos.m128_f32[1] -= 0.8f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
}

void CStoneSphere::Late_Tick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	CCollider* m_pPlayerCollBox = m_pPlayer->Get_SphereCollider();
	if (m_pSphereCom->Collision(m_pPlayerCollBox) && m_pPlayer->Get_StoneHit() <= 0.f)
	{
		if (m_pPlayer->Get_Heart() > 0)
		{
			m_pPlayer->Set_Heart(-1);
			m_pPlayer->Set_StoneHit(0.7f);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_SE_GuardHit.wav"), g_fEffect);
		}
		else
		{
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_SE_GuardHit.wav"), g_fEffect);
			m_pPlayer->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-556.3474f, 53.5169f, -61.9126f, 1.f));
			m_pPlayer->Get_NavigationCom()->Cheak_Cell(XMVectorSet(-556.3474f, 53.5169f, -61.9126f, 1.f));
			m_pPlayer->Set_Heart(2);
		}
	}
	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pSphereCom);
	}
}

HRESULT CStoneSphere::Render()
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
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;

	}


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CStoneSphere::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (m_bShadowAlphaIncrease == false)
		m_ShadowMatrix = m_pTransformCom->Get_WorldMatrix();

	if (m_bShadowAlphaDecrease == true)
		m_ShadowMatrix = m_pTransformCom->Get_WorldMatrix();


	_float4 vTemp = *(_float4*)&m_ShadowMatrix.r[3];

	if (m_bShadowAlphaIncrease == true)
	{
		vTemp.x += 100.f;
		vTemp.y += 100.f;
		vTemp.z += 100.f;
	}

	*(_float4*)&m_ShadowMatrix.r[3] = vTemp;

	//_float4x4 WorldMatrix = m_pTransformCom->Get_World4x4();

	_float4x4	TransposeMatrix;
	XMStoreFloat4x4(&TransposeMatrix, XMMatrixTranspose(m_ShadowMatrix));


	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &TransposeMatrix, sizeof(_float4x4))))
		return E_FAIL;




	_vector vLightEye, vLightAt, vLightUp;
	_matrix matLightView;
	if (g_iLevel == LEVEL_GAMEPLAY)
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	}
	else
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	}
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
HRESULT CStoneSphere::SetUp_ShaderResources()
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
HRESULT CStoneSphere::Ready_Stone(char * pFileName)
{
	int		iIndex = 0;

	char		szFilePath[MAX_PATH] = "../Bin/Resources/Data/CamActions/";
	strcat_s(szFilePath, pFileName);
	strcat_s(szFilePath, ".cma");

	_tchar		szRealPath[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, szFilePath, (_int)strlen(szFilePath), szRealPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szRealPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Failed to Load : CamAction File");

		return E_FAIL;
	}


	enum CAMTYPE { CAM_EYE, CAM_AT, CAM_END };
	std::vector<_float4>		vecCam[CAM_END];
	std::vector<_float>			vecCamTime;

	DWORD	dwByte = 0;
	_float4*	pCamEye = new _float4;
	_float4*	pCamAt = new _float4;
	_float*		pCamTime = new _float;

	_float2*	pBlur = new _float2;
	ReadFile(hFile, pBlur, sizeof(_float2), &dwByte, nullptr);


	while (true)
	{
		ReadFile(hFile, pCamEye, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, pCamAt, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, pCamTime, sizeof(_float), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pCamEye);
			Safe_Delete(pCamAt);
			Safe_Delete(pCamTime);
			Safe_Delete(pBlur);

			vecCamTime.erase(vecCamTime.end() - 1);
			break;
		}
		_float4		vCamEye = *pCamEye;
		_float4		vCamAt = *pCamAt;
		_float		vCamTime = *pCamTime;

		vecCam[CAM_EYE].push_back(vCamEye);
		vecCam[CAM_AT].push_back(vCamAt);
		vecCamTime.push_back(vCamTime);
	}
	CloseHandle(hFile);

	Safe_Delete(pCamEye);
	Safe_Delete(pCamAt);
	Safe_Delete(pCamTime);

	m_vecStoneEye.push_back(vecCam[CAM_EYE]);
	m_vecStoneAt.push_back(vecCam[CAM_AT]);
	m_vecStoneTime.push_back(vecCamTime);

	return S_OK;
}
_bool CStoneSphere::Move_Stone(STONEPOS _eStone, _float fTimeDelta)
{
	if (STONEPOS_END <= _eStone)
		return false;


	m_bStone = Play_Stone(m_vecStoneEye[_eStone], m_vecStoneAt[_eStone], m_vecStoneTime[_eStone], &m_fCurrentCutSceneTime, fTimeDelta);

	
	return m_bStone;
}
_bool CStoneSphere::Play_Stone(vector<_float4> vecPositions, vector<_float4> vecLookAts, vector<_float> vecUseTime, _float * pOut, _float fTimeDelta)
{
	_int iSize = (_int)vecPositions.size();

	if (iSize < 4 ||
		(iSize != (_int)vecLookAts.size() || iSize != (_int)vecUseTime.size() + 1))
		return false;

	_float	fUsedTime = m_fCullTime;
	_int	iFrame = max(_int(m_fCullTime), 0) + 1;				//	현재 프레임. 첫 번째는 읽지않음.(None)
	_float	fDecimal = max(m_fCullTime, 0.f) - (iFrame - 1);


	if (iFrame + 1 >= iSize)		//	끝 Check
	{
		m_fCullTime = *pOut = 0.f;
		return false;
	}

	if (0.f == vecUseTime[iFrame])	//	부여된 시간이 0이면 스킵 ( 깔끔한 루트를 위한 구체는 스킵함. )
	{
		while (true)
		{
			if (0.f != vecUseTime[iFrame] ||
				iFrame >= (_int)vecUseTime.size())
				break;

			++iFrame;
		}
	}

	*pOut += min((fTimeDelta) / (vecUseTime[iFrame]), 1.f);

	m_fCullTime = *pOut;


	fDecimal = max(m_fCullTime, 0.f) - (iFrame - 1);


	_vector vCamPos, vCamAt;
	_vector vAt[4];
	_vector vPos[4];

	for (_int j = 0; j < 4; ++j)
	{
		_int	iIndex = max(min(iFrame + j - 1, iSize - 1), 1);		//	최소 = 0, 최대 = Size, 첫번째는 읽지 않음.
		vPos[j] = XMLoadFloat4(&vecPositions[iIndex]);
		vAt[j] = XMLoadFloat4(&vecLookAts[iIndex]);
	}



	_float fRatio = fDecimal;		//	다음 프레임의 할당 비율
	vCamPos = XMVectorCatmullRom(vPos[0], vPos[1], vPos[2], vPos[3], fRatio);
	vCamAt = XMVectorCatmullRom(vAt[0], vAt[1], vAt[2], vAt[3], fRatio);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vCamPos);
	m_pTransformCom->LookAt(vCamAt);

	return true;
}
HRESULT CStoneSphere::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_StoneSphere"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CStoneSphere * CStoneSphere::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CStoneSphere*	pInstance = new CStoneSphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CStoneSphere"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CStoneSphere::Clone(void * pArg)
{
	CGameObject*	pInstance = new CStoneSphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CStoneSphere"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStoneSphere::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pModelCom);
}

