#include "stdafx.h"
#include "ShinobuGuardState.h"
#include "ShinobuIdleState.h"
#include "ShinobuGuardAdvState.h"
#include "GameInstance.h"
#include "Layer.h"

using namespace Shinobu;


CGuardState::CGuardState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CShinobuState * CGuardState::HandleInput(CShinobu* pShinobu)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pShinobu->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_O) && m_eStateType != STATE_TYPE::TYPE_END)
		{
			pShinobu->Set_bGuard(true);
			if (pGameInstance->Key_Down(DIK_W) || pGameInstance->Key_Down(DIK_S) || pGameInstance->Key_Down(DIK_A) || pGameInstance->Key_Down(DIK_D))
				return new CGuardAdvState();
			else
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
		}
		else
			return new CGuardState(STATE_TYPE::TYPE_END);
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_C) && m_eStateType != STATE_TYPE::TYPE_END)
		{
			pShinobu->Set_bGuard(true);
			if (pGameInstance->Key_Down(DIK_UP) || pGameInstance->Key_Down(DIK_DOWN) || pGameInstance->Key_Down(DIK_LEFT) || pGameInstance->Key_Down(DIK_RIGHT))
				return new CGuardAdvState();
			else
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
		}
		else
			return new CGuardState(STATE_TYPE::TYPE_END);
		break;
	default:
		break;
	}

	return nullptr;
}

CShinobuState * CGuardState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_GUARD_0);
	//pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_GUARD_1);
	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_GUARD_2);
	pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_GUARD_0, 0.0f);
	pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_GUARD_1, 0.0f);
	pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_GUARD_2, 0.01f);

	//printf_s("type : %d state : %d anim : %d \n", (int)m_eStateType, (int)m_eStateId, (int)pShinobu->Get_AnimIndex());

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
			break;
		case Client::CShinobuState::TYPE_END:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CShinobuState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}


	return nullptr;
}

CShinobuState * CGuardState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pShinobu->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pShinobu->Get_Transform()->Set_PlayerLookAt(vLooAt);


	pShinobu->Get_Model()->Play_Animation(fTimeDelta * 1.2f);

	RELEASE_INSTANCE(CGameInstance);
	return nullptr;
}

void CGuardState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_GUARD;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_GUARD_0);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_GUARD_0);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_GUARD_1);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_GUARD_1);
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_GUARD_2);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_GUARD_2);
		break;
	case Client::CShinobuState::TYPE_DEFAULT:
		break;
	}


}

void CGuardState::Exit(CShinobu* pShinobu)
{
	//if (m_eStateType == CTanjiroState::TYPE_START || m_eStateType == CTanjiroState::TYPE_END)
	//	pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIMID::ANIM_GUARD_0);
}

