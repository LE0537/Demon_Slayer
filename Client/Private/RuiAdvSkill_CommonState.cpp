#include "stdafx.h"
#include "RuiAdvSkill_CommonState.h"
#include "GameInstance.h"
#include "RuiIdleState.h"
#include "Effect_Manager.h"
using namespace Rui;

CAdvSkill_CommonState::CAdvSkill_CommonState()
{

}

CRuiState * CAdvSkill_CommonState::HandleInput(CRui * pRui)
{
	return nullptr;
}

CRuiState * CAdvSkill_CommonState::Tick(CRui * pRui, _float fTimeDelta)
{
	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

		return new CIdleState();
	}



	return nullptr;
}

CRuiState * CAdvSkill_CommonState::Late_Tick(CRui * pRui, _float fTimeDelta)
{
	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pRui->Get_Transform()->LookAt(vLooAt);

		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		m_fMove += fTimeDelta;

		CRuiSphere::RUISPHEREINFO	tInfo;
		tInfo.pPlayer = pRui;
		tInfo.pTarget = m_pTarget;

		if (m_iHit < 1)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			tInfo.iIndex = 1;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiSphere"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;


			RELEASE_INSTANCE(CGameInstance);
			m_fMove = 0.f;
			++m_iHit;
		}

		RELEASE_INSTANCE(CGameInstance);
	


	pRui->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CAdvSkill_CommonState::Enter(CRui * pRui)
{
	m_eStateId = STATE_ID::STATE_ADVSKILL_COMMON;

	pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_ADVSKILL_COMMON);
	pRui->Set_AnimIndex(CRui::ANIM_ADVSKILL_COMMON);

	pRui->Get_Model()->Set_Loop(CRui::ANIM_ADVSKILL_COMMON);
	pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
}

void CAdvSkill_CommonState::Exit(CRui * pRui)
{

}


