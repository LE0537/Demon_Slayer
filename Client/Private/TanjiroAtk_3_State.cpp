#include "stdafx.h"
#include "TanjiroAtk_3_State.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroAtk_4_State.h"

using namespace Tanjiro;


CAtk_3_State::CAtk_3_State()
{
}

CTanjiroState * CAtk_3_State::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	

	//if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
	//	m_bAtkCombo = true;

	if (pGameInstance->Key_Down(DIK_J))
		m_bAtkCombo = true;


	return nullptr;
}

CTanjiroState * CAtk_3_State::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_ATTACK_3);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_ATTACK_3, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;
	//printf_s("AttackTime : %f \n", (_float)m_fComboDelay);



	if (m_bAtkCombo == true && m_fTime >= 40.f)
		return new CAtk_4_State();

	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_ATTACK_3))
	{
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_ATTACK_3);
		return new CIdleState();
	}


	return nullptr;
}

CTanjiroState * CAtk_3_State::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.1f);




	return nullptr;
}

void CAtk_3_State::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_ATK_3;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_ATTACK_3);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_ATTACK_3);

}

void CAtk_3_State::Exit(CTanjiro * pTanjiro)
{
}

