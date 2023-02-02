#include "stdafx.h"
#include "ShinobuSkill_Move.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Shinobu;


CSkill_MoveState::CSkill_MoveState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	m_eStateType = eType;
	

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;
}

CShinobuState * CSkill_MoveState::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();




	return nullptr;
}

CShinobuState * CSkill_MoveState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	
	

	if (m_eStateType == TYPE_LOOP)
	{
		Move(pShinobu, fTimeDelta);
	}



	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			//pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			return new CSkill_MoveState(STATE_TYPE::TYPE_LOOP);
			break;
		//case Client::CShinobuState::TYPE_LOOP:
		//	pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
		//	return new CSkill_MoveState(STATE_TYPE::TYPE_LOOP);
		//	break;
		case Client::CShinobuState::TYPE_END:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}

	return nullptr;
}

CShinobuState * CSkill_MoveState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	CCharacters* m_pTarget = pShinobu->Get_BattleTarget();


	pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	if (m_bNextAnim == true)
	{
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			//pShinobu->Get_Model()->Reset_Anim(pShinobu->Get_AnimIndex());
			return new CSkill_MoveState(STATE_TYPE::TYPE_END);
		}
	}

	return nullptr;
}

void CSkill_MoveState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_SKILL_MOVE;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_MOVE_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_MOVE_0, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_MOVE_0);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex(), false);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_MOVE_1);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_MOVE_1, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_MOVE_1);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex(), true);
		m_eStateId = STATE_ID::STATE_SKILL_MOVE_ALPHA;
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_MOVE_2);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_MOVE_2, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_MOVE_2);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex(),false);
		break;
	}

	

}

void CSkill_MoveState::Exit(CShinobu* pShinobu)
{
	m_pCollBox->Set_Dead();
}

void CSkill_MoveState::Move(CShinobu * pShinobu, _float fTimeDelta)
{
	_vector vMyPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));


	if (fDistance < 5.f)
	{
		//pShinobu->Get_Transform()->Go_Straight(fTimeDelta, pShinobu->Get_NavigationCom());
		m_bNextAnim = true;
	}
	else
	{
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.6f, pShinobu->Get_NavigationCom());
	}

}

