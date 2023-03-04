#include "stdafx.h"
#include "..\Public\AkazaScene.h"
#include "GameInstance.h"
#include "SceneState.h"
#include "Layer.h"
#include "Tanjiro.h"
#include "Kyoujuro.h"
#include "Akaza.h"

IMPLEMENT_SINGLETON(CAkazaScene)

using namespace AkazaScene;

CAkazaScene::CAkazaScene()
{
}

HRESULT CAkazaScene::Initialize()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	for (_uint i = 0; i < m_AkazaSceneList.size(); ++i)
	{
		if (m_AkazaSceneList[i]->Get_PlayerInfo().strName == L"¾ÆÄ«ÀÚ")
		{
			pAkaza = m_AkazaSceneList[i];
		}

		if (m_AkazaSceneList[i]->Get_PlayerInfo().strName == L"ÄìÁÖ·Î")
		{
			pKyoujuro = m_AkazaSceneList[i];
		}

		if (m_AkazaSceneList[i]->Get_PlayerInfo().strName == L"ÅºÁö·Î")
		{
			pTanjiro = m_AkazaSceneList[i];
		}

	}
	
	m_AkazaSceneList.clear();

	m_AkazaSceneList.push_back(pTanjiro);
	m_AkazaSceneList.push_back(pKyoujuro);
	m_AkazaSceneList.push_back(pAkaza);


	CAkazaSceneState* pState = new CSceneState(CSceneState::SCENE_0);
	m_pAkazaSceneState = m_pAkazaSceneState->ChangeState(this, m_pAkazaSceneState, pState);
	

	return S_OK;
}

HRESULT CAkazaScene::Add_Character(CCharacters * pCharacter)
{
	if (pCharacter == nullptr)
		return E_FAIL;

	else if (pCharacter != nullptr)
		m_AkazaSceneList.push_back(pCharacter);

	int a = 0;	


	return S_OK;
}


void CAkazaScene::CAkazaScene_Tick(_float fTimeDelta)
{
	HandleInput();
	TickState(fTimeDelta);
}


void CAkazaScene::CAkazaScene_LateTick(_float fTimeDelta)
{
	LateTickState(fTimeDelta);
}

void CAkazaScene::HandleInput()
{
	CAkazaSceneState* pNewState = m_pAkazaSceneState->HandleInput(this);

	if (pNewState)
		m_pAkazaSceneState = m_pAkazaSceneState->ChangeState(this, m_pAkazaSceneState, pNewState);
}

void CAkazaScene::TickState(_float fTimeDelta)
{
	CAkazaSceneState* pNewState = m_pAkazaSceneState->Tick(this, fTimeDelta);

	if (pNewState)
		m_pAkazaSceneState = m_pAkazaSceneState->ChangeState(this, m_pAkazaSceneState, pNewState);
}

void CAkazaScene::LateTickState(_float fTimeDelta)
{
	CAkazaSceneState* pNewState = m_pAkazaSceneState->Late_Tick(this,fTimeDelta);

	if (pNewState)
		m_pAkazaSceneState = m_pAkazaSceneState->ChangeState(this, m_pAkazaSceneState, pNewState);
}

void CAkazaScene::Free()
{
}
