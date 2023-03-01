#include "stdafx.h"
#include "AkazaTargetRushState.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"
#include "AkazaMoveState.h"
#include "AkazaDashState.h"
#include "AkazaAdvSkill_Common.h"
#include "AkazaAdvSkill_Move.h"
#include "AkazaSkill_Destroy.h"
#include "AkazaSkill_Punch.h"
#include "AkazaSkill_Shoot.h"
#include "AkazaTargetRushState.h"
#include "AkazaJumpState.h"
#include "AkazaAtk_1_State.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "AkazaSplSkrStartState.h"
using namespace Akaza;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CAkazaState * CTargetRushState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

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

	return CommandCheck(pAkaza);
}

CAkazaState * CTargetRushState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START: 
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP: 
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());;
			break;
		case Client::CAkazaState::TYPE_END: 
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_LOOP:
		Move(pAkaza, fTimeDelta);

		if (m_bNextAnim == true)
		{
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CAkazaState * CTargetRushState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	switch (m_eStateType)
	{
	case CAkazaState::TYPE_START:
		if (!m_bEffect)
		{
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_START, pAkaza);
			m_bEffect = true;
		}
		break;
	case CAkazaState::TYPE_LOOP:
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_MOVE, pAkaza);
		break;
	default:
		break;
	}

	RELEASE_INSTANCE(CEffect_Manager);

	return nullptr;
}

void CTargetRushState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_TARGET_RUSH_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_TARGET_RUSH_0);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_TARGET_RUSH_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_TargetRush.wav"), fEFFECT);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_TARGET_RUSH_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_TARGET_RUSH_1);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pAkaza);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_TARGET_RUSH_2);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_TARGET_RUSH_2);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_TARGET_RUSH_1, false);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_TARGET_RUSH_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CAkaza* pAkaza)
{
}

void CTargetRushState::Move(CAkaza* pAkaza, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (pAkaza->Get_SphereCollider()->Collision(pAkaza->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
		_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		pAkaza->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(vPos);

		if (pAkaza->Get_BattleTarget()->Get_PlayerInfo().bGuard && pAkaza->Get_BattleTarget()->Get_PlayerInfo().fGuardTime <= 0.f)
		{
			pAkaza->Get_BattleTarget()->Get_GuardHit(0);
		}
		else
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_HIT, pAkaza);
			RELEASE_INSTANCE(CEffect_Manager);
			pAkaza->Get_BattleTarget()->Take_Damage(0.3f, false);
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
			RELEASE_INSTANCE(CGameInstance);

			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pAkaza->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
		}
	}
	else
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CAkaza* pAkaza)
{
	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pAkaza->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

CAkazaState * CTargetRushState::CommandCheck(CAkaza * pAkaza)
{



	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pAkaza->Get_Model()->Get_Duration_Index(pAkaza->Get_AnimIndex());
	m_fCurrentDuration = pAkaza->Get_Model()->Get_CurrentTime_Index(pAkaza->Get_AnimIndex());

	_float fRatio = m_fCurrentDuration / m_fDuration;


	switch (pAkaza->Get_i1P())
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
			//return new CTargetRushState(TYPE_START);
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
		if (pGameInstance->Key_Pressing(DIK_RSHIFT))
		{
			//	pTanjiro->Get_BattleTarget()->Play_Scene();
			return new CSplSkrStartState(TYPE_START);
		}

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
			//return new CTargetRushState(TYPE_START);
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



	return nullptr;
}

