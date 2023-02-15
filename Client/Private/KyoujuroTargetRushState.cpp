#include "stdafx.h"
#include "KyoujuroTargetRushState.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroMoveState.h"
#include "KyoujuroDashState.h"
#include "KyoujuroSkill_Common.h"
#include "KyoujuroSkill_DashSlash.h"
#include "KyoujuroSkill_DoubleUpper.h"
#include "KyoujuroJumpState.h"
#include "KyoujuroTargetRushState.h"
#include "KyoujuroAtk_1_State.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CKyoujuroState * CTargetRushState::HandleInput(CKyoujuro* pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



		switch (pKyoujuro->Get_i1P())
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
	
	return CommandCheck(pKyoujuro);
}

CKyoujuroState * CTargetRushState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

	pKyoujuro->Get_Transform()->Set_PlayerLookAt(pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START: 
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CKyoujuroState::TYPE_LOOP: 
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			break;
		case Client::CKyoujuroState::TYPE_END: 
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_LOOP:
		Move(pKyoujuro, fTimeDelta);

		if (m_bNextAnim == true)
		{
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return CommandCheck(pKyoujuro);
}

CKyoujuroState * CTargetRushState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	switch (m_eStateType)
	{
	case CKyoujuroState::TYPE_START:
		if (!m_bEffect)
		{
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_START, pKyoujuro);
			m_bEffect = true;
		}
		break;
	case CKyoujuroState::TYPE_LOOP:
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_MOVE, pKyoujuro);
		break;
	default:
		break;
	}

	RELEASE_INSTANCE(CEffect_Manager);

	return nullptr;
}

void CTargetRushState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_0);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_TARGET_RUSH_0);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_0);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pKyoujuro->Get_Transform()->Set_PlayerLookAt(pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_1);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_TARGET_RUSH_1);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pKyoujuro);
		break;
	case Client::CKyoujuroState::TYPE_END:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_2);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_TARGET_RUSH_2);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_1, false);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_2);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CKyoujuro* pKyoujuro)
{
}

void CTargetRushState::Move(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (pKyoujuro->Get_SphereCollider()->Collision(pKyoujuro->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
		_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		pKyoujuro->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(vPos);

		if (pKyoujuro->Get_BattleTarget()->Get_PlayerInfo().bGuard && pKyoujuro->Get_BattleTarget()->Get_PlayerInfo().iGuard > 0)
		{
			pKyoujuro->Get_BattleTarget()->Get_GuardHit(0);
		}
		else
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_HIT, pKyoujuro);
			RELEASE_INSTANCE(CEffect_Manager);
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
			RELEASE_INSTANCE(CGameInstance);
			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			RELEASE_INSTANCE(CGameInstance);
			pKyoujuro->Get_BattleTarget()->Take_Damage(0.3f, false);
		}
	}
	else
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CKyoujuro* pKyoujuro)
{
	m_vPosition.x = XMVectorGetX(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pKyoujuro->Set_NavigationHeight(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pKyoujuro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

CKyoujuroState * CTargetRushState::CommandCheck(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pKyoujuro->Get_Model()->Get_Duration();
	m_fCurrentDuration = pKyoujuro->Get_Model()->Get_CurrentTime();

	_float fRatio = m_fCurrentDuration / m_fDuration;

	switch (pKyoujuro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_J))
			return new CAtk_1_State();

		if (pGameInstance->Key_Pressing(DIK_I)) // ╫╨еЁ е╟ 
		{
			if (pKyoujuro->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_O))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_DoubleUpperState();
				}
				else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_DashSlashState(); // move skill
				}
				else
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_CommonState();
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
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_Z))
			return new CAtk_1_State();

		if (pGameInstance->Key_Pressing(DIK_X)) // ╫╨еЁ е╟ 
		{
			if (pKyoujuro->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_C))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_DoubleUpperState();
				}
				else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);

					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_DashSlashState(); // move skill
				}
				else
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_CommonState();
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
		break;
	}



	return nullptr;
}

