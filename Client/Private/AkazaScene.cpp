#include "stdafx.h"
#include "..\Public\AkazaScene.h"
#include "GameInstance.h"
#include "SceneState.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CAkazaScene)

using namespace AkazaScene;

CAkazaScene::CAkazaScene()
{
}

HRESULT CAkazaScene::Initialize()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	CCharacters* pTanjiro = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, L"Layer_Tanjiro")->Get_LayerFront();
	CCharacters* pKyoujuro = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, L"Layer_Kyoujuro")->Get_LayerFront();
	CCharacters* pAkaza = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, L"Layer_Akaza")->Get_LayerFront();

	Add_Character(pTanjiro);
	Add_Character(pKyoujuro);
	Add_Character(pAkaza);

	CAkazaSceneState* pState = new CSceneState(CSceneState::TYPE_START);
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
