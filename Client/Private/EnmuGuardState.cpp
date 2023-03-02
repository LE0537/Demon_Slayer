#include "stdafx.h"
#include "EnmuGuardState.h"
#include "EnmuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "EnmuIdleState.h"
#include "SoundMgr.h"

using namespace Enmu;


CGuardState::CGuardState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CEnmuState * CGuardState::HandleInput(CEnmu* pEnmu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



	return nullptr;
}

CEnmuState * CGuardState::Tick(CEnmu* pEnmu, _float fTimeDelta)
{

	pEnmu->Get_Model()->Set_Loop(CEnmu::ANIM_GUARD_0);
	//pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_1);
	pEnmu->Get_Model()->Set_Loop(CEnmu::ANIM_GUARD_2);
	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_GUARD_0, 0.0f);
	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_GUARD_1, 0.01f);
	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_GUARD_2, 0.0f);

	

	static _float fContinueTime = 0.f;

	fContinueTime += fTimeDelta;

	pEnmu->Set_bGuard(true);

	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
			break;
		case Client::CEnmuState::TYPE_LOOP:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());

			if (fContinueTime >= 1.f)
				return new CGuardState(STATE_TYPE::TYPE_END);
			break;
		case Client::CEnmuState::TYPE_END:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}

	return nullptr;

}

CEnmuState * CGuardState::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pEnmu->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pEnmu->Get_Transform()->Set_PlayerLookAt(vLooAt);


	pEnmu->Get_Model()->Play_Animation(fTimeDelta * 1.2f);

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CGuardState::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_GUARD;

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_GUARD_0);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_GUARD_0);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Enmu_SE_Guard.wav"), g_fEffect);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_GUARD_1);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_GUARD_1);
		break;
	case Client::CEnmuState::TYPE_END:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_GUARD_2);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_GUARD_2);
		break;
	case Client::CEnmuState::TYPE_DEFAULT:
		break;
	}

}

void CGuardState::Exit(CEnmu* pEnmu)
{
}

