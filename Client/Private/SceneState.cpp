#include "stdafx.h"
#include "..\Public\SceneState.h"
#include "AkazaScene.h"
#include "GameInstance.h"
#include "Model.h"

#include "Tanjiro.h"
#include "Kyoujuro.h"
#include "Akaza.h"
#include "Level_GamePlay.h"

using namespace AkazaScene;


CSceneState::CSceneState(SCENE eScene)
{
	m_eScene = eScene;
}

CAkazaSceneState * CSceneState::HandleInput(CAkazaScene * pAkazaScene)
{
	return nullptr;
}

CAkazaSceneState * CSceneState::Tick(CAkazaScene * pAkazaScene, _float fTimeDelta)
{

	m_fSceneTimer += fTimeDelta;

	switch (m_eScene)
	{
	case Client::AkazaScene::CSceneState::SCENE_0:
		if (m_fSceneTimer >= 3.f)
			return new CSceneState(SCENE_1);
		break;
	case Client::AkazaScene::CSceneState::SCENE_1:
		Fall_Height(pAkazaScene, fTimeDelta);

		if (m_bNextAnim == true)
			return new CSceneState(SCENE_2);
		break;
	case Client::AkazaScene::CSceneState::SCENE_2:
	
		if (pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_AnimIndex() == CAkaza::ANIM_IDLE)
				m_fSceneTimer += fTimeDelta;

		if (pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Get_End(CAkaza::ANIM_JUMP_END))
		{
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_End(CAkaza::ANIM_JUMP_END);

			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_IDLE);
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_LinearTime(CAkaza::ANIM_IDLE, 0.01f);
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Set_AnimIndex(CAkaza::ANIM_IDLE);
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_Loop(CAkaza::ANIM_IDLE, true);

			if(m_fSceneTimer >= 1.5f)
				return new CSceneState(SCENE_3);
		}
	case Client::AkazaScene::CSceneState::SCENE_3:
		if (pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Get_Model()->Get_CurrentTime_Index(CKyoujuro::ANIM_BATTLE_START) >= 0.6f)
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Set_SceneRender(true);


		if (pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Get_End(CAkaza::ANIM_BATTLE_START) &&
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Get_Model()->Get_End(CKyoujuro::ANIM_BATTLE_START))
		{
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Get_Model()->Set_End(CKyoujuro::ANIM_BATTLE_START);
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_End(CAkaza::ANIM_BATTLE_START);

			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_IDLE);
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_LinearTime(CAkaza::ANIM_IDLE, 0.01f);
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Set_AnimIndex(CAkaza::ANIM_IDLE);
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_Loop(CAkaza::ANIM_IDLE, true);

			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_IDLE);
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_IDLE, 0.01f);
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Set_AnimIndex(CKyoujuro::ANIM_IDLE);
			pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Get_Model()->Set_Loop(CKyoujuro::ANIM_IDLE, true);

			CAkazaScene::Get_Instance()->Set_Start(true);
		}
		break;
	case Client::AkazaScene::CSceneState::SCENE_END:
		break;
	default:
		break;
	}

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
	switch (m_eScene)
	{
	case Client::AkazaScene::CSceneState::SCENE_0:
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, 20.f, 38.5f,1.f));
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.459f, 0.f, 53.669f, 1.f));

		_vector vAkazaPosition = XMVectorSet(64.f, 0.f, 38.5f, 1.f);
		_vector vKyoujuroPosition = XMVectorSet(50.459f, 0.f, 53.669f, 1.f);

		_vector vDirection = vAkazaPosition - vKyoujuroPosition;
		_vector vDistance = XMVector3Length(vDirection);

		_vector vTanjiroPosition = vAkazaPosition + (vDistance * 0.5f);

		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_TANJIRO]->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTanjiroPosition);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_TANJIRO]->Get_NavigationCom()->Find_CurrentCellIndex(vTanjiroPosition);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_TANJIRO]->Get_Model()->Reset_Anim(CTanjiro::ANIM_HIT_DMG_DOWN_0);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_TANJIRO]->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_HIT_DMG_DOWN_0);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_TANJIRO]->Get_Model()->Set_LinearTime(CTanjiro::ANIM_HIT_DMG_DOWN_0, 0.01f);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_TANJIRO]->Get_Model()->Set_Loop(CTanjiro::ANIM_HIT_DMG_DOWN_0, true);


		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Set_SceneRender(false);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Set_SceneRender(false);



		break;
	case Client::AkazaScene::CSceneState::SCENE_1:
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_LOOP_END);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_LOOP_END, 0.01f);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_Loop(CAkaza::ANIM_JUMP_LOOP_END, true);

		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Set_SceneRender(true);

		break;
	case Client::AkazaScene::CSceneState::SCENE_2:
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_END);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_END, 0.01f);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_Loop(CAkaza::ANIM_JUMP_END);
		break;
	case Client::AkazaScene::CSceneState::SCENE_3:
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_BATTLE_START);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_LinearTime(CAkaza::ANIM_BATTLE_START, 0.2f);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Model()->Set_Loop(CAkaza::ANIM_BATTLE_START);

		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_BATTLE_START);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_BATTLE_START, 0.2f);
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_KYOUJURO]->Get_Model()->Set_Loop(CKyoujuro::ANIM_BATTLE_START);
		break;
	case Client::AkazaScene::CSceneState::SCENE_END:
		break;
	default:
		break;
	}


}

void CSceneState::Exit(CAkazaScene * pAkazaScene)
{

}

void CSceneState::Fall_Height(CAkazaScene* pAkazaScene, _float fTimeDelta)
{
	static _float fGravity = -200.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	m_fOriginPosY = pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_NavigationHeight().y;

	vPosition.x = XMVectorGetX(pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
		pAkazaScene->Get_AkazaSceneList()[CAkazaScene::SCENE_AKAZA]->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
}
