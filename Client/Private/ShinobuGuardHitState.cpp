#include "stdafx.h"
#include "ShinobuGuardHitState.h"
#include "ShinobuIdleState.h"
#include "ShinobuGuardState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Shinobu;


CGuardHitState::CGuardHitState(STATE_TYPE eState)
{
	m_eStateType = eState;
}

CShinobuState * CGuardHitState::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CShinobuState * CGuardHitState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Set_bGuard(true);
	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_GUARD_HIT_0);
	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_GUARD_HIT_1);
	//pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_GUARD_HIT_0, 0.2f);
	//pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_GUARD_HIT_1, 0.2f);

	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			if (pShinobu->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(TYPE_LOOP);
			else
			{
				pShinobu->Set_bGuard(false);
				return new CIdleState();
			}
			break;
		case Client::CShinobuState::TYPE_LOOP:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			if (pShinobu->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(TYPE_LOOP);
			else
			{
				pShinobu->Set_bGuard(false);
				return new CIdleState();
			}
			break;
		default:
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}

	return nullptr;

}

CShinobuState * CGuardHitState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	m_fTime += fTimeDelta;
	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			_float fHP = (_float)pShinobu->Get_PlayerInfo().iGuard / (_float)pShinobu->Get_PlayerInfo().iMaxGuard;
			if (fHP > 0.6f && pShinobu->Get_PlayerInfo().fGuardTime <= 0.f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD1, pShinobu);
			else if (fHP > 0.3f && pShinobu->Get_PlayerInfo().fGuardTime <= 0.f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD2, pShinobu);
			else if (fHP <= 0.3f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3, pShinobu);
			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.3f, pShinobu->Get_NavigationCom());
		break;
	case Client::CShinobuState::TYPE_LOOP:
		if (m_fTime < 0.3f)
		{
			pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.6f, pShinobu->Get_NavigationCom());
		}
		break;
	default:
		break;
	}
	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CGuardHitState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_GUARD_HIT;


	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_GUARD_HIT_0);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_GUARD_HIT_0);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_GUARD_HIT_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_GUARD_HIT_0, 0.01f);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_GuardHit.wav"), g_fEffect);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_GUARD_HIT_1);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_GUARD_HIT_1);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_GUARD_HIT_1);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_GUARD_HIT_1, 0.01f);
		break;
	}



}

void CGuardHitState::Exit(CShinobu* pShinobu)
{
	//pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIMID::ANIM_GUARD_ADV);
}

