#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Data_Manager.h"	// 추가

#include "Camera_Dynamic.h"
#include "BackGround.h"
#include "Terrain.h"

//Map
#include "BattleField.h"
#include "MeshObj_Static.h"

//Player 
#include "Player.h"
//char
#include "Rui.h"


CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

unsigned int APIENTRY Thread_Main(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlayLevel();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	CoInitializeEx(nullptr, 0);

	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("                       텍스쳐 로딩 중."));

	/* 모델 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("                     모델 로딩 중."));



	_matrix			PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	////	Tree
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_BigTree1",
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/BigTree1.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_BigTree2",
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/BigTree2.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_BigTree3",
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/BigTree3.fbx", PivotMatrix))))
	//	return E_FAIL;

	////	cliff
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff1",
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff1.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff2",
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff2.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff3",
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff3.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Cliff_Small",
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/rui/Cliff_Small.fbx", PivotMatrix))))
	//	return E_FAIL;

	////	grass
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass1",
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass1.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass2",
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass2.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass3",
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass3.fbx", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Grass4",
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NonAnim/static/hisanoie/grass4.fbx", PivotMatrix))))
	//	return E_FAIL;

	////	MeshObj_Static
	///*For.Prototype_GameObject_MeshObj_Static */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MeshObj_Static"),
	//	CMeshObj_Static::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;






	/* 카메라 객체 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* 케릭터 객체 */

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Player/Player.fbx", PivotMatrix))))
	//	return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Player"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rui"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Anim/Rui/Rui.fbx", PivotMatrix))))
		return E_FAIL;*/
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Rui"), LEVEL_STATIC, CData_Manager::DATA_ANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rui"),
		CRui::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//Map
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BattleField"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BattleField"),
		CBattleField::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	//// Navi
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation"),
	//	CNavigation::Create(m_pDevice, m_pContext, TEXT("../Data/NaviPos.dat")))))
	//	return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("                        로딩이 완료되었습니다."));

	m_isFinished = true;
	RELEASE_INSTANCE(CData_Manager);
	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		ERR_MSG(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
