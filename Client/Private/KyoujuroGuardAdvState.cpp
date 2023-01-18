#include "stdafx.h"
#include "KyoujuroGuardAdvState.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroGuardState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
using namespace Kyoujuro;


CGuardAdvState::CGuardAdvState()
{
}

CKyoujuroState * CGuardAdvState::HandleInput(CKyoujuro* pKyoujuro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_GUARD_ADV))
	{
		pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_GUARD_ADV);

		if (pGameInstance->Key_Pressing(DIK_O))
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
		else
			return new CGuardState(STATE_TYPE::TYPE_END);
	}


	return nullptr;
}

CKyoujuroState * CGuardAdvState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_ADV);
	pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_GUARD_ADV, 0.02f);


	printf_s("type : %d state : %d anim : %d \n", (int)m_eStateType, (int)m_eStateId, (int)pKyoujuro->Get_AnimIndex());

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();





	return nullptr;
}

CKyoujuroState * CGuardAdvState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CGuardAdvState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_GUARD_ADV;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_GUARD_ADV);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_GUARD_ADV);

}

void CGuardAdvState::Exit(CKyoujuro* pKyoujuro)
{
	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_ADV);
}

