#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "SoundMgr.h"
#include "Data_Manager.h"	// 추가
#include "Effect_Manager.h"
#include "BackGround.h"
#include "UI_Manager.h"
#include "ImGuiManager.h"
#include "EnmuBoss.h"

//로딩객체
#include "LoadingAnim.h"
#include "LoadingBar.h"
#include "LoadingBaseTitle.h"
#include "LoadingBaseTxt.h"
#include "LoadingCloud.h"
#include "LoadingFixedImg.h"
#include "LoadingShoji.h"
#include "LoadingShojiLeft.h"
#include "LoadingShojiRight.h"

bool			g_bDebug = false;
bool			g_bCollBox = false;
int		    	g_iLevel = 0;
bool			g_bThread = false;
bool			g_bDeathTime = false;
bool			g_bSpecialSkillHit = false;
float			g_fFar = 1800.f;
float			g_fBGM = 0.7f;
float			g_fEffect = 0.8f;
float			g_fVoice = 0.7f;
float			g_fDialog = 1.f;
float			g_fLoading = 0.f;
bool			g_bMiniGame = false;

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
	, m_pImGuiManager(CImGuiManager::Get_Instance())
{
	/*D3D11_SAMPLER_DESC*/
	
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pImGuiManager);
}

HRESULT CMainApp::Initialize()
{
	GRAPHIC_DESC		Graphic_Desc;
	ZeroMemory(&Graphic_Desc, sizeof(GRAPHIC_DESC));

	Graphic_Desc.hWnd = g_hWnd;
	Graphic_Desc.iWinSizeX = g_iWinSizeX;
	Graphic_Desc.iWinSizeY = g_iWinSizeY;
	Graphic_Desc.eWinMode = GRAPHIC_DESC::MODE_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, Graphic_Desc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(CData_Manager::Get_Instance()->Init(m_pDevice, m_pContext)))
		return E_FAIL;	// 추가

	if (FAILED(CUI_Manager::Get_Instance()->Init(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(CEffect_Manager::Get_Instance()->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Fonts(m_pDevice, m_pContext, TEXT("Font_Nexon"), TEXT("../Bin/Resources/Fonts/DemonSlayer.spritefont"))))
		return E_FAIL;
//
//#ifdef _DEBUG
//	if (FAILED(Open_DebugCMD()))
//		return E_FAIL;
//#endif // DEBUG

	if (FAILED(m_pImGuiManager->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	CSoundMgr::Get_Instance()->Initialize();
	
	srand((_uint)time(NULL));

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	if(g_bDeathTime == true)
		fTimeDelta *= 0.2f;

	if (g_bSpecialSkillHit == true)
		fTimeDelta *= 0.05f;

	if (g_bSpecialSkillHit == true)
	{
		static _float fTimeDelay = 0.f;
		fTimeDelay += 1.f / 60.f;

		if (fTimeDelay >= 1.f)
		{
			fTimeDelay = 0.f;
			g_bSpecialSkillHit = false;
		}
	}


	if (!g_bThread)
	{
		m_pImGuiManager->Tick(fTimeDelta);

		m_pGameInstance->Tick_Engine(fTimeDelta,&g_bThread);

		m_fTimeAcc += fTimeDelta;
		m_fTimeDelta = fTimeDelta;
	}
	else if (g_bThread)
	{
		CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
		pUIManager->Tick_Loading(fTimeDelta);
		RELEASE_INSTANCE(CUI_Manager);
		m_fTimeDelta = fTimeDelta;
	}
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 0.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Render_GameObjects(m_fTimeDelta, g_bDebug, g_iLevel, g_bThread);

	m_pImGuiManager->Render();
	m_pGameInstance->Present();

	++m_iNumRender;

	if (m_fTimeAcc > 1.0f)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumRender);
		
	//	OutputDebugString(m_szFPS);
		SetWindowText(g_hWnd, m_szFPS);
		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}

	if (m_pGameInstance->Key_Down(DIK_F1))
		g_bDebug = !g_bDebug;

	if (m_pGameInstance->Key_Down(DIK_F2))
		g_bCollBox = !g_bCollBox;

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevel)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CLevel_Loading*			pLevel_Loading = CLevel_Loading::Create(m_pDevice, m_pContext, eLevel);
	if (nullptr == pLevel_Loading)
		return E_FAIL;

	m_pGameInstance->Open_Level(LEVEL_LOADING, pLevel_Loading);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBufferTrailCom"), CVIBuffer_Trail::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_NewTerrain"), CVIBuffer_NewTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Instance_20"),
		CVIBuffer_Particle_Instance::Create(m_pDevice, m_pContext, 20))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Instance_50"),
		CVIBuffer_Particle_Instance::Create(m_pDevice, m_pContext, 50))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Instance_100"),
		CVIBuffer_Particle_Instance::Create(m_pDevice, m_pContext, 100))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Instance_150"),
		CVIBuffer_Particle_Instance::Create(m_pDevice, m_pContext, 150))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Instance_200"),
		CVIBuffer_Particle_Instance::Create(m_pDevice, m_pContext, 200))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_NewParticle_Instance"),
		CVIBuffer_NewParticle_Instance::Create(m_pDevice, m_pContext, 2000))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Hexagon"),
		CVIBuffer_Hexagon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	/* For.Prototype_Component_Shader*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UIVtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_UIVtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxCubeTexture.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxUIModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxUIModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxRectInstance.hlsl"), VTXRECTINSTANCE_DECLARATION::Elements, VTXRECTINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxPointInstance.hlsl"), VTXPOINTINSTANCE_DECLARATION::Elements, VTXPOINTINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxModelInstance.hlsl"), VTXMODELINSTANCE_DECLARATION::Elements, VTXMODELINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxEffModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxEffModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxEffParticleInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxEffPointInstance.hlsl"), PARTICLEINSTANCE_DECLARATION::Elements, PARTICLEINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Shader_VtxEffTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxEffTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNewParticleInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxPointInstance_New.hlsl"), NEWPARTICLEINSTANCE_DECLARATION::Elements, NEWPARTICLEINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxLine"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_VtxLine.hlsl"), VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements))))
		return E_FAIL;

	//로딩객체
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingAnim"),
		CLoadingAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingBar"),
		CLoadingBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingBaseTitle"),
		CLoadingBaseTitle::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingBaseTxt"),
		CLoadingBaseTxt::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingCloud"),
		CLoadingCloud::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingFixedImg"),
		CLoadingFixedImg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingShoji"),
		CLoadingShoji::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingShojiLeft"),
		CLoadingShojiL::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingShojiRight"),
		CLoadingShojiR::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp::Open_DebugCMD()
{
	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pImGuiManager);

	CImGuiManager::Destroy_Instance();
	CEnmuBoss::Destroy_Instance();
	CGameInstance::Release_Engine();
	CData_Manager::Destroy_Instance();	// 추가
	CUI_Manager::Destroy_Instance();
	CEffect_Manager::Destroy_Instance();

}

