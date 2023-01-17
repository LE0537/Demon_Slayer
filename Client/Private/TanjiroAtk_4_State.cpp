#include "stdafx.h"
#include "TanjiroAtk_4_State.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"

using namespace Tanjiro;


CAtk_4_State::CAtk_4_State()
{
}

CTanjiroState * CAtk_4_State::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	return nullptr;
}

CTanjiroState * CAtk_4_State::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_ATTACK_4);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_ATTACK_4, 0.01f);


	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_ATTACK_4))
	{
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_ATTACK_4);
		return new CIdleState();
	}


	return nullptr;
}

CTanjiroState * CAtk_4_State::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);




	return nullptr;
}

void CAtk_4_State::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_ATK_4;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_ATTACK_4);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_ATTACK_4);

}

void CAtk_4_State::Exit(CTanjiro * pTanjiro)
{
}

