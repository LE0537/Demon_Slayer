#include "stdafx.h"
#include "AkazaJumpState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "AkazaJumpAttackState.h"
#include "AkazaJumpSkill_Common.h"
#include "Effect_Manager.h"
using namespace Akaza;

CJumpState::CJumpState(STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CAkazaState * CJumpState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pAkaza->Get_i1P())
	{
	case 1:
	if (pGameInstance->Key_Down(DIK_J))
		{
			return new CJumpAttackState(TYPE_START);
		}
	else if(pGameInstance->Key_Down(DIK_I))
	{
		if (200 <= pAkaza->Get_PlayerInfo().iSkBar)
		{
			pAkaza->Set_SkillBar(-200);
			return new CJumpSkill_CommmonState(STATE_TYPE::TYPE_START);
		}
	}
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_Z))
		{
			return new CJumpAttackState(TYPE_START);
		}
		else if (pGameInstance->Key_Down(DIK_X))
		{
			if (200 <= pAkaza->Get_PlayerInfo().iSkBar)
			{
				pAkaza->Set_SkillBar(-200);
				return new CJumpSkill_CommmonState(STATE_TYPE::TYPE_START);
			}
		}
		break;
	}


	return nullptr;
}

CAkazaState * CJumpState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // 애니메이션 총 시간
	_float fCurrentTime = 0.f; // 애니메이션 현재 시간

	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_JUMP_START);
	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_JUMP_LOOP);
	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_JUMP_END);



	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

			return new CJumpState(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

			//return new CJumpState(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState(STATE_ID::STATE_JUMP);
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	  
	return nullptr;
}

CAkazaState * CJumpState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
	{
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_UP, pAkaza);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}
	pAkaza->Get_Model()->Play_Animation(fTimeDelta,true);

	m_fJumpTime += 0.05f;

	if (m_eStateType != TYPE_END)
		Jump(pAkaza, fTimeDelta + m_fJumpTime);

	return nullptr;
}

void CJumpState::Enter(CAkaza* pAkaza)
{
	m_eStateId = CAkazaState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_START);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_START, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_START);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_LOOP);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_LOOP, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_LOOP);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_END);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_END, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_END);
		break;
	}
}

void CJumpState::Exit(CAkaza* pAkaza)
{
}

CAkazaState * CJumpState::Jump(CAkaza* pAkaza, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_eStateType = CAkazaState::TYPE_END;
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_END);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_END);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_END, 0.01f);
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_DOWN, pAkaza);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}

	pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}



