#include "stdafx.h"
#include "..\Public\SceneState.h"
#include "AkazaScene.h"
#include "GameInstance.h"
#include "Model.h"

using namespace AkazaScene;


CSceneState::CSceneState(STATE_TYPE eType)
{
}

CAkazaSceneState * CSceneState::HandleInput(CAkazaScene * pAkazaScene)
{
	return nullptr;
}

CAkazaSceneState * CSceneState::Tick(CAkazaScene * pAkazaScene, _float fTimeDelta)
{
	return nullptr;
}

CAkazaSceneState * CSceneState::Late_Tick(CAkazaScene * pAkazaScene, _float fTimeDelta)
{
	for (_uint i = 0; i < pAkazaScene->Get_AkazaSceneList().size(); ++i)
	{
		pAkazaScene->Get_AkazaSceneList()[i]->Get_Model()->Play_Animation(fTimeDelta);
	}


	return nullptr;
}

void CSceneState::Enter(CAkazaScene * pAkazaScene)
{
}

void CSceneState::Exit(CAkazaScene * pAkazaScene)
{
}
