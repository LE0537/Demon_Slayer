#include "stdafx.h"
#include "AkazaAdvSkill_Move.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Akaza;


CAdvSkill_MoveState::CAdvSkill_MoveState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;
}

CAkazaState * CAdvSkill_MoveState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	return nullptr;
}

CAkazaState * CAdvSkill_MoveState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CAdvSkill_MoveState(CAkazaState::TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}


	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_MOVE_START);
		Move(pAkaza, fTimeDelta);

		if(m_bNextAnim)
			return new CAdvSkill_MoveState(CAkazaState::TYPE_END);
		break;

	
	}

	return nullptr;
}

CAkazaState * CAdvSkill_MoveState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	
	if(m_eStateType == TYPE_LOOP)
		pAkaza->Get_Model()->Play_Animation2(fTimeDelta * 2.f);
	else
		pAkaza->Get_Model()->Play_Animation(fTimeDelta );

	return nullptr;
}

void CAdvSkill_MoveState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_SKILL_PUNCH;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_ADVSKILL_MOVE_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_ADVSKILL_MOVE_0, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_ADVSKILL_MOVE_0);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_ADVSKILL_MOVE_0, false);

		_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
		m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_ADVSKILL_MOVE_1);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_ADVSKILL_MOVE_1);
		//pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_ADVSKILL_MOVE_1, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_ADVSKILL_MOVE_1);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_ADVSKILL_MOVE_1, true);
		m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 1.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 1.f;
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_ADVSKILL_MOVE_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_ADVSKILL_MOVE_2, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_ADVSKILL_MOVE_2);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_ADVSKILL_MOVE_2, false);
		break;
	}



}

void CAdvSkill_MoveState::Exit(CAkaza* pAkaza)
{
	m_pCollBox->Set_Dead();
}

void CAdvSkill_MoveState::Move(CAkaza * pAkaza, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);



	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);


	if (fDistance <= 3.f)
	{
		m_bNextAnim = true;
	}
	else
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


}

