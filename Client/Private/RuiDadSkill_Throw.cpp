#include "stdafx.h"
#include "RuiDadSkill_Throw.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Layer.h"
#include "RuiDadBigStone.h"

using namespace RuiDad;


CSkill_ThrowState::CSkill_ThrowState(STATE_TYPE eType)
{
	m_eStateType = eType;

}

CRuiDadState * CSkill_ThrowState::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	


	return nullptr;
}

CRuiDadState * CSkill_ThrowState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	//pRuiDad->Get_Model()->Set_Loop(pRuiDad->Get_AnimIndex());

	pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiDadState::TYPE_START:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CRuiDadState::TYPE_LOOP:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			//return new CSkill_JumpDropState(CRuiDadState::TYPE_END);
			break;
		case Client::CRuiDadState::TYPE_END:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
	}

	return nullptr;
}

CRuiDadState * CSkill_ThrowState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	if (m_iHit == 0)
	{
		CRuiDadBigStone::RUIDADBIGSTONE	tInfo;
		tInfo.pPlayer = pRuiDad;
		tInfo.pTarget = pRuiDad->Get_BattleTarget();
		tInfo.iIndex = 0;
		CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiDadBigStone"), LEVEL_GAMEPLAY, TEXT("Layer_CollBox"), &tInfo)))
			return nullptr;

		RELEASE_INSTANCE(CGameInstance);
		++m_iHit;
	}
	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_ThrowState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL0);
	pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL0, 0.01f);
	pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL0);
	pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL0);

}

void CSkill_ThrowState::Exit(CRuiDad* pRuiDad)
{

}

