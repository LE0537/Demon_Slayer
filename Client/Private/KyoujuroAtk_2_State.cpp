#include "stdafx.h"
#include "KyoujuroAtk_2_State.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroAtk_3_State.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Tanjiro.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;


CAtk_2_State::CAtk_2_State()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CAtk_2_State::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
		m_bAtkCombo = true;

	//if (pGameInstance->Key_Down(DIK_J) && m_fComboDelay <= 43.f)
	//	m_bAtkCombo = true;

	return nullptr;
}

CKyoujuroState * CAtk_2_State::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_ATTACK_2);
	pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_ATTACK_2, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;
	//printf_s("AttackTime : %f \n", (_float)m_fComboDelay);
	//printf_s("AttackTime : %f \n", (_float)m_fTime);

	if (m_bAtkCombo == true && m_fTime >= 30.f)
		return new CAtk_3_State();



	if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_ATTACK_2))
	{
		pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_ATTACK_2);
		return new CIdleState();
	}


	return nullptr;
}

CKyoujuroState * CAtk_2_State::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"))->Get_LayerFront();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pKyoujuro->Get_Transform()->LookAt(vLooAt);
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	m_fMove += fTimeDelta;
	if (m_fMove < 0.3f)
	{
		if (m_iHit < 1)
		{
			_vector vCollPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), TEXT("Com_SPHERE"));

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_float4 vTagetPos;
				XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = 0.f;
				m_pTarget->Get_Transform()->LookAt(vPos);
				m_pTarget->Set_Hp(-pKyoujuro->Get_PlayerInfo().iDmg);
				dynamic_cast<CTanjiro*>(m_pTarget)->Take_Damage(0.0f);

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				vTagetPos.y += 2.f;
				pEffectManger->Create_Effect(CEffect_Manager::EFFECT_HIT, vTagetPos);

				RELEASE_INSTANCE(CEffect_Manager);

				++m_iHit;
			}
		}
	}
	else if (m_fMove >= 0.3f)
	{
		if (!m_bHit)
		{
			_vector vCollPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), TEXT("Com_SPHERE"));

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_float4 vTagetPos;
				XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = 0.f;
				m_pTarget->Get_Transform()->LookAt(vPos);
				m_pTarget->Set_Hp(-pKyoujuro->Get_PlayerInfo().iDmg);
				dynamic_cast<CTanjiro*>(m_pTarget)->Take_Damage(0.3f);

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				vTagetPos.y += 2.f;
				pEffectManger->Create_Effect(CEffect_Manager::EFFECT_HIT, vTagetPos);

				RELEASE_INSTANCE(CEffect_Manager);

				m_bHit = true;
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CAtk_2_State::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_ATK_2;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_ATTACK_2);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_ATTACK_2);

}

void CAtk_2_State::Exit(CKyoujuro * pKyoujuro)
{
	m_pCollBox->Set_Dead(); //추가
}

