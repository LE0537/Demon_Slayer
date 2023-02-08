#include "stdafx.h"
#include "TanjiroGuardState.h"
#include "TanjiroIdleState.h"
#include "TanjiroGuardAdvState.h"
#include "GameInstance.h"
#include "Layer.h"

using namespace Tanjiro;


CGuardState::CGuardState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CTanjiroState * CGuardState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_O) && m_eStateType != STATE_TYPE::TYPE_END && pTanjiro->Get_PlayerInfo().fGuardTime <= 0.f)
		{
			pTanjiro->Set_bGuard(true);
			if (pGameInstance->Key_Down(DIK_W) || pGameInstance->Key_Down(DIK_S) || pGameInstance->Key_Down(DIK_A) || pGameInstance->Key_Down(DIK_D))
				return new CGuardAdvState();
			else
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
		}
		else
			return new CGuardState(STATE_TYPE::TYPE_END);
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_C) && m_eStateType != STATE_TYPE::TYPE_END && pTanjiro->Get_PlayerInfo().fGuardTime <= 0.f)
		{
			pTanjiro->Set_bGuard(true);
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

CTanjiroState * CGuardState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_GUARD_0);
	//pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_GUARD_1);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_GUARD_2);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_GUARD_0, 0.0f);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_GUARD_1, 0.0f);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_GUARD_2, 0.01f);

	//printf_s("type : %d state : %d anim : %d \n", (int)m_eStateType, (int)m_eStateId, (int)pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}


	return nullptr;
}

CTanjiroState * CGuardState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pTanjiro->Get_Transform()->Set_PlayerLookAt(vLooAt);


	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);

	RELEASE_INSTANCE(CGameInstance);
	return nullptr;
}

void CGuardState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_GUARD;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_GUARD_0);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_GUARD_0);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_GUARD_1);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_GUARD_1);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_GUARD_2);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_GUARD_2);
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		break;
	}


}

void CGuardState::Exit(CTanjiro * pTanjiro)
{
	//if (m_eStateType == CTanjiroState::TYPE_START || m_eStateType == CTanjiroState::TYPE_END)
	//	pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_0);
}

