#include "stdafx.h"
#include "NezukoGuardState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "NezukoGuardAdvState.h"
#include "Layer.h"

using namespace Nezuko;


CGuardState::CGuardState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CNezukoState * CGuardState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pNezuko->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_O) && m_eStateType != STATE_TYPE::TYPE_END && pNezuko->Get_PlayerInfo().fGuardTime <= 0.f)
		{
			pNezuko->Set_bGuard(true);
			if (pGameInstance->Key_Down(DIK_A) || pGameInstance->Key_Down(DIK_W) || pGameInstance->Key_Down(DIK_S) || pGameInstance->Key_Down(DIK_D))
				return new CGuardAdvState();
			else
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
		}
		else
			return new CGuardState(STATE_TYPE::TYPE_END);
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_C) && m_eStateType != STATE_TYPE::TYPE_END && pNezuko->Get_PlayerInfo().fGuardTime <= 0.f)
		{
			pNezuko->Set_bGuard(true);
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

CNezukoState * CGuardState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_GUARD_0);
	//pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_1);
	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_GUARD_2);
	pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_GUARD_0, 0.0f);
	pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_GUARD_1, 0.01f);
	pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_GUARD_2, 0.0f);

	

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{  
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CNezukoState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}


	return nullptr;
}

CNezukoState * CGuardState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	pNezuko->Get_Transform()->Set_PlayerLookAt(vLooAt);


	pNezuko->Get_Model()->Play_Animation(fTimeDelta * 1.2f);

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CGuardState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_GUARD;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_GUARD_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_GUARD_0);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_SE_Guard.wav"), fEFFECT);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_GUARD_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_GUARD_1);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_GUARD_2);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_GUARD_2);
		break;
	case Client::CNezukoState::TYPE_DEFAULT:
		break;
	}

}

void CGuardState::Exit(CNezuko* pNezuko)
{
	
}

