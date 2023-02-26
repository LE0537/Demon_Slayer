#include "stdafx.h"
#include "..\Public\EnmuBoss.h"
#include "GameInstance.h"

#include "Enmu_Chaos_Head.h"
#include "Enmu_Left_Hand.h"
#include "Enmu_Right_Hand.h"
#include "Enmu_Chok.h"
#include "Enmu_Shield.h"

#include "EnmuBossToolState.h"

using namespace EnmuBoss;

IMPLEMENT_SINGLETON(CEnmuBoss)

CEnmuBoss::CEnmuBoss()
{
}

HRESULT CEnmuBoss::Add_EnmuParts(CCharacters * pCharacter)
{
	if (pCharacter == nullptr)
		return E_FAIL;

	else if (pCharacter != nullptr)
		m_EnmuParts.push_back(pCharacter);

	return S_OK;
}

void CEnmuBoss::Set_ToolState(_uint iPartsIndex, _uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue)
{
	CEnmuBossState* pState = new CBossToolState(iPartsIndex, iAnimIndex, iAnimIndex_2, iAnimIndex_3, static_cast<CEnmuBossState::STATE_TYPE>(iTypeIndex), bIsContinue);
	m_pEnmuBossState = m_pEnmuBossState->ChangeState(this, m_pEnmuBossState, pState);
}



void CEnmuBoss::Free()
{
	
}
