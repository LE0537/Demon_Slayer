#include "stdafx.h"
#include "KyoujuroJumpState.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroJumpSkill_Common.h"


using namespace Kyoujuro;

CJumpState::CJumpState(STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CKyoujuroState * CJumpState::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pKyoujuro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_I))
		{
			if (200 <= pKyoujuro->Get_PlayerInfo().iSkBar)
			{
				pKyoujuro->Set_SkillBar(-200);
				return new CJumpSkill_CommonState(m_fCurrentPosY);
			}
		}
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_X))
		{
			if (200 <= pKyoujuro->Get_PlayerInfo().iSkBar)
			{
				pKyoujuro->Set_SkillBar(-200);
				return new CJumpSkill_CommonState(m_fCurrentPosY);
			}
		}
		break;
	}
	

	return nullptr;
}

CKyoujuroState * CJumpState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // 애니메이션 총 시간
	_float fCurrentTime = 0.f; // 애니메이션 현재 시간

	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_START);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_LOOP_START);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_LOOP_END);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_END);



	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());

			return new CJumpState(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CKyoujuroState::TYPE_LOOP:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());

			return new CJumpState(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CKyoujuroState::TYPE_END:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());

			//return new CJumpstate(STATE_TYPE::TYPE_DEFAULT, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CKyoujuroState::TYPE_DEFAULT:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState(STATE_ID::STATE_JUMP);
			break;
		}
		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}

	  
	return nullptr;
}

CKyoujuroState * CJumpState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	if (m_eStateType == TYPE_START)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);
	else if (m_eStateType == TYPE_LOOP)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else if (m_eStateType == TYPE_DEFAULT)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	m_fJumpTime += 0.05f;

	if (m_eStateType != TYPE_DEFAULT)
		Jump(pKyoujuro, fTimeDelta + m_fJumpTime);

	return nullptr;
}

void CJumpState::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = CKyoujuroState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_START);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_JUMP_START, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_START);
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_LOOP_START);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_JUMP_LOOP_START, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_LOOP_START);
		break;
	case Client::CKyoujuroState::TYPE_END:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_LOOP_END);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_JUMP_LOOP_END, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_LOOP_END);
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_END);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_END);
		break;
	}
}

void CJumpState::Exit(CKyoujuro * pKyoujuro)
{
}

CKyoujuroState * CJumpState::Jump(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_eStateType = CKyoujuroState::TYPE_DEFAULT;
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_END);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_END);
	}

	pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}



