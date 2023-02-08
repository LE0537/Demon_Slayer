#include "stdafx.h"
#include "AkazaGuardHitState.h"
#include "AkazaIdleState.h"
#include "AkazaGuardState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Akaza;


CGuardHitState::CGuardHitState(STATE_TYPE eState)
{
	m_eStateType = eState;
}

CAkazaState * CGuardHitState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CAkazaState * CGuardHitState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Set_bGuard(true);
	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_GUARD_HIT_0);
	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_GUARD_HIT_1);
	//pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_GUARD_HIT_0, 0.2f);
	//pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_GUARD_HIT_1, 0.2f);

	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CGuardState(TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CGuardState(TYPE_LOOP);
			break;
		default:
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	return nullptr;


}

CAkazaState * CGuardHitState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	m_fTime += fTimeDelta;
	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD1, pAkaza);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		pAkaza->Get_Transform()->Go_Backward(fTimeDelta * 0.3f, pAkaza->Get_NavigationCom());
		break;
	case Client::CAkazaState::TYPE_LOOP:
		if (m_fTime < 0.3f)
		{
			pAkaza->Get_Transform()->Go_Backward(fTimeDelta * 0.6f, pAkaza->Get_NavigationCom());
		}
		break;
	default:
		break;
	}
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CGuardHitState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_GUARD_HIT;


	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_GUARD_HIT_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_GUARD_HIT_0);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_GUARD_HIT_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_GUARD_HIT_0, 0.01f);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_GUARD_HIT_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_GUARD_HIT_1);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_GUARD_HIT_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_GUARD_HIT_1, 0.01f);
		break;
	}



}

void CGuardHitState::Exit(CAkaza* pAkaza)
{

	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_ADV);
}

