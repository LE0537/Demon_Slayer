#include "stdafx.h"
#include "TanjiroJumpstate.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"
#include "TanjiroMoveState.h"
#include "TanjiroJumpAttackState.h"
#include "TanjiroJumpSkillCommonState.h"
#include "TanjiroKaguraJumpSkill_Common.h"
#include "Effect_Manager.h"
using namespace Tanjiro;

CJumpstate::CJumpstate(STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CTanjiroState * CJumpstate::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_I))
		{
			if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
			{
				if (pTanjiro->Get_KaguraMode() == true)
				{
					pTanjiro->Set_SkillBar(-200);
					return new CKaguraJumpSkill_CommonState();
				}
				else
				{
					pTanjiro->Set_SkillBar(-200);
					return new CJumpSkillCommonState(TYPE_START);
				}
			}
			
		}
		else if (pGameInstance->Key_Down(DIK_J))
		{
			return new CJumpAttackState(TYPE_START);
		}
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_X))
		{
			if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
			{
				if (pTanjiro->Get_KaguraMode() == true)
				{
					pTanjiro->Set_SkillBar(-200);
					return new CKaguraJumpSkill_CommonState();
				}
				else
				{
					pTanjiro->Set_SkillBar(-200);
					return new CJumpSkillCommonState(TYPE_START);
				}
			}
		}
		else if (pGameInstance->Key_Down(DIK_Z))
		{
			return new CJumpAttackState(TYPE_START);
		}
		break;
	default:
		break;
	}

	return nullptr;
}

CTanjiroState * CJumpstate::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // 애니메이션 총 시간
	_float fCurrentTime = 0.f; // 애니메이션 현재 시간


	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_START);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_START);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_END);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END);


	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CJumpstate(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CJumpstate(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			//return new CJumpstate(STATE_TYPE::TYPE_DEFAULT, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState(STATE_ID::STATE_JUMP);
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}


	return nullptr;
}

CTanjiroState * CJumpstate::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	if (m_eStateType == TYPE_START)
	{
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_UP, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	//	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 3.f);
	}
	//else if (m_eStateType == TYPE_LOOP)
	//	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
	//else if (m_eStateType == TYPE_DEFAULT)
	//	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
	//else
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	m_fJumpTime += 0.035f;

	if (m_eStateType != TYPE_DEFAULT)
		Jump(pTanjiro, fTimeDelta + m_fJumpTime);

	return nullptr;
}

void CJumpstate::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = CTanjiroState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_START);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_START, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_START);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_START);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_LOOP_START, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_START);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_LOOP_END, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END, true);
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END);
		break;
	}


}

void CJumpstate::Exit(CTanjiro * pTanjiro)
{
	pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_JUMP_START);
	pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_JUMP_LOOP_START);
	pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_JUMP_LOOP_END);
	pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_JUMP_END);
}


CTanjiroState* CJumpstate::Jump(CTanjiro* pTanjiro, _float fTimeDelta)
{
	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pTanjiro->Get_NavigationHeight().y;
	pTanjiro->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 30.f;



	_vector      vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_eStateType = CTanjiroState::TYPE_DEFAULT;
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_END, 0.01f);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END);
		pTanjiro->Get_Transform()->Set_Jump(false);

		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_DOWN, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}

	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}


