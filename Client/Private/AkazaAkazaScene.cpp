#include "stdafx.h"
#include "..\Public\AkazaAkazaScene.h"
#include "GameInstance.h"
#include "Kyoujuro.h"
#include "Layer.h"
#include "AkazaIdleState.h"
#include "Effect_Manager.h"
using namespace Akaza;

CAkazaAkazaScene::CAkazaAkazaScene(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CAkazaState * CAkazaAkazaScene::HandleInput(CAkaza * pAkaza)
{
	return nullptr;
}

CAkazaState * CAkazaAkazaScene::Tick(CAkaza * pAkaza, _float fTimeDelta)
{
	m_fSceneTimer += fTimeDelta;
	pAkaza->Get_Transform()->Set_PlayerLookAt(XMVectorSet(50.459f, 0.f, 53.669f, 1.f));

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		if (m_fSceneTimer >= 7.5f)
			return new CAkazaAkazaScene(TYPE_LOOP);
		break;
	case Client::CAkazaState::TYPE_LOOP:
			Fall_Height(pAkaza, fTimeDelta);

			if(m_bNextAnim == true)
				return new CAkazaAkazaScene(TYPE_END);
		break;
	case Client::CAkazaState::TYPE_END:
		if (pAkaza->Get_Model()->Get_End(CAkaza::ANIM_JUMP_END))
		{
			pAkaza->Get_Model()->Set_End(CAkaza::ANIM_JUMP_END);


			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_IDLE);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_IDLE, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_IDLE);
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_IDLE, true);

			m_bNextAnim = true;
		}


		if (m_bNextAnim == true)
			m_fSceneTimer += fTimeDelta;

		if (m_fSceneTimer >= 5.f)
			return new CAkazaAkazaScene(TYPE_DEFAULT);
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		if (pAkaza->Get_Model()->Get_End(CAkaza::ANIM_BATTLE_START))
		{
			pAkaza->Get_Model()->Set_End(CAkaza::ANIM_BATTLE_START);
			return new CIdleState();
		}

		break;
	case Client::CAkazaState::TYPE_CHANGE:
		break;
	default:
		break;
	}

	return nullptr;
}

CAkazaState * CAkazaAkazaScene::Late_Tick(CAkaza * pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CAkazaAkazaScene::Enter(CAkaza * pAkaza)
{
	
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, 20.f, 38.5f, 1.f));
		pAkaza->Get_Transform()->Set_PlayerLookAt(XMVectorSet(50.459f, 0.f, 53.669f, 1.f));
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_LOOP_END);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_LOOP_END, 0.01f);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_JUMP_LOOP_END, true);

		pAkaza->Set_SceneRender(true);
		break;
	case Client::CAkazaState::TYPE_END:

		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_END);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_END, 0.01f);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_JUMP_END);
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		
		dynamic_cast<CKyoujuro*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"))->Get_LayerFront())->Play_AkazaScene();
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_BATTLE_START);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_BATTLE_START, 0.2f);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_BATTLE_START);
		break;
	case Client::CAkazaState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CAkazaAkazaScene::Exit(CAkaza * pAkaza)
{
}

void CAkazaAkazaScene::Fall_Height(CAkaza * pAkaza, _float fTimeDelta)
{


	static _float fGravity = -200.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	m_fOriginPosY = pAkaza->Get_NavigationHeight().y;

	vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		
		CEffect_Manager*  pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_END_GROUND_NOTIME, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bNextAnim = true;
	}
	else
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

}
