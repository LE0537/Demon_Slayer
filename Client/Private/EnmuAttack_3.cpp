#include "stdafx.h"
#include "EnmuAttack_3.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "EnmuIdleState.h"

using namespace Enmu;

CEnmuAttack3::CEnmuAttack3(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CEnmuState * CEnmuAttack3::HandleInput(CEnmu* pEnmu)
{
	return nullptr;
}

CEnmuState * CEnmuAttack3::Tick(CEnmu* pEnmu, _float fTimeDelta)
{


	if (pEnmu->Get_Model()->Get_End(CEnmu::ANIM_ATTACK_3))
	{
		pEnmu->Get_Model()->Set_End(CEnmu::ANIM_ATTACK_3);
		return new CIdleState();
	}






	return nullptr;
}

CEnmuState * CEnmuAttack3::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{

	pEnmu->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CEnmuAttack3::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_ATK_3;
	pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_ATTACK_3);
	pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_ATTACK_3);
	pEnmu->Set_AnimIndex(CEnmu::ANIM_ATTACK_3);
	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_ATTACK_3, 0.2f);
	pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());

}

void CEnmuAttack3::Exit(CEnmu* pEnmu)
{
}

