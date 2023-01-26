#include "stdafx.h"
#include "ImGuiManager.h"
#include "GameInstance.h"

#include "Kyoujuro.h"
#include "Tanjiro.h"
#include "Akaza.h"
#include "Rui.h"


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

	static float fAOValue[CRenderer::VALUE_END] = { 1.36f, 0.4f, 1.f, 20.f, 5.f, 0.07f };
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
	ImVec2 vObjSize(100, 20);
	static int selected = 999;


	if (ImGui::BeginListBox("Character", vListSize))
	{
		for (_uint i = 0; i < m_vecObjList.size(); ++i)
		{
			wstring wStrName = m_vecObjList[i]->Get_PlayerInfo().strName;
			string strName;
			
			if (wStrName == L"탄지로")
				strName = "Tanjiro";
			else if (wStrName == L"쿄주로")
				strName = "Kyoujuro";
			else if (wStrName == L"루이")
				strName = "Rui";
			else if (wStrName == L"아카자")
				strName = "Akaza";

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
	ImVec2 vListSize(300, 200);
	ImVec2 vObjSize(300, 15);
	static int selected = 0;

	if (_iIndex == 0)
	{
		if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
		{
			m_vecAnimation = ((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
		{
			m_vecAnimation = ((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
		{
			m_vecAnimation = ((CRui*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
		{
			m_vecAnimation = ((CAkaza*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}


	}
	else if (_iIndex == 1)
	{
		if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
		{
			m_vecAnimation = ((CKyoujuro*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
		else if(m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
		{
			m_vecAnimation = ((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
		{
			m_vecAnimation = ((CRui*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
		{
			m_vecAnimation = ((CAkaza*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
	}


	if (ImGui::BeginListBox("..", vListSize))
	{
		for (_uint i = 0; i < m_vecAnimation.size(); ++i)
		{
			string Temp = to_string(i) + ". ";
			string strName = m_vecAnimation[i]->Get_AnimName();

			if (_iIndex == 0)
			{
				if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
				{
					strName.erase(strName.begin(), strName.begin() + 53);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				//strName.erase(strName.begin(), strName.begin() + 38);
			}
			else if (_iIndex == 1)
			{
				if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
				{
					strName.erase(strName.begin(), strName.begin() + 53);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
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


	ImGui::Text("-----------add animation------------");
	

	ImVec2 vListSize1(300, 100);
	ImVec2 vObjSize1(300, 20);
	static int selected1 = 0;
	if (ImGui::BeginListBox(".", vListSize1))
	{

		for (_uint i = 0; i < m_vecAnimIndex.size(); ++i)
		{
			string str = to_string(i);
			string Temp = to_string(i) + ". ";

			str = Temp + str;

			if (ImGui::Selectable(str.c_str(), stoi(str) == i, 0, vObjSize1))
			{
				selected1 = i;
			}
			ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}

	if (ImGui::Button("Play Animation"))
	{
		if (_iIndex == 0)
		{
			if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
			{
				((CTanjiro*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0,0,0 , false);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
			{
				((CKyoujuro*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
			{
				((CRui*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
			{
				((CAkaza*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
		}
		else if (_iIndex == 1)
		{
			if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
			{
				((CKyoujuro*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
			{
				((CTanjiro*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
			{
				((CRui*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
			{
				((CAkaza*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
		}
	}




	if(ImGui::Button("Add Animation"))
	{
		if (_iIndex == 0)
		{
			if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
			{
				m_vecAnimIndex.push_back(selected);
			}
		}
		else if (_iIndex == 1)
		{
			if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
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
			size_t iIndexSize = m_vecAnimIndex.size();

			if (_iIndex == 0)
			{
				if (iIndexSize < 3)
				{
					m_vecAnimIndex.push_back(-1);
					m_vecAnimIndex.push_back(-1);
					m_vecAnimIndex.push_back(-1);
				}

				if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
				{
					((CTanjiro*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
				{
					((CKyoujuro*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
				{
					((CRui*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
				{
					((CAkaza*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
			}
			else if (_iIndex == 1)
			{

				if (iIndexSize < 3)
				{
					m_vecAnimIndex.push_back(-1);
					m_vecAnimIndex.push_back(-1);
					m_vecAnimIndex.push_back(-1);
				}

				if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
				{
					((CKyoujuro*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
				{
					((CTanjiro*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
				{
					((CRui*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
				{
					((CAkaza*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
			}
		}
	}

	
	//ImGui::SliderFloat("slider Animation", &m_fDuration, 0.f, m_vecAnimation[]->);


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
