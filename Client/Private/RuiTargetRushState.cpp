#include "stdafx.h"
#include "RuiTargetRushState.h"
#include "GameInstance.h"
#include "RuiIdleState.h"
#include "RuiMoveState.h"
#include "RuiDashState.h"
#include "RuiSkill_Shoot.h"
#include "RuiSkill_ShootNet.h"
#include "RuiSkill_Sphere.h"
#include "RuiAdvSkill_CommonState.h"
#include "RuiAdvSkill_MoveState.h"
#include "RuiJumpState.h"
#include "RuiDashState.h"
#include "RuiAtk_1_State.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiSplSkrStartState.h"
using namespace Rui;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CRuiState * CTargetRushState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pRui->Get_Model()->Get_Duration_Index(pRui->Get_AnimIndex());
	m_fCurrentDuration = pRui->Get_Model()->Get_CurrentTime_Index(pRui->Get_AnimIndex());

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.0f)
	{
		switch (pRui->Get_i1P())
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





	return CommandCheck(pRui);
}

CRuiState * CTargetRushState::Tick(CRui* pRui, _float fTimeDelta)
{

	pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START: 
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_TargetRush.wav"), fEFFECT);
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP: 
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());;
			break;
		case Client::CRuiState::TYPE_END: 
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_LOOP:
		Move(pRui, fTimeDelta);

		if (m_bNextAnim == true)
		{
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CRuiState * CTargetRushState::Late_Tick(CRui* pRui, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pRui->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pRui->Get_Model()->Play_Animation(fTimeDelta);

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	switch (m_eStateType)
	{
	case CRuiState::TYPE_START:
		if (!m_bEffect)
		{
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_START, pRui);
			m_bEffect = true;
		}
		break;
	case CRuiState::TYPE_LOOP:
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_MOVE, pRui);
		break;
	default:
		break;
	}

	RELEASE_INSTANCE(CEffect_Manager);

	return nullptr;
}

void CTargetRushState::Enter(CRui* pRui)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_TARGET_RUSH_0);
		pRui->Set_AnimIndex(CRui::ANIM_TARGET_RUSH_0);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_TARGET_RUSH_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_TARGET_RUSH_1);
		pRui->Set_AnimIndex(CRui::ANIM_TARGET_RUSH_1);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pRui);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_TARGET_RUSH_2);
		pRui->Set_AnimIndex(CRui::ANIM_TARGET_RUSH_2);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_TARGET_RUSH_1, false);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_TARGET_RUSH_2);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CRui* pRui)
{
}

void CTargetRushState::Move(CRui* pRui, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (pRui->Get_SphereCollider()->Collision(pRui->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
		_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		pRui->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(vPos);

		if (pRui->Get_BattleTarget()->Get_PlayerInfo().bGuard && pRui->Get_BattleTarget()->Get_PlayerInfo().iGuard > 0)
		{
			pRui->Get_BattleTarget()->Get_GuardHit(0);
		}
		else
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_HIT, pRui);
			RELEASE_INSTANCE(CEffect_Manager);
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pRui->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
			pRui->Get_BattleTarget()->Take_Damage(0.3f, false);
		}
	}
	else
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CRui* pRui)
{
	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pRui->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

CRuiState * CTargetRushState::CommandCheck(CRui * pRui)
{


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pRui->Get_Model()->Get_Duration_Index(CRui::ANIM_SKILL_SPHERE_2);
	m_fCurrentDuration = pRui->Get_Model()->Get_CurrentTime_Index(CRui::ANIM_SKILL_SPHERE_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	//if (fRatio >= 0.7f)
	{
		switch (pRui->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_E))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				return new CSplSkrStartState(TYPE_START);
			}

			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();

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
			//	return new CTargetRushState(TYPE_START);
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

			if (pGameInstance->Key_Pressing(DIK_RSHIFT))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				return new CSplSkrStartState(TYPE_START);
			}

			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

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
			//	return new CTargetRushState(TYPE_START);
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

