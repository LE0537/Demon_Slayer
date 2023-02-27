#include "stdafx.h"
#include "..\Public\Skill1_EnmuState.h"
#include "GameInstance.h"
#include "EnmuIdleState.h"
#include "SoundMgr.h"

using namespace Enmu;

CSkill1_EnmuState::CSkill1_EnmuState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CEnmuState * CSkill1_EnmuState::HandleInput(CEnmu * pEnmu)
{
	return nullptr;
}

CEnmuState * CSkill1_EnmuState::Tick(CEnmu * pEnmu, _float fTimeDelta)
{
	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CSkill1_EnmuState(TYPE_LOOP);
			break;
		case Client::CEnmuState::TYPE_LOOP:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CIdleState();
			//return new CSkill1_EnmuState(TYPE_END);
			break;
		//case Client::CEnmuState::TYPE_END:
		//	pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
		//	return new CIdleState();
		//	break;
		case Client::CEnmuState::TYPE_DEFAULT:
			break;
		case Client::CEnmuState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}


	return nullptr;
}

CEnmuState * CSkill1_EnmuState::Late_Tick(CEnmu * pEnmu, _float fTimeDelta)
{
	pEnmu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill1_EnmuState::Enter(CEnmu * pEnmu)
{
	m_eStateId = STATE_SKILL1;

	_uint iRand = rand() % 2;

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_1_0);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_1_0);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_1_0, 0.01f);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_1_0);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Enmu_SE_Skill_0.wav"), fEFFECT);
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("Enmu_Skill1_0.wav"), fVOICE);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("Enmu_Skill1_1.wav"), fVOICE);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_1_1);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_1_1);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_1_1, 0.01f);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_1_1);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Enmu_SE_Skill_0.wav"), fEFFECT);
		break;
	//case Client::CEnmuState::TYPE_END:
	//	pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_1_2);
	//	pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_1_2);
	//	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_1_2, 0.01f);
	//	pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_1_2);
		break;
	case Client::CEnmuState::TYPE_DEFAULT:
		break;
	case Client::CEnmuState::TYPE_CHANGE:
		break;
	default:
		break;
	}



}

void CSkill1_EnmuState::Exit(CEnmu * pEnmu)
{
}
