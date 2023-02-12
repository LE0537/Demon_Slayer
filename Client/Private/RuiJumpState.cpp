#include "stdafx.h"
#include "RuiJumpState.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "RuiJumpAttackState.h"
#include "RuiJumpSkill_Common.h"
#include "RuiJumpMoveAttackState.h"
#include "Effect_Manager.h"
using namespace Rui;

CJumpState::CJumpState(STATE_TYPE eType, _float fPositionY, _float fJumpTime, _bool bAiMoveAttack)
{
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
	m_bMoveAtkJump = bAiMoveAttack;
}

CRuiState * CJumpState::HandleInput(CRui * pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pRui->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_I))
		{
			if (200 <= pRui->Get_PlayerInfo().iSkBar)
			{
				pRui->Set_SkillBar(-200);
				return new CJumpSkill_CommonState(TYPE_START);
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
			if (200 <= pRui->Get_PlayerInfo().iSkBar)
			{
				pRui->Set_SkillBar(-200);
				return new CJumpSkill_CommonState(TYPE_START);
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

CRuiState * CJumpState::Tick(CRui * pRui, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // 애니메이션 총 시간
	_float fCurrentTime = 0.f; // 애니메이션 현재 시간

	pRui->Get_Model()->Set_Loop(CRui::ANIM_JUMP_START);
	pRui->Get_Model()->Set_Loop(CRui::ANIM_JUMP_LOOP);
	pRui->Get_Model()->Set_Loop(CRui::ANIM_JUMP_END);


	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

			return new CJumpState(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

			//return new CJumpState(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState(STATE_ID::STATE_JUMP);
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}


	return nullptr;
}

CRuiState * CJumpState::Late_Tick(CRui * pRui, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
	{
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_UP, pRui);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}
	pRui->Get_Model()->Play_Animation(fTimeDelta);

	m_fJumpTime += 0.035f;

	if (m_eStateType != TYPE_END)
		Jump(pRui, fTimeDelta + m_fJumpTime);

	if (m_fJumpTime >= 0.45f && pRui->Get_IsAIMode() && m_bMoveAtkJump == true)
		return new CJumpMoveAttackState(TYPE_START);


	return nullptr;
}

void CJumpState::Enter(CRui * pRui)
{
	m_eStateId = CRuiState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMP_START);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_JUMP_START, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_START);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMP_LOOP);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_JUMP_LOOP, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_LOOP);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMP_END);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_JUMP_END, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_END);
		break;
	}
}

void CJumpState::Exit(CRui * pRui)
{
}

CRuiState * CJumpState::Jump(CRui * pRui, _float fTimeDelta)
{
	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pRui->Get_NavigationHeight().y;
	pRui->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 30.f;


	_vector      vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		pRui->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_eStateType = CRuiState::TYPE_END;
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMP_END);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_END);
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_DOWN, pRui);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}

	pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}



