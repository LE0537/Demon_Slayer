#include "stdafx.h"
#include "KyoujuroGuardState.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "KyoujuroGuardAdvState.h"
#include "Layer.h"

using namespace Kyoujuro;


CGuardState::CGuardState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CKyoujuroState * CGuardState::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Pressing(DIK_O) && m_eStateType != STATE_TYPE::TYPE_END)
	{
		if (pGameInstance->Key_Down(DIK_LEFT) || pGameInstance->Key_Down(DIK_UP) || pGameInstance->Key_Down(DIK_DOWN) || pGameInstance->Key_Down(DIK_RIGHT))
			return new CGuardAdvState();
		else
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
	}
	else
		return new CGuardState(STATE_TYPE::TYPE_END);



	return nullptr;
}

CKyoujuroState * CGuardState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_0);
	//pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_1);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_2);
	pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_GUARD_0, 0.0f);
	pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_GUARD_1, 0.01f);
	pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_GUARD_2, 0.0f);

	

	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{  
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
			break;
		case Client::CKyoujuroState::TYPE_LOOP:
			break;
		case Client::CKyoujuroState::TYPE_END:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CKyoujuroState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}


	return nullptr;
}

CKyoujuroState * CGuardState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);



	return nullptr;
}

void CGuardState::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_GUARD;

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_GUARD_0);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_GUARD_0);
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_GUARD_1);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_GUARD_1);
		break;
	case Client::CKyoujuroState::TYPE_END:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_GUARD_2);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_GUARD_2);
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		break;
	}

}

void CGuardState::Exit(CKyoujuro * pKyoujuro)
{
}

