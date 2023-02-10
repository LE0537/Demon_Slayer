#include "stdafx.h"
#include "KyoujuroGuardHitState.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroGuardState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;


CGuardHitState::CGuardHitState(STATE_TYPE eState)
{
	m_eStateType = eState;
}

CKyoujuroState * CGuardHitState::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CKyoujuroState * CGuardHitState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Set_bGuard(true);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_HIT_0);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_HIT_1);
	//pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_GUARD_HIT_0, 0.2f);
	//pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_GUARD_HIT_1, 0.2f);

	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			if (pKyoujuro->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(TYPE_LOOP);
			else
			{
				pKyoujuro->Set_bGuard(false);
				return new CIdleState();
			}
			break;
		case Client::CKyoujuroState::TYPE_LOOP:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			if (pKyoujuro->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(TYPE_LOOP);
			else
			{
				pKyoujuro->Set_bGuard(false);
				return new CIdleState();
			}
			break;
		default:
			break;
		}
		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}

	return nullptr;

}

CKyoujuroState * CGuardHitState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	m_fTime += fTimeDelta;
	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			_float fHP = (_float)pKyoujuro->Get_PlayerInfo().iGuard / (_float)pKyoujuro->Get_PlayerInfo().iMaxGuard;
			if (fHP > 0.6f && pKyoujuro->Get_PlayerInfo().fGuardTime <= 0.f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD1, pKyoujuro);
			else if (fHP > 0.3f && pKyoujuro->Get_PlayerInfo().fGuardTime <= 0.f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD2, pKyoujuro);
			else if (fHP <= 0.3f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3, pKyoujuro);
			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		pKyoujuro->Get_Transform()->Go_Backward(fTimeDelta * 0.3f, pKyoujuro->Get_NavigationCom());
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		if (m_fTime < 0.3f)
		{
			pKyoujuro->Get_Transform()->Go_Backward(fTimeDelta * 0.6f, pKyoujuro->Get_NavigationCom());
		}
		break;
	default:
		break;
	}
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CGuardHitState::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_GUARD_HIT;


	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_GUARD_HIT_0);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_GUARD_HIT_0);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_HIT_0);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_GUARD_HIT_0, 0.01f);
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_GUARD_HIT_1);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_GUARD_HIT_1);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_HIT_1);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_GUARD_HIT_1, 0.01f);
		break;
	}



}

void CGuardHitState::Exit(CKyoujuro * pKyoujuro)
{
	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_ADV);
}

