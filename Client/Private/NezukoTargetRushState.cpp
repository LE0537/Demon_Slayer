#include "stdafx.h"
#include "NezukoTargetRushState.h"
#include "GameInstance.h"
#include "NezukoIdleState.h"
#include "NezukoMoveState.h"
#include "Effect_Manager.h"
#include "NezukoDashState.h"
#include "NezukoSkill_Common.h"
#include "NezukoSkill_FallCut.h"
#include "NezukoSkill_Move.h"
#include "NezukojumpState.h"
#include "NezukoTargetRushState.h"
#include "NezukoAtk_1_State.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "NezukoSplSkrStartState.h"
using namespace Nezuko;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CNezukoState * CTargetRushState::HandleInput(CNezuko* pNezuko)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



	if (m_fComboDelay <= 35.f)
	{
		switch (pNezuko->Get_i1P())
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


	return CommandCheck(pNezuko);
}

CNezukoState * CTargetRushState::Tick(CNezuko * pNezuko, _float fTimeDelta)
{

	pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START: 
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP: 
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());;
			break;
		case Client::CNezukoState::TYPE_END: 
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_LOOP:
		Move(pNezuko, fTimeDelta);

		if (m_bNextAnim == true)
		{
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CNezukoState * CTargetRushState::Late_Tick(CNezuko * pNezuko, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pNezuko->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	switch (m_eStateType)
	{
	case CNezukoState::TYPE_START:
		if (!m_bEffect)
		{
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_START, pNezuko);
			m_bEffect = true;
		}
		break;
	case CNezukoState::TYPE_LOOP:
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_MOVE, pNezuko);
		break;
	default:
		break;
	}

	RELEASE_INSTANCE(CEffect_Manager);

	return nullptr;
}

void CTargetRushState::Enter(CNezuko * pNezuko)
{
	m_eStateId = STATE_RUSH;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_TARGET_RUSH_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_TARGET_RUSH_0);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_TARGET_RUSH_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_TargetRush.wav"), g_fEffect);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_TARGET_RUSH_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_TARGET_RUSH_1);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pNezuko);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_TARGET_RUSH_2);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_TARGET_RUSH_2);
		//pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_TARGET_RUSH_1, false);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_TARGET_RUSH_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CNezuko * pNezuko)
{
}

void CTargetRushState::Move(CNezuko * pNezuko, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vMyPosition = XMVectorSetY(vMyPosition, pNezuko->Get_NavigationHeight().y);
	vTargetPosition = XMVectorSetY(vTargetPosition, pNezuko->Get_BattleTarget()->Get_NavigationHeight().y);



	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	if (pNezuko->Get_BattleTarget()->Get_Transform()->Get_Jump())
	{
		if (fDistance <= 5.f)
		{
			m_bNextAnim = true;
		}
	}


	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (pNezuko->Get_SphereCollider()->Collision(pNezuko->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
		_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		pNezuko->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(vPos);

		if (pNezuko->Get_BattleTarget()->Get_PlayerInfo().bGuard && pNezuko->Get_BattleTarget()->Get_PlayerInfo().fGuardTime <= 0.f)
		{
			pNezuko->Get_BattleTarget()->Get_GuardHit(0);
		}
		else
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_HIT, pNezuko);
			RELEASE_INSTANCE(CEffect_Manager);
			pNezuko->Get_BattleTarget()->Take_Damage(0.3f, false);
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
			
			RELEASE_INSTANCE(CGameInstance);

			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pNezuko->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
		}
	}
	else
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CNezuko * pNezuko)
{
	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

CNezukoState * CTargetRushState::CommandCheck(CNezuko * pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pNezuko->Get_Model()->Get_Duration_Index(CNezuko::ANIM_SKILL_MOVE_2);
	m_fCurrentDuration = pNezuko->Get_Model()->Get_CurrentTime_Index(CNezuko::ANIM_SKILL_MOVE_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	//if (fRatio >= 0.5f)
	{
		switch (pNezuko->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_E))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				if (pNezuko->Get_PlayerInfo().iUnicCount > 0)
				{
					pNezuko->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}

			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_I)) // ╫╨еЁ е╟ 
			{
				if (pNezuko->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_FallCutState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_MoveState(TYPE_START); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_CommonState(TYPE_START);
					}
				}
			}
			else if (pGameInstance->Key_Pressing(DIK_L))
			{
				//return new CTargetRushState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_SPACE))
			{
				return new CJumpState(TYPE_START, 0.f, 0.f);
			}
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_RSHIFT))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				if (pNezuko->Get_PlayerInfo().iUnicCount > 0)
				{
					pNezuko->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}

			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_X)) // ╫╨еЁ е╟ 
			{
				if (pNezuko->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_FallCutState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);

						pNezuko->Set_SkillBar(-200);
						return new CSkill_MoveState(TYPE_START); // move skill

					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_CommonState(TYPE_START);
					}
				}
			}
			else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				//return new CTargetRushState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_LCONTROL))
			{
				return new CJumpState(TYPE_START, 0.f, 0.f);
			}
			break;
		}

	}

	return nullptr;
}

