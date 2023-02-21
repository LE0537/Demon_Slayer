#include "stdafx.h"
#include "TanjiroJumpSkillCommonState.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"
#include "TanjiroMoveState.h"
#include "Effect_Manager.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
using namespace Tanjiro;

CJumpSkillCommonState::CJumpSkillCommonState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_WindMill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CTanjiroState * CJumpSkillCommonState::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * CJumpSkillCommonState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	//if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	//{
	//	switch (m_eStateType)
	//	{
	//	case Client::CTanjiroState::TYPE_START: // 공격 모션
	//		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	//		return new CJumpSkillCommonState(TYPE_LOOP);
	//		break;
	//	case Client::CTanjiroState::TYPE_LOOP: // 떨어지는 모션
	//		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());;
	//		break;
	//	case Client::CTanjiroState::TYPE_END: // 착지 모션
	//		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	//		return new CIdleState(STATE_JUMP);
	//		break;
	//	}
	//	pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	//}

	//switch (m_eStateType)
	//{
	//case Client::CTanjiroState::TYPE_LOOP:
	//	Jump(pTanjiro, fTimeDelta);

	//	if(m_bNextAnim == true)
	//		return new CJumpSkillCommonState(TYPE_END);
	//}

	if(m_eStateType != TYPE_CHANGE && m_bNextAnim != true)
		Jump(pTanjiro, fTimeDelta);



	//if (m_bNextAnim == true && pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	//{
	//	pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	//	//return new CJumpSkillCommonState(TYPE_CHANGE);
	//}

	
	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{

		pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_COMMON);


		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		if(m_bNextAnim == true)
			return new CIdleState();
	}



	return nullptr;
}

CTanjiroState * CJumpSkillCommonState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
	if (!m_bLook)
	{
		_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		XMStoreFloat4(&m_vLook, vLooAt);
		pTanjiro->Get_Transform()->Set_PlayerLookAt(vLooAt);
		m_bLook = true;
	}
	_int iHit = pTanjiro->Get_SkillHit();
	m_fDelay += fTimeDelta;
	if (m_fDelay > 0.3f)
	{
		m_fTime += fTimeDelta;
		m_fHitTime += fTimeDelta;
	}
	if (m_fTime < 0.35f)
	{
		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.3f, pTanjiro->Get_NavigationCom());
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pTanjiro->Get_SphereCollider();
		if (m_eStateType != Client::CTanjiroState::TYPE_CHANGE)
		{
			_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
																							 //m_pCollBox->Get_Transform()->LookAt(vLooAt);
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가

			if (m_fHitTime > 0.07f && iHit < 5)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(_int(-40 * pTanjiro->Get_PlayerInfo().fPowerUp));
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							  pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else if(pTanjiro->Get_BattleTarget()->Get_GodMode() == false)
					{
						CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
						dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_DOWN, 0.4f);
						RELEASE_INSTANCE(CGameInstance);
						m_pTarget->Set_Hp(_int(-15 * pTanjiro->Get_PlayerInfo().fPowerUp));

						if (iHit >= 3)
							m_pTarget->Player_TakeDown(0.3f);
						else
							m_pTarget->Take_Damage(0.1f, false);

						pTanjiro->Set_Combo(1);
						pTanjiro->Set_ComboTime(0.f);
					}

					if (pTanjiro->Get_BattleTarget()->Get_GodMode() == false)
					{
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

						pTanjiro->Set_SkillHit();
						m_fHitTime = 0.f;
					}
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

	RELEASE_INSTANCE(CGameInstance);

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_WATER8_1, pTanjiro);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}




	return nullptr;
}

void CJumpSkillCommonState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_JUMP_ATTACK;

	//switch (m_eStateType)
	//{
	//case Client::CTanjiroState::TYPE_START:
	//	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_JUMPCOMMON_0);
	//	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_JUMPCOMMON_0);
	//	pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//	break;
	//case Client::CTanjiroState::TYPE_LOOP:
	//	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_JUMPCOMMON_1);
	//	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_JUMPCOMMON_1);
	//	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_SKILL_JUMPCOMMON_1, false);
	//	Initialize_value(pTanjiro);
	//	break;
	//case Client::CTanjiroState::TYPE_END:
	//	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_JUMPCOMMON_2);
	//	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_JUMPCOMMON_2);
	//	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_SKILL_JUMPCOMMON_2, false);
	//	break;
	//}

	if (m_eStateType == TYPE_CHANGE)
	{
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_JUMP_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
	}
	else
	{
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_COMMON);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_COMMON);
		pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_SKILL_COMMON, false);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_COMMON, 0.01f);
		Initialize_value(pTanjiro);
	}


	CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_CommonSkill.wav"), fVOICE);
	CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_SE_CommonSkill.wav"), fEFFECT);

}

void CJumpSkillCommonState::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead();
	pTanjiro->Reset_SkillHit();
}

void CJumpSkillCommonState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	pTanjiro->Get_Transform()->Set_Jump(true);

	static _float fGravity = -100.f;
	static _float fVelocity = 0.f; 
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));



	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{ 
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;
		pTanjiro->Get_Transform()->Set_Jump(false);
		_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
		{
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
			m_bNextAnim = true;
		}
	}
	else
	{
		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpSkillCommonState::Initialize_value(CTanjiro * pTanjiro)
{
	m_vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;
}

