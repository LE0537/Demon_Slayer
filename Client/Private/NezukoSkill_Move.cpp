#include "stdafx.h"
#include "NezukoSkill_Move.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Nezuko;


CSkill_MoveState::CSkill_MoveState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);

}

CNezukoState * CSkill_MoveState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	return nullptr;
}

CNezukoState * CSkill_MoveState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();

	pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
		return new CIdleState();
	}



	return nullptr;
}

CNezukoState * CSkill_MoveState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	
	pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_MoveState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_SKILL_MOVE;

	pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_MOVE);
	pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_MOVE, 0.2f);
	pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_MOVE);


}

void CSkill_MoveState::Exit(CNezuko* pNezuko)
{
	m_pCollBox->Set_Dead();
}

