#include "stdafx.h"
#include "RuiSkill_Shoot.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiBall.h"
#include "RuiTargetRushState.h"
#include "RuiSkill_Shoot.h"
#include "RuiSkill_ShootNet.h"
#include "RuiSkill_Sphere.h"
#include "RuiAdvSkill_CommonState.h"
#include "RuiAdvSkill_MoveState.h"
#include "RuiJumpState.h"
#include "RuiDashState.h"
#include "RuiAtk_1_State.h"
using namespace Rui;


CSkill_ShootState::CSkill_ShootState(STATE_TYPE eType)
{

	m_eStateType = eType;

}

CRuiState * CSkill_ShootState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pRui->Get_Model()->Get_Duration_Index(pRui->Get_AnimIndex());
	m_fCurrentDuration = pRui->Get_Model()->Get_CurrentTime_Index(pRui->Get_AnimIndex());

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.6f)
	{
		switch (pRui->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();

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
			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

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


	return CommandCheck(pRui);
}

CRuiState * CSkill_ShootState::Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());





	return nullptr;
}

CRuiState * CSkill_ShootState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	_int i = pRui->Get_Model()->Get_CurrentFrame();

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pRui->Get_Transform()->LookAt(vLooAt);

	m_fMove += fTimeDelta;

	CRuiBall::RUISHOOTINFO	tInfo;
	tInfo.pPlayer = pRui;
	tInfo.pTarget = m_pTarget;

	if (m_fMove > 0.17f && m_iHit < 1)
	{
		CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiBall"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
			return nullptr;

		RELEASE_INSTANCE(CGameInstance);



		m_fMove = 0.f;
		++m_iHit;
	}

	RELEASE_INSTANCE(CGameInstance);

	pRui->Get_Model()->Play_Animation(fTimeDelta * 1.3f);

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_ShootState(CRuiState::TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_ShootState(CRuiState::TYPE_END);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		}
	}
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUI_SHOTBALL_START_1, pRui);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUI_SHOTBALL_START_2, pRui);
			break;
		case Client::CRuiState::TYPE_END:
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUI_SHOTBALL_START_3, pRui);
			break;
		}
	
		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}

	return nullptr;
}

void CSkill_ShootState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_SKILL_SHOOTCOMMON;

	pRui->Get_Model()->Reset_Anim(pRui->Get_AnimIndex());
	
	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOT_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOT_0, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOT_0);
		pRui->Get_Model()->Set_FrameNum(CRui::ANIM_SKILL_SHOOT_0, 100);
		pRui->Get_Model()->Set_FrameTime(CRui::ANIM_SKILL_SHOOT_0, 1, 30, 1.2f);
		pRui->Get_Model()->Set_UsingFrame(CRui::ANIM_SKILL_SHOOT_0, 1, 30);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_Commonn.wav"), fEFFECT);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOT_1);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOT_1, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOT_1);
		pRui->Get_Model()->Set_FrameNum(CRui::ANIM_SKILL_SHOOT_1, 100);
		pRui->Get_Model()->Set_FrameTime(CRui::ANIM_SKILL_SHOOT_1, 1, 30, 1.2f);
		pRui->Get_Model()->Set_UsingFrame(CRui::ANIM_SKILL_SHOOT_1, 1, 30);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOT_2);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOT_2, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOT_2);
		pRui->Get_Model()->Set_FrameNum(CRui::ANIM_SKILL_SHOOT_2, 100);
		pRui->Get_Model()->Set_FrameTime(CRui::ANIM_SKILL_SHOOT_2, 1, 30, 1.2f);
		pRui->Get_Model()->Set_UsingFrame(CRui::ANIM_SKILL_SHOOT_2, 1, 100);
		break;
	}

}

void CSkill_ShootState::Exit(CRui* pRui)
{
	if (m_eStateType == TYPE_END)
	{
		pRui->Get_Model()->Set_FrameNum(CRui::ANIM_SKILL_SHOOT_2, 100);
		pRui->Get_Model()->Set_FrameTime(CRui::ANIM_SKILL_SHOOT_2, 0, 100, 1.f);
		pRui->Get_Model()->Set_UsingFrame(CRui::ANIM_SKILL_SHOOT_2, 0, 100);
		pRui->Get_Model()->Reset_Anim(pRui->Get_AnimIndex());
	}
}

CRuiState * CSkill_ShootState::CommandCheck(CRui * pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pRui->Get_Model()->Get_Duration_Index(pRui->Get_AnimIndex());
	m_fCurrentDuration = pRui->Get_Model()->Get_CurrentTime_Index(pRui->Get_AnimIndex());

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.7f)
	{
		switch (pRui->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_I)) // ╫╨еЁ е╟ 
			{
				if (pRui->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pRui->Set_SkillBar(-200);
						return new CSkill_SphereState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pRui->Set_SkillBar(-200);
						return new CSkill_ShootNetState(TYPE_START); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pRui->Set_SkillBar(-200);
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
				if (pRui->Get_PlayerInfo().iFriendBar >= 500)
				{
					if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						pRui->Set_FriendSkillBar(-500);
						return new CAdvSkill_CommonState();
					}
				}
			}

			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_X)) // ╫╨еЁ е╟ 
			{
				if (pRui->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pRui->Set_SkillBar(-200);
						return new CSkill_SphereState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pRui->Set_SkillBar(-200);
						return new CSkill_ShootNetState(TYPE_START); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pRui->Set_SkillBar(-200);
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
				if (pRui->Get_PlayerInfo().iFriendBar >= 500)
				{
					if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						pRui->Set_FriendSkillBar(-500);
						return new CAdvSkill_CommonState();
					}
				}
			}
		}
	}


	return nullptr;
}

