#include "stdafx.h"
#include "TanjiroDashState.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "Layer.h"

using namespace Tanjiro;

CDashState::CDashState(OBJDIR eDir, _bool bJump)
	: m_eDir(eDir), m_bJump(bJump)
{
	
}

CTanjiroState * CDashState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CDashState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	

	return nullptr;
}

CTanjiroState * CDashState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CDashState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_DASH;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CCharacters* m_pTarget = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"))->Get_LayerFront();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pTanjiro->Get_Transform()->LookAt(vLooAt);

	

	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_F);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_F);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_F);
		break;
	case Client::DIR_LEFT:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_01);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_01);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_01);
		break;
	case Client::DIR_RIGHT:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_01);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_01);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_01);
		break;
	case Client::DIR_BACK:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_B);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_B);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_B);
		break;
	case Client::DIR_LF:
		break;
	case Client::DIR_RF:
		break;
	case Client::DIR_LB:
		break;
	case Client::DIR_RB:
		break;
	case Client::DIR_STOP:
		break;
	case Client::DIR_END:
		break;
	default:
		break;
	}


}

void CDashState::Exit(CTanjiro * pTanjiro)
{
	
}



