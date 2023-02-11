#include "stdafx.h"
#include "RuiAtk_4_State.h"
#include "RuiIdleState.h"
#include "GameInstance.h"

#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiDashState.h"
#include "RuiTargetRushState.h"
#include "RuiSkill_Shoot.h"
#include "RuiSkill_ShootNet.h"
#include "RuiSkill_Sphere.h"
#include "RuiAdvSkill_CommonState.h"
#include "RuiAdvSkill_MoveState.h"
#include "RuiJumpState.h"
#include "Camera_Dynamic.h"
#include "AiState.h"
using namespace Rui;


CAtk_4_State::CAtk_4_State()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CRuiState * CAtk_4_State::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (m_fComboDelay <= 35.f)
	{
		switch (pRui->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_W)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ��
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ��
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

			else if (pGameInstance->Key_Pressing(DIK_S)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ��
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LB);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // �� 
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


			else if (pGameInstance->Key_Pressing(DIK_A)) // ��
			{

				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RIGHT);
			}
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_UP)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
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

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LB);


				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // �� 
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


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RIGHT);

			}
			break;
		}
	}


	return CommandCheck(pRui);
}

CRuiState * CAtk_4_State::Tick(CRui* pRui, _float fTimeDelta)
{
	if (pRui->Get_IsAIMode() == true)
	{

		if (pRui->Get_Model()->Get_End(CRui::ANIM_ATTACK_4))
		{
			pRui->Get_Model()->Set_End(CRui::ANIM_ATTACK_4);
			return new CAiState();
		}

	}

	pRui->Get_Model()->Set_Loop(CRui::ANIM_ATTACK_4);
	pRui->Get_Model()->Set_LinearTime(CRui::ANIM_ATTACK_4, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;
	//printf_s("AttackTime : %f \n", (_float)m_fTime);


	//if (m_bAtkCombo == true && m_fTime >= 33.f)
	//	return new CAtk_2_State();



	if (pRui->Get_Model()->Get_End(CRui::ANIM_ATTACK_4))
	{
		pRui->Get_Model()->Set_End(CRui::ANIM_ATTACK_4);
		return new CIdleState();
	}


	return nullptr;
}

CRuiState * CAtk_4_State::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pRui->Get_Transform()->Set_PlayerLookAt(vLooAt);

	m_fMove += fTimeDelta;

	if (m_fMove >= 1.f && m_fMove < 1.3f)
		pRui->Get_Transform()->Go_Straight(fTimeDelta * 0.3f, pRui->Get_NavigationCom());
	if (m_fMove < 0.5f)
	{
		_vector vCollPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //�߰�
		_vector vCollLook = pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK); //�߰�
		vCollPos += XMVector3Normalize(vCollLook) * 4.f; //�߰�
		vCollPos.m128_f32[1] = 1.f; //�߰�
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //�߰�
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //�߰�
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pRui->Get_SphereCollider();
		if (m_fMove > 0.1f && m_iHit == 0)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(-30 * pRui->Get_PlayerInfo().fPowerUp);
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else
				{
					CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
					RELEASE_INSTANCE(CGameInstance);
					m_pTarget->Set_Hp(-pRui->Get_PlayerInfo().iDmg * pRui->Get_PlayerInfo().fPowerUp);
					m_pTarget->Take_Damage(0.3f, false);
					pRui->Set_Combo(1);
					pRui->Set_ComboTime(0.f);
				}

				_int iDest = rand() % 5;
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				switch (iDest)
				{
				case 0:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);
					break;
				case 1:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT2, m_pTarget);
					break;
				case 2:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT3, m_pTarget);
					break;
				case 3:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT4, m_pTarget);
					break;
				case 4:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT5, m_pTarget);
					break;
				default:
					break;
				}


				RELEASE_INSTANCE(CEffect_Manager);

				++m_iHit;
			}

		}


		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pRui->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pRui->Get_NavigationCom()->Cheak_Cell(vPos))
				pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pRui->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pRui->Get_NavigationCom());
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	pRui->Get_Model()->Play_Animation(fTimeDelta);
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIATK4, pRui);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CAtk_4_State::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_ATK_4;

	pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_ATTACK_4);
	pRui->Set_AnimIndex(CRui::ANIM_ATTACK_4);

	_uint iRand = rand() % 2;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_Attack4_1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_Attack4_2.wav"), fEFFECT);

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("KnifeEff1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("KnifeEff2.wav"), fEFFECT);
}

void CAtk_4_State::Exit(CRui* pRui)
{
	m_pCollBox->Set_Dead(); //�߰�
}

CRuiState * CAtk_4_State::CommandCheck(CRui * pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pRui->Get_Model()->Get_Duration();
	m_fCurrentDuration = pRui->Get_Model()->Get_CurrentTime();

	_float fRatio = m_fCurrentDuration / m_fDuration;


	switch (pRui->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_I)) // ��ų Ű 
		{
			if (pRui->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_O))
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_SphereState(TYPE_START);
				}
				else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_ShootNetState(TYPE_START); // move skill
				}
				else
				{
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
					return new CAdvSkill_MoveState();
				}
				else
				{
					pRui->Set_FriendSkillBar(-500);
					return new CAdvSkill_CommonState();
				}
			}
		}

		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_X)) // ��ų Ű 
		{
			if (pRui->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_C))
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_SphereState(TYPE_START);
				}
				else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_ShootNetState(TYPE_START); // move skill
				}
				else
				{
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
					return new CAdvSkill_MoveState();
				}
				else
				{
					pRui->Set_FriendSkillBar(-500);
					return new CAdvSkill_CommonState();
				}
			}
		}
	}



	return nullptr;
}

