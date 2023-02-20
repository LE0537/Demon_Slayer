#include "stdafx.h"
#include "..\Public\AngryState.h"
#include "GameInstance.h"
#include "RuiDadIdleState.h"
#include "Effect_Manager.h"
using namespace RuiDad;

CAngryState::CAngryState()
{
}

CRuiDadState * CAngryState::HandleInput(CRuiDad * pRuiDad)
{




	return nullptr;
}

CRuiDadState * CAngryState::Tick(CRuiDad * pRuiDad, _float fTimeDelta)
{
	if (pRuiDad->Get_Model()->Get_End(0))

	{
		pRuiDad->Get_Model()->Set_End(0);
		
		return new CIdleState();
	}

	return nullptr;
}

CRuiDadState * CAngryState::Late_Tick(CRuiDad * pRuiDad, _float fTimeDelta)
{
	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_ANGRY, pRuiDad);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CAngryState::Enter(CRuiDad * pRuiDad)
{
	//pRuiDad->Get_Model()->Reset_Anim(0);
	pRuiDad->Get_Model()->Set_CurrentAnimIndex(0);
	pRuiDad->Set_AnimIndex(static_cast<CRuiDad::ANIMID>(0));
	pRuiDad->Get_Model()->Set_Loop(0, false);
	pRuiDad->Get_Model()->Set_LinearTime(0, 0.01f);
}

void CAngryState::Exit(CRuiDad * pRuiDad)
{
}
