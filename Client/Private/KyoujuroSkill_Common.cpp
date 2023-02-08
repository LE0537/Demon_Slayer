#include "stdafx.h"
#include "KyoujuroSkill_Common.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Tanjiro.h"
#include "Effect_Manager.h"
#include "KyoujuroDashState.h"
#include "KyoujuroDashState.h"
#include "KyoujuroSkill_Common.h"
#include "KyoujuroSkill_DashSlash.h"
#include "KyoujuroSkill_DoubleUpper.h"
#include "KyoujuroJumpState.h"
#include "KyoujuroTargetRushState.h"
#include "KyoujuroAtk_1_State.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
using namespace Kyoujuro;


CSkill_CommonState::CSkill_CommonState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_KyoujuroSkill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox2)))
		return;
	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CSkill_CommonState::HandleInput(CKyoujuro * pKyojuro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pKyojuro->Get_Model()->Get_Duration_Index(pKyojuro->Get_AnimIndex());
	m_fCurrentDuration = pKyojuro->Get_Model()->Get_CurrentTime_Index(pKyojuro->Get_AnimIndex());

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.6f && pKyojuro->Get_SubSkill() == 0)
	{
		switch (pKyojuro->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_W)) // 앞
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // 좌
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // 우
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

			else if (pGameInstance->Key_Pressing(DIK_S)) // 뒤
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // 좌
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LB);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // 우 
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


			else if (pGameInstance->Key_Pressing(DIK_A)) // 좌
			{

				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // 우
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RIGHT);
			}
			break;
		case 2:
			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_UP)) // 앞
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우
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

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 뒤
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LB);


				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우 
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


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RIGHT);

			}
			break;
		}
	}


	return CommandCheck(pKyojuro);
}

CKyoujuroState * CSkill_CommonState::Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	pKyojuro->Get_Model()->Set_Loop(pKyojuro->Get_AnimIndex());

	if (pKyojuro->Get_Model()->Get_End(pKyojuro->Get_AnimIndex()))
	{
		pKyojuro->Get_Model()->Set_End(pKyojuro->Get_AnimIndex());
		
		if(pKyojuro->Get_SubSkill() != 0)
		{
			pKyojuro->Set_Sub(true);
			pKyojuro->Set_SubSkill(0);
		}
		return new CIdleState();
	}

	return nullptr;
}

CKyoujuroState * CSkill_CommonState::Late_Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	CGameInstance*      pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pKyojuro->Get_BattleTarget();
	if (!m_bLook)
	{
		_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		XMStoreFloat4(&m_vLook, vLooAt);
		pKyojuro->Get_Transform()->Set_PlayerLookAt(vLooAt);
		m_bLook = true;
	}
	_vector vCollPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가

	if (!m_bTrue)
	{
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox2->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		m_pCollBox2->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
		m_bTrue = true;
	}
	m_fTime += fTimeDelta;

	if (m_fTime < 0.5f)
	{
		pKyojuro->Get_Transform()->Go_Straight(fTimeDelta * 0.7f, pKyojuro->Get_NavigationCom());

		if (!m_bHit && m_fTime > 0.3f)
		{
	
			_vector vCollLook = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 1.f; //추가
			vCollPos.m128_f32[1] = 0.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
			CCollider*   pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*   pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);
				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(-40);
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(1.f);
					}
				}
				else
				{
					m_pTarget->Set_Hp(-15);
					m_pTarget->Take_Damage(0.2f, false);
					pKyojuro->Set_Combo(1);
					pKyojuro->Set_ComboTime(1.f);
				}
				_matrix vTagetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();

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

				m_bHit = true;
			}
		}


		CCollider*   pMyCollider = pKyojuro->Get_SphereCollider();
		CCollider*   pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_float fSpeed = pKyojuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pKyojuro->Get_NavigationCom()->Cheak_Cell(vPos))
				pKyojuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pKyojuro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pKyojuro->Get_NavigationCom());
		}
	}
	else if (m_fTime >= 0.5f && m_fTime < 1.5f)
	{

		if (m_fTime < 0.9f)
		{
			pKyojuro->Get_Transform()->Go_Straight(fTimeDelta * 0.3f, pKyojuro->Get_NavigationCom());
		}
		if (m_fTime < 1.5f)
		{
			m_pCollBox2->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 2.f);
		}
		m_fHitTime += fTimeDelta;
		if (m_iHit < 5 && m_fHitTime > 0.08f)
		{

			CCollider*   pMyCollider = m_pCollBox2->Get_Collider(); //추가
			CCollider*   pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(-40);
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(1.f);
					}
				}
				else
				{
					CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_DOWN, 0.1f);
					RELEASE_INSTANCE(CGameInstance);
					m_pTarget->Set_Hp(-15);
					m_pTarget->Take_Damage(0.2f, false);
					pKyojuro->Set_Combo(1);
					pKyojuro->Set_ComboTime(1.f);
				}

				_matrix vTagetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();

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
				m_fHitTime = 0.f;
				++m_iHit;
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	pKyojuro->Get_Model()->Play_Animation(fTimeDelta);
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_5TIGER_CHARGE, pKyojuro);
		//pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_5TIGER_MAIN, &m_pEffect);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}


	return nullptr;
}

void CSkill_CommonState::Enter(CKyoujuro * pKyojuro)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	pKyojuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SKILL_COMMON);
	pKyojuro->Set_AnimIndex(CKyoujuro::ANIM_SKILL_COMMON);
	pKyojuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_SKILL_COMMON, 0.01f);

}

void CSkill_CommonState::Exit(CKyoujuro * pKyojuro)
{
	pKyojuro->Get_Model()->Reset_Anim(CKyoujuro::ANIMID::ANIM_SKILL_COMMON);
	m_pCollBox->Set_Dead();

}

CKyoujuroState * CSkill_CommonState::CommandCheck(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pKyoujuro->Get_Model()->Get_Duration_Index(CKyoujuro::ANIM_SKILL_COMMON);
	m_fCurrentDuration = pKyoujuro->Get_Model()->Get_CurrentTime_Index(CKyoujuro::ANIM_SKILL_COMMON);

	_float fRatio = m_fCurrentDuration / m_fDuration;


	if (fRatio >= 0.5f)
	{
		switch (pKyoujuro->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_I)) // 스킬 키 
			{
				if (pKyoujuro->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						pKyoujuro->Set_SkillBar(-200);
						return new CSkill_DoubleUpperState();
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						pKyoujuro->Set_SkillBar(-200);
						return new CSkill_DashSlashState(); // move skill
					}
					else
					{
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
			if (pGameInstance->Key_Pressing(DIK_X)) // 스킬 키 
			{
				if (pKyoujuro->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						pKyoujuro->Set_SkillBar(-200);
						return new CSkill_DoubleUpperState();
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{


						pKyoujuro->Set_SkillBar(-200);
						return new CSkill_DashSlashState(); // move skill

					}
					else
					{
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

	}

	return nullptr;
}

