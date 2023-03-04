#include "stdafx.h"
#include "..\Public\TanjiroAkazaScene.h"
#include "GameInstance.h"


#include "Akaza.h"
#include "Kyoujuro.h"
#include "Layer.h"
using namespace Tanjiro;

CTanjiroAkazaScene::CTanjiroAkazaScene()
{
}

CTanjiroState * CTanjiroAkazaScene::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * CTanjiroAkazaScene::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	return nullptr;
}

CTanjiroState * CTanjiroAkazaScene::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CTanjiroAkazaScene::Enter(CTanjiro * pTanjiro)
{
	_vector vAkazaPosition = XMVectorSet(64.f, 0.f, 38.5f, 1.f);
	_vector vKyoujuroPosition = XMVectorSet(51.055f, 0.f, 48.931f, 1.f);

	_vector vDirection = vAkazaPosition - vKyoujuroPosition;
	_vector vDistance = XMVector3Length(vDirection);

	_vector vTanjiroPosition = XMVectorSet(51.055f, 0.f, 48.931f, 1.f);

	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTanjiroPosition);
	pTanjiro->Get_NavigationCom()->Find_CurrentCellIndex(vTanjiroPosition);
	pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_HIT_DMG_DOWN_0);
	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_HIT_DMG_DOWN_0);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_HIT_DMG_DOWN_0, 0.01f);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_HIT_DMG_DOWN_0, true);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	dynamic_cast<CKyoujuro*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"))->Get_LayerFront())->Set_SceneRender(false);
	dynamic_cast<CAkaza*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Akaza"))->Get_LayerFront())->Set_SceneRender(false);

	dynamic_cast<CAkaza*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Akaza"))->Get_LayerFront())->Play_AkazaScene();
}

void CTanjiroAkazaScene::Exit(CTanjiro * pTanjiro)
{
	pTanjiro->Set_AkazaScene();
}


