#include "stdafx.h"
#include "TanjiroTargetRushState.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"
#include "TanjiroMoveState.h"
#include "TanjiroDashState.h"
#include "TanjiroJumpState.h"
#include "TanjiroSkill_Common.h"
#include "TanjiroSkill_WaterMill.h"
#include "TanjiroSkill_WindMill.h"
#include "TanjiroAtk_1_State.h"
#include "UI_Manager.h"

#include "TanjiroKaguraSkill_Common.h"
#include "TanjiroKaguraSkill_Move.h"
#include "TanjiroKaguraSkill_Sphere.h"
#include "TanjiroKaguraJumpSkill_Common.h"
#include "TanjiroKaguraJumpSkill_Move.h"
#include "TanjiroKaguraAtk_1_State.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Tanjiro;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CTanjiroState * CTargetRushState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_J))
		{
			if (pTanjiro->Get_KaguraMode() == true)
			{
				return new CAtk_1_KaguraState();
			}
			else
				return new CAtk_1_State();
		}
			

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
		{
			if (pTanjiro->Get_KaguraMode() == true)
			{
				return new CAtk_1_KaguraState();
			}
			else
				return new CAtk_1_State();
		}

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


	return CommandCheck(pTanjiro);
}

CTanjiroState * CTargetRushState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());;
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_LOOP:
		Move(pTanjiro, fTimeDelta);

		if (m_bNextAnim == true)
		{
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CTanjiroState * CTargetRushState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta);
	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	switch (m_eStateType)
	{
	case CTanjiroState::TYPE_START:
		if (!m_bEffect)
		{
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_START, pTanjiro);
			m_bEffect = true;
		}
		break;
	case CTanjiroState::TYPE_LOOP:
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_MOVE, pTanjiro);
		break;
	default:
		break;
	}

	RELEASE_INSTANCE(CEffect_Manager);
	return nullptr;
}

void CTargetRushState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_RUSH;

	_uint iRand = rand() % 2;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_TARGET_RUSH_0);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_TARGET_RUSH_0);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_TARGET_RUSH_0);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_TargetRush_0.wav"), fVOICE);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_TargetRush_1.wav"), fVOICE);

		CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_TargetRush.wav"), fEFFECT);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_TARGET_RUSH_1);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_TARGET_RUSH_1);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pTanjiro);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_TARGET_RUSH_2);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_TARGET_RUSH_2);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_TARGET_RUSH_1, false);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_TARGET_RUSH_2);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CTanjiro * pTanjiro)
{
}

void CTargetRushState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	if (g_iLevel == 12)
	{
		vTargetPosition = XMVectorSet(-0.012f, 16.6f, 181.f, 1.f);
	}

	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);



	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}

	if (g_iLevel == 12)
	{
		if (fDistance <= 3.f)
		{
			m_bNextAnim = true;

			if (pTanjiro->Get_BattleTarget()->Get_PlayerInfo().bGuard && pTanjiro->Get_BattleTarget()->Get_PlayerInfo().iGuard > 0)
			{
				pTanjiro->Get_BattleTarget()->Get_GuardHit(0);

			}
			else
			{
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_HIT, pTanjiro);
				RELEASE_INSTANCE(CEffect_Manager);
				pTanjiro->Get_BattleTarget()->Take_Damage(0.3f, false);

				CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
				dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
				dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
				dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pTanjiro->Get_Renderer());
				RELEASE_INSTANCE(CGameInstance);
			}
		}
		else
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	}
	
	else if (pTanjiro->Get_SphereCollider()->Collision(pTanjiro->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
		_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		pTanjiro->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(vPos);

		if (pTanjiro->Get_BattleTarget()->Get_PlayerInfo().bGuard && pTanjiro->Get_BattleTarget()->Get_PlayerInfo().iGuard > 0)
		{
			pTanjiro->Get_BattleTarget()->Get_GuardHit(0);
		
		}
		else
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_HIT, pTanjiro);
			RELEASE_INSTANCE(CEffect_Manager);
			pTanjiro->Get_BattleTarget()->Take_Damage(0.3f, false);

			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pTanjiro->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
		}
	}
	else
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CTanjiro * pTanjiro)
{
	m_vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

CTanjiroState * CTargetRushState::CommandCheck(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pTanjiro->Get_Model()->Get_Duration();
	m_fCurrentDuration = pTanjiro->Get_Model()->Get_CurrentTime();

	_float fRatio = m_fCurrentDuration / m_fDuration;


	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_I)) // ╫╨еЁ е╟ 
		{
			if (pTanjiro->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_O))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);
					if (pTanjiro->Get_KaguraMode() == true)
						return new CKaguraSkill_SphereState();
					else
						return new CSkill_WindMillState(TYPE_START);
				}
				else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);

					if (pTanjiro->Get_KaguraMode() == true)
						return new CKaguraSkill_MoveState();
					else
					return new CSkill_WaterMillState(TYPE_START); // move skill
				}
				else
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);

					if (pTanjiro->Get_KaguraMode() == true)
						return new CKaguraSkill_CommonState();
					else
					return new CSkill_CommonState();
				}
			}
		}
		else if (pGameInstance->Key_Pressing(DIK_L))
		{
			//return new CTargetRushState(TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_SPACE))
		{
			return new CJumpstate(TYPE_START, 0.f, 0.f);
		}
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_X)) // ╫╨еЁ е╟ 
		{
			if (pTanjiro->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_C))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);
					if (pTanjiro->Get_KaguraMode() == true)
						return new CKaguraSkill_SphereState();
					else
					return new CSkill_WindMillState(TYPE_START);
				}
				else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);
					if (pTanjiro->Get_KaguraMode() == true)
						return new CKaguraSkill_MoveState();
					else
					return new CSkill_WaterMillState(TYPE_START); // move skill
				}
				else
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);
					if (pTanjiro->Get_KaguraMode() == true)
						return new CKaguraSkill_CommonState();
					else
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
			return new CJumpstate(TYPE_START, 0.f, 0.f);
		}
		break;
	}



	return nullptr;
}

