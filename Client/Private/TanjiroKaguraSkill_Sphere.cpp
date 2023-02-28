#include "stdafx.h"
#include "TanjiroKaguraSkill_Sphere.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
#include "UI_Manager.h"

#include "TanjiroJumpState.h"
#include "TanjiroKaguraSkill_Common.h"
#include "TanjiroKaguraSkill_Move.h"
#include "TanjiroKaguraSkill_Sphere.h"
#include "TanjiroDashState.h"
#include "TanjiroTargetRushState.h"
#include "TanjiroJumpState.h"
using namespace Tanjiro;


CKaguraSkill_SphereState::CKaguraSkill_SphereState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (eType == TYPE_START)
	{
		if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_KaguraSkill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
			return;
	}
	m_eStateType = eType;

	RELEASE_INSTANCE(CGameInstance);
	//m_fHitTime = 0.1;
}

CTanjiroState * CKaguraSkill_SphereState::HandleInput(CTanjiro * pTanjiro)
{

	return CommandCheck(pTanjiro);
}

CTanjiroState * CKaguraSkill_SphereState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Set_EffectTime(fTimeDelta);

	//pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CKaguraSkill_SphereState(TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			if (m_bNextAnim == true)
			{
				return new CKaguraSkill_SphereState(TYPE_END);
			}
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			break;
		case Client::CTanjiroState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}

	m_fTime += fTimeDelta;

	if (m_eStateType == TYPE_LOOP)
		Fall_Height(pTanjiro, fTimeDelta);


	return nullptr;
}

CTanjiroState * CKaguraSkill_SphereState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();

	if (m_eStateType == TYPE_START && m_bTrue)
	{
		if (!m_bLook)
		{
			_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			XMStoreFloat4(&m_vLook, vLooAt);
			pTanjiro->Get_Transform()->Set_PlayerLookAt(vLooAt);
			pTanjiro->Set_Render(true);

			_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTarget = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vLook2 = vPos - vTarget;

			_float fDist = XMVectorGetX(XMVector3Length(vLook2));

			if (fDist < 15.f)
			{
				_vector vTargetPos = vPos - vTarget;
				vTarget += XMVector3Normalize(vTargetPos) * 2.f;
				if (g_iLevel == LEVEL_BATTLEENMU)
					vTarget.m128_f32[1] = 20.f;
				else
					vTarget.m128_f32[1] = 4.f;
				if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vTarget))
					pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTarget);
			}
			else
			{
				_vector vLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK);
				vPos += XMVector3Normalize(vLook) * 10.f;
				if (g_iLevel == LEVEL_BATTLEENMU)
					vTarget.m128_f32[1] = 20.f;
				else
					vPos.m128_f32[1] = 4.f;
				if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vPos))
					pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			}
			m_bLook = true;

		}


		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pTanjiro->Get_SphereCollider();
		m_fMove += fTimeDelta;

		if (m_fMove > 0.2f)
		{
			pTanjiro->Set_Render(false);
			if (!m_bEffect2)
			{
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOSPHERE_MAIN1, pTanjiro);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOSPHERE_MAIN2, pTanjiro);

				RELEASE_INSTANCE(CEffect_Manager);
				m_bEffect2 = true;
			}
		}
		if (m_fMove < 0.45f)
		{
			if (m_iHit < 1 && m_fMove > 0.7f)
			{
				_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
				_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
				//vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
				//vCollPos.m128_f32[1] = 1.f; //추가
				m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
				m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
				CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가


				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(_int(-60 * pTanjiro->Get_PlayerInfo().fPowerUp));
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
						m_pTarget->Set_Hp(_int(-40 * pTanjiro->Get_PlayerInfo().fPowerUp));
						m_pTarget->Take_Damage(0.3f, false);
						pTanjiro->Set_Combo(1);
						pTanjiro->Set_ComboTime(0.f);
					}
					CSoundMgr::Get_Instance()->PlayEffect(TEXT("Hinokami_Hit.wav"), fEFFECT);
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
		}
		else if (m_fMove < 0.85f && m_fMove > 0.65f)
		{
			if (!m_bHit)
			{
				_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
				_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
				//vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
				//vCollPos.m128_f32[1] = 1.f; //추가
				m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
				m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
				CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
				CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(_int(-60 * pTanjiro->Get_PlayerInfo().fPowerUp));
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
						m_pTarget->Set_Hp(_int(-40 * pTanjiro->Get_PlayerInfo().fPowerUp));
						m_pTarget->Take_Damage(0.5f, false);
						pTanjiro->Set_Combo(1);
						pTanjiro->Set_ComboTime(0.f);
					}
					CSoundMgr::Get_Instance()->PlayEffect(TEXT("Hinokami_Hit.wav"), fEFFECT);
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
		}

		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pTanjiro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vPos))
				pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pTanjiro->Get_NavigationCom());
		}

	}
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	if (m_eStateType == TYPE_START && !m_bTrue)
	{

		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOSPERE_START, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		m_bTrue = true;
	}
	return nullptr;
}

void CKaguraSkill_SphereState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_SKILL_KAGURA_SPHERE;
	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
	//	pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_SKILL_KAGURA_SPHERE);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_KAGURA_SPHERE);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_KAGURA_SPHERE);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.01f);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Hinokami_SE_Sphere_0.wav"), fEFFECT);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_KAGURA_SPHERE_1);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_KAGURA_SPHERE_1);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex(), true);
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.01f);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Hinokami_SE_Sphere_1.wav"), fEFFECT);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_KAGURA_SPHERE_2);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_KAGURA_SPHERE_2);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.01f);
		break;
	default:
		break;
	}


	
}
void CKaguraSkill_SphereState::Fall_Height(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;

	static _float fGravity = -100.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);
		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
	{
		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}


}
void CKaguraSkill_SphereState::Exit(CTanjiro * pTanjiro)
{
	if(m_pCollBox !=nullptr)
		m_pCollBox->Set_Dead();

	if(m_eStateType == TYPE_END)
		pTanjiro->Reset_SkillHit();

}

CTanjiroState * CKaguraSkill_SphereState::CommandCheck(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pTanjiro->Get_Model()->Get_Duration_Index(CTanjiro::ANIM_SKILL_KAGURA_SPHERE_2);
	m_fCurrentDuration = pTanjiro->Get_Model()->Get_CurrentTime_Index(CTanjiro::ANIM_SKILL_KAGURA_SPHERE_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	//printf_s("Ratio : %f \n", fRatio);
	//printf_s("Duration : %f \n", m_fDuration);
	//printf_s("current : %f \n", m_fCurrentDuration);

	if (fRatio >= 0.2f)
	{
		switch (pTanjiro->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_I)) // 스킬 키 
			{
				if (pTanjiro->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_SphereState();
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_MoveState(); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_CommonState();
					}
				}
			}
			else if (pGameInstance->Key_Pressing(DIK_L))
			{
				return new CTargetRushState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_SPACE))
			{
				return new CJumpstate(TYPE_START, 0.f, 0.f);
			}
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_X)) // 스킬 키 
			{
				if (pTanjiro->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_SphereState();
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_MoveState(); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_CommonState();
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
	}

	return nullptr;
}

