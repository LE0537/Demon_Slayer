#include "stdafx.h"
#include "..\Public\EnmuBoss.h"
#include "GameInstance.h"

#include "Enmu_Chaos_Head.h"
#include "Enmu_Left_Hand.h"
#include "Enmu_Right_Hand.h"
#include "Enmu_Chok.h"
#include "Enmu_Shield.h"

#include "EnmuBossToolState.h"
#include "EnmuBossIdleState.h"
#include "EnmuBoss_Hit.h"


IMPLEMENT_SINGLETON(CEnmuBoss)

using namespace EnmuBoss;

CEnmuBoss::CEnmuBoss()
{
}

HRESULT CEnmuBoss::Initialize()
{
	CEnmuBossState* pState = new CIdleState();
	m_pEnmuBossState = m_pEnmuBossState->ChangeState(this, m_pEnmuBossState, pState);

	return S_OK;
}

HRESULT CEnmuBoss::Add_EnmuParts(CCharacters * pCharacter)
{
	if (pCharacter == nullptr)
		return E_FAIL;

	else if (pCharacter != nullptr)
		m_EnmuParts.push_back(pCharacter);

	int a = 0;

	return S_OK;
}

void CEnmuBoss::Set_ToolState(_uint iPartsIndex, _uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue)
{
	CEnmuBossState* pState = new CBossToolState(iPartsIndex, iAnimIndex, iAnimIndex_2, iAnimIndex_3, static_cast<CEnmuBossState::STATE_TYPE>(iTypeIndex), bIsContinue);
	m_pEnmuBossState = m_pEnmuBossState->ChangeState(this, m_pEnmuBossState, pState);
}

void CEnmuBoss::BossEnmu_Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	HandleInput();
	TickState(fTimeDelta);

}

void CEnmuBoss::BossEnmu_LateTick(_float fTimeDelta)
{
	LateTickState(fTimeDelta);
}

_uint CEnmuBoss::Get_BossState()
{
	if(m_pEnmuBossState != nullptr)
		return m_pEnmuBossState->Get_EnmuBossState();
}

void CEnmuBoss::HandleInput()
{
	CEnmuBossState* pNewState = m_pEnmuBossState->HandleInput(this);

	if (pNewState)
		m_pEnmuBossState = m_pEnmuBossState->ChangeState(this, m_pEnmuBossState, pNewState);
}

void CEnmuBoss::TickState(_float fTimeDelta)
{

	CEnmuBossState* pNewState = m_pEnmuBossState->Tick(this, fTimeDelta);

	if (pNewState)
		m_pEnmuBossState = m_pEnmuBossState->ChangeState(this, m_pEnmuBossState, pNewState);

}

void CEnmuBoss::LateTickState(_float fTimeDelta)
{
	CEnmuBossState* pNewState = m_pEnmuBossState->Late_Tick(this, fTimeDelta);

	if (pNewState)
		m_pEnmuBossState = m_pEnmuBossState->ChangeState(this, m_pEnmuBossState, pNewState);
}



void CEnmuBoss::Free()
{

}
