#include "stdafx.h"
#include "..\Public\ShinobuSplSkrStartState.h"
#include "GameInstance.h"
#include "Shinobu_CinemaState.h"
#include "ShinobuIdleState.h"

using namespace Shinobu;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CShinobuState * CSplSkrStartState::HandleInput(CShinobu* pShinobu)
{
	return nullptr;
}

CShinobuState * CSplSkrStartState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_LOOP);
			break;
		case Client::CShinobuState::TYPE_LOOP:
			if (m_bCollision == true)
			{
				pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
				pShinobu->Get_BattleTarget()->Play_Scene();
				return new CShinobu_CinemaState(CShinobu_CinemaState::SCENE_START);
			}
			else
			{
				pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
				return new CIdleState();
			}
			break;
			break;
		case Client::CShinobuState::TYPE_END:
			break;
		case Client::CShinobuState::TYPE_DEFAULT:
			break;
		case Client::CShinobuState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());

	}


	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		break;
	case Client::CShinobuState::TYPE_LOOP:
		if (pShinobu->Get_Model()->Get_CurrentTime_Index(CShinobu::ANIMID::ANIM_SPLSKL_START_1) >= 10.f)
		{
			m_fDuration += fTimeDelta;

			if (m_fDuration <= 0.5f)
				Move(pShinobu, fTimeDelta);
		}

		break;
	case Client::CShinobuState::TYPE_END:

	case Client::CShinobuState::TYPE_DEFAULT:
		break;
	case Client::CShinobuState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CShinobuState * CSplSkrStartState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSplSkrStartState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_SPLSKL_START_0);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SPLSKL_START_0);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_SPLSKL_START_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SPLSKL_START_0, 0.1f);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_SPLSKL_START_1);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SPLSKL_START_1);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_SPLSKL_START_1);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SPLSKL_START_1, 0.1f);
		break;
	case Client::CShinobuState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CShinobu* pShinobu)
{
}

void CSplSkrStartState::Move(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
}
