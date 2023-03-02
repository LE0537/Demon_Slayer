#include "stdafx.h"
#include "EnmuGuardHitState.h"
#include "EnmuIdleState.h"
#include "EnmuGuardState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "SoundMgr.h"

using namespace Enmu;


CGuardHitState::CGuardHitState(STATE_TYPE eState)
{
	m_eStateType = eState;
}

CEnmuState * CGuardHitState::HandleInput(CEnmu* pEnmu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CEnmuState * CGuardHitState::Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	pEnmu->Set_bGuard(true);
	pEnmu->Get_Model()->Set_Loop(CEnmu::ANIM_GUARDHIT_0);

	//pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_GUARD_HIT_0, 0.2f);
	//pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_GUARD_HIT_1, 0.2f);

	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			if (pEnmu->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(TYPE_LOOP);
			else
			{
				pEnmu->Set_bGuard(false);
				return new CIdleState();
			}
			break;
		case Client::CEnmuState::TYPE_LOOP:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			if (pEnmu->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(TYPE_LOOP);
			else
			{
				pEnmu->Set_bGuard(false);
				return new CIdleState();
			}
			break;
		default:
			break;
		}
		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}

	return nullptr;






	return nullptr;
}

CEnmuState * CGuardHitState::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	m_fTime += fTimeDelta;
	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			_float fHP = (_float)pEnmu->Get_PlayerInfo().iGuard / (_float)pEnmu->Get_PlayerInfo().iMaxGuard;
			if (fHP > 0.6f && pEnmu->Get_PlayerInfo().fGuardTime <= 0.f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD1, pEnmu);
			else if (fHP > 0.3f && pEnmu->Get_PlayerInfo().fGuardTime <= 0.f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD2, pEnmu);
			else if (fHP <= 0.3f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3, pEnmu);
			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.3f, pEnmu->Get_NavigationCom());
		break;
	case Client::CEnmuState::TYPE_LOOP:
		if (m_fTime < 0.3f)
		{
			pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.6f, pEnmu->Get_NavigationCom());
		}
		break;
	default:
		break;
	}
	pEnmu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CGuardHitState::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_GUARD_HIT;


	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_GUARDHIT_0);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_GUARDHIT_0);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIM_GUARDHIT_0);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_GUARDHIT_0, 0.01f);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Enmu_SE_GaurdHit.wav"), g_fEffect);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_GUARDHIT_0);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_GUARDHIT_0);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIM_GUARDHIT_0);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_GUARDHIT_0, 0.01f);
		break;
	}



}

void CGuardHitState::Exit(CEnmu* pEnmu)
{
	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_ADV);
}

