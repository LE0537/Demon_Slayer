#include "stdafx.h"
#include "EnmuIdlestate.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "EnmuAttack_1.h"

using namespace Enmu;

CIdleState::CIdleState(STATE_ID eState)
	: ePreState(eState)
{
	m_ePreState = eState;
}

CEnmuState * CIdleState::HandleInput(CEnmu* pEnmu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Down(DIK_R))
		return new CEnmuAttack1(TYPE_START);


	return nullptr;
}

CEnmuState * CIdleState::Tick(CEnmu* pEnmu, _float fTimeDelta)
{


	return nullptr;
}

CEnmuState * CIdleState::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{

	pEnmu->Get_Model()->Play_Animation(fTimeDelta);

	_vector vPlayerY = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pEnmu->Set_NavigationHeight(vPlayerY);
	if (vPlayerY.m128_f32[1] > pEnmu->Get_NavigationHeight().y + 0.1f)
	{
		pEnmu->Set_PlayerOriginPosY(fTimeDelta);
	}

	if (pEnmu->Get_PlayerInfo().iGuard < pEnmu->Get_PlayerInfo().iMaxGuard)
	{
		pEnmu->Set_GuardHp(1);
		if (pEnmu->Get_PlayerInfo().iGuard > pEnmu->Get_PlayerInfo().iMaxGuard)
			pEnmu->Set_ResetGuardHp();
	}

	return nullptr;
}

void CIdleState::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_IDLE;


	pEnmu->Set_GodMode(false);
	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_IDLE);
	pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_IDLE);
	pEnmu->Set_AnimIndex(CEnmu::ANIM_IDLE);
	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_IDLE, 0.05f);
	

}

void CIdleState::Exit(CEnmu* pEnmu)
{
}

