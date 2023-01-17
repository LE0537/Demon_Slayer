#include "stdafx.h"
#include "KyoujuroAtk_4_State.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"


using namespace Kyoujuro;


CAtk_4_State::CAtk_4_State()
{
}

CKyoujuroState * CAtk_4_State::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CKyoujuroState * CAtk_4_State::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_ATTACK_4);
	pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_ATTACK_4, 0.01f);


	if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_ATTACK_4))
	{
		pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_ATTACK_4);
		return new CIdleState();
	}


	return nullptr;
}

CKyoujuroState * CAtk_4_State::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);




	return nullptr;
}

void CAtk_4_State::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_ATK_4;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_ATTACK_4);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_ATTACK_4);

}

void CAtk_4_State::Exit(CKyoujuro * pKyoujuro)
{
}

