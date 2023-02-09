#include "stdafx.h"
#include "RuiDadGuardHitState.h"
#include "RuiDadIdleState.h"
#include "RuiDadGuardState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace RuiDad;


CGuardHitState::CGuardHitState(STATE_TYPE eState)
{
	m_eStateType = eState;
}

CRuiDadState * CGuardHitState::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CRuiDadState * CGuardHitState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	pRuiDad->Set_bGuard(true);
	pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_GUARDHIT_0);

	//pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_GUARD_HIT_0, 0.2f);
	//pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_GUARD_HIT_1, 0.2f);

	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiDadState::TYPE_START:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			if (pRuiDad->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(TYPE_LOOP);
			else
			{
				pRuiDad->Set_bGuard(false);
				return new CIdleState();
			}
			break;
		case Client::CRuiDadState::TYPE_LOOP:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			if (pRuiDad->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(TYPE_LOOP);
			else
			{
				pRuiDad->Set_bGuard(false);
				return new CIdleState();
			}
			break;
		default:
			break;
		}
		pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
	}

	return nullptr;






	return nullptr;
}

CRuiDadState * CGuardHitState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	m_fTime += fTimeDelta;
	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			_float fHP = (_float)pRuiDad->Get_PlayerInfo().iGuard / (_float)pRuiDad->Get_PlayerInfo().iMaxGuard;
			if (fHP > 0.6f && pRuiDad->Get_PlayerInfo().fGuardTime <= 0.f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD1, pRuiDad);
			else if (fHP > 0.3f && pRuiDad->Get_PlayerInfo().fGuardTime <= 0.f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD2, pRuiDad);
			else if (fHP <= 0.3f)
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3, pRuiDad);
			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.3f, pRuiDad->Get_NavigationCom());
		break;
	case Client::CRuiDadState::TYPE_LOOP:
		if (m_fTime < 0.3f)
		{
			pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.6f, pRuiDad->Get_NavigationCom());
		}
		break;
	default:
		break;
	}
	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CGuardHitState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_GUARD_HIT;


	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_GUARDHIT_0);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_GUARDHIT_0);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_GUARDHIT_0);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_GUARDHIT_0, 0.01f);
		break;
	case Client::CRuiDadState::TYPE_LOOP:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_GUARDHIT_0);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_GUARDHIT_0);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_GUARDHIT_0);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_GUARDHIT_0, 0.01f);
		break;
	}



}

void CGuardHitState::Exit(CRuiDad* pRuiDad)
{
	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_ADV);
}

