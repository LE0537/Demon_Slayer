#include "stdafx.h"
#include "ImGuiManager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CImGuiManager)

CImGuiManager::CImGuiManager()
{
}

HRESULT CImGuiManager::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	return S_OK;
}

void CImGuiManager::Tick(_float fTimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ShowGui(fTimeDelta);

	ImGui::EndFrame();
}

void CImGuiManager::ShowGui(_float fTimeDelta)
{
	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Terrain"))
		{
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Object"))
		{
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Cam_Action"))
		{
			ImGui::EndTabItem();
		}


		ImGui::EndTabBar();
	}


	ImGui::End();

	RELEASE_INSTANCE(CGameInstance);
}

void CImGuiManager::Render()
{
	if (m_bImguiEnable)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

void CImGuiManager::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
