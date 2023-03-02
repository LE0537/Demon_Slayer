#include "stdafx.h"
#include "RuiDadSkill_Rush.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "Camera_Dynamic.h"
using namespace RuiDad;


CSkill_RushState::CSkill_RushState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_GAMEPLAY, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CRuiDadState * CSkill_RushState::HandleInput(CRuiDad* pRuiDad)
{

	return nullptr;
}

CRuiDadState * CSkill_RushState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	pRuiDad->Set_EffectTime(fTimeDelta);
	//pRuiDad->Get_Model()->Set_Loop(pRuiDad->Get_AnimIndex());
	pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	CCharacters* m_pTarget = pRuiDad->Get_BattleTarget();
	m_fMove += fTimeDelta;
	if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiDadState::TYPE_START:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CSkill_RushState(CRuiDadState::TYPE_LOOP);
			break;
		case Client::CRuiDadState::TYPE_LOOP:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			//return new CSkill_JumpDropState(CRuiDadState::TYPE_END);
			break;
		case Client::CRuiDadState::TYPE_END:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_DASH_STARTGROUND, pRuiDad);
	
			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		break;
	case Client::CRuiDadState::TYPE_LOOP:
		Move(pRuiDad, fTimeDelta);
		EffectTime += fTimeDelta;
		if (EffectTime > 0.2f)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_DASH_PLAYER, pRuiDad);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_DASH_GROUND, pRuiDad);

			RELEASE_INSTANCE(CEffect_Manager);
			EffectTime = 0.f;
		}
		if (m_bNextAnim == true)
		{
			return new CSkill_RushState(CRuiDadState::TYPE_END);
		}
		break;
	case Client::CRuiDadState::TYPE_END:
		if (m_fMove < 0.2f)
		{
			_vector vCollPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
			CCollider*	pMyCollider2 = pRuiDad->Get_SphereCollider();
			if (m_iHit == 0)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(_int(-100 * pRuiDad->Get_PlayerInfo().fPowerUp));
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else if (pRuiDad->Get_BattleTarget()->Get_GodMode() == false)
					{
						m_pTarget->Set_Hp(_int(-70 * pRuiDad->Get_PlayerInfo().fPowerUp));
						m_pTarget->Take_Damage(0.3f, true);
						pRuiDad->Set_Combo(1);
						pRuiDad->Set_ComboTime(0.f);
					}


					if (pRuiDad->Get_BattleTarget()->Get_GodMode() == false)
					{
						CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiDad_SE_Hit_RushSkill.wav"), g_fEffect);
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

			if (pMyCollider2->Collision(pTargetCollider))
			{
				_float fSpeed = pRuiDad->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 2.f;

				_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
				_vector vMyLook = vTargetLook * -1.f;

				_vector vPow = XMVector3Dot(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

				_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

				vPos += vMyLook * (fSpeed - fSpeed * fPow);
				vTargetPos += vTargetLook * fSpeed * fPow;
				vPos.m128_f32[1] = 0.f;
				_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
				if (pRuiDad->Get_NavigationCom()->Cheak_Cell(vPos))
					pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
				if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
					m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
				else
					pRuiDad->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pRuiDad->Get_NavigationCom());
			}
		}
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_DASH_ENDGROUND, pRuiDad);
		
			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		break;
	case Client::CRuiDadState::TYPE_DEFAULT:
		break;
	case Client::CRuiDadState::TYPE_CHANGE:
		break;
	default:
		break;
	}

	return nullptr;
}

CRuiDadState * CSkill_RushState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	
	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_RushState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	_uint iRand = rand() % 2;

	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL2_0);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL2_0, 0.01f);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL2_0);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL2_0);
		break;
	case Client::CRuiDadState::TYPE_LOOP:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL2_1);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL2_1, 0.01f);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL2_1);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL2_1, true);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiDad_SE_RushSkill.wav"), g_fEffect);
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_RushSkill_0.wav"), g_fVoice);
		else
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_RushSkill_1.wav"), g_fVoice);
		Initialize_value(pRuiDad);
		break;
	case Client::CRuiDadState::TYPE_END:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL2_2);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL2_2, 0.01f);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL2_2);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL2_2);
		break;
	}




}

void CSkill_RushState::Exit(CRuiDad* pRuiDad)
{
	m_pCollBox->Set_Dead();
}

void CSkill_RushState::Move(CRuiDad * pRuiDad, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	if (fDistance <= 3.f) // 임시
	{
		m_bNextAnim = true;
	}
	else
		pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	//if (pRuiDad->Get_SphereCollider()->Collision(pRuiDad->Get_BattleTarget()->Get_SphereCollider()))
	//{
	//	m_bNextAnim = true;
	//	_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	//	pRuiDad->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(vPos);

	//	if (pRuiDad->Get_BattleTarget()->Get_PlayerInfo().bGuard && pRuiDad->Get_BattleTarget()->Get_PlayerInfo().iGuard > 0)
	//	{
	//		pRuiDad->Get_BattleTarget()->Get_GuardHit(0);
	//	}
	//	else
	//	{
	//		CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	//		dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
	//		RELEASE_INSTANCE(CGameInstance);
	//		pRuiDad->Get_BattleTarget()->Take_Damage(0.3f, false);
	//	}
	//}
	//else
	//	pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


}

void CSkill_RushState::Initialize_value(CRuiDad * pRuiDad)
{
	m_vPosition.x = XMVectorGetX(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pRuiDad->Set_NavigationHeight(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pRui->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

