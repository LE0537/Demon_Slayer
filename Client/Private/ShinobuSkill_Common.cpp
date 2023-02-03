#include "stdafx.h"
#include "ShinobuSkill_Common.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Shinobu;


CSkill_CommonState::CSkill_CommonState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;
}

CShinobuState * CSkill_CommonState::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();




	return nullptr;
}

CShinobuState * CSkill_CommonState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
		return new CIdleState();

	}

	if (pShinobu->Get_Model()->Get_CurrentTime() <= 31.f)
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());

	if(pShinobu->Get_Model()->Get_CurrentTime() >= 40.f && pShinobu->Get_Model()->Get_CurrentTime() <= 50.f)
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.1f , pShinobu->Get_NavigationCom());


	return nullptr;
}

CShinobuState * CSkill_CommonState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	CCharacters* m_pTarget = pShinobu->Get_BattleTarget();

	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_CommonState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_COMMON_2);
	pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_COMMON_2, 0.01f);
	pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_COMMON_2);
	pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
}

void CSkill_CommonState::Exit(CShinobu* pShinobu)
{
	m_pCollBox->Set_Dead();
}

