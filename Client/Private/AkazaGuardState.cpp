#include "stdafx.h"
#include "AkazaGuardState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "AkazaGuardAdvState.h"
#include "Layer.h"

using namespace Akaza;


CGuardState::CGuardState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CAkazaState * CGuardState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pAkaza->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_O) && m_eStateType != STATE_TYPE::TYPE_END)
		{
			pAkaza->Set_bGuard(true);
			if (pGameInstance->Key_Down(DIK_A) || pGameInstance->Key_Down(DIK_W) || pGameInstance->Key_Down(DIK_S) || pGameInstance->Key_Down(DIK_D))
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
			pAkaza->Set_bGuard(true);
			if (pGameInstance->Key_Down(DIK_LEFT) || pGameInstance->Key_Down(DIK_UP) || pGameInstance->Key_Down(DIK_DOWN) || pGameInstance->Key_Down(DIK_RIGHT))
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

CAkazaState * CGuardState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_GUARD_0);
	//pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_1);
	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_GUARD_2);
	pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_GUARD_0, 0.0f);
	pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_GUARD_1, 0.01f);
	pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_GUARD_2, 0.0f);

	

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{  
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CAkazaState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}


	return nullptr;
}

CAkazaState * CGuardState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pAkaza->Get_Transform()->LookAt(vLooAt);


	pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.2f);

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CGuardState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_GUARD;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_GUARD_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_GUARD_0);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_GUARD_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_GUARD_1);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_GUARD_2);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_GUARD_2);
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		break;
	}

}

void CGuardState::Exit(CAkaza* pAkaza)
{
}

