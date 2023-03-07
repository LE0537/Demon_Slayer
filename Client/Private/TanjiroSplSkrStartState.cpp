#include "stdafx.h"
#include "..\Public\TanjiroSplSkrStartState.h"
#include "GameInstance.h"
#include "HinoCami_CinemaState.h"
#include "TanjiroIdleState.h"
#include "Effect_Manager.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
using namespace Tanjiro;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CTanjiroState * CSplSkrStartState::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * CSplSkrStartState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			/*if (m_bCollision == true)
			{
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				pTanjiro->Get_BattleTarget()->Play_Scene();
				g_bSpecialSkillHit = true;
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.5f);
				return new CHinoCami_CinemaState(CHinoCami_CinemaState::SCENE_START);
			}*/
			//else
			{
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				return new CIdleState();
			}
			break;
		case Client::CTanjiroState::TYPE_END:
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


	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		m_fDuration += fTimeDelta;

		if (m_fDuration <= 0.7f)
			Move(pTanjiro, fTimeDelta);

		if (m_bCollision == true && m_bCreate == false)
		{
			m_bPlayScene = true;
			g_bSpecialSkillHit = true;
			m_bCreate = true;
			pTanjiro->Get_BattleTarget()->Take_Damage(0.f, false);
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 1.f);
		}

		if (m_bPlayScene == true)
		{
			if (g_bSpecialSkillHit == false)
			{
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				pTanjiro->Get_BattleTarget()->Play_Scene();
				return new CHinoCami_CinemaState(CHinoCami_CinemaState::SCENE_START);
			}
		}



		break;
	case Client::CTanjiroState::TYPE_END:
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		break;
	case Client::CTanjiroState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CTanjiroState * CSplSkrStartState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (m_eStateType == CTanjiroState::TYPE_LOOP)
	{
		CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();

		m_fMove += fTimeDelta;

		if(pTanjiro->Get_Model()->Get_CurrentTime_Index(CTanjiro::ANIMID::ANIM_SPLSKL_START_1) >= 14.5f)
		//if (m_fMove > 0.5f)
		{
			_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] += 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
			CCollider*	pMyCollider2 = pTanjiro->Get_SphereCollider();
			//if (m_fMove < 0.5f && !m_bHit)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

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
					else if (pTanjiro->Get_BattleTarget()->Get_GodMode() == false)
					{
						m_bCollision = true;
					}
					if (pTanjiro->Get_BattleTarget()->Get_GodMode() == false && m_bCollision == false)
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
				_float fSpeed = pTanjiro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

				_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
				_vector vMyLook = vTargetLook * -1.f;

				_vector vPow = XMVector3Dot(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

				_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

				vPos += vMyLook * (fSpeed - fSpeed * fPow);
				vTargetPos += vTargetLook * fSpeed * fPow;
				if (g_iLevel != LEVEL_BATTLEENMU && g_iLevel != LEVEL_BOSSENMU)
					vPos.m128_f32[1] = 0.f;
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
	}
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);
	if (m_eStateType == CTanjiroState::TYPE_START)
	{
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_START, pTanjiro);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_STARTSLASH, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}
	return nullptr;

}
void CSplSkrStartState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SPLSKL_START_0);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SPLSKL_START_0);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_SPLSKL_START_0);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SPLSKL_START_0, 0.01f);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_SplSkrStart.wav"), g_fVoice);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_SE_SplSkr_Start.wav"), g_fEffect);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SPLSKL_START_1);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SPLSKL_START_1);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_SPLSKL_START_1);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SPLSKL_START_1, 0.01f);
		break;
	case Client::CTanjiroState::TYPE_END:
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		break;
	case Client::CTanjiroState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead();
}

void CSplSkrStartState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if (fDistance >= 5.f)
		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
}
