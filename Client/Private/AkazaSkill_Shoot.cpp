#include "stdafx.h"
#include "AkazaSkill_Shoot.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "AkazaDashState.h"
#include "AkazaAdvSkill_Common.h"
#include "AkazaAdvSkill_Move.h"
#include "AkazaSkill_Destroy.h"
#include "AkazaSkill_Punch.h"
#include "AkazaSkill_Shoot.h"
#include "AkazaTargetRushState.h"
#include "AkazaJumpState.h"
#include "AkazaAtk_1_State.h"
using namespace Akaza;


CSkill_ShootState::CSkill_ShootState(STATE_TYPE eType)
{
	m_eStateType = eType;
	



}

CAkazaState * CSkill_ShootState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pAkaza->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_I))
			m_bCombo = true;
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_X))
			m_bCombo = true;
		break;
	}
	
	


	m_fDuration = pAkaza->Get_Model()->Get_Duration_Index(CAkaza::ANIM_SKILL_SHOOT_1);
	m_fCurrentDuration = pAkaza->Get_Model()->Get_CurrentTime_Index(CAkaza::ANIM_SKILL_SHOOT_1);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.6f)
	{
		switch (pAkaza->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_W)) // ╬у
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // аб
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_RF);
				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_STRAIGHT);
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_S)) // ╣з
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // аб
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LB);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л 
				{

					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_RB);

				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_BACK);
				}
			}


			else if (pGameInstance->Key_Pressing(DIK_A)) // аб
			{

				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RIGHT);
			}
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_UP)) // ╬у
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_RF);


				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_STRAIGHT);
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ╣з
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LB);


				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л 
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_RB);

				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_BACK);

				}
			}


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RIGHT);

			}
			break;
		}
	}

	return CommandCheck(pAkaza);
}

CAkazaState * CSkill_ShootState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Set_EffectTime(fTimeDelta);
	pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			if(m_bCombo == true)
				return new CSkill_ShootState(CAkazaState::TYPE_LOOP);
			else if(m_bCombo ==false)
				return new CSkill_ShootState(CAkazaState::TYPE_END);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CSkill_ShootState(CAkazaState::TYPE_END);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	return nullptr;
}

CAkazaState * CSkill_ShootState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pAkaza->Get_Transform()->LookAt(vLooAt);

	m_fDelay += fTimeDelta;
	if(m_fDelay > 0.23f)
		m_fMove += fTimeDelta;
	
	CAkazaShoot::AKAZASHOOTINFO	tInfo;
	tInfo.pPlayer = pAkaza;
	tInfo.pTarget = m_pTarget;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		if (m_fMove > 0.23f && m_iHit < 3 && pAkaza->Get_BattleTarget()->Get_GodMode() == false)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_AkazaShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;

			RELEASE_INSTANCE(CGameInstance);
			m_fMove = 0.f;
			++m_iHit;
		}
		break;
	case Client::CAkazaState::TYPE_LOOP:
		if (m_fDelay > 0.15f && m_iHit < 2 && pAkaza->Get_BattleTarget()->Get_GodMode() == false)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_AkazaShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;

			RELEASE_INSTANCE(CGameInstance);
			m_fDelay = 0.f;
			++m_iHit;
		}
		break;
	case Client::CAkazaState::TYPE_END:
		break;
	}


	RELEASE_INSTANCE(CGameInstance);

	pAkaza->Get_Model()->Play_Animation(fTimeDelta);
	if (!m_bEffect && m_eStateType == TYPE_START)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_COM_START, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_COM_HAND_1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_COM_HAND_2, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_COM_HAND_3, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	else if (!m_bEffect && m_eStateType == TYPE_LOOP)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_COM_HAND_4, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_COM_HAND_5, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}

	return nullptr;
}

void CSkill_ShootState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_SKILL_SHOOT;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_SHOOT_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_SHOOT_0, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_SHOOT_0);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_Shoot.wav"), fEFFECT);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_SE_CommonSkill.wav"), fEFFECT);
		if (pAkaza->Get_BattleTarget()->Get_TargetState() == STATE_HIT)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_SE_Hit_CommonSkill.wav"), fEFFECT);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_SHOOT_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_SHOOT_1, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_SHOOT_1);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_SHOOT_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_SHOOT_2, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_SHOOT_2);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_SE_CommonSkill.wav"), fEFFECT);
		if (pAkaza->Get_BattleTarget()->Get_TargetState() == STATE_HIT)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_SE_Hit_CommonSkill.wav"), fEFFECT);
		break;
	}

}

void CSkill_ShootState::Exit(CAkaza* pAkaza)
{
	
}

CAkazaState * CSkill_ShootState::CommandCheck(CAkaza * pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pAkaza->Get_Model()->Get_Duration_Index(CAkaza::ANIM_SKILL_SHOOT_1);
	m_fCurrentDuration = pAkaza->Get_Model()->Get_CurrentTime_Index(CAkaza::ANIM_SKILL_SHOOT_1);

	_float fRatio = m_fCurrentDuration / m_fDuration;


	if (fRatio >= 0.7f)
	{
		switch (pAkaza->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();


			if (pGameInstance->Key_Pressing(DIK_I)) // ╫╨еЁ е╟ 
			{
				if (pAkaza->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pAkaza->Set_SkillBar(-200);
						return new CSkill_DestoryState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pAkaza->Set_SkillBar(-200);
						return new CSkill_PunchState(TYPE_START); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pAkaza->Set_SkillBar(-200);
						return new CSkill_ShootState(TYPE_START);
					}
				}
			}
			else if (pGameInstance->Key_Pressing(DIK_L))
			{
				return new CTargetRushState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_SPACE))
			{
				return new CJumpState(TYPE_START, 0.f, 0.f);
			}
			else if (pGameInstance->Key_Pressing(DIK_U))
			{
				if (pAkaza->Get_PlayerInfo().iFriendBar >= 500)
				{
					if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						pAkaza->Set_FriendSkillBar(-500);
						return new CAdvSkill_MoveState(TYPE_START);
					}
					else
					{
						pAkaza->Set_FriendSkillBar(-500);
						return new CAdvSkill_CommmonState(TYPE_START);
					}
				}
			}

			break;
		case 2:
			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_X)) // ╫╨еЁ е╟ 
			{
				if (pAkaza->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pAkaza->Set_SkillBar(-200);
						return new CSkill_DestoryState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pAkaza->Set_SkillBar(-200);
						return new CSkill_PunchState(TYPE_START); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pAkaza->Set_SkillBar(-200);
						return new CSkill_ShootState(TYPE_START);
					}
				}
			}
			else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				return new CTargetRushState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_LCONTROL))
			{
				return new CJumpState(TYPE_START, 0.f, 0.f);
			}
			else if (pGameInstance->Key_Pressing(DIK_V))
			{
				if (pAkaza->Get_PlayerInfo().iFriendBar >= 500)
				{
					if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						pAkaza->Set_FriendSkillBar(-500);
						return new CAdvSkill_MoveState(TYPE_START);
					}
					else
					{
						pAkaza->Set_FriendSkillBar(-500);
						return new CAdvSkill_CommmonState(TYPE_START);
					}
				}
			}
		}
	}


	return nullptr;
}

