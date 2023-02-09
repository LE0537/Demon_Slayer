#include "stdafx.h"
#include "RuiDadGuardState.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "AiState.h"
using namespace RuiDad;


CGuardState::CGuardState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CRuiDadState * CGuardState::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



	return nullptr;
}

CRuiDadState * CGuardState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{

	pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_GUARD_0);
	//pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_1);
	pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_GUARD_2);
	pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_GUARD_0, 0.0f);
	pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_GUARD_1, 0.01f);
	pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_GUARD_2, 0.0f);

	

	static _float fContinueTime = 0.f;

	fContinueTime += fTimeDelta;

	pRuiDad->Set_bGuard(true);

	if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiDadState::TYPE_START:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
			break;
		case Client::CRuiDadState::TYPE_LOOP:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());

			if (fContinueTime >= 1.f)
				return new CGuardState(STATE_TYPE::TYPE_END);
			break;
		case Client::CRuiDadState::TYPE_END:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
	}

	return nullptr;

	return nullptr;
}

CRuiDadState * CGuardState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pRuiDad->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pRuiDad->Get_Transform()->Set_PlayerLookAt(vLooAt);


	pRuiDad->Get_Model()->Play_Animation(fTimeDelta * 1.2f);

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CGuardState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_GUARD;

	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_GUARD_0);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_GUARD_0);
		break;
	case Client::CRuiDadState::TYPE_LOOP:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_GUARD_1);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_GUARD_1);
		break;
	case Client::CRuiDadState::TYPE_END:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_GUARD_2);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_GUARD_2);
		break;
	case Client::CRuiDadState::TYPE_DEFAULT:
		break;
	}

}

void CGuardState::Exit(CRuiDad* pRuiDad)
{
}

