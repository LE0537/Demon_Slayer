#include "stdafx.h"
#include "..\Public\Skill1_EnmuState.h"
#include "GameInstance.h"
#include "EnmuIdleState.h"
#include "SoundMgr.h"
#include "EnmuShoot.h"

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
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pEnmu->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	pEnmu->Get_Transform()->Set_PlayerLookAt(vLooAt);

	m_fDelay += fTimeDelta;
	if (m_fDelay > 0.23f)
		m_fMove += fTimeDelta;

	CEnmuShoot::ENMUSHOOTINFO	tInfo;
	tInfo.pPlayer = pEnmu;
	tInfo.pTarget = m_pTarget;

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		if (m_fMove > 0.35f && m_iHit < 3 && pEnmu->Get_BattleTarget()->Get_GodMode() == false)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_EnmuShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;

			RELEASE_INSTANCE(CGameInstance);
			m_fMove = 0.f;
			++m_iHit;
		}
		break;
	case Client::CEnmuState::TYPE_LOOP:
		if (m_fDelay > 0.15f && m_iHit == 0  && pEnmu->Get_BattleTarget()->Get_GodMode() == false)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_EnmuShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;

			RELEASE_INSTANCE(CGameInstance);
			m_fDelay = 0.f;
			++m_iHit;
		}
		break;
	case Client::CEnmuState::TYPE_END:
		break;
	}


	RELEASE_INSTANCE(CGameInstance);
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
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Enmu_SE_Skill_0.wav"), g_fEffect);
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("Enmu_Skill1_0.wav"), g_fVoice);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("Enmu_Skill1_1.wav"), g_fVoice);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_1_1);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_1_1);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_1_1, 0.01f);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_1_1);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Enmu_SE_Skill_0.wav"), g_fEffect);
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
