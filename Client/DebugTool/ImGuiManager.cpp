#include "stdafx.h"
#include "ImGuiManager.h"
#include "GameInstance.h"

#include "Kyoujuro.h"
#include "Tanjiro.h"


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



	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	m_bImguiEnable = true;

	CComponent* pOut = pGameInstance->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Renderer");
	m_pRendererCom = (CRenderer*)pOut; 

	if(nullptr == m_pRendererCom)
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CImGuiManager::Tick(_float fTimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Key_Pressing(DIK_LCONTROL))
		if (pGameInstance->Key_Down(DIK_1))
			m_bImguiEnable = !m_bImguiEnable;


	if (m_bImguiEnable)
	{
		ShowGui(fTimeDelta);
	}

	RELEASE_INSTANCE(CGameInstance);
	ImGui::EndFrame();
}

void CImGuiManager::ShowGui(_float fTimeDelta)
{
	ImGui::Begin("Close, Open = Ctrl + 1");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Post Processing"))
		{
			PostProcessing(fTimeDelta);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Animation"))
		{
			AnimationDebug(fTimeDelta);
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

void CImGuiManager::PostProcessing(_float fTimeDelta)
{
	ImGuiIO& io = ImGui::GetIO();


	static char*	AOButton = ("AO Button");
	static _bool	bAO_OnOff = true;
	if (ImGui::Button(AOButton, ImVec2(ImGui::GetWindowWidth() * 0.35f, 25.f)))
		bAO_OnOff = !bAO_OnOff;
	if (nullptr != m_pRendererCom)
		m_pRendererCom->AO_OnOff(bAO_OnOff);

	static float fAOValue[CRenderer::VALUE_END] = { 1.36f, 0.4f, 1.f, 20.f };
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("AO Value", &fAOValue[CRenderer::VALUE_AO], 0.02f, 0.f);
	
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("AO Radius", &fAOValue[CRenderer::VALUE_AORADIUS], 0.02f, 0.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Glow Blurring Count", &fAOValue[CRenderer::VALUE_GLOWBLURCOUNT], 0.02f, 0.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Distortion", &fAOValue[CRenderer::VALUE_DISTORTION], 0.02f, 0.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("OutLine", &fAOValue[CRenderer::VALUE_OUTLINE], 0.02f, 0.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("InnerLine", &fAOValue[CRenderer::VALUE_INNERLINE], 0.001f, 0.f);

	if (nullptr != m_pRendererCom)
	{
		for (_int i = 0; i < CRenderer::VALUE_END; ++i)
			m_pRendererCom->Set_Value(CRenderer::VALUETYPE(i), fAOValue[i]);
	}
}



void CImGuiManager::AnimationDebug(_float fTimeDelta)
{
	LiveCharacterList();
	


}

void CImGuiManager::Add_LiveCharacter(CCharacters * pCharacter)
{
	if (pCharacter == nullptr)
		return;

	else if (pCharacter != nullptr)
		m_vecObjList.push_back(pCharacter);

}

void CImGuiManager::LiveCharacterList()
{

	ImVec2 vListSize(100, 60);
	ImVec2 vObjSize(100, 30);
	static int selected = 999;


	if (ImGui::BeginListBox("Character", vListSize))
	{
		for (_uint i = 0; i < m_vecObjList.size(); ++i)
		{
			wstring wStrName = m_vecObjList[i]->Get_PlayerInfo().strName;
			string strName;
			
			if (wStrName == L"ź����")
				strName = "Tanjiro";
			else if (wStrName == L"���ַ�")
				strName = "Kyoujuro";

			if (ImGui::Selectable(strName.c_str(), selected == i, 0, vObjSize))
			{
				selected = i;
			}
			ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}


	if (selected != 999)
		CharacterAnimationList(selected);
}

void CImGuiManager::CharacterAnimationList(_uint _iIndex)
{
	ImVec2 vListSize(250, 300);
	ImVec2 vObjSize(200, 30);
	static int selected = 0;

	if (_iIndex == 0)
	{
		if (m_vecObjList[0]->Get_PlayerInfo().strName == L"ź����")
		{
			m_vecAnimation = ((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"���ַ�")
		{
			m_vecAnimation = ((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}
		
	}
	else if (_iIndex == 1)
	{
		if (m_vecObjList[1]->Get_PlayerInfo().strName == L"���ַ�")
		{
			m_vecAnimation = ((CKyoujuro*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
		else if(m_vecObjList[1]->Get_PlayerInfo().strName == L"ź����")
		{
			m_vecAnimation = ((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
	}


	if (ImGui::BeginListBox("Animation", vListSize))
	{
		for (_uint i = 0; i < m_vecAnimation.size(); ++i)
		{
			string Temp = to_string(i) + ". ";
			string strName = m_vecAnimation[i]->Get_AnimName();

			if (_iIndex == 0)
			{
				if (m_vecObjList[0]->Get_PlayerInfo().strName == L"ź����")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"���ַ�")
				{
					strName.erase(strName.begin(), strName.begin() + 53);
				}
				//strName.erase(strName.begin(), strName.begin() + 38);
			}
			else if (_iIndex == 1)
			{
				if (m_vecObjList[1]->Get_PlayerInfo().strName == L"���ַ�")
				{
					strName.erase(strName.begin(), strName.begin() + 53);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"ź����")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}

				//strName.erase(strName.begin(), strName.begin() + 53);
			}

			strName = Temp + strName;

			if (ImGui::Selectable(strName.c_str(), selected == i, 0, vObjSize))
			{
				selected = i;
			}
			ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	ImGui::SameLine();
	if (ImGui::BeginListBox("add Animation", vListSize))
	{

		for (_uint i = 0; i < m_vecAnimIndex.size(); ++i)
		{
			string str = to_string(i);

			if (ImGui::Selectable(str.c_str(), stoi(str) == i, 0, vObjSize))
			{

			}
			ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}

	if (ImGui::Button("Play Animation"))
	{
		if (_iIndex == 0)
		{
			if (m_vecObjList[0]->Get_PlayerInfo().strName == L"ź����")
			{
				((CTanjiro*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0,0,0 , false);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"���ַ�")
			{
			}
		}
		else if (_iIndex == 1)
		{
			if (m_vecObjList[1]->Get_PlayerInfo().strName == L"���ַ�")
			{
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"ź����")
			{
				((CTanjiro*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
		}
	}






	if(ImGui::Button("Add Animation"))
	{
		if (_iIndex == 0)
		{
			if (m_vecObjList[0]->Get_PlayerInfo().strName == L"ź����")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"���ַ�")
			{
				m_vecAnimIndex.push_back(selected);
			}
		}
		else if (_iIndex == 1)
		{
			if (m_vecObjList[1]->Get_PlayerInfo().strName == L"���ַ�")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"ź����")
			{
				m_vecAnimIndex.push_back(selected);
			}
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Clear Animation"))
	{
		m_vecAnimIndex.clear();
	}


	if (ImGui::Button("Play All Animation"))
	{
		if (!m_vecAnimIndex.empty())
		{
			_uint iIndexSize = m_vecAnimIndex.size();

			((CTanjiro*)(m_vecObjList[_iIndex]))->
				Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0 ,true);
		}
	}

	

}


void CImGuiManager::Clear_CharacterList()
{
	for (auto& iter : m_vecObjList)
		Safe_Release(iter);

	m_vecObjList.clear();
}

void CImGuiManager::Clear_AnimationList()
{
	for (auto& iter : m_vecAnimation)
		Safe_Release(iter);

	m_vecAnimation.clear();
}

void CImGuiManager::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pRendererCom);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

//	Clear_CharacterList();
//	Clear_AnimationList();

}
