#include "stdafx.h"
#include "EnmuAttack_2.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "EnmuAttack_3.h"
#include "EnmuIdleState.h"

using namespace Enmu;

CEnmuAttack2::CEnmuAttack2(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CEnmuState * CEnmuAttack2::HandleInput(CEnmu* pEnmu)
{
	return nullptr;
}

CEnmuState * CEnmuAttack2::Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	if (pEnmu->Get_Model()->Get_CurrentTime_Index(CEnmu::ANIMID::ANIM_ATTACK_2) >= 30.f)
	{
		_vector vEnmuPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetPosition = pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_float fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vEnmuPosition));

		if (pEnmu->Get_TargetState() == 7 || fDistance <= 10.f)
		{
			return new CEnmuAttack3(TYPE_START);
		}
	}

	if (pEnmu->Get_Model()->Get_End(CEnmu::ANIM_ATTACK_2))
	{
		pEnmu->Get_Model()->Set_End(CEnmu::ANIM_ATTACK_2);
		return new CIdleState();
	}


	return nullptr;
}

CEnmuState * CEnmuAttack2::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{

	pEnmu->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CEnmuAttack2::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_ATK_2;
	pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_ATTACK_2);
	pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_ATTACK_2);
	pEnmu->Set_AnimIndex(CEnmu::ANIM_ATTACK_2);
	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_ATTACK_2, 0.2f);
	pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());

}

void CEnmuAttack2::Exit(CEnmu* pEnmu)
{
}

