#include "stdafx.h"
#include "..\Public\KyoujuroSplSkrStartState.h"
#include "GameInstance.h"
#include "Kyoujuro_CinemaState.h"
#include "KyoujuroIdleState.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CSplSkrStartState::HandleInput(CKyoujuro* pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CSplSkrStartState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_LOOP);
			break;
		case Client::CKyoujuroState::TYPE_LOOP:
			if (m_bCollision == true)
			{
				pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
				pKyoujuro->Get_BattleTarget()->Play_Scene();
				return new CKyoujuro_CinemaState(CKyoujuro_CinemaState::SCENE_START);
			}
			else
			{
				pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
				return new CIdleState();
			}
			break;
		case Client::CKyoujuroState::TYPE_END:
			break;
		case Client::CKyoujuroState::TYPE_DEFAULT:
			break;
		case Client::CKyoujuroState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());

	}


	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		m_fDuration += fTimeDelta;

		if(m_fDuration <= 0.7f)
			Move(pKyoujuro, fTimeDelta);
		break;
	case Client::CKyoujuroState::TYPE_END:
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		break;
	case Client::CKyoujuroState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CKyoujuroState * CSplSkrStartState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	if (m_eStateType == CKyoujuroState::TYPE_LOOP)
	{
		CCharacters* m_pTarget = pKyoujuro->Get_BattleTarget();

		m_fMove += fTimeDelta;

		if (m_fMove > 0.3f)
		{
			_vector vCollPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] += 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
			CCollider*	pMyCollider2 = pKyoujuro->Get_SphereCollider();
			if (m_fMove < 0.5f && !m_bHit)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().fGuardTime <= 0.f)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(-1);
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else if (pKyoujuro->Get_BattleTarget()->Get_GodMode() == false)
					{
						m_bCollision = true;
					}
					if (pKyoujuro->Get_BattleTarget()->Get_GodMode() == false)
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

						m_bHit = true;
					}
				}

			}


			if (pMyCollider2->Collision(pTargetCollider))
			{
				_float fSpeed = pKyoujuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

				_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
				_vector vMyLook = vTargetLook * -1.f;

				_vector vPow = XMVector3Dot(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

				_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

				vPos += vMyLook * (fSpeed - fSpeed * fPow);
				vTargetPos += vTargetLook * fSpeed * fPow;
				if (g_iLevel != LEVEL_BATTLEENMU && g_iLevel != LEVEL_BOSSENMU)
					vPos.m128_f32[1] = 0.f;
				_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
				if (pKyoujuro->Get_NavigationCom()->Cheak_Cell(vPos))
					pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
				if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
					m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
				else
					pKyoujuro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pKyoujuro->Get_NavigationCom());
			}
		}
	}
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSplSkrStartState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Transform()->Set_PlayerLookAt(pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SPLSKL_START_0);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_SPLSKL_START_0);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_SPLSKL_START_0);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_SPLSKL_START_0, 0.01f);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Kyojuro_SplSkr_Start.wav"), g_fVoice);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Kyojuro_SE_SplSkr_Start.wav"), g_fEffect);
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SPLSKL_START_1);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_SPLSKL_START_1);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_SPLSKL_START_1);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_SPLSKL_START_1, 0.01f);
		break;
	case Client::CKyoujuroState::TYPE_END:
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		break;
	case Client::CKyoujuroState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CKyoujuro* pKyoujuro)
{
	m_pCollBox->Set_Dead();
}

void CSplSkrStartState::Move(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	_vector vMyPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if (fDistance >= 5.f)
		pKyoujuro->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
}
