#include "stdafx.h"
#include "ShinobuJumpstate.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"
#include "ShinobuMoveState.h"
#include "ShinobuJumpAttackState.h"
#include "ShinobuJumpSkill_Common.h"
#include "ShinobuJumpMoveAttackState.h"
#include "Effect_Manager.h"
using namespace Shinobu;

CJumpstate::CJumpstate(STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CShinobuState * CJumpstate::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pShinobu->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_J))
		{
			return new CJumpAttackState(TYPE_START);
		}
		else if (pGameInstance->Key_Down(DIK_I))
		{
			if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
			{
				pShinobu->Set_SkillBar(-200);
				return new CJumpCommonSkillState(STATE_TYPE::TYPE_START);
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
			if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
			{
				pShinobu->Set_SkillBar(-200);
				return new CJumpCommonSkillState(STATE_TYPE::TYPE_START);
			}
		}
		break;
	}


	return nullptr;
}

CShinobuState * CJumpstate::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // 애니메이션 총 시간
	_float fCurrentTime = 0.f; // 애니메이션 현재 시간


	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_JUMP_START);
	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_JUMP_LOOP);
	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_JUMP_END);


	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());

			return new CJumpstate(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CShinobuState::TYPE_LOOP:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());

			//return new CJumpState(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CShinobuState::TYPE_END:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}


	return nullptr;
}

CShinobuState * CJumpstate::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (m_eStateType == CShinobuState::TYPE_START)
	{
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_UP, pShinobu);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}
	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	m_fJumpTime += 0.035f;

	if (m_eStateType != TYPE_END)
		Jump(pShinobu, fTimeDelta + m_fJumpTime);

	return nullptr;
}

void CJumpstate::Enter(CShinobu* pShinobu)
{
	m_eStateId = CShinobuState::STATE_JUMP;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_JUMP_START);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_JUMP_START, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_START);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_JUMP_LOOP);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_JUMP_LOOP, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_LOOP);
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_JUMP_END);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_JUMP_END, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_END);
		break;
	}


}

void CJumpstate::Exit(CShinobu* pShinobu)
{

}


CShinobuState* CJumpstate::Jump(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pShinobu->Get_NavigationHeight().y;
	pShinobu->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 30.f;


	_vector      vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		pShinobu->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_eStateType = CShinobuState::TYPE_END;
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_JUMP_END);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_JUMP_END, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_END);
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_DOWN, pShinobu);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}




	pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}


